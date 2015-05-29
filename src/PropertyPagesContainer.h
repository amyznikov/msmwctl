/*
 * PropertyPagesContainer.h
 *
 *  Created on: Mar 30, 2015
 *      Author: amyznikov
 */

#ifndef __PropertyPagesContainer_h__
#define __PropertyPagesContainer_h__

class PropertyPagesContainer
  : public WContainerWidget {
public:
  typedef WContainerWidget Base;
  PropertyPagesContainer(WContainerWidget * parent = 0);

  void setLegend(const WString & legend);
  void addPage(const WString & label, WWidget * widget);

  WWidget * getCurrentPage();
  void setCurrentPage(WWidget * widget);

  int getCurrentPageIndex();

  void showSaveChanges(bool fshow);

protected:
  virtual void onPageSelectionChanged(WWidget * prev, WWidget * curent);
  virtual void onSaveChanges();

private:
  WTemplate * pageTemplate_;
  WText * legend_;
  WComboBox * selector_;
  WStackedWidget * stack_;
  WPushButton * save_;
  void selectorChanged(int curSel);
};

#endif /* __PropertyPagesContainer_h__ */
