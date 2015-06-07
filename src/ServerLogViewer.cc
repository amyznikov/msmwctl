/*
 * ServerLogViewer.cc
 *
 *  Created on: Apr 3, 2015
 *      Author: amyznikov
 */
#include "msmwctl.h"
#include "ServerLogViewer.h"

ServerLogViewer::ServerLogViewer(WContainerWidget * parent)
  : Base(parent)
{
  addStyleClass("eventlog");

  table = new WTable(this);
  table->setHeaderCount(1);

  table->elementAt(0, 0)->addWidget(new WText("Event"));
  table->elementAt(0, 0)->setWidth("20%");
  table->elementAt(0, 1)->addWidget(new WText("Details"));
  table->elementAt(0, 1)->setWidth("80%");

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
    WServer::instance()->post(sessionId,
        boost::bind(&ServerLogViewer::updateLog, this, WString(event->name), WString(event->severity),
            WString(event->params)));
  }
}


void ServerLogViewer::updateLog(const WString & eventName, const WString & severity, const WString & details)
{
  const int r = table->rowCount();

  table->insertRow(1);
  table->rowAt(1)->addStyleClass(severity);
  table->elementAt(1, 0)->addWidget(new WText(eventName));
  table->elementAt(1, 1)->addWidget(new WText(details));

//  table->elementAt(1, 0)->addWidget(new WText(eventName));
//  table->elementAt(1, 1)->addWidget(new WText(details));

  WApplication::instance()->triggerUpdate();
}
