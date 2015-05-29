/*
 * PropertyPagesContainer.cc
 *
 *  Created on: Mar 30, 2015
 *      Author: amyznikov
 */

#include "msmwctl.h"
#include "PropertyPagesContainer.h"

static const char template_text [] =
    "<fieldset>"
    " <legend>${legend}</legend>"
    "   ${selector}"
    "   <p>${stack}</p>"
    "   <p>${save}</p>"
    "</fieldset>";


PropertyPagesContainer::PropertyPagesContainer(WContainerWidget * parent)
  : Base(parent)
{
  pageTemplate_ = new WTemplate(template_text, this);
  // pageTemplate_->decorationStyle().setBackgroundColor(Wt::lightGray);
  pageTemplate_->setMargin(WLength(0, WLength::Pixel));
  pageTemplate_->setOffsets(WLength(0, WLength::Pixel));


  bindWidget(pageTemplate_,"legend", &legend_);
  bindWidget(pageTemplate_,"selector", &selector_, 0);
  bindWidget(pageTemplate_,"stack", &stack_);
  bindWidget(pageTemplate_,"save", &save_, 0);


  //  this->decorationStyle().setBackgroundColor(Wt::cyan);
  this->setMargin(WLength(0, WLength::Pixel));
  this->setOffsets(WLength(0, WLength::Pixel));
  this->setPadding(WLength(0, WLength::Pixel));

  save_->setText("Save Changes...");
  selector_->hide();

  save_->clicked().connect(this, &PropertyPagesContainer::onSaveChanges);
  selector_->activated().connect(this, &PropertyPagesContainer::selectorChanged);
}


void PropertyPagesContainer::showSaveChanges(bool fshow) {
  if ( fshow ) {
    save_->show();
  }
  else if ( !fshow ) {
    save_->hide();
  }
}


void PropertyPagesContainer::selectorChanged(int curSel)
{
  WWidget * prev, * current;
  prev = stack_->currentWidget();
  stack_->setCurrentIndex(curSel);
  current = stack_->currentWidget();
  if ( prev != current ) {
    onPageSelectionChanged(prev, current);
  }
}

void PropertyPagesContainer::onPageSelectionChanged(WWidget * /*prev*/, WWidget * /*curent*/) {
  // do nothing: must be overriden
}

void PropertyPagesContainer::onSaveChanges() {
  // do nothing: must be overriden
}

void PropertyPagesContainer::setLegend(const WString & legend) {
  legend_->setText(legend);
}

void PropertyPagesContainer::addPage(const WString & label, WWidget * widget)
{
  selector_->addItem(label);
  stack_->addWidget(widget);
  if ( !selector_->isVisible() ) {
    selector_->show();
  }
}

WWidget * PropertyPagesContainer::getCurrentPage() {
  return stack_->currentWidget();
}

int PropertyPagesContainer::getCurrentPageIndex() {
  return stack_->currentIndex();
}

void PropertyPagesContainer::setCurrentPage(WWidget * widget) {
  stack_->setCurrentWidget(widget);
  selector_->setCurrentIndex(stack_->currentIndex());
}
