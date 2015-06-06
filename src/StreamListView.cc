/*
 * StreamListView.cc
 *
 *  Created on: Apr 4, 2015
 *      Author: amyznikov
 */
#include "msmwctl.h"
#include "StreamListView.h"

#define count_of(x) (sizeof(x)/sizeof((x)[0]))

namespace {

struct column_decs {
  const char * name;
  WLength width;
};

#define INPUT_COLUMN_NAME           0
#define INPUT_COLUMN_STATE          1
#define INPUT_COLUMN_ENABLED        2
#define INPUT_COLUMN_START_TIME     3
#define INPUT_COLUMN_STATUS         4
static const column_decs input_columns[] = {
    { "input name", WLength(20, WLength::FontEx) },
    { "state", WLength(20, WLength::FontEx) },
    { "enabled", WLength(20, WLength::FontEx) },
    { "start time", WLength(20, WLength::FontEx) },
    { "status", WLength(20, WLength::FontEx) },
};


#define OUTPUT_COLUMN_NAME          0
#define OUTPUT_COLUMN_STATE         1
#define OUTPUT_COLUMN_ENABLED       2
static const column_decs output_columns[] = {
    { "output name", WLength(20, WLength::FontEx)},
    { "state", WLength(20, WLength::FontEx)},
    { "enabled", WLength(20, WLength::FontEx)},
};


#define SINK_COLUMN_NAME            0
#define SINK_COLUMN_STATE           1
#define SINK_COLUMN_FORMAT          2
#define SINK_COLUMN_URL             3
static const column_decs sink_columns[] = {
    { "sink name", WLength(20,WLength::FontEx)},
    { "state", WLength(20, WLength::FontEx)},
    { "format", WLength(20, WLength::FontEx)},
    { "URL", WLength(20, WLength::FontEx)},
};


#define SESSION_COLUMN_NAME         0
#define SESSION_COLUMN_ADDRESS      1
#define SESSION_COLUMN_AGENT        2
static const column_decs session_columns[] = {
    { "session name", WLength(20, WLength::FontEx) },
    { "address", WLength(20, WLength::FontEx)},
    { "agent", WLength(20, WLength::FontEx)},
};

}


static void setRowCount(WStandardItemModel * model, int count)
{
  const int rc = model->rowCount();
  if ( rc < count ) {
    model->insertRows(rc, count - rc);
  }
  else {
    model->removeRows(count, rc - count);
  }
}


static inline void setColumnWidth(WTableView * table, int index, size_t width)
{
  table->setColumnWidth(index, WLength(width, WLength::FontEx));
}

static inline void adjustTableSize(WTableView * tbl, const column_decs columns[], const size_t cw[], size_t nb_columns)
{
  size_t i, n, x;
  for ( i = 0, n = 0; i < nb_columns; ++i ) {
    n += (x = std::max(cw[i], strlen(columns[i].name) + 4));
    tbl->setColumnWidth(i, WLength(x, WLength::FontEx));
  }
}

static WTableView * createTable(const column_decs columns[], int nb_columns, WContainerWidget * parent = 0)
{
  WTableView * table;
  WStandardItemModel * model;

  table = new WTableView(parent);
  table->setModel(model = new WStandardItemModel(0, 0, table));
  model->insertColumns(0, nb_columns);

  table->setSortingEnabled(true);
  table->setColumnResizeEnabled(true);

  for ( int i = 0, n = nb_columns; i < n; ++i ) {
    model->setHeaderData(i, Wt::Orientation::Horizontal, boost::any(WString::fromUTF8(columns[i].name)));
    table->setHeaderAlignment(i, Wt::AlignLeft);
    table->setColumnAlignment(i, Wt::AlignLeft);
    setColumnWidth(table, i, strlen(columns[i].name) + 4);
  }

  return table;
}


class StreamsTab
    : public WContainerWidget
{
protected:
  WTableView * table;
  WPushButton * refreshButton;

  StreamsTab(const column_decs columns[], int nb_columns, WContainerWidget * parent = 0)
      : WContainerWidget(parent)
  {
    addStyleClass("streamstab");

    WVBoxLayout * vbox = new WVBoxLayout(this);
    vbox->addWidget(table = createTable(columns, nb_columns));
    vbox->addWidget(refreshButton = new WPushButton("Refresh"),1, Wt::AlignLeft);
    refreshButton->clicked().connect(this, &StreamsTab::populateTable);
  }

public:
  virtual void populateTable() = 0;
};


