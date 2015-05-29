/*
 * LeftPane.h
 *
 *  Created on: Mar 28, 2015
 *      Author: amyznikov
 */

#ifndef __leftpane_h__
#define __leftpane_h__

class TWrap;
class LeftPane
  : public WContainerWidget {
public:
  typedef WContainerWidget Base;
  LeftPane(WContainerWidget * parent = 0);

  Signal<> & addInput();
  Signal<WString> & editInput();
  Signal<WString> & addOutput();
  Signal<WString,WString> & editOutput();
  Signal<WString,WString> & addSink();
  Signal<WString,WString,WString> & editSink();
  Signal<> & streamsRootSelected();
  Signal<> & nothingSelected();

  void onInputChanged(const WString & inputName, const MsmInput * input);
  void onInputAdded(const WString & inputName);
  void onInputDeleted(const WString & inputName);

  void onOutputAdded(const WString & inputName, const WString & outputName);
  void onOutputDeleted(const WString & inputName, const WString & outputName);
  void onOutputChanged(const WString & inputName, const WString & outputName, const MsmOutput * output);

  void onSinkAdded(const WString & inputName, const WString & outputName, const WString & sinkName);
  void onSinkDeleted(const WString & inputName, const WString & outputName, const WString & sinkName);
  void onSinkChanged(WString inputName, WString outputName, WString sinkName, const MsmSink * sink);

private:
  void onServerAddressChanged();
  void clearTree();
  void populateTree();
  void onReloadClicked();
  void onTreeViewSelectionChanged();

private:
  WVBoxLayout * vbox;
  WTable * table;
  //WText * label;
  WPushButton * reloadStreams;
  WTree * streamsTreeView;

  Signal<> addInput_;
  Signal<WString> editInput_;
  Signal<WString> addOutput_;
  Signal<WString,WString> editOutput_;
  Signal<WString,WString> addSink_;
  Signal<WString,WString,WString> editSink_;
  Signal<> nothingSelected_;
  Signal<> streamsRootSelected_;
};





#endif /* __leftpane_h__ */
