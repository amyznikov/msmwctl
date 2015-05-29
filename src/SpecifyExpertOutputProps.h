/*
 * SpecifyExpertOutputProps.h
 *
 *  Created on: Mar 29, 2015
 *      Author: amyznikov
 */

#ifndef __SpecifyExpertOutputProps_h__
#define __SpecifyExpertOutputProps_h__

class SpecifyExpertOutputProps
    : public WContainerWidget {
public:

  typedef WContainerWidget Base;
  SpecifyExpertOutputProps(WContainerWidget * parent = 0);

  void setOutputProps(const MsmOutput * output);
  bool getChanges(MsmOutput * output);

private:
  WTable * table;
  WLineEdit * Name;
  WLineEdit * Format;
  WLineEdit * CtxOpts;
  WLineEdit * GopSize;
  WLineEdit * Pps;
  WLineEdit * Eps;
  WLineEdit * Enabled;
  WLineEdit * Maxqv;
  WLineEdit * SBuf;
  WLineEdit * PBuf;
  WLineEdit * STmo;
  WLineEdit * StartOnKey;
  WLineEdit * VideoCodec;
  WLineEdit * VideoCodecOpts;
  WLineEdit * VideoWidth;
  WLineEdit * VideoHeight;
  WLineEdit * AudioCodec;
  WLineEdit * AudioCodecOpts;
  WLineEdit * SampleRate;
};


#endif /* __SpecifyExpertOutputProps_h__ */
