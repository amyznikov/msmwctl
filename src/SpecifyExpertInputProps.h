/*
 * SpecifyExpertInputProps.h
 *
 *  Created on: Mar 29, 2015
 *      Author: amyznikov
 */

#ifndef __SpecifyExpertInputProps_h__
#define __SpecifyExpertInputProps_h__

class PropertyTable;
class SpecifyExpertInputProps
    : public WContainerWidget {
public:
  typedef WContainerWidget Base;
  SpecifyExpertInputProps(WContainerWidget * parent = 0);

  void setInputProps(const MsmInput * input);
  bool getChanges(MsmInput * input);

private:
  PropertyTable * table;
  WLineEdit * Name;
  WLineEdit * Source;
  WLineEdit * Format;
  WLineEdit * Enabled;
  WLineEdit * CtxOpts;
  WLineEdit * DecOpts;
  WLineEdit * Live;
  WLineEdit * IgnoreErrors;
  WLineEdit * FastProbe;
  WLineEdit * GenPts;
  WLineEdit * RE;
};



#endif /* __SpecifyExpertInputProps_h__ */
