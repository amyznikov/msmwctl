/*
 * InputProps.h
 *
 *  Created on: Mar 28, 2015
 *      Author: amyznikov
 */

#ifndef __inputprops_h__
#define __inputprops_h__

#include "PropertyPagesContainer.h"

class MediaPreviewPage;
class SpecifyExpertInputProps;
class ProbeInputPage;
class DeteleConfirmationPage;

class InputProps
  : public PropertyPagesContainer {
public:
  typedef PropertyPagesContainer Base;
  InputProps( WContainerWidget * parent = 0);
  ~InputProps();

  void editInput(const WString & name);

  Signal<WString> & inputAdded();
  Signal<WString> & inputDeleted();
  Signal<WString, const MsmInput *> & inputChanged();

private:
  bool getChanges(MsmInput * input);
  void updateControls();
  void onPageSelectionChanged(WWidget * prev, WWidget * curent);
  void onSaveChanges();
  void onDeleteConfirmed(bool force);
  void onDeleteCancelled();

private:
  MsmInput * input;
  std::string inputName;
  WWidget * previousPage;
  //MediaPreviewPage * mediaPreview;
  ProbeInputPage * probeInputPage;
  SpecifyExpertInputProps * specifyExpertInputProps;
  DeteleConfirmationPage * confirmDelete;

  Signal<WString> inputAdded_;
  Signal<WString> inputDeleted_;
  Signal<WString, const MsmInput *> inputChanged_;
};





#endif /* __inputprops_h__ */
