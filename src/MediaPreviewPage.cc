/*
 * MediaPreviewPage.cc
 *
 *  Created on: Mar 29, 2015
 *      Author: amyznikov
 */

#include "msmwctl.h"
#include "MediaPreviewPage.h"


MediaPreviewPage::MediaPreviewPage(WContainerWidget * parent)
  : Base(parent)/*, embed(0)*/, flash(0)
{
}

void MediaPreviewPage::setMediaSource(const char * source)
{
  // delete embed, embed = 0;
  delete flash, flash = 0;

  if ( source && *source ) {
#if 0
//    embed->setTemplateText(WString("<object><embed src=\"{1}\" "
//        "width=\"400\" height=\"300\" CONTROLLER=\"true\" LOOP=\"false\" AUTOPLAY=\"true\" PLAY=\"true\">"
//        "</embed></object>").arg(url), XHTMLUnsafeText);
//    embed->show();
#elif 0
    //const std::string source = std::string(url) + "?fmt=flv";

    flash = new WFlashObject("JarisFLVPlayer.swf", this);
    flash->setFlashParameter("allowScriptAccess","always");
    flash->setFlashParameter("allowFullscreen", "true");
    flash->setFlashParameter("quality", "high");
    flash->setFlashParameter("wmode", "opaque");
    flash->resize(400, 300);
    flash->setFlashVariable("source", source);
    flash->setFlashVariable("type", "video");
    // flash->setFlashVariable("streamtype", "http");
    flash->setFlashVariable("controls","true");
    flash->setFlashVariable("controltype", "1");
    flash->setFlashVariable("autostart", "false");
    flash->setFlashVariable("duration", "300");
    flash->setFlashVariable("seekcolor","D3D3D3");
    flash->setFlashVariable("jsapi","false");
#else
    flash = new WFlashObject("uppod.swf", this);
    flash->setFlashParameter("allowScriptAccess","always");
    flash->setFlashParameter("allowFullscreen", "true");
    //flash->setFlashParameter("quality", "high");
    //flash->setFlashParameter("wmode", "direct");
    flash->resize(400, 300);
    flash->setFlashVariable("file", source);
    flash->setFlashVariable("m", "video");
    // flash->setFlashVariable("streamtype", "http");
    // flash->setFlashVariable("controls","true");
    //flash->setFlashVariable("controltype", "1");
    //flash->setFlashVariable("autostart", "true");
    flash->setFlashVariable("st", "41AEcv1cZrxYo0xaff1kOkZ2MiJzC5k42rQi63GktQj1ozC5k5N3Q3Q3Qe3zkOk1mRwXRWNvzN31X1kw68zkOk03wBhT31X1Cna4Hj1l1Zk0nTWwjRG3NXzBGtDY31X1SfQD6dwyL62GfTk06BQ3QIzd5kcL2QjR350bTCGfF5dGJTC0nR9m0Xa2MD3Q2Nf9BR5cYsMX6dsNXHB5XkT2G3TSQQD6dwfF3fNXR3GXeH2wITWwWETCwXFWsK6I2McTS9QD6dwXiFsNXHms6sI2McTfMhjF3NXis8NXHv1l1rf0XR30chLWwjzfUrY");

#endif
  }
}

