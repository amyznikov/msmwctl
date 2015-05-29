/*
 * GuiRoot.h
 *
 *  Created on: Apr 3, 2015
 *      Author: amyznikov
 */

#ifndef __GuiRoot_h__
#define __GuiRoot_h__


class MainForm;
class ServerLogViewer;

class GuiRoot
  : public WContainerWidget {
public:

  typedef WContainerWidget
      Base;

  GuiRoot(WContainerWidget * parent = 0);

private:
  WNavigationBar * nav;
  WMenu * menu;
  WStackedWidget * stack;
  MainForm * mainForm;
  ServerLogViewer * logView;
};





#endif /* __GuiRoot_h__ */
