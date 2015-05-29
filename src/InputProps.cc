/*
 * InputProps.cc
 *
 *  Created on: Mar 28, 2015
 *      Author: amyznikov
 */
#include "msmwctl.h"
#include "InputProps.h"
#include "MediaPreviewPage.h"
#include "ProbeInputPage.h"
#include "SpecifyExpertInputProps.h"
#include "DeteleConfirmationPage.h"


static void setMsmInputDefaults(MsmInput * input)
{
  *input = MsmInput();
  input->setEnabled(true);
  input->setRateEmulationReferenceStream(-1);
  input->setFastProbe(true);
  input->setCtxOpts("-rtsp_transport tcp -rtsp_flags +prefer_tcp -fpsprobesize 0");
  input->setIgnoreErrors(true);
  input->setLive(true);
  input->setDecOpts("-threads 1");
}



InputProps::InputProps(WContainerWidget * parent)
  : Base(parent), previousPage(nullptr)
{
  input = new MsmInput();

  // addPage("Preview", mediaPreview = new MediaPreviewPage());
  addPage("Edit properties", specifyExpertInputProps = new SpecifyExpertInputProps());
  addPage("Probe input", probeInputPage = new ProbeInputPage());
  addPage("Delete this input...", confirmDelete = new DeteleConfirmationPage());

  setCurrentPage(specifyExpertInputProps);
  showSaveChanges(true);

  specifyExpertInputProps->enterPressed().connect(this, &InputProps::onSaveChanges);
  confirmDelete->confirmed().connect(this, &InputProps::onDeleteConfirmed);
  confirmDelete->cancelled().connect(this, &InputProps::onDeleteCancelled);
}

InputProps :: ~InputProps() {
  delete input;
}

Signal<WString, const MsmInput *> & InputProps::inputChanged() {
  return inputChanged_;
}

Signal<WString> & InputProps::inputAdded() {
  return inputAdded_;
}

Signal<WString> & InputProps::inputDeleted() {
  return inputDeleted_;
}

void InputProps::onPageSelectionChanged(WWidget * prev, WWidget * /*curent*/) {
  if ( prev != confirmDelete ) {
    previousPage = prev;
  }
  updateControls();
}

void InputProps::editInput(const WString & name)
{
  if ( (inputName = name.toUTF8()).empty() || !msmGetInput(name, input) ) {
    setMsmInputDefaults(input);
    setCurrentPage(specifyExpertInputProps);
  }
  updateControls();
}

void InputProps::updateControls()
{
  WWidget * currentPage;

  if ( inputName.empty() ) {
    setLegend(WString("<h3>Specify input name and properties</h3>"));
  }
  else {
    setLegend(WString("<h3>{1}</h3>").arg(inputName));
  }

  // mediaPreview->setMediaSource(NULL);
  specifyExpertInputProps->setInputProps(NULL);


  if ( (currentPage = getCurrentPage()) == specifyExpertInputProps ) {
    specifyExpertInputProps->setInputProps(input);
  }
  else if ( currentPage == confirmDelete ) {
    if ( inputName.empty() ) {
      confirmDelete->setWarningMessage(WString("This input is not added yet"));
      confirmDelete->showButtons(false);
    }
    else {
      confirmDelete->setWarningMessage(WString("Are you sure to delete input {1} and all it's outpust and sinks?").arg(inputName));
      confirmDelete->showButtons(true);
    }
  }
  else if ( currentPage == probeInputPage ) {
    probeInputPage->setInput(input->getName());
  }


//  else if ( currentPage == mediaPreview ) {
//    const char * source = input->getSource();
//    if ( source && *source && !strstr(source, "://") ) {
//      mediaPreview->setMediaSource((std::string("file://") + source).c_str());
//    }
//    else if ( !source || !*source ) {
//      WMessageBox::show("Caution:", "No video source URL specified", Wt::Ok);
//    }
//    else {
//      mediaPreview->setMediaSource(source);
//    }
//  }

  showSaveChanges(currentPage == specifyExpertInputProps);
}


bool InputProps::getChanges(MsmInput * input)
{
  WWidget * currentPage = getCurrentPage();
  if ( currentPage == specifyExpertInputProps ) {
    return specifyExpertInputProps->getChanges(input);
  }
  return false;
}

void InputProps::onSaveChanges()
{
  if ( getChanges(input) ) {

    const WString oldName = inputName;
    const char * newName;

    if ( !(newName = input->getName()) || !*newName ) {
      WMessageBox::show("No Input Name specified", "Input Name is mandatory", Wt::Ok);
    }
    else if ( oldName.empty() ) {    // add new input
      if ( msmAddInput(*input) && msmGetInput(inputName = input->getName(), input) ) {
        inputAdded_.emit(inputName);
      }
    }
    else {    // edit existing input
      if ( msmSetInput(oldName, *input) && msmGetInput(inputName = input->getName(), input) ) {
        inputChanged_.emit(oldName, input);
      }
    }
    updateControls();
  }
}

void InputProps::onDeleteConfirmed(bool force) {
  const WString inputName = this->inputName;
  if ( !inputName.empty() && msmRemoveInput(inputName, force) ) {
    inputDeleted_.emit(inputName);
    WMessageBox::show("Removed", WString("Input {1} was removed").arg(inputName), Wt::Ok);
    setCurrentPage(previousPage ? previousPage : specifyExpertInputProps);
    updateControls();
  }
}

void InputProps::onDeleteCancelled() {
  setCurrentPage(previousPage ? previousPage : specifyExpertInputProps);
  updateControls();
}
