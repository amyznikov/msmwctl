/*
 * msmwctl.cc
 *
 *  Created on: May 20, 2015
 *      Author: amyznikov
 */

#include "msmwctl.h"
#include "GuiRoot.h"
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

      if ( (pid = become_daemon()) < 0 ) {
        LOG_INFO_S(&server, "fork() fails: " << strerror(errno));
        return 1;
      }

      if ( pid != 0 ) {
        /* exit parent process */
        return 0;
      }


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
