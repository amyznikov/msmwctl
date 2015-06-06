/*
 * msmwctl.h
 *
 *  Created on: May 20, 2015
 *      Author: amyznikov
 */

#ifndef __msmwctl_h__
#define __msmwctl_h__

#define WT_BUILDING /* for logger */
#include <Wt/WLogger>

#include <Wt/WServer>
#include <Wt/WApplication>
#include <Wt/WEnvironment>
#include <Wt/WString>
#include <Wt/WBootstrapTheme>
#include <Wt/WCssTheme>
#include <Wt/WCssDecorationStyle>
#include <Wt/WMessageBox>
#include <Wt/WContainerWidget>
#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>
#include <Wt/WGridLayout>
#include <Wt/WToolBar>
#include <Wt/WNavigationBar>
#include <Wt/WStackedWidget>
#include <Wt/WSelectionBox>
#include <Wt/WMenu>
#include <Wt/WLabel>
#include <Wt/WPushButton>
#include <Wt/WCheckBox>
#include <Wt/WComboBox>
#include <Wt/WLink>
#include <Wt/WText>
#include <Wt/WTextArea>
#include <Wt/WTextEdit>
#include <Wt/WTreeView>
#include <Wt/WLineEdit>
#include <Wt/WTree>
#include <Wt/WTreeNode>
#include <Wt/WColor>
#include <Wt/WVideo>
#include <Wt/WFlashObject>
#include <Wt/WMediaPlayer>
#include <Wt/WTemplate>
#include <Wt/WDoubleSpinBox>
#include <Wt/WScrollArea>
#include <Wt/WObject>
#include <Wt/WSignal>
#include <Wt/WFitLayout>
#include <Wt/WTabWidget>
#include <Wt/WTable>
#include <Wt/WTableView>
#include <Wt/WStandardItemModel>
#include <Wt/WStandardItem>
#include <Wt/WTimer>
#include <Wt/WPopupMenu>
#include <Wt/WPopupMenuItem>

#include "msm.h"

using namespace Wt;


template<class T>
static inline void bindWidget(WTemplate * t, const std::string & name, T ** obj, int width = 100) {
  * obj = new T();
  t->bindWidget(name, *obj);
  if ( width > 0 ) {
    (*obj)->setWidth(WLength(width, WLength::Percentage));
  }
}


template<class T>
static void addwidget(WTable * table, const char * legend, T ** obj, int width = 0)
{
  WLabel * label;
  int row;

  row = table->rowCount();
  label = new WLabel(legend, table->elementAt(row, 0));
  (*obj) = new T(table->elementAt(row, 1));
//  table->elementAt(row, 0)->setWidth(WLength(25, WLength::Percentage));
//  table->elementAt(row, 1)->setWidth(WLength(70, WLength::Percentage));
  label->setBuddy((*obj));
  if ( width > 0 ) {
    (*obj)->setWidth(WLength(width, WLength::Percentage));
  }
}

static inline std::string t2s(bool t) {
  return t ? "yes" : "no";
}

static inline std::string t2s(int t) {
  char s[128] = ""; snprintf(s, sizeof(s), "%d", t);
  return s;
}

static inline std::string t2s(int64_t t) {
  char s[128] = ""; snprintf(s, sizeof(s), "%" PRId64 "", t);
  return s;
}

static inline std::string t2s(double t) {
  char s[128] = ""; snprintf(s, sizeof(s), "%g", t);
  return s;
}

template<class T1, class T2>
static bool cast(const T1 & x, T2 * y) {
  *y = x;
  return true;
}

template<>
bool cast(const std::string & x, int * y) {
  return sscanf(x.c_str(),"%d", y) == 1;
}

template<>
bool cast(const std::string & x, unsigned int * y) {
  return sscanf(x.c_str(),"%d", y) == 1;
}

template<> bool cast(const std::string & x, int64_t * y) {
  return sscanf(x.c_str(),"%" PRId64 "", y) == 1;
}

template<> bool cast(const std::string & x, uint64_t * y) {
  return sscanf(x.c_str(),"%" PRIu64 "", y) == 1;
}

template<>
bool cast(const std::string & x, double * y) {
  return sscanf(x.c_str(),"%lf", y) == 1;
}

template<> bool cast(const std::string & x, bool * y) {
  const char * s = x.c_str();
  if ( strcmp(s, "true") == 0 || strcmp(s, "yes") == 0 || strcmp(s, "1") == 0 ) {
    *y = true; return true;
  }
  else if ( strcmp(s, "false") == 0 || strcmp(s, "no") == 0 || strcmp(s, "0") == 0 ) {
    *y = false; return true;
  }
  return false;
}

template<class T, class _ArgT>
inline bool convert(const WString & s, T * t, void (T::*fn)(_ArgT arg), bool (*cvt)(const WString&, _ArgT *) = 0) {
  _ArgT arg;
  if ( cvt ? cvt(s, &arg) : cast(s.toUTF8(), &arg) ) {
    (t->*fn)(arg);
    return true;
  }
  return false;
}

inline bool s2br(const WString & s, double * y)
{
  double x;
  char suffix[16] = "";

  if ( sscanf(s.toUTF8().c_str(), "%lf%15s", &x, suffix) >= 1 && x >= 0 ) {
    if ( strcasecmp(suffix, "K") == 0 || strcasecmp(suffix, "Kb") == 0 ) {
      *y = (x * 1000.0);
      return true;
    }
    if ( strcasecmp(suffix, "M") == 0 || strcasecmp(suffix, "Mb") == 0 ) {
      *y = (x * 1000000.0);
      return true;
    }
    if ( strcasecmp(suffix, "G") == 0 || strcasecmp(suffix, "Gb") == 0 ) {
      *y = (x * 1000000000.0);
      return true;
    }
    if ( (!*suffix || !strcasecmp(suffix, "B") || !strcasecmp(suffix, "Bps")) ) {
      *y = x;
      return true;
    }
  }
  return false;
}

inline bool splitopts(const char * s, std::map<std::string, std::string> * options)
{
  int n = 0;
  if ( s && *s ) {
    static const char delims[] = " \t\n\r";
    char opts[strlen(s) + 1];
    const char * opt, *val;

    opt = strtok(strcpy(opts, s), delims);
    while ( opt ) {
      if ( *opt == '-' && *(opt + 1) && (val = strtok(NULL, delims)) ) {
        options->insert(std::make_pair(opt, val)), ++n;
      }
      opt = strtok(NULL, delims);
    }
  }
  return n != 0;
}



#endif /* __msmwctl_h__ */
