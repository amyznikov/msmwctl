/*
 * ProbeInputPage.cc
 *
 *  Created on: May 29, 2015
 *      Author: amyznikov
 */

#define __USE_GNU   1

#include "msmwctl.h"
#include "ProbeInputPage.h"
#include <stdarg.h>

static std::string & append_format(std::string & s, const char * format, ...)
  __attribute__ ((__format__ (__printf__, 2, 3)));

static std::string & append_format(std::string & s, const char * format, ...)
{
  va_list arglist;
  char * temp = NULL;

  va_start(arglist, format);
  vasprintf(&temp, format, arglist);
  va_end(arglist);
  s.append(temp);
  free(temp);

  return s;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ProbeInputPage::ProbeInputPage(WContainerWidget * parent)
    : Base(parent), doingProbeNow(false)
{
  WVBoxLayout * vbox = new WVBoxLayout(this);
  vbox->addWidget(probeStatus = new WText("", Wt::XHTMLUnsafeText));
  vbox->addWidget(probeNow = new WPushButton("Probe now"),0, Wt::AlignLeft);
  probeNow->clicked().connect(this, &ProbeInputPage::onProbeNowClicked);
}

ProbeInputPage::~ProbeInputPage()
{
}

void ProbeInputPage::setInput(const WString & inputName)
{
  if ( (this->inputName = inputName).empty() ) {
    probeStatus->setText("Input not set<br>");
    probeNow->hide();
  }
  else {
    probeStatus->setText("");
    probeNow->setText("Probe now...");
    probeNow->show();
  }
}


void ProbeInputPage::onProbeNowClicked()
{
  struct ffs * ffs = NULL;

  if ( doingProbeNow || inputName.empty() ) {
    return;
  }

  doingProbeNow = 1;

  decorationStyle().setCursor(Wt::WaitCursor);

  probeStatus->setText("Probing, please wait....");
  probeNow->hide();
  WApplication::instance()->processEvents();


  if ( !msmProbeInput(inputName, &ffs) ) {
    probeStatus->setText("Probe failed");
  }
  else
  {
    std::string report;

    append_format(report, "input:%s<br>format=%s<br>", inputName.toUTF8().c_str(), ffs->format);

    for ( unsigned int i = 0; i < ffs->nb_streams; ++i ) {

      append_format(report, "stream %u: %s", i, ffs->streams[i].type);

      if ( strcmp(ffs->streams[i].type, "video") == 0 ) {
        append_format(report, " %s %dx%d %s %dbps profile:%s level:%d stb:%d/%d ctb:%d/%d<br>",
            ffs->streams[i].video.pix_fmt, ffs->streams[i].video.width, ffs->streams[i].video.height,
            ffs->streams[i].codec, ffs->streams[i].bit_rate, ffs->streams[i].profile, ffs->streams[i].level,
            ffs->streams[i].stbnum, ffs->streams[i].stbden,
            ffs->streams[i].stbden, ffs->streams[i].ctbden );

      }
      else if ( strcmp(ffs->streams[i].type, "audio") == 0 ) {
        append_format(report, " %s %s %dHz %dbps channels:%d %s stb:%d/%d ctb:%d/%d<br>",
            ffs->streams[i].audio.sample_fmt, ffs->streams[i].codec,
            ffs->streams[i].audio.sample_rate, ffs->streams[i].bit_rate,
            ffs->streams[i].audio.channels, ffs->streams[i].audio.channel_layout,
            ffs->streams[i].stbnum, ffs->streams[i].stbden,
            ffs->streams[i].stbden, ffs->streams[i].ctbden);
      }
      else if ( strcmp(ffs->streams[i].type, "subtitle") == 0 ) {
        append_format(report, " %dbps width:%d stb:%d/%d ctb:%d/%d<br>", ffs->streams[i].bit_rate,
            ffs->streams[i].subtitle.width, ffs->streams[i].stbnum, ffs->streams[i].stbden, ffs->streams[i].stbden,
            ffs->streams[i].ctbden);
      }

      probeStatus->setText(report);
    }

    msm_ffs_free(&ffs);
  }

  decorationStyle().setCursor(Wt::ArrowCursor);

  doingProbeNow = false;
  probeNow->setText("Probe again...");
  probeNow->show();
}
