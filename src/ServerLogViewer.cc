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
  decorationStyle().setBackgroundColor(Wt::white);

  table = new WTable(this);
  table->setHeaderCount(1);
  table->setWidth(WLength("100%"));

  table->elementAt(0, 0)->addWidget(new WText("Event"));
  table->elementAt(0, 1)->addWidget(new WText("Details"));

  sessionId = (app = WApplication::instance())->sessionId();

  fprintf(stderr,"******** msmRegisterEventListener(this=%p) sessionId=%s\n", this, sessionId.c_str());
  msmRegisterEventListener(this);

}

ServerLogViewer :: ~ServerLogViewer()
{
  fprintf(stderr,"******** msmUnRegisterEventListener(this=%p)\n", this);
  msmUnRegisterEventListener(this);
}

void ServerLogViewer::on_msm_event(const struct msm_event * event)
{
  fprintf(stderr, "******** on_msm_event(this=%p) enter tid=%ld sessionId=%s app=%p event=%s\n", this, pthread_self(),
	  sessionId.c_str(), app, event->name );

  if ( event->name && strcmp(event->name,"event-timer") != 0 ) {
    WServer::instance()->post(sessionId,
        boost::bind(&ServerLogViewer::updateLog, this, WString(event->name), WString(event->params)));
  }
}


void ServerLogViewer::updateLog(const WString & eventName, const WString & details)
{
  const int r = table->rowCount();

  fprintf(stderr, "******** updateLog: r = %d tid=%ld\n", r, pthread_self());

  table->insertRow(1);
  table->elementAt(1, 0)->addWidget(new WText(eventName));

  fprintf(stderr, "******** timeout: 2 tid=%ld\n", pthread_self());

  table->elementAt(1, 1)->addWidget(new WText(details));

  fprintf(stderr, "******** timeout: 3 tid=%ld\n", pthread_self());

  WApplication::instance()->triggerUpdate();
}
