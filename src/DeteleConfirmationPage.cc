/*
 * DeteleConfirmationPage.cc
 *
 *  Created on: Apr 1, 2015
 *      Author: amyznikov
 */

#include "msmwctl.h"
#include "DeteleConfirmationPage.h"

static const char template_text[] =
    "<p>"
    " <H3>"
    "   ${message}"
    " </H3>"
    "</p>"
    "<p>"
    " ${confirm} ${cancel}"
    "</p>"
    "<p>"
    "${force}"
    "</p>"
    "";


DeteleConfirmationPage::DeteleConfirmationPage(WContainerWidget * parent)
  : Base(parent)
{
  addStyleClass("confirm-delete");
  pageTemplate = new WTemplate(template_text, this);
  bindWidget(pageTemplate,"message", &message_);
  bindWidget(pageTemplate,"confirm", &confirm_, 0);
  bindWidget(pageTemplate,"cancel", &cancel_, 0);

  //bindWidget(pageTemplate,"force", &force_, 0);
  // Workaround Wt bug....
  force_ = new WCheckBox("Force");
  pageTemplate->bindWidget("force", force_);

  confirm_->setText("YES");
  confirm_->addStyleClass("btn-danger");
  confirm_->clicked().connect(this, &DeteleConfirmationPage::onConfirmClicked);

  cancel_->setText("NO");
  cancel_->addStyleClass("btn-primary");
  cancel_->clicked().connect(this, &DeteleConfirmationPage::onCancelClicked);
}

void DeteleConfirmationPage::setWarningMessage(const WString & msg) {
  message_->setText(msg);
}

void DeteleConfirmationPage::showButtons(bool fshow)
{
  if ( fshow ) {
    force_->show();
    confirm_->show();
    cancel_->show();
  }
  else {
    force_->hide();
    confirm_->hide();
    cancel_->hide();
  }
}

Signal<bool> & DeteleConfirmationPage::confirmed() {
  return confirmed_;
}

Signal<> & DeteleConfirmationPage::cancelled() {
  return cancelled_;
}

void DeteleConfirmationPage::onConfirmClicked() {
  confirmed_.emit(force_->isVisible() && force_->isChecked());
}

void DeteleConfirmationPage::onCancelClicked() {
  cancelled_.emit();
}
