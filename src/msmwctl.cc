/*
 * msmwctl.cc
 *
 *  Created on: May 20, 2015
 *      Author: amyznikov
 */

#include "msmwctl.h"
#include "ServerLogViewer.h"
#include "LeftPane.h"
#include "RightPane.h"

#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>

#define MSMWCTL_CONFIGURATION "/etc/msmwctl.cfg"
#define MSMWCTL_LOGDIR        "/var/log"
#define MSMWCTL_LOGFILE       "/var/log/msmwctl.log"

namespace Wt {
  LOGGER("msmwctl");
}


class WCustomBootstrapTheme :
    public WBootstrapTheme
{
public:

  typedef WBootstrapTheme
      Base;

  WCustomBootstrapTheme(WObject *parent = 0)
      : Base(parent)
  {
    setVersion(WBootstrapTheme::Version3);
    setResponsive(true);
  }

  virtual std::vector<WCssStyleSheet> styleSheets() const
  {
    /* bootstrap themes:
     *  http://bootswatch.com
     */

    static const char * filenames[] = {
        "3/css/bootstrap.css",
        "3/css/custom.css",
        "3/wt.css",
    };

    std::vector<WCssStyleSheet> result;
    const std::string themePath = resourcesUrl();

    for ( size_t i = 0; i < sizeof(filenames) / sizeof(filenames[0]); ++i ) {
      result.push_back(WCssStyleSheet(WLink(themePath + filenames[i])));
    }

    return result;
  }

};


