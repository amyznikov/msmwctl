/*
 * StreamListView.cc
 *
 *  Created on: Apr 4, 2015
 *      Author: amyznikov
 */
#include "msmwctl.h"
#include "StreamListView.h"

#define count_of(x) (sizeof(x)/sizeof((x)[0]))

static const char template_text [] =
    " <legend>${legend}</legend>"
    "   ${selector} ${refresh}"
    "   <p>${stack}</p>"
    "";



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
    { "state", WLength(10, WLength::FontEx) },
    { "enabled", WLength(10, WLength::FontEx) },
    { "start time", WLength(20, WLength::FontEx) },
    { "status", WLength(20, WLength::FontEx) },
};


#define OUTPUT_COLUMN_NAME          0
#define OUTPUT_COLUMN_STATE         1
#define OUTPUT_COLUMN_ENABLED       2
static const column_decs output_columns[] = {
    { "output name", WLength(20, WLength::FontEx)},
    { "state", WLength(10, WLength::FontEx)},
    { "enabled", WLength(4, WLength::FontEx)},
};


#define SINK_COLUMN_NAME            0
#define SINK_COLUMN_STATE           1
#define SINK_COLUMN_FORMAT          2
#define SINK_COLUMN_URL             3
static const column_decs sink_columns[] = {
    { "sink name", WLength(20,WLength::FontEx)},
    { "state", WLength(10, WLength::FontEx)},
    { "format", WLength(10, WLength::FontEx)},
    { "URL", WLength(30, WLength::FontEx)},
};


#define SESSION_COLUMN_NAME         0
#define SESSION_COLUMN_ADDRESS      1
#define SESSION_COLUMN_AGENT        2
static const column_decs session_columns[] = {
    { "session name", WLength(20, WLength::FontEx) },
    { "address", WLength(10, WLength::FontEx)},
    { "agent", WLength(10, WLength::FontEx)},
};

}


