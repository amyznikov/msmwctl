/*
 * msmwctl.cc
 *
 *  Created on: May 20, 2015
 *      Author: amyznikov
 */

#include "msmwctl.h"
#include "GuiRoot.h"

class MSMApplication
    : public WApplication
{
  typedef WApplication
      Base;

public:

  MSMApplication(const WEnvironment& env)
      : Base(env)
  {
    setTitle("MSM2 Control Gui");
    // setDefaultMsmAddress(env);
    setMediaServerAddress(env);

    // messageResourceBundle().use(appRoot() + "templates");

    const std::string theme = get_theme(env);

    if ( theme == "bootstrap3" ) {
      WBootstrapTheme *bootstrapTheme = new WBootstrapTheme(this);
      bootstrapTheme->setVersion(WBootstrapTheme::Version3);
      bootstrapTheme->setResponsive(true);
      setTheme(bootstrapTheme);
      //useStyleSheet("resources/themes/bootstrap/3/bootstrap-theme.min.css");
    }
    else if ( theme == "bootstrap2" ) {
      WBootstrapTheme *bootstrapTheme = new WBootstrapTheme(this);
      bootstrapTheme->setResponsive(true);
      setTheme(bootstrapTheme);
    }
    else {
      setTheme(new WCssTheme(theme));
    }

//    useStyleSheet("style/everywidget.css");
//    useStyleSheet("style/dragdrop.css");
//    useStyleSheet("style/combostyle.css");
//    useStyleSheet("style/pygments.css");
//    useStyleSheet("style/layout.css");

//    root()->resize(WLength(100, WLength::Percentage),WLength(1000, WLength::Percentage));
//    root()->decorationStyle().setBorder(WBorder(WBorder::Solid, WBorder::Thin, Wt::black));

//    WVBoxLayout * vbox = new WVBoxLayout(root());
//    vbox->addWidget(new MainForm());

    enableUpdates(true);

    new GuiRoot(root());
  }

  static std::string get_theme(const WEnvironment& env) {
    const std::string * themePtr = env.getParameter("theme");
    return themePtr ? *themePtr : "bootstrap3";
  }

  static WApplication * create(const WEnvironment& env) {
    return new MSMApplication(env);
  }

//  static void setDefaultMsmAddress(const WEnvironment& env)
//  {
//    char addrs[256] = "";
//    char host[256] = "";
//    uint16_t port;
//
//    sscanf(env.hostName().c_str(), "%255[^:]:%hu", host, &port);
//    sprintf(addrs, "%s:%u", host, MSM_DEFAULT_CTRL_PORT);
//
//    setMsmAddress(addrs);
//  }

  static void setMediaServerAddress(const WEnvironment& env)
  {
    char addrs[256] = "";
    char host[256] = "";
    uint16_t port;

    sscanf(env.hostName().c_str(), "%255[^:]:%hu", host, &port);
    sprintf(addrs, "%s:%u", host, MSM_DEFAULT_CTRL_PORT);

    ::setMediaServerAddress(addrs);
  }

};



int main(int argc, char *argv[])
{
  return WRun(argc, argv, &MSMApplication::create);
}


