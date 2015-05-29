/*
 * RightPane.h
 *
 *  Created on: Mar 28, 2015
 *      Author: amyznikov
 */

#ifndef __rightpane_h__
#define __rightpane_h__

class StreamListView;
class InputProps;
class OutputProps;
class SinkProps;

class RightPane
  : public WContainerWidget {
public:
  typedef WContainerWidget Base;

  RightPane(WContainerWidget * parent = 0);

  void AddInput();
  void EditInput(const WString & inputName);
  void AddOutput(const WString & inputName);
  void EditOutput(const WString & inputName, const WString & outputName);
  void AddSink(const WString & inputName, const WString & outputName);
  void EditSink(const WString & inputName, const WString & outputName, const WString & sinkName);
  void showStreams();
  void showNothingSelected();

  Signal<WString> & inputAdded();
  Signal<WString> & inputDeleted();
  Signal<WString,const MsmInput*> & inputChanged();

  Signal<WString,WString> & outputAdded();
  Signal<WString, WString> & outputDeleted();
  Signal<WString,WString,const MsmOutput*> & outputChanged();

  Signal<WString, WString, WString> & sinkAdded();
  Signal<WString, WString, WString> & sinkDeleted();
  Signal<WString,WString,WString,const MsmSink*> & sinkChanged();


private:
  WVBoxLayout * vbox;
  WStackedWidget * stack;
  StreamListView * streamListView;
  InputProps * inputProps;
  OutputProps * outputProps;
  SinkProps * sinkProps;
};



#endif /* __rightpane_h__ */