class InputsTab
    : public StreamsTab
{
public:
  InputsTab(WContainerWidget* parent = 0)
    : StreamsTab(input_columns, count_of(input_columns), parent) {}

  void populateTable()
  {
    WStandardItemModel * model;
    std::vector<MsmInput> inputs;

    if ( !msmLoadInputs(&inputs) ) {
      setRowCount((WStandardItemModel*) table->model(), 0);
    }
    else {

      size_t cw [count_of(input_columns)] = {0};
      size_t i, n, x;
      std::string s;

      setRowCount(model = (WStandardItemModel*) table->model(), n = inputs.size());

      for ( i = 0; i < n; ++i ) {
        const MsmInput & input = inputs[i];

        cw[INPUT_COLUMN_NAME] = std::max(cw[INPUT_COLUMN_NAME], (s = input.getName()).size());
        model->setData(i, INPUT_COLUMN_NAME, boost::any(s));

        cw[INPUT_COLUMN_STATE] = std::max(cw[INPUT_COLUMN_STATE], (s = msm_state_string(input.getState())).size());
        model->setData(i, INPUT_COLUMN_STATE, boost::any(s));

        cw[INPUT_COLUMN_ENABLED] = std::max(cw[INPUT_COLUMN_ENABLED], (s = t2s(input.getEnabled())).size());
        model->setData(i, INPUT_COLUMN_ENABLED, boost::any(s));

        cw[INPUT_COLUMN_START_TIME] = std::max(cw[INPUT_COLUMN_START_TIME], (s = input.getStartTime()).size());
        model->setData(i, INPUT_COLUMN_START_TIME, boost::any(s));
      }

      adjustTableSize(table, input_columns, cw, count_of(input_columns));
    }

  }
};


class OutputsTab
    : public StreamsTab
{
public:
  OutputsTab(WContainerWidget* parent = 0)
    : StreamsTab(output_columns, count_of(output_columns), parent) {}

  void populateTable()
  {
    WStandardItemModel * model;
    std::vector<MsmOutput> outputs;

    if ( !msmLoadOutputs(&outputs) ) {
      setRowCount((WStandardItemModel*) table->model(), 0);
    }
    else {

      size_t cw [count_of(output_columns)] = {0};
      size_t i, n;
      std::string s;
      WString ws;

      setRowCount(model = (WStandardItemModel*) table->model(), n = outputs.size());

      for ( i = 0; i < n; ++i ) {
        const MsmOutput & output = outputs[i];

        ws = WString("{1}/{2}").arg(output.getInputName()).arg(output.getName());
        cw[OUTPUT_COLUMN_NAME] = std::max(cw[OUTPUT_COLUMN_NAME], ws.toUTF8().size());
        model->setData(i, OUTPUT_COLUMN_NAME, boost::any(ws));

        cw[OUTPUT_COLUMN_STATE] = std::max(cw[OUTPUT_COLUMN_STATE], (s = msm_state_string(output.getState())).size());
        model->setData(i, OUTPUT_COLUMN_STATE, boost::any(s));

        cw[OUTPUT_COLUMN_ENABLED] = std::max(cw[OUTPUT_COLUMN_ENABLED],(s=t2s(output.getEnabled())).size());
        model->setData(i, OUTPUT_COLUMN_ENABLED, boost::any(s));
      }

      for ( i = 0; i < count_of(output_columns); ++i ) {
        setColumnWidth(table, i, std::max(cw[i], strlen(output_columns[i].name) + 4));
      }

    }
  }
};


