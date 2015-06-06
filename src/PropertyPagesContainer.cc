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

  bindWidget(pageTemplate_,"legend", &legend_);
  bindWidget(pageTemplate_,"selector", &selector_, 0);
  bindWidget(pageTemplate_,"stack", &stack_);
  bindWidget(pageTemplate_,"save", &save_, 0);

  selector_->addStyleClass("btn-primary dropdown-toggle page_selector");
  selector_->hide();

  selector_->setMenu(menu_ = new WPopupMenu());
  menu_->itemSelected().connect(this, &PropertyPagesContainer::selectorChanged);


  save_->setText("Save Changes...");
  save_->clicked().connect(this, &PropertyPagesContainer::onSaveChanges);
}


void PropertyPagesContainer::showSaveChanges(bool fshow) {
  if ( fshow ) {
    save_->show();
  }
  else if ( !fshow ) {
    save_->hide();
  }
}


//void PropertyPagesContainer::selectorChanged(int curSel)
//{
//  WWidget * prev, * current;
//  prev = stack_->currentWidget();
//  stack_->setCurrentIndex(curSel);
//  current = stack_->currentWidget();
//  if ( prev != current ) {
//    onPageSelectionChanged(prev, current);
//  }
//}

void PropertyPagesContainer::selectorChanged(WMenuItem * item)
{
  int index = (int) (ssize_t) item->data();
  if ( index >= 0 ) {
    WWidget * prev, * current;
    prev = stack_->currentWidget();
    stack_->setCurrentIndex(index);
    current = stack_->currentWidget();
    if ( prev != current ) {
      selector_->setText(item->text());
      onPageSelectionChanged(prev, current);
    }
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
  stack_->addWidget(widget);
  (menu_->addItem(label))->setData((void*) (ssize_t) stack_->indexOf(widget));
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
  WMenuItem * item = menu_->itemAt(stack_->currentIndex());
  if ( item ) {
    selector_->setText(item->text());
  }
}
