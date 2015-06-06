/*
 * SpecifyDynamicAndStaticQuality.h
 *
 *  Created on: Mar 29, 2015
 *      Author: amyznikov
 */

#ifndef __SpecifyDynamicAndStaticQuality_h__
#define __SpecifyDynamicAndStaticQuality_h__

class PropertyTable;
class SpecifyDynamicAndStaticQuality
    : public WContainerWidget {
public:
  typedef WContainerWidget Base;
  SpecifyDynamicAndStaticQuality(WContainerWidget * parent = 0);

  void setOutputProps(const MsmOutput * output);
  bool getChanges(MsmOutput * output);

private:
  PropertyTable * table;
  WLineEdit * Name;
  WLineEdit * Format;
  WLineEdit * Enabled;
  WLineEdit * VideoWidth;
  WLineEdit * VideoHeight;
  WLineEdit * StaticQuality;
  WLineEdit * DynamicQuality;
  WLineEdit * MotionThreshold;
  WLineEdit * BgWeight;
  WLineEdit * AudioCodec;
  WLineEdit * AudioCodecOpts;
  WLineEdit * SampleRate;
};





#endif /* __SpecifyDynamicAndStaticQuality_h__ */
