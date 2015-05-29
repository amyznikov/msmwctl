/*
 * StreamListView.h
 *
 *  Created on: Apr 4, 2015
 *      Author: amyznikov
 */

#ifndef __StreamListView_h__
#define __StreamListView_h__

class StreamListView
  : public WContainerWidget
{
public:
  typedef WContainerWidget
      Base;

  StreamListView(WContainerWidget * parent = 0);
  ~StreamListView();
  void refreshStreams();

private:
  void selectorChanged(int curSel);
  void populateInputs();
  void populateOutputs();
  void populateSinks();
  void populateSessions();

private:
  WTemplate * pageTemplate;
  WText * legend;
  WComboBox * selector;
  WPushButton * refresh;
  WStackedWidget * stack;
  WTableView * inputsTable;
  WTableView * outputsTable;
  WTableView * sinksTable;
  WTableView * sessionsTable;
};




#endif /* __StreamListView_h__ */
