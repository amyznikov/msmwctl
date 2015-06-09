/*
 * ServerLogViewer.cc
 *
 *  Created on: Apr 3, 2015
 *      Author: amyznikov
 */
#include "msmwctl.h"
#include "ServerLogViewer.h"
#include <time.h>

static inline WString gents()
{
  time_t t;
  struct tm * tm;
  char bfr[256];

  t = time(0);
  tm = localtime(&t);

  sprintf(bfr, "%.2d/%.2d/%.4d %.2d:%.2d:%.2d", tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900, tm->tm_hour,
      tm->tm_min, tm->tm_sec);

  return bfr;
}


ServerLogViewer::ServerLogViewer(WContainerWidget * parent)
  : Base(parent)
{
  addStyleClass("eventlog");

  table = new WTable(this);
  table->setHeaderCount(1);

  table->elementAt(0, 0)->addWidget(new WText("TS"));
  table->elementAt(0, 0)->setWidth("15%");
  table->elementAt(0, 1)->addWidget(new WText("Event"));
  table->elementAt(0, 1)->setWidth("15%");
  table->elementAt(0, 2)->addWidget(new WText("Details"));
  table->elementAt(0, 2)->setWidth("70%");

  sessionId = (app = WApplication::instance())->sessionId();

  msmRegisterEventListener(this);

}

ServerLogViewer :: ~ServerLogViewer()
{
  msmUnRegisterEventListener(this);
}

void ServerLogViewer::on_msm_event(const struct msm_event * event)
{
  if ( event->name && strcmp(event->name,"event-timer") != 0 ) {
    WServer::instance()->schedule(10, sessionId,
        boost::bind(&ServerLogViewer::updateLog, this, gents(), WString(event->name), WString(event->severity),
            WString(event->params)));
  }

}


void ServerLogViewer::updateLog(const WString & eventTs, const WString & eventName, const WString & severity,
    const WString & details)
{
  const int r = table->rowCount();

  if ( r > 30 ) {
    table->deleteRow(r - 1);
  }

  table->insertRow(1);
  table->rowAt(1)->addStyleClass(severity);
  table->elementAt(1, 0)->addWidget(new WText(eventTs));
  table->elementAt(1, 1)->addWidget(new WText(eventName));
  table->elementAt(1, 2)->addWidget(new WText(details));

  WApplication::instance()->triggerUpdate();
}
