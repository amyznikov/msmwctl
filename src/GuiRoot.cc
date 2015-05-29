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
  this->decorationStyle().setBackgroundColor(WColor("#CCFFCC"));
  this->setMargin(WLength(0, WLength::Pixel));
  this->setOffsets(WLength(0, WLength::Pixel));
  this->setPadding(WLength(0, WLength::Pixel));

  nav = new Wt::WNavigationBar(this);
  nav->setTitle("MSM2 Configuration Utility");
  nav->setResponsive(true);

  stack = new WStackedWidget(this);
  stack->addStyleClass("contents");
  // stack->decorationStyle().setBackgroundColor(Wt::red);
  stack->setMargin(WLength(0, WLength::Pixel));
  stack->setOffsets(WLength(0, WLength::Pixel));
  stack->setPadding(WLength(0, WLength::Pixel));


  nav->addMenu(menu = new WMenu(stack, this));
  menu->addItem("Manage streams", mainForm = new MainForm());
  menu->addItem("Server Events", logView = new ServerLogViewer());

  mainForm->decorationStyle().setBackgroundColor(Wt::white);

}
