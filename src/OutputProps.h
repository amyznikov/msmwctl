/*
 * OutputProps.h
 *
 *  Created on: Mar 28, 2015
 *      Author: amyznikov
 */

#ifndef __outputprops_h__
#define __outputprops_h__

#include "PropertyPagesContainer.h"

class MsmOutput;
class MediaPreviewPage;
class SpecifyExpertOutputProps;
class SpecifyDynamicAndStaticQuality;
class SpecifyQualityAndMaxRate;
class DeteleConfirmationPage;

class OutputProps
  : public PropertyPagesContainer {
public:
  typedef PropertyPagesContainer Base;
  OutputProps(WContainerWidget * parent = 0);
  ~OutputProps();

  void editOutput(const WString & inputName, const WString & outputName);

  Signal<WString, WString> & outputAdded();
  Signal<WString, WString> & outputDeleted();
  Signal<WString, WString, const MsmOutput*> & outputChanged();

private:
  bool getChanges(MsmOutput * output);
  void updateControls();
  void onPageSelectionChanged(WWidget * prev, WWidget * curent);
  void onSaveChanges();
  void onDeleteConfirmed(bool force);
  void onDeleteCancelled();

private:
  MsmOutput * output;
  WString inputName, outputName;
  WWidget * previousPage;
  MediaPreviewPage * mediaPreview;
  SpecifyDynamicAndStaticQuality * specifyDynamicAndStaticQuality;
  SpecifyQualityAndMaxRate * specifyQualityAndMaxRate;
  SpecifyExpertOutputProps * specifyExpertOutputProps;
  DeteleConfirmationPage * confirmDelete;

  Signal<WString, WString> outputAdded_;
  Signal<WString, WString> outputDeleted_;
  Signal<WString, WString, const MsmOutput*> outputChanged_;
};



#endif /* __outputprops_h__ */
