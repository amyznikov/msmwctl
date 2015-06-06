/*
 * SpecifyQualityAndMaxRate.h
 *
 *  Created on: Mar 29, 2015
 *      Author: amyznikov
 */

#ifndef __specifyqualityandmaxrate_h__
#define __specifyqualityandmaxrate_h__


class PropertyTable;
class SpecifyQualityAndMaxRate
    : public WContainerWidget {
public:
  typedef WContainerWidget Base;
  SpecifyQualityAndMaxRate(WContainerWidget * parent = 0);

  void setOutputProps(const MsmOutput * output);
  bool getChanges(MsmOutput * output);

private:
  PropertyTable * table;
  WLineEdit * Name;
  WLineEdit * Format;
  WLineEdit * Enabled;
  WLineEdit * VideoWidth;
  WLineEdit * VideoHeight;
  WLineEdit * VideoQuality;
  WLineEdit * MaxRate;
  WLineEdit * AudioCodec;
  WLineEdit * AudioCodecOpts;
  WLineEdit * SampleRate;
};


#endif /* __specifyqualityandmaxrate_h__ */
