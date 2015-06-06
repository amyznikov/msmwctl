/*
 * PropertyTable.h
 *
 *  Created on: Jun 6, 2015
 *      Author: amyznikov
 */

#ifndef __propertytable_h__
#define __propertytable_h__


class PropertyTable
  : public WTable
{
public:
  typedef WTable Base;
  PropertyTable(WContainerWidget *parent = 0);

  template<class T>
  void add(const char * legend, T ** widget) {
    const int row = rowCount();
    WLabel * label = new WLabel(legend, elementAt(row, 0));
    label->setBuddy((*widget) = new T(elementAt(row, 1)));
    elementAt(row, 0)->setWidth(WLength(25, WLength::Percentage));
    elementAt(row, 1)->setWidth(WLength(70, WLength::Percentage));
  }

};



#endif /* __propertytable_h__ */
