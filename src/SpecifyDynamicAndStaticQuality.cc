/*
 * SpecifyDynamicAndStaticQuality.cc
 *
 *  Created on: Mar 29, 2015
 *      Author: amyznikov
 */

#include "msmwctl.h"
#include "SpecifyDynamicAndStaticQuality.h"


SpecifyDynamicAndStaticQuality::SpecifyDynamicAndStaticQuality(WContainerWidget * parent)
    : Base(parent)
{
  WVBoxLayout * vbox = new WVBoxLayout(this);
  vbox->addWidget(table = new WTable());
  vbox->addSpacing(WLength(4, WLength::FontEx));

  table->setWidth(WLength(95, WLength::Percentage));
  addwidget(table, "Name", &Name);
  addwidget(table, "Format", &Format);
  addwidget(table, "Enabled", &Enabled);
  addwidget(table, "VideoWidth",&VideoWidth);
  addwidget(table, "VideoHeight", &VideoHeight);
  addwidget(table, "StaticQuality", &StaticQuality);
  addwidget(table, "DynamicQuality", &DynamicQuality);
  addwidget(table, "MotionThreshold", &MotionThreshold);
  addwidget(table, "BgWeight", &BgWeight);
  addwidget(table, "AudioCodec", &AudioCodec);
  addwidget(table, "AudioCodecOpts", &AudioCodecOpts);
  addwidget(table, "SampleRate", &SampleRate);
}



void SpecifyDynamicAndStaticQuality::setOutputProps(const MsmOutput * output)
{
  if ( output ) {

    std::map<std::string, std::string> opts;
    std::map<std::string, std::string>::const_iterator ii;
    const char * codec;
    double qp = -1, crf = -1, ip = -1, mt = -1, mqp = -1, mbgw = -1;

    Name->setText(output->getName());
    Format->setText(output->getFormat());
    Enabled->setText(t2s(output->getEnabled()));
    VideoWidth->setText(t2s(output->getVideoWidth()));
    VideoHeight->setText(t2s(output->getVideoHeight()));
    AudioCodec->setText(output->getAudioCodec());
    AudioCodecOpts->setText(output->getAudioOpts());
    SampleRate->setText(t2s(output->getAudioSampleRate()));

    if ( splitopts(output->getVideoOpts(), &opts) ) {

      if ( (ii = opts.find("-mt")) != opts.end() && cast(ii->second, &mt) ) {
        MotionThreshold->setText(t2s(mt));
      }

      if ( (ii = opts.find("-mbgw")) != opts.end() && cast(ii->second, &mbgw) ) {
        BgWeight->setText(t2s(mbgw));
      }

      if ( (ii = opts.find("-qp")) != opts.end() ) {
        cast(ii->second, &qp);
      }

      if ( (ii = opts.find("-crf")) != opts.end() ) {
        cast(ii->second, &crf);
      }

      if ( (ii = opts.find("-mqp")) != opts.end() ) {
        cast(ii->second, &mqp);
      }

      if ( (ii = opts.find("-i_qfactor")) != opts.end() ) {
        cast(ii->second, &ip);
      }


      if ( !(codec = output->getVideoCodec()) || strcmp(codec, "x264-roi") != 0 ) {
        if ( qp > 0 && ip > 0 ) {
          StaticQuality->setText(t2s(qp2q(ip * qp)));
          DynamicQuality->setText(t2s(qp2q(qp)));
        }
        else if ( crf > 0 && ip > 0 ) {
          StaticQuality->setText(t2s(qp2q(crf)));
          DynamicQuality->setText(t2s(qp2q(crf/ip)));
        }
        else if ( qp > 0 ) {
          DynamicQuality->setText(t2s(qp2q(qp)));
        }
        else if ( crf > 0 ) {
          DynamicQuality->setText(t2s(qp2q(crf)));
        }
      }
      else {

        if ( crf > 0 ) {
          StaticQuality->setText(t2s(qp2q(crf)));
        }

        if ( mqp > 0 ) {
          DynamicQuality->setText(t2s(qp2q(mqp)));
        }
        else if ( qp > 0 ) {
          DynamicQuality->setText(t2s(qp2q(qp)));
        }
      }
    }
  }
}