class MSMApplication
    : public WApplication
{
  typedef WApplication
      Base;

  enum {
    LangDefault = 0,
    LangEn = 1,
    LangRu = 2,
  } language;


  WVBoxLayout * vbox1;
  WHBoxLayout * hbox1;
  WVBoxLayout * vbox2;

  WNavigationBar * nav;
  LeftPane * leftPane;
  RightPane * rightPane;
  ServerLogViewer * logView;

  WPushButton * optionsButton;
  WPopupMenu * options;
  WMenuItem * optionShowLog;
  WPopupMenu * langPopup;
  WMenuItem * langEn;
  WMenuItem * langRu;

//  WMessageResourceBundle * bundleRu;
//  WMessageResourceBundle * bundleEn;

public:

  MSMApplication(const WEnvironment& env)
      : Base(env), language(LangDefault), logView(0)
  {
    setTitle("MSM2 Control Gui");
    setMediaServerAddress(env);

    setTheme(new WCustomBootstrapTheme(this));
    useStyleSheet("style/site.css");

    enableUpdates(true);

//    (bundleRu = new WMessageResourceBundle())->use(appRoot() + "messages.ru");
//    (bundleEn = new WMessageResourceBundle())->use(appRoot() + "messages.en");
//
//    const std::string current_locate = locale();
//    fprintf(stderr, "current_locate = '%s'\n",current_locate.c_str());
//
//    setLocalizedStrings(bundleRu);

    createGUI();
  }

  /* Create GUI */
  void createGUI()
  {
    WContainerWidget * root_ = root();

    vbox1 = new WVBoxLayout(root_);
    vbox1->setContentsMargins(0, 0, 0, 0);

    vbox1->addWidget(nav = new WNavigationBar(), 0, Wt::AlignTop);
    nav->setResponsive(true);
    nav->addStyleClass("navbar-static-top");
    nav->setTitle("MSM2 configuration tool");


    nav->addWidget(optionsButton = new WPushButton("Settings"), Wt::AlignRight);
    optionsButton->setMargin("8px", Wt::Right);
    optionsButton->setMenu(options = new WPopupMenu());


    optionShowLog = options->addItem("Show Server Events");
    optionShowLog->setCheckable(true);
    optionShowLog->triggered().connect(this, &MSMApplication::onShowEventLog);

    options->addSeparator();

    options->addMenu("Lagnuage", langPopup= new WPopupMenu());
    langEn = langPopup->addItem("English");
    langEn->setCheckable(true);
    langEn->triggered().connect(this, &MSMApplication::selectLangEn);

    langRu = langPopup->addItem(WString::fromUTF8("Русский"));
    langRu->setCheckable(true);
    langRu->triggered().connect(this, &MSMApplication::selectLangRu);



    vbox1->addLayout(hbox1 = new WHBoxLayout(), 1);
    hbox1->setContentsMargins(0, 0, 0, 0);


    hbox1->addWidget(leftPane = new LeftPane());
    hbox1->setResizable(0, true, "25%");

    hbox1->addLayout(vbox2 = new WVBoxLayout());
    vbox2->addWidget(rightPane = new RightPane());



    leftPane->addInput().connect(rightPane, &RightPane::AddInput);
    leftPane->editInput().connect(rightPane, &RightPane::EditInput);
    leftPane->addOutput().connect(rightPane, &RightPane::AddOutput);
    leftPane->editOutput().connect(rightPane, &RightPane::EditOutput);
    leftPane->addSink().connect(rightPane, &RightPane::AddSink);
    leftPane->editSink().connect(rightPane, &RightPane::EditSink);
    leftPane->streamsRootSelected().connect(rightPane, &RightPane::showStreams);
    leftPane->nothingSelected().connect(rightPane, &RightPane::showNothingSelected);

    rightPane->inputAdded().connect(leftPane, &LeftPane::onInputAdded);
    rightPane->inputDeleted().connect(leftPane, &LeftPane::onInputDeleted);
    rightPane->inputChanged().connect(leftPane, &LeftPane::onInputChanged);

    rightPane->outputAdded().connect(leftPane, &LeftPane::onOutputAdded);
    rightPane->outputDeleted().connect(leftPane, &LeftPane::onOutputDeleted);
    rightPane->outputChanged().connect(leftPane, &LeftPane::onOutputChanged);

    rightPane->sinkAdded().connect(leftPane, &LeftPane::onSinkAdded);
    rightPane->sinkDeleted().connect(leftPane, &LeftPane::onSinkDeleted);
    rightPane->sinkChanged().connect(leftPane, &LeftPane::onSinkChanged);

  }

  void selectLangEn()
  {
    langRu->setChecked(false);
////    WLocalizedStrings * s = localizedStrings();
////    if ( s ) {
////      localizedStrings()->hibernate();
////    }
//    setLocalizedStrings(bundleEn);
//    refresh();
  }

  void selectLangRu()
  {
    langEn->setChecked(false);
////    WLocalizedStrings * s = localizedStrings();
////    if ( s ) {
////      localizedStrings()->hibernate();
////    }
//    setLocalizedStrings(bundleRu);
//    refresh();
  }

  void onShowEventLog()
  {
    if ( optionShowLog->isChecked() ) {
      if ( !logView ) {
        logView = new ServerLogViewer();
      }
      vbox2->addWidget(logView);
      vbox2->setResizable(0, true, "80%");
    }
    else {
      if ( logView != 0 ) {
        vbox2->removeWidget(logView);
        vbox2->setResizable(0, false);
      }
    }
  }

  static WApplication * create(const WEnvironment& env) {
    return new MSMApplication(env);
  }

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


/**
 * fork() and become daemon
 */
static pid_t become_daemon()
{
  pid_t pid;

  if ( (pid = fork()) == 0 ) {
    /*
     * In child process:
     * Redirect stdin/stdout/stderr to /dev/null
     */
    // umask(0);
    for ( int fd = 0; fd < 3; ++fd ) {
      close(fd);
      open("/dev/null", O_RDWR);
    }
  }

  return pid;
}


int main(int argc, char *argv[])
{
  try {

    char logfile[PATH_MAX] = "";
    char home[PATH_MAX] = "";
    struct passwd * pw;
    pid_t pid;
    uid_t uid;
    int sig;

    WServer server(argv[0], "");


    try {
      server.setServerConfiguration(argc, argv, MSMWCTL_CONFIGURATION);
      server.addEntryPoint(Application, &MSMApplication::create);

//      if ( (pid = become_daemon()) < 0 ) {
//        LOG_INFO_S(&server, "fork() fails: " << strerror(errno));
//        return 1;
//      }
//
//      if ( pid != 0 ) {
//        /* exit parent process */
//        return 0;
//      }

      /* Determine user's home direcory */
      if ( (uid = geteuid()) == 0 || !(pw = getpwuid(uid)) ) {
        /* Follow 'Filesystem Hierarchy Standard' http://www.pathname.com/fhs/2.2/fhs-5.8.html */
        strcpy(home, "/var/lib/msmwctl");
      }
      else if ( !pw->pw_dir || strcmp(pw->pw_dir, "/") == 0 || access(pw->pw_dir, F_OK) != 0 ) {
        strcpy(home, "/var/lib/msmwctl");
      }
      else {
        strcpy(home, pw->pw_dir);
      }

      /* Setup log file name */
      if ( access(MSMWCTL_LOGFILE, W_OK) == 0
          || (access(MSMWCTL_LOGFILE, F_OK) != 0 && access(MSMWCTL_LOGDIR, W_OK) == 0) ) {
        strcpy(logfile, MSMWCTL_LOGFILE);
      }
      else {
        sprintf(logfile, "%s/msmwctl.log", home);
      }

      server.initLogger(logfile, "");


      /* Start server */

      if ( server.start() ) {
#ifdef WT_THREADED
        sig = WServer::waitForShutdown(argv[0]);
        LOG_INFO_S(&server, "shutdown (signal = " << sig << ")");
#endif
        server.stop();

#ifdef WT_THREADED
        if ( sig == SIGHUP )
          WServer::restart(argc, argv, 0);
#endif // WT_THREADED
      }

      return 0;
    }

    catch ( std::exception& e ) {
      LOG_INFO_S(&server, "fatal: " << e.what());
      return 1;
    }
  }

  catch ( Wt::WServer::Exception& e ) {
    LOG_ERROR("fatal: " << e.what());
    return 1;
  }

  catch ( std::exception& e ) {
    LOG_ERROR("fatal exception: " << e.what());
    return 1;
  }

  return 0;
}