class SinksTab
  : public StreamsTab
{
public:
  SinksTab(WContainerWidget* parent = 0)
    : StreamsTab(sink_columns, count_of(sink_columns), parent) {}

  void populateTable()
  {
    WStandardItemModel * model;
    std::vector<MsmSink> sinks;

    if ( !msmLoadSinks(&sinks) ) {
      setRowCount((WStandardItemModel*) table->model(), 0);
    }
    else {

      size_t cw [count_of(sink_columns)] = {0};
      size_t i, n;
      std::string s;
      WString ws;

      setRowCount(model = (WStandardItemModel*) table->model(), n = sinks.size());

      for ( i = 0; i < n; ++i ) {
        const MsmSink & sink = sinks[i];

        ws = WString("{1}/{2}/{3}").arg(sink.getInputName()).arg(sink.getOutputName()).arg(sink.getName());
        cw[SINK_COLUMN_NAME] = std::max(cw[SINK_COLUMN_NAME],ws.toUTF8().size());
        model->setData(i, SINK_COLUMN_NAME, boost::any(ws));


        cw[SINK_COLUMN_STATE] = std::max(cw[SINK_COLUMN_STATE], (s = msm_state_string(sink.getState())).size());
        model->setData(i, SINK_COLUMN_STATE, s);

        cw[SINK_COLUMN_FORMAT] = std::max(cw[SINK_COLUMN_FORMAT], (s = sink.getFormat()).size());
        model->setData(i, SINK_COLUMN_FORMAT, s);

        cw[SINK_COLUMN_URL] = std::max(cw[SINK_COLUMN_URL], (s = sink.getUrl()).size());
        model->setData(i, SINK_COLUMN_URL, s);
      }

      for ( i = 0; i < count_of(sink_columns); ++i ) {
        setColumnWidth(table, i, std::max(cw[i], strlen(sink_columns[i].name) + 4));
      }

    }
  }
};



class SessionsTab
    : public StreamsTab
{
public:
  SessionsTab(WContainerWidget* parent = 0)
    : StreamsTab(session_columns, count_of(session_columns), parent) {}

  void populateTable()
  {
    WStandardItemModel * model;
    std::vector<MsmSession> sessions;

    if ( !msmLoadSessions(&sessions) ) {
      setRowCount((WStandardItemModel*) table->model(), 0);
    }
    else {

      size_t cw[count_of(session_columns)] = { 0 };
      size_t i, n;
      std::string s;

      setRowCount(model = (WStandardItemModel*) table->model(), n = sessions.size());

      for ( i = 0; i < n; ++i ) {

        const MsmSession & session = sessions[i];

        cw[SESSION_COLUMN_NAME] = std::max(cw[SESSION_COLUMN_NAME],(s=session.getName()).size());
        model->setData(i, SESSION_COLUMN_NAME, s);

        cw[SESSION_COLUMN_ADDRESS] = std::max(cw[SESSION_COLUMN_ADDRESS], (s = session.getAddress()).size());
        model->setData(i, SESSION_COLUMN_ADDRESS, s);

        cw[SESSION_COLUMN_AGENT] = std::max(cw[SESSION_COLUMN_AGENT], (s = session.getAgent()).size());
        model->setData(i, SESSION_COLUMN_AGENT, s);
      }

      for ( i = 0; i < count_of(session_columns); ++i ) {
        setColumnWidth(table, i, std::max(cw[i], strlen(session_columns[i].name) + 4));
      }

    }
  }
};






StreamListView::StreamListView(WContainerWidget * parent)
  : Base(parent)
{
  tab = new WTabWidget(this);
  tab->setStyleClass("tabwidget");
  tab->addTab(inputs = new InputsTab(), "Inputs");
  tab->addTab(outputs = new OutputsTab(), "Outputs");
  tab->addTab(sinks = new SinksTab(), "Sinks");
  tab->addTab(sessions = new SessionsTab(), "Sessions");
}


void StreamListView::refreshStreams()
{
  StreamsTab * currentPage;

  switch (tab->currentIndex()) {
    case 0: currentPage = (StreamsTab * ) inputs; break;
    case 1: currentPage = (StreamsTab * ) outputs; break;
    case 2: currentPage = (StreamsTab * ) sinks; break;
    case 3: currentPage = (StreamsTab * ) sessions; break;
    default: currentPage = 0; break;
  }

  if ( currentPage != 0 ) {
    currentPage->populateTable();
  }
}

