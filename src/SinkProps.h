/*
 * SinkProps.h
 *
 *  Created on: Mar 28, 2015
 *      Author: amyznikov
 */

#ifndef __SinkProps_h__
#define __SinkProps_h__

#include "PropertyPagesContainer.h"

class SpecifySinkProps;
class DeteleConfirmationPage;

class SinkProps
  : public PropertyPagesContainer {
public:
  typedef PropertyPagesContainer Base;
  SinkProps(WContainerWidget * parent = 0);

  void editSink(const WString & inputName, const WString & outputName, const WString & sinkName);

  Signal<WString, WString, WString> & sinkAdded();
  Signal<WString, WString, WString> & sinkDeleted();
  Signal<WString, WString, WString, const MsmSink*> & sinkChanged();

private:
  bool getChanges(MsmSink * sink);
  void updateControls();
  void onPageSelectionChanged(WWidget * prev, WWidget * curent);
  void onSaveChanges();
  void onDeleteConfirmed(bool force);
  void onDeleteCancelled();

private:
  MsmSink * sink;
  WString inputName, outputName, sinkName;
  WWidget * previousPage;
  SpecifySinkProps * specifySinkProps;
  DeteleConfirmationPage * confirmDelete;
  Signal<WString, WString, WString> sinkAdded_;
  Signal<WString, WString, WString> sinkDeleted_;
  Signal<WString, WString, WString, const MsmSink*> sinkChanged_;
};




#endif /* __SinkProps_h__ */
