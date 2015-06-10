/*
 * MediaPreviewPage.cc
 *
 *  Created on: Mar 29, 2015
 *      Author: amyznikov
 */

#include "msmwctl.h"
#include "MediaPreviewPage.h"

#if 0
    "<script type=\"text/javascript\" src=\"jPlayer/jplayer/jquery.min.js\"></script>"
    "<script type=\"text/javascript\" src=\"jPlayer/jplayer/jquery.jplayer.min.js\"></script>"
    "<script type=\"text/javascript\">"
    "//<![CDATA["
    "$(document).ready(function(){"
    "alert(\"1!\");"
    "  $(\"#jquery_jplayer_1\").jPlayer({"
    "    ready: function () {"
    "      alert(\"2!\");"
    "      $(this).jPlayer(\"setMedia\", {"
    "        title: \"Big Buck Bunny\","
    "        m4v: \"http://www.jplayer.org/video/m4v/Big_Buck_Bunny_Trailer.m4v\","
    "        ogv: \"http://www.jplayer.org/video/ogv/Big_Buck_Bunny_Trailer.ogv\","
    "        webmv: \"http://www.jplayer.org/video/webm/Big_Buck_Bunny_Trailer.webm\","
    "        poster: \"http://www.jplayer.org/video/poster/Big_Buck_Bunny_Trailer_480x270.png\""
    "      });"
    "    },"
    "    swfPath: \"../../dist/jplayer\","
    "    supplied: \"webmv, ogv, m4v\","
    "    size: {"
    "      width: \"640px\","
    "      height: \"360px\","
    "      cssClass: \"jp-video-360p\""
    "    },"
    "    useStateClassSkin: true,"
    "    autoBlur: false,"
    "    smoothPlayBar: true,"
    "    keyEnabled: true,"
    "    remainingDuration: true,"
    "    toggleDuration: true"
    "  });"
    "});"
    "//]]>"
    "</script>"
#endif

static const char template_text[] =
    "<div id=\"jp_container_1\" class=\"jp-video jp-video-360p\" role=\"application\" aria-label=\"media player\">"
    "  <div class=\"jp-type-single\">"
    "    <div id=\"jquery_jplayer_1\" class=\"jp-jplayer\"></div>"
    "    <div class=\"jp-gui\">"
    "      <div class=\"jp-video-play\">"
    "        <button class=\"jp-video-play-icon\" role=\"button\" tabindex=\"0\">play</button>"
    "      </div>"
    "      <div class=\"jp-interface\">"
    "        <div class=\"jp-progress\">"
    "          <div class=\"jp-seek-bar\">"
    "            <div class=\"jp-play-bar\"></div>"
    "          </div>"
    "        </div>"
    "        <div class=\"jp-current-time\" role=\"timer\" aria-label=\"time\">&nbsp;</div>"
    "        <div class=\"jp-duration\" role=\"timer\" aria-label=\"duration\">&nbsp;</div>"
    "        <div class=\"jp-details\">"
    "          <div class=\"jp-title\" aria-label=\"title\">&nbsp;</div>"
    "        </div>"
    "        <div class=\"jp-controls-holder\">"
    "          <div class=\"jp-volume-controls\">"
    "            <button class=\"jp-mute\" role=\"button\" tabindex=\"0\">mute</button>"
    "            <button class=\"jp-volume-max\" role=\"button\" tabindex=\"0\">max volume</button>"
    "            <div class=\"jp-volume-bar\">"
    "              <div class=\"jp-volume-bar-value\"></div>"
    "            </div>"
    "          </div>"
    "          <div class=\"jp-controls\">"
    "            <button class=\"jp-play\" role=\"button\" tabindex=\"0\">play</button>"
    "            <button class=\"jp-stop\" role=\"button\" tabindex=\"0\">stop</button>"
    "          </div>"
    "          <div class=\"jp-toggles\">"
    "            <button class=\"jp-repeat\" role=\"button\" tabindex=\"0\">repeat</button>"
    "            <button class=\"jp-full-screen\" role=\"button\" tabindex=\"0\">full screen</button>"
    "          </div>"
    "        </div>"
    "      </div>"
    "    </div>"
    "    <div class=\"jp-no-solution\">"
    "      <span>Update Required</span>"
    "      To play the media you will need to either update your browser to a recent version or update your <a href=\"http://get.adobe.com/flashplayer/\" target=\"_blank\">Flash plugin</a>."
    "    </div>"
    "  </div>"
    "</div>"
    "";

