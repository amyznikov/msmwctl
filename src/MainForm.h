/*
 * MainForm.h
 *
 *  Created on: Mar 27, 2015
 *      Author: amyznikov
 */

#ifndef __main_form_h__
#define __main_form_h__

class Footer;
class LeftPane;
class RightPane;

class MainForm
  : public WContainerWidget
{
  typedef WContainerWidget
      Base;

public:
  MainForm(WContainerWidget * parent = 0);

private:

private:
  WHBoxLayout * hbox;
  LeftPane * leftPane;
  RightPane * rightPane;
};




#endif /* __main_form_h__ */
