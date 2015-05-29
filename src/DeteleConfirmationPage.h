/*
 * DeteleConfirmationPage.h
 *
 *  Created on: Apr 1, 2015
 *      Author: amyznikov
 */

#ifndef __DeteleConfirmationPage_h__
#define __DeteleConfirmationPage_h__


class DeteleConfirmationPage
    : public WContainerWidget {
public:
  typedef WContainerWidget Base;
  DeteleConfirmationPage(WContainerWidget * parent = 0);

  void setWarningMessage(const WString & msg);
  void showButtons(bool fshow);

  Signal<> & cancelled();
  Signal<bool> & confirmed();

private:
  void onConfirmClicked();
  void onCancelClicked();

private:
  WTemplate * pageTemplate;
  WText * message_;
  WPushButton * confirm_;
  WPushButton * cancel_;
  WCheckBox * force_;
  Signal<> cancelled_;
  Signal<bool> confirmed_;
};

#endif /* __DeteleConfirmationPage_h__ */
