/*
 * OutputProps.cc
 *
 *  Created on: Mar 28, 2015
 *      Author: amyznikov
 */
#include "msmwctl.h"
#include "OutputProps.h"
#include "MediaPreviewPage.h"
#include "SpecifyDynamicAndStaticQuality.h"
#include "SpecifyExpertOutputProps.h"
#include "SpecifyQualityAndMaxRate.h"
#include "DeteleConfirmationPage.h"


static void setMsmOutputDefaults(const WString & inputName, MsmOutput * output)
{
  *output = MsmOutput();
  output->setInputName(inputName.toUTF8().c_str());
  output->setAudioCodec("none");
  output->setEnabled(true);
  output->setFormat("asf");
  output->setStartOnKey(true);
  output->setVideoCodec("libx264");
  output->setVideoOpts("-crf 32");
}


OutputProps::OutputProps(WContainerWidget * parent)
  : Base(parent), previousPage(nullptr)
{
  output = new MsmOutput();

  addPage("Preview", mediaPreview = new MediaPreviewPage());
  addPage("Specify static and dynamic quality", specifyDynamicAndStaticQuality = new SpecifyDynamicAndStaticQuality());
  addPage("Specify quality and max rate", specifyQualityAndMaxRate = new SpecifyQualityAndMaxRate());
  addPage("Expert mode", specifyExpertOutputProps = new SpecifyExpertOutputProps());
  addPage("Delete this output...", confirmDelete = new DeteleConfirmationPage());

  setCurrentPage(specifyExpertOutputProps);
  showSaveChanges(true);

  specifyDynamicAndStaticQuality->enterPressed().connect(this, &OutputProps::onSaveChanges);
  specifyQualityAndMaxRate->enterPressed().connect(this, &OutputProps::onSaveChanges);
  specifyExpertOutputProps->enterPressed().connect(this, &OutputProps::onSaveChanges);

  confirmDelete->confirmed().connect(this, &OutputProps::onDeleteConfirmed);
  confirmDelete->cancelled().connect(this, &OutputProps::onDeleteCancelled);
}

OutputProps::~OutputProps() {
  delete output;
}

Signal<WString, WString> & OutputProps::outputAdded() {
  return outputAdded_;
}

Signal<WString, WString> & OutputProps::outputDeleted() {
  return outputDeleted_;
}

Signal<WString, WString, const MsmOutput*> & OutputProps::outputChanged() {
  return outputChanged_;
}

void OutputProps::onPageSelectionChanged(WWidget * prev, WWidget * /*curent*/) {
  if ( prev != confirmDelete ) {
    previousPage = prev;
  }
  updateControls();
}


void OutputProps::editOutput(const WString & inputName, const WString & outputName)
{
  this->inputName = inputName, this->outputName = outputName;
  if ( outputName.empty() || !msmGetOutput(inputName, outputName, output) ) {
    setMsmOutputDefaults(inputName, output);
    setCurrentPage(specifyDynamicAndStaticQuality);
  }
  updateControls();
}

void OutputProps::updateControls()
{
  WWidget * currentPage;

  if ( outputName.empty() ) {
    setLegend(WString("<h3>Specify output name and properties</h3>"));
  }
  else {
    setLegend(WString("<h3>{1}/{2}</h3>").arg(inputName).arg(outputName));
  }

  mediaPreview->setMediaSource(NULL);

  if ( (currentPage = getCurrentPage()) == specifyExpertOutputProps ) {
    specifyExpertOutputProps->setOutputProps(output);
  }
  else if ( currentPage == specifyDynamicAndStaticQuality ) {
    specifyDynamicAndStaticQuality->setOutputProps(output);
  }
  else if ( currentPage == specifyQualityAndMaxRate ) {
    specifyQualityAndMaxRate->setOutputProps(output);
  }
  else if ( currentPage == confirmDelete ) {
    if ( outputName.empty() ) {
      confirmDelete->setWarningMessage(WString("This output is not added yet"));
      confirmDelete->showButtons(false);
    }
    else {
      confirmDelete->setWarningMessage(
          WString("Are you sure to delete output {1}/{2} and all it's sinks?").arg(inputName).arg(outputName));
      confirmDelete->showButtons(true);
    }
  }
  else if ( currentPage == mediaPreview ) {
    const char * inputName = NULL, *outputName = NULL;
    char address[256] = "", url[PATH_MAX] = "";
    if ( (inputName = output->getInputName()) && (outputName = output->getName()) && *inputName && *outputName
        && sscanf(getMediaServerAddress().c_str(), "%255[^:]", address) == 1 ) {
      snprintf(url, sizeof(url) - 1, "http://%s:8082/%s/%s", address, inputName, outputName);
    }
    if ( *url ) {
      mediaPreview->setMediaSource(url);
    }
    else {
      WMessageBox::show("Caution:", "No video URL specified", Wt::Ok);
    }
  }

  if ( currentPage == mediaPreview || currentPage == confirmDelete ) {
    showSaveChanges(false);
  }
  else {
    showSaveChanges(true);
  }
}


bool OutputProps::getChanges(MsmOutput * output)
{
  WWidget * currentPage = getCurrentPage();
  if ( currentPage == specifyExpertOutputProps ) {
    return specifyExpertOutputProps->getChanges(output);
  }
  if ( currentPage == specifyDynamicAndStaticQuality ) {
    return specifyDynamicAndStaticQuality->getChanges(output);
  }
  if ( currentPage == specifyQualityAndMaxRate ) {
    return specifyQualityAndMaxRate->getChanges(output);
  }
  return false;
}

void OutputProps::onSaveChanges()
{
  if ( getChanges(output) ) {

    const WString oldName = outputName;
    const char * newName;

    if ( !(newName = output->getName()) || !*newName ) {
      WMessageBox::show("No Output Name Specified", "Output Name is mandatory", Wt::Ok);
    }
    else if ( oldName.empty() ) {    // add new input
      if ( msmAddOutput(*output) && msmGetOutput(inputName, outputName = output->getName(), output) ) {
        outputAdded_.emit(inputName, outputName);
      }
    }
    else {    // edit existing input
      if ( msmSetOutput(inputName, oldName, *output)
          && msmGetOutput(inputName, outputName = output->getName(), output) ) {
        outputChanged_.emit(inputName, oldName, output);
      }
    }

    updateControls();
  }
}

void OutputProps::onDeleteConfirmed(bool force)
{
  const WString inputName = this->inputName;
  const WString outputName = this->outputName;

  if ( !outputName.empty() && msmRemoveOutput(*output, force) ) {
    outputDeleted_.emit(inputName, outputName);
    WMessageBox::show("Removed", WString("Output {1}/{2} was removed").arg(inputName).arg(outputName), Wt::Ok);
    setCurrentPage(previousPage ? previousPage : specifyExpertOutputProps);
    updateControls();
  }
}

void OutputProps::onDeleteCancelled()
{
  setCurrentPage(previousPage ? previousPage : specifyExpertOutputProps);
  updateControls();
}
