/*
 * SpecifySinkProps.cc
 *
 *  Created on: Mar 30, 2015
 *      Author: amyznikov
 */

#include "msmwctl.h"
#include "SpecifySinkProps.h"


SpecifySinkProps :: SpecifySinkProps(WContainerWidget * parent)
  : Base(parent)
{
  WVBoxLayout * vbox = new WVBoxLayout(this);
  vbox->setContentsMargins(0,0,0,0);
  vbox->addWidget(table = new WTable(), 1, Wt::AlignTop);

  table->setMargin(0);
  table->setWidth(WLength(95,WLength::Percentage));
  addwidget(table, "Name", &Name);
  addwidget(table, "Url", &Url);
  addwidget(table, "Format", &Format);
}

void SpecifySinkProps :: setSinkProps(const MsmSink * sink)
{
  if ( sink ) {
    Name->setText(sink->getName());
    Url->setText(sink->getUrl());
    Format->setText(sink->getFormat());
  }
}

bool SpecifySinkProps :: getChanges(MsmSink * sink)
{
  sink->setName(Name->text().toUTF8().c_str());
  sink->setFormat(Format->text().toUTF8().c_str());
  sink->setUrl(Url->text().toUTF8().c_str());
  return true;
}



