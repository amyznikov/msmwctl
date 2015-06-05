/*
 * SpecifyQualityAndMaxRate.cc
 *
 *  Created on: Mar 29, 2015
 *      Author: amyznikov
 */

#include "msmwctl.h"
#include "SpecifyQualityAndMaxRate.h"

SpecifyQualityAndMaxRate::SpecifyQualityAndMaxRate(WContainerWidget * parent)
    : Base(parent)
{
  WVBoxLayout * vbox = new WVBoxLayout(this);
  vbox->addWidget(table = new WTable());
  vbox->addSpacing(WLength(4, WLength::FontEx));

  table->addStyleClass("prop_table");
  addwidget(table, "Name", &Name);
  addwidget(table, "Format", &Format);
  addwidget(table, "Enabled", &Enabled);
  addwidget(table, "VideoWidth",&VideoWidth);
  addwidget(table, "VideoHeight", &VideoHeight);
  addwidget(table, "VideoQuality", &VideoQuality);
  addwidget(table, "MaxRate", &MaxRate);
  addwidget(table, "AudioCodec", &AudioCodec);
  addwidget(table, "AudioCodecOpts", &AudioCodecOpts);
  addwidget(table, "SampleRate", &SampleRate);
}


void SpecifyQualityAndMaxRate::setOutputProps(const MsmOutput * output)
{
  if ( output ) {

    std::map<std::string, std::string> opts;
    std::map<std::string, std::string>::const_iterator ii;
    double qp = -1;

    Name->setText(output->getName());
    Format->setText(output->getFormat());
    Enabled->setText(t2s(output->getEnabled()));
    VideoWidth->setText(t2s(output->getVideoWidth()));
    VideoHeight->setText(t2s(output->getVideoHeight()));
    AudioCodec->setText(output->getAudioCodec());
    AudioCodecOpts->setText(output->getAudioOpts());
    SampleRate->setText(t2s(output->getAudioSampleRate()));
    VideoQuality->setText("");
    MaxRate->setText("");

    if ( splitopts(output->getVideoOpts(), &opts) ) {
      if ( (ii = opts.find("-maxrate")) != opts.end() ) {
        MaxRate->setText(ii->second);
      }
      if ( (ii = opts.find("-crf")) != opts.end() ) {
        if ( cast(ii->second, &qp) ) {
          VideoQuality->setText(t2s(qp2q(qp)));
        }
      }
      else if ( (ii = opts.find("-qp")) != opts.end() ) {
        if ( cast(ii->second, &qp) ) {
          VideoQuality->setText(t2s(qp2q(qp)));
        }
      }
    }
  }
}

bool SpecifyQualityAndMaxRate::getChanges(MsmOutput * output)
{
  double q = 0, maxrate = 0;
  std::map<std::string, std::string> opts;
  char stmp[256];

  output->setName(Name->text().toUTF8().c_str());

  if ( !VideoQuality->text().empty() && (!cast(VideoQuality->text().toUTF8(), &q) || q <= 0 || q > 100 )) {
    WMessageBox::show("Syntax Error:", WString("Invalid value: VideoQuality={1}").arg(VideoQuality->text()), Wt::Ok);
    return false;
  }

  if ( !MaxRate->text().empty() && (!s2br(MaxRate->text(), &maxrate) || maxrate <= 0) ) {
    WMessageBox::show("Syntax Error:", WString("Invalid value: MaxRate={1}").arg(MaxRate->text()), Wt::Ok);
    return false;
  }

  if ( !Enabled->text().empty() && !convert(Enabled->text(), output, &MsmOutput::setEnabled) ) {
    WMessageBox::show("Syntax Error:", WString("Invalid value: Enabled={1}").arg(Enabled->text()), Wt::Ok);
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

  if ( SampleRate->text().empty() && !convert(SampleRate->text(), output, &MsmOutput::setAudioSampleRate) ) {
    WMessageBox::show("Syntax Error:", WString("Invalid value: SampleRate={1}").arg(SampleRate->text()), Wt::Ok);
    return false;
  }


  if ( !VideoQuality->text().empty() || !MaxRate->text().empty() ) {
    splitopts(output->getVideoOpts(), &opts);    // save codec options
    opts.erase("-i_qfactor"), opts.erase("-qp"), opts.erase("-crf"), opts.erase("-maxrate"), opts.erase("-bufsize");
  }

  if ( q > 0 ) {    //  calculate desired crf quantizer
    sprintf(stmp, "%g", q2qp(q));
    opts.insert(std::make_pair("-crf", stmp));
  }

  if ( maxrate > 0 ) { //  calculate desired maxrate and VBV buffer size
    sprintf(stmp, "%d", (int) (maxrate * 5));
    opts.insert(std::make_pair("-maxrate", MaxRate->text().toUTF8()));
    opts.insert(std::make_pair("-bufsize", stmp));
  }


  if ( !opts.empty() ) {
    std::string s;
    for ( std::map<std::string, std::string>::const_iterator ii = opts.begin(); ii != opts.end(); ++ii ) {
      s += ii->first, s += " ", s += ii->second, s += " ";
    }
    output->setVideoOpts(s.erase(s.find_last_not_of(" ") + 1).c_str());
  }

  output->setVideoCodec("libx264");
  output->setFormat(Format->text().toUTF8().c_str());
  output->setAudioCodec(AudioCodec->text().toUTF8().c_str());
  output->setAudioOpts(AudioCodecOpts->text().toUTF8().c_str());

  return true;
}
