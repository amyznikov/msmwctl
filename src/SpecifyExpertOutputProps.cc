/*
 * OutputPropsExpert.cc
 *
 *  Created on: Mar 29, 2015
 *      Author: amyznikov
 */

#include "msmwctl.h"
#include "SpecifyExpertOutputProps.h"



SpecifyExpertOutputProps::SpecifyExpertOutputProps(WContainerWidget * parent)
    : Base(parent)
{
  WVBoxLayout * vbox = new WVBoxLayout(this);
  vbox->setContentsMargins(0, 0, 0, 0);
  vbox->addWidget(table = new WTable(), 1, Wt::AlignTop);

  table->setMargin(0);
  table->setWidth(WLength(95,WLength::Percentage));
  addwidget(table, "Name", &Name);
  addwidget(table, "Format", &Format);
  addwidget(table, "CtxOpts", &CtxOpts);
  addwidget(table, "GopSize", &GopSize);
  addwidget(table, "Pps", &Pps);
  addwidget(table, "Eps", &Eps);
  addwidget(table, "Enabled", &Enabled);
  addwidget(table, "Maxqv", &Maxqv);
  addwidget(table, "SBuf", &SBuf);
  addwidget(table, "PBuf", &PBuf);
  addwidget(table, "STmo", &STmo);
  addwidget(table, "StartOnKey",&StartOnKey);
  addwidget(table, "VideoCodec", &VideoCodec);
  addwidget(table, "VideoCodecOpts", &VideoCodecOpts);
  addwidget(table, "VideoWidth",&VideoWidth);
  addwidget(table, "VideoHeight", &VideoHeight);
  addwidget(table, "AudioCodec", &AudioCodec);
  addwidget(table, "AudioCodecOpts", &AudioCodecOpts);
  addwidget(table, "SampleRate", &SampleRate);
}


void SpecifyExpertOutputProps::setOutputProps(const MsmOutput * output)
{
  if ( output ) {
    Name->setText(output->getName());
    Format->setText(output->getFormat());
    CtxOpts->setText(output->getCtxOpts());
    GopSize->setText(t2s(output->getGopSize()));
    Pps->setText(t2s(output->getPps()));
    Eps->setText(t2s(output->getEps()));
    Enabled->setText(t2s(output->getEnabled()));
    Maxqv->setText(t2s(output->getMaxqv()));
    SBuf->setText(t2s(output->getSBuf()));
    PBuf->setText(t2s(output->getPBuf()));
    STmo->setText(t2s(output->getSTmo()));
    StartOnKey->setText(t2s(output->getStartOnKey()));
    VideoCodec->setText(output->getVideoCodec());
    VideoCodecOpts->setText(output->getVideoOpts());
    VideoWidth->setText(t2s(output->getVideoWidth()));
    VideoHeight->setText(t2s(output->getVideoHeight()));
    AudioCodec->setText(output->getAudioCodec());
    AudioCodecOpts->setText(output->getAudioOpts());
    SampleRate->setText(t2s(output->getAudioSampleRate()));
  }
}


bool SpecifyExpertOutputProps::getChanges(MsmOutput * output)
{
  output->setName(Name->text().toUTF8().c_str());

  if ( !GopSize->text().empty() && !convert(GopSize->text(), output, &MsmOutput::setGopSize) ) {
    WMessageBox::show("Syntax Error:", WString("Invalid value: GopSize={1}").arg(GopSize->text()), Wt::Ok);
    return false;
  }

  if ( !Pps->text().empty() && !convert(Pps->text(), output, &MsmOutput::setPps) ) {
    WMessageBox::show("Syntax Error:", WString("Invalid value: Pps={1}").arg(Pps->text()), Wt::Ok);
    return false;
  }

  if ( !Eps->text().empty() && !convert(Eps->text(), output, &MsmOutput::setEps) ) {
    WMessageBox::show("Syntax Error:", WString("Invalid value: Eps={1}").arg(Eps->text()), Wt::Ok);
    return false;
  }

  if ( !Enabled->text().empty() && !convert(Enabled->text(), output, &MsmOutput::setEnabled) ) {
    WMessageBox::show("Syntax Error:", WString("Invalid value: Enabled={1}").arg(Enabled->text()), Wt::Ok);
    return false;
  }

  if ( !Maxqv->text().empty() && !convert(Maxqv->text(), output, &MsmOutput::setMaxqv) ) {
    WMessageBox::show("Syntax Error:", WString("Invalid value: Maxqv={1}").arg(Maxqv->text()), Wt::Ok);
    return false;
  }

  if ( !SBuf->text().empty() && !convert(SBuf->text(), output, &MsmOutput::setSBuf) ) {
    WMessageBox::show("Syntax Error:", WString("Invalid value: SBuf={1}").arg(SBuf->text()), Wt::Ok);
    return false;
  }

  if ( !PBuf->text().empty() && !convert(PBuf->text(), output, &MsmOutput::setPBuf) ) {
    WMessageBox::show("Syntax Error:", WString("Invalid value: PBuf={1}").arg(PBuf->text()), Wt::Ok);
    return false;
  }

  if ( !STmo->text().empty() && !convert(STmo->text(), output, &MsmOutput::setSTmo) ) {
    WMessageBox::show("Syntax Error:", WString("Invalid value: STmo={1}").arg(STmo->text()), Wt::Ok);
    return false;
  }

  if ( !StartOnKey->text().empty() && !convert(StartOnKey->text(), output, &MsmOutput::setStartOnKey) ) {
    WMessageBox::show("Syntax Error:", WString("Invalid value: StartOnKey={1}").arg(StartOnKey->text()), Wt::Ok);
    return false;
  }

  if ( !VideoWidth->text().empty() && !convert(VideoWidth->text(), output, &MsmOutput::setVideoWidth) ) {
    WMessageBox::show("Syntax Error:", WString("Invalid value: VideoWidth={1}").arg(VideoWidth->text()), Wt::Ok);
    return false;
  }

  if ( !VideoHeight->text().empty() && !convert(VideoHeight->text(), output, &MsmOutput::setVideoHeight) ) {
    WMessageBox::show("Syntax Error:", WString("Invalid value: VideoHeight={1}").arg(VideoHeight->text()), Wt::Ok);
    return false;
  }

  if ( !SampleRate->text().empty() && !convert(SampleRate->text(), output, &MsmOutput::setAudioSampleRate) ) {
    WMessageBox::show("Syntax Error:", WString("Invalid value: SampleRate={1}").arg(SampleRate->text()), Wt::Ok);
    return false;
  }

  output->setFormat(Format->text().toUTF8().c_str());
  output->setCtxOpts(CtxOpts->text().toUTF8().c_str());
  output->setVideoCodec(VideoCodec->text().toUTF8().c_str());
  output->setVideoOpts(VideoCodecOpts->text().toUTF8().c_str());
  output->setAudioCodec(AudioCodec->text().toUTF8().c_str());
  output->setAudioOpts(AudioCodecOpts->text().toUTF8().c_str());

  return true;
}

