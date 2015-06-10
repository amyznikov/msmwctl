/*
 * MediaPreviewPage.cc
 *
 *  Created on: Mar 29, 2015
 *      Author: amyznikov
 */

#include "msmwctl.h"
#include "MediaPreviewPage.h"
#include <stdarg.h>

MediaPreviewPage::MediaPreviewPage(WContainerWidget * parent)
  : Base(parent), pageTemplate(0)
{
}


void MediaPreviewPage::setMediaSource(const char * source)
{
  delete pageTemplate, pageTemplate = 0;

  if ( source && *source ) {
    static const char template_text[] = ""
        "<object classid=\"clsid:9BE31822-FDAD-461B-AD51-BE1D1C159921\" codebase=\"http://download.videolan.org/pub/videolan/vlc/last/win32/axvlc.cab\" id=\"vlc\">"
          "<embed type=\"application/x-vlc-plugin\" pluginspage=\"http://www.videolan.org\" version=\"VideoLAN.VLCPlugin.2\"\n"
          "  width=\"320\"\n"
          "  height=\"240\"\n"
          "  autoplay=\"yes\"\n"
          "  allowfullscreen=\"yes\"\n"
          "  id=\"vlc\"\n"
          "  target=\"{1}\">\n"
          "</embed>\n"
        "</object>"
        "";

    pageTemplate = new WTemplate(this);
    pageTemplate->setTemplateText(WString(template_text).arg(source), XHTMLUnsafeText);
    pageTemplate->show();
  }
}

