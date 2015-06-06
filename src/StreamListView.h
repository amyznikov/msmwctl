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
  void refreshStreams();


private:
  WTabWidget * tab;
  WWidget * inputs;
  WWidget * outputs;
  WWidget * sinks;
  WWidget * sessions;
};




#endif /* __StreamListView_h__ */
