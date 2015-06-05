/*
 * GuiRoot.cc
 *
 *  Created on: Apr 3, 2015
 *      Author: amyznikov
 */
#include "msmwctl.h"
#include "GuiRoot.h"
#include "MainForm.h"
#include "ServerLogViewer.h"


GuiRoot::GuiRoot(WContainerWidget * parent)
  : Base(parent)
{
  /*
   * bgnd: 0097a7
   * text: b3e0e5
   *
   */
  nav = new Wt::WNavigationBar(this);
  nav->setTitle("MSM2 configuration tool");
  nav->setResponsive(true);

  stack = new WStackedWidget(this);
  stack->addStyleClass("contents");

  nav->addMenu(menu = new WMenu(stack, this));
  menu->addItem("Manage streams", mainForm = new MainForm());
  menu->addItem("Server Events", logView = new ServerLogViewer());
}