static const char show_player[] = ""
    "function() {"
    " alert('show_player:1'); "
    "try {"
    "  new jPlayerPlaylist({"
    "    jPlayer: \"#jquery_jplayer_1\","
    "    cssSelectorAncestor: \"#jp_container_1\""
    "  }, ["
    "    {"
    "      title:\"Big Buck Bunny Trailer\","
    "      artist:\"Blender Foundation\","
    "      free:true,"
    "      m4v: \"http://www.jplayer.org/video/m4v/Big_Buck_Bunny_Trailer.m4v\","
    "      ogv: \"http://www.jplayer.org/video/ogv/Big_Buck_Bunny_Trailer.ogv\","
    "      webmv: \"http://www.jplayer.org/video/webm/Big_Buck_Bunny_Trailer.webm\","
    "      poster:\"http://www.jplayer.org/video/poster/Big_Buck_Bunny_Trailer_480x270.png\""
    "    },"
    "    {"
    "      title:\"Finding Nemo Teaser\","
    "      artist:\"Pixar\","
    "      m4v: \"http://www.jplayer.org/video/m4v/Finding_Nemo_Teaser.m4v\","
    "      ogv: \"http://www.jplayer.org/video/ogv/Finding_Nemo_Teaser.ogv\","
    "      webmv: \"http://www.jplayer.org/video/webm/Finding_Nemo_Teaser.webm\","
    "      poster: \"http://www.jplayer.org/video/poster/Finding_Nemo_Teaser_640x352.png\""
    "    },"
    "    {"
    "      title:\"Incredibles Teaser\","
    "      artist:\"Pixar\","
    "      m4v: \"http://www.jplayer.org/video/m4v/Incredibles_Teaser.m4v\","
    "      ogv: \"http://www.jplayer.org/video/ogv/Incredibles_Teaser.ogv\","
    "      webmv: \"http://www.jplayer.org/video/webm/Incredibles_Teaser.webm\","
    "      poster: \"http://www.jplayer.org/video/poster/Incredibles_Teaser_640x272.png\""
    "    }"
    "  ], {"
    "    swfPath: \"jPlayer/jplayer\","
    "    supplied: \"webmv, ogv, m4v\","
    "    useStateClassSkin: true,"
    "    autoBlur: false,"
    "    smoothPlayBar: true,"
    "    keyEnabled: true"
    "  });"
    "alert('show_player:2');"
    "} catch (e) {"
    "alert(e.message);"
    "alert(e.stack);"
    "}"
    ""
    "}"
    "";


static const char test_func[] = ""
    "function(){"
    " if (window.jQuery) {"
    "  alert('yes');"
    " } else {"
    "  alert('no');"
    " }"
    "}"
    "";



MediaPreviewPage::MediaPreviewPage(WContainerWidget * parent)
  : Base(parent)/*, embed(0)*//*, flash(0)*/
{
  WApplication * app = WApplication::instance();
  app->useStyleSheet("jPlayer/skin/pink.flag/css/jplayer.pink.flag.css");
  pageTemplate = new WTemplate(template_text, this);

  // app->require("jPlayer/lib/jquery.min.js");
  app->require("jPlayer/jplayer/jquery.jplayer.min.js");
  app->require("jPlayer/add-on/jplayer.playlist.min.js");
  app->declareJavaScriptFunction("show_player", show_player);
  app->declareJavaScriptFunction("test", test_func);
  app->doJavaScript(app->javaScriptClass() + ".show_player();");


}

void MediaPreviewPage::setMediaSource(const char * source)
{
  // delete embed, embed = 0;
  //delete flash, flash = 0;

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
#elif 0
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
#else

    WApplication * app = WApplication::instance();
//    app->require("jPlayer/jplayer/jquery.jplayer.js");
//    app->declareJavaScriptFunction("init_player", init_player);
//    app->doJavaScript(app->javaScriptClass() + ".test();");

#endif
  }
}

