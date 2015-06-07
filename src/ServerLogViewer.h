/*
 * ServerLogViewer.h
 *
 *  Created on: Apr 3, 2015
 *      Author: amyznikov
 */

#ifndef __ServerLogViewer_h__
#define __ServerLogViewer_h__

class ServerLogViewer
  : public WContainerWidget,
    msm_event_listener
{
  typedef WContainerWidget
      Base;
public:
  ServerLogViewer(WContainerWidget * parent = 0);
  ~ServerLogViewer();

private:
  void on_msm_event(const struct msm_event * event);
  void updateLog(const WString & eventName, const WString & severity, const WString & details);


private:
  WTable * table;
  std::string sessionId;
  WApplication * app;
};




#endif /* __ServerLogViewer_h__ */
