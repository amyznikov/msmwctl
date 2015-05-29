/*
 * SpecifyInputProps.cc
 *
 *  Created on: Mar 29, 2015
 *      Author: amyznikov
 */

#include "msmwctl.h"
#include "SpecifyExpertInputProps.h"


SpecifyExpertInputProps::SpecifyExpertInputProps(WContainerWidget * parent)
    : Base(parent)
{
  WVBoxLayout * vbox = new WVBoxLayout(this);
  vbox->setContentsMargins(0, 0, 0, 0);
  vbox->addWidget(table = new WTable(), 1, Wt::AlignTop);

  table->setMargin(0);
  table->setWidth(WLength(90,WLength::Percentage));
  addwidget(table, "Name", &Name);
  addwidget(table, "Source", &Source);
  addwidget(table, "Format", &Format);
  addwidget(table, "Enabled", &Enabled);
  addwidget(table, "CtxOpts", &CtxOpts);
  addwidget(table, "DecOpts", &DecOpts);
  addwidget(table, "Live", &Live);
  addwidget(table, "IgnoreErrors", &IgnoreErrors);
  addwidget(table, "FastProbe", &FastProbe);
  addwidget(table, "GenPts", &GenPts);
  addwidget(table, "RE", &RE);

  // table->enterPressed().connect(this, &SpecifyExpertInputProps::onSaveChanges);
}

void SpecifyExpertInputProps::setInputProps(const MsmInput * input)
{
  if ( input ) {
    Name->setText(input->getName());
    Source->setText(input->getSource());
    Format->setText(input->getFormat());
    Enabled->setText(t2s(input->getEnabled()));
    CtxOpts->setText(input->getCtxOpts());
    DecOpts->setText(input->getDecOpts());
    Live->setText(t2s(input->getLive()));
    IgnoreErrors->setText(t2s(input->getIgnoreErrors()));
    FastProbe->setText(t2s(input->getFastProbe()));
    GenPts->setText(t2s(input->getGenPts()));
    RE->setText(t2s(input->getRateEmulationReferenceStream()));
  }
  else {
    Name->setText("");
    Source->setText("");
    Format->setText("");
    Enabled->setText("");
    CtxOpts->setText("");
    DecOpts->setText("");
    Live->setText("");
    IgnoreErrors->setText("");
    FastProbe->setText("");
    GenPts->setText("");
    RE->setText("");
  }
}


bool SpecifyExpertInputProps::getChanges(MsmInput * input)
{
  input->setName(Name->text().toUTF8().c_str());

  if ( !Live->text().empty() && !convert(Live->text(), input, &MsmInput::setLive) ) {
    WMessageBox::show("Syntax Error:", WString("Invalid value: Live={1}").arg(Live->text()), Wt::Ok);
    return false;
  }

  if ( !Enabled->text().empty() && !convert(Enabled->text(), input, &MsmInput::setEnabled) ) {
    WMessageBox::show("Syntax Error:", WString("Invalid value: Enabled={1}").arg(Enabled->text()), Wt::Ok);
    return false;
  }

  if ( !IgnoreErrors->text().empty() && !convert(IgnoreErrors->text(), input, &MsmInput::setIgnoreErrors) ) {
    WMessageBox::show("Syntax Error:", WString("Invalid value: IgnoreErrors={1}").arg(IgnoreErrors->text()), Wt::Ok);
    return false;
  }

  if ( !FastProbe->text().empty() && !convert(FastProbe->text(), input, &MsmInput::setFastProbe) ) {
    WMessageBox::show("Syntax Error:", WString("Invalid value: FastProbe={1}").arg(FastProbe->text()), Wt::Ok);
    return false;
  }

  if ( !RE->text().empty() && !convert(RE->text(), input, &MsmInput::setRateEmulationReferenceStream) ) {
    WMessageBox::show("Syntax Error:", WString("Invalid value: RE={1}").arg(RE->text()), Wt::Ok);
    return false;
  }

  input->setFormat(Format->text().toUTF8().c_str());
  input->setSource(Source->text().toUTF8().c_str());
  input->setCtxOpts(CtxOpts->text().toUTF8().c_str());
  input->setDecOpts(DecOpts->text().toUTF8().c_str());

  return true;
}

