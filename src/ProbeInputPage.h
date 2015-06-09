/*
 * ProbeInputPage.h
 *
 *  Created on: May 29, 2015
 *      Author: amyznikov
 */

#ifndef __probeinputpage_h__
#define __probeinputpage_h__

class ProbeInputPage
    : public WContainerWidget {
public:
  typedef WContainerWidget Base;
  ProbeInputPage(WContainerWidget * parent = 0);
  ~ProbeInputPage();
  void setInput(const MsmInput * input);

private:
  void onProbeNowClicked();

private:
  //WTable * table;
  bool doingProbeNow;
  WPushButton * probeNow;
  WText * inputInfo;
  WText * probeStatus;
  WString inputName;

};




#endif /* __probeinputpage_h__ */
