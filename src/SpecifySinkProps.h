/*
 * SpecifySinkProps.h
 *
 *  Created on: Mar 30, 2015
 *      Author: amyznikov
 */

#ifndef __SpecifySinkProps_h__
#define __SpecifySinkProps_h__


class PropertyTable;
class SpecifySinkProps
    : public WContainerWidget {
public:
  typedef WContainerWidget Base;
  SpecifySinkProps(WContainerWidget * parent = 0);

  void setSinkProps(const MsmSink * sink);
  bool getChanges(MsmSink * sink);

private:
  PropertyTable * table;
  WLineEdit * Name;
  WLineEdit * Url;
  WLineEdit * Format;
};




#endif /* __SpecifySinkProps_h__ */