static WTableView * createTable(const column_decs columns[], int nb_columns)
{
  WTableView * table;
  WStandardItemModel * model;

  table = new WTableView();
  table->setModel(model = new WStandardItemModel(0, 0, table));
  model->insertColumns(0, nb_columns);

  table->setWidth(WLength(100, WLength::Percentage));
  table->setSortingEnabled(true);
  table->setColumnResizeEnabled(true);
  table->setAlternatingRowColors(true);

  for ( int i = 0, n = nb_columns; i < n; ++i ) {
    model->setHeaderData(i, Wt::Orientation::Horizontal, boost::any(WString::fromUTF8(columns[i].name)));
    table->setHeaderAlignment(i, Wt::AlignCenter);
    table->setColumnAlignment(i, Wt::AlignLeft);
    table->setColumnWidth(i, columns[i].width);
  }

  return table;
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



StreamListView::StreamListView(WContainerWidget * parent)
  : Base(parent)
{
  pageTemplate = new WTemplate(template_text, this);
  bindWidget(pageTemplate,"legend", &legend);
  bindWidget(pageTemplate,"selector", &selector, 0);
  bindWidget(pageTemplate,"refresh", &refresh, 0);
  bindWidget(pageTemplate,"stack", &stack);

  selector->addItem("Show Inputs");
  stack->addWidget(inputsTable = createTable(input_columns, count_of(input_columns)));

  selector->addItem("Show Outputs");
  stack->addWidget(outputsTable = createTable(output_columns, count_of(output_columns)));

  selector->addItem("Show Sinks");
  stack->addWidget(sinksTable = createTable(sink_columns, count_of(sink_columns)));

  selector->addItem("Show Sessions");
  stack->addWidget(sessionsTable = createTable(session_columns, count_of(session_columns)));

  selector->activated().connect(this, &StreamListView::selectorChanged);

  refresh->setText("Refresh...");
  refresh->clicked().connect(this, &StreamListView::refreshStreams);
}

StreamListView::~StreamListView()
{
}

void StreamListView::selectorChanged(int /*cursel*/) {
  refreshStreams();
}

void StreamListView::refreshStreams()
{
  switch (selector->currentIndex()) {
    case 0: /* show inputs */
    default:
      populateInputs();
    break;

    case 1: /* show outputs */
      populateOutputs();
    break;

    case 2: /* show sinks */
      populateSinks();
    break;

    case 3: /* show sessions */
      populateSessions();
    break;
  }
}

void StreamListView::populateInputs()
{
  WStandardItemModel * model;
  std::vector<MsmInput> inputs;
  size_t i, n;

  stack->setCurrentWidget(inputsTable);

  if ( !msmLoadInputs(&inputs) ) {
    setRowCount((WStandardItemModel*) inputsTable->model(), 0);
  }
  else {
    setRowCount(model = (WStandardItemModel*) inputsTable->model(), n = inputs.size());
    for ( i = 0; i < n; ++i ) {
      const MsmInput & input = inputs[i];
      model->setData(i, INPUT_COLUMN_NAME, boost::any(WString::fromUTF8(input.getName())));
      model->setData(i, INPUT_COLUMN_STATE, boost::any(WString::fromUTF8(msm_state_string(input.getState()))));
      model->setData(i, INPUT_COLUMN_ENABLED, boost::any(WString::fromUTF8(t2s(input.getEnabled()))));
      model->setData(i, INPUT_COLUMN_START_TIME, boost::any(WString::fromUTF8(input.getStartTime())));
    }
  }
}

void StreamListView::populateOutputs()
{
  WStandardItemModel * model;
  std::vector<MsmOutput> outputs;
  size_t i, n;

  stack->setCurrentWidget(outputsTable);

  if ( !msmLoadOutputs(&outputs) ) {
    setRowCount((WStandardItemModel*) outputsTable->model(), 0);
  }
  else {
    setRowCount(model = (WStandardItemModel*) outputsTable->model(), n = outputs.size());
    for ( i = 0; i < n; ++i ) {
      const MsmOutput & output = outputs[i];
      model->setData(i, OUTPUT_COLUMN_NAME, boost::any(WString("{1}/{2}").arg(output.getInputName()).arg(output.getName())));
      model->setData(i, OUTPUT_COLUMN_STATE, boost::any(WString(msm_state_string(output.getState()))));
      model->setData(i, OUTPUT_COLUMN_ENABLED, boost::any(WString(t2s(output.getEnabled()))));
    }
  }
}

void StreamListView::populateSinks()
{
  WStandardItemModel * model;
  std::vector<MsmSink> sinks;
  size_t i, n;

  stack->setCurrentWidget(sinksTable);

  if ( !msmLoadSinks(&sinks) ) {
    setRowCount((WStandardItemModel*) sinksTable->model(), 0);
  }
  else {
    setRowCount(model = (WStandardItemModel*) sinksTable->model(), n = sinks.size());
    for ( i = 0; i < n; ++i ) {
      const MsmSink & sink = sinks[i];
      model->setData(i, SINK_COLUMN_NAME, boost::any(WString("{1}/{2}/{3}")
          .arg(sink.getInputName())
          .arg(sink.getOutputName())
          .arg(sink.getName())));

      model->setData(i, SINK_COLUMN_STATE, boost::any(WString(msm_state_string(sink.getState()))));
      model->setData(i, SINK_COLUMN_FORMAT, boost::any(WString::fromUTF8(sink.getFormat())));
      model->setData(i, SINK_COLUMN_URL, boost::any(WString::fromUTF8(sink.getUrl())));
    }
  }
}

void StreamListView::populateSessions()
{
  WStandardItemModel * model;
  std::vector<MsmSession> sessions;
  size_t i, n;

  stack->setCurrentWidget(sessionsTable);

  if ( !msmLoadSessions(&sessions) ) {
    setRowCount((WStandardItemModel*) sessionsTable->model(), 0);
  }
  else {
    setRowCount(model = (WStandardItemModel*) sessionsTable->model(), n = sessions.size());
    for ( i = 0; i < n; ++i ) {
      const MsmSession & session = sessions[i];
      model->setData(i, SESSION_COLUMN_NAME, boost::any(WString::fromUTF8(session.getName())));
      model->setData(i, SESSION_COLUMN_ADDRESS, boost::any(WString::fromUTF8(session.getAddress())));
      model->setData(i, SESSION_COLUMN_AGENT, boost::any(WString::fromUTF8(session.getAgent())));
    }
  }
}