bool SpecifyDynamicAndStaticQuality::getChanges(MsmOutput * output)
{
  std::map<std::string, std::string> opts;
  std::map<std::string, std::string>::const_iterator ii;
  double sq = 0, dq = 0, qi = 0, qp = 0, ip = 0, mt = -1, mbgw = -1;
  char stmp[256];

  output->setName(Name->text().toUTF8().c_str());

  if ( !StaticQuality->text().empty() && (!cast(StaticQuality->text().toUTF8(), &sq) || sq <= 0 || sq > 100) ) {
    WMessageBox::show("Syntax Error:", WString("StaticQuality={1}").arg(StaticQuality->text()), Wt::Ok);
    return false;
  }

  if ( !DynamicQuality->text().empty() && (!cast(DynamicQuality->text().toUTF8(), &dq) || dq <= 0 || dq > 100) ) {
    WMessageBox::show("Syntax Error:", WString("DynamicQuality={1}").arg(DynamicQuality->text()), Wt::Ok);
    return false;
  }

  if ( !MotionThreshold->text().empty() && !cast(MotionThreshold->text().toUTF8(), &mt) ) {
    WMessageBox::show("Syntax Error:", WString("MotionThreshold={1}").arg(MotionThreshold->text()), Wt::Ok);
    return false;
  }

  if ( !BgWeight->text().empty() && !cast(BgWeight->text().toUTF8(), &mbgw) ) {
    WMessageBox::show("Syntax Error:", WString("BgWeight={1}").arg(BgWeight->text()), Wt::Ok);
    return false;
  }

  if ( !Enabled->text().empty() && !convert(Enabled->text(), output, &MsmOutput::setEnabled) ) {
    WMessageBox::show("Syntax Error:", WString("Invalid Enabled={1}").arg(Enabled->text()), Wt::Ok);
    return false;
  }

  if ( !VideoWidth->text().empty() && !convert(VideoWidth->text(), output, &MsmOutput::setVideoWidth) ) {
    WMessageBox::show("Syntax Error:", WString("Invalid VideoWidth={1}").arg(VideoWidth->text()), Wt::Ok);
    return false;
  }

  if ( !VideoHeight->text().empty() && !convert(VideoHeight->text(), output, &MsmOutput::setVideoHeight) ) {
    WMessageBox::show("Syntax Error:", WString("Invalid VideoHeight={1}").arg(VideoHeight->text()), Wt::Ok);
    return false;
  }

  if ( !SampleRate->text().empty() && !convert(SampleRate->text(), output, &MsmOutput::setAudioSampleRate) ) {
    WMessageBox::show("Syntax Error:", WString("Invalid SampleRate={1}").arg(SampleRate->text()), Wt::Ok);
    return false;
  }

  if ( !StaticQuality->text().empty() || !DynamicQuality->text().empty() ) {
    splitopts(output->getVideoOpts(), &opts);    // save codec options
    opts.erase("-qp"), opts.erase("-crf"), opts.erase("-i_qfactor"), opts.erase("-maxrate"), opts.erase("-bufsize");
  }

  if ( sq > 0 ) { // calculate desired I-frame quantizer
    qi = q2qp(sq);
  }

  if ( dq > 0 ) { // calculate desired P-frame quantizer
    qp = q2qp(dq);
  }

  output->setVideoCodec("libx264");

  if ( mt >= 0 ) {
    sprintf(stmp, "%g", mt), opts.erase("-mt"), opts.insert(std::make_pair("-mt", stmp));
  }

  if ( mbgw >= 0 ) {
    sprintf(stmp, "%g", mbgw ), opts.erase("-mbgw"), opts.insert(std::make_pair("-mbgw", stmp));
  }

  if ( qi > 0 && qp > 0 ) {
    if ( qi <= qp  ) { /* normal case: dynamic quality is lower than static quality */
      ip = qi / qp;
      sprintf(stmp, "%g", qp), opts.insert(std::make_pair("-qp", stmp));
      sprintf(stmp, "%g", ip), opts.insert(std::make_pair("-i_qfactor", stmp));
    }
    else { /* inverted case: involve motion detector */
      output->setVideoCodec("x264-roi");
      sprintf(stmp, "%g", qi), opts.erase("-crf"), opts.insert(std::make_pair("-crf", stmp));
      sprintf(stmp, "%g", qp), opts.erase("-mqp"), opts.insert(std::make_pair("-mqp", stmp));
    }
  }
  else {
    if ( qi > 0 ) {
      sprintf(stmp, "%g", qp), opts.insert(std::make_pair("-qp", stmp));
    }
    else if ( qp > 0 ) {
      sprintf(stmp, "%g", qp), opts.insert(std::make_pair("-crf", stmp));
    }
  }

  if ( !opts.empty() ) {
    std::string s;
    for ( std::map<std::string, std::string>::const_iterator ii = opts.begin(); ii != opts.end(); ++ii ) {
      s += ii->first, s += " ", s += ii->second, s += " ";
    }
    output->setVideoOpts(s.erase(s.find_last_not_of(" ") + 1).c_str());
  }

  output->setFormat(Format->text().toUTF8().c_str());
  output->setAudioCodec(AudioCodec->text().toUTF8().c_str());
  output->setAudioOpts(AudioCodecOpts->text().toUTF8().c_str());

  return true;
}

