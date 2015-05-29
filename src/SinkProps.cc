/*
 * SinkProps.cc
 *
 *  Created on: Mar 28, 2015
 *      Author: amyznikov
 */

#include "msmwctl.h"
#include "SinkProps.h"
#include "SpecifySinkProps.h"
#include "DeteleConfirmationPage.h"



static void setMsmSinkDefaults(const WString & inputName, const WString & outputName, MsmSink * sink)
{
  *sink = MsmSink();
  sink->setInputName(inputName.toUTF8().c_str());
  sink->setOutputName(outputName.toUTF8().c_str());
  sink->setFormat("asf");
}


SinkProps::SinkProps(WContainerWidget * parent)
  : Base(parent), previousPage(nullptr)
{
  sink = new MsmSink();

  addPage("Edit properties", specifySinkProps = new SpecifySinkProps());
  addPage("Delete this sink...", confirmDelete = new DeteleConfirmationPage());

  setCurrentPage(specifySinkProps);
  showSaveChanges(false);

  specifySinkProps->enterPressed().connect(this, &SinkProps::onSaveChanges);
  confirmDelete->confirmed().connect(this, &SinkProps::onDeleteConfirmed);
  confirmDelete->cancelled().connect(this, &SinkProps::onDeleteCancelled);
}


Signal<WString, WString, WString> & SinkProps::sinkAdded() {
  return sinkAdded_;
}

Signal<WString, WString, WString> & SinkProps::sinkDeleted() {
  return sinkDeleted_;
}

Signal<WString, WString, WString, const MsmSink*> & SinkProps::sinkChanged() {
  return sinkChanged_;
}

void SinkProps::onPageSelectionChanged(WWidget * prev, WWidget * /*curent*/) {
  if ( prev != confirmDelete ) {
    previousPage = prev;
  }
  updateControls();
}

void SinkProps::editSink(const WString & inputName, const WString & outputName, const WString & sinkName)
{
  this->inputName = inputName, this->outputName = outputName, this->sinkName = sinkName;
  if ( sinkName.empty() || !msmGetSink(inputName, outputName,sinkName, sink) ) {
    setMsmSinkDefaults(inputName, outputName, sink);
    setCurrentPage(specifySinkProps);
  }
  updateControls();
}


void SinkProps::updateControls()
{
  WWidget * currentPage;

  if ( sinkName.empty() ) {
    setLegend(WString("<h3>Specify sink name and properties</h3>"));
  }
  else {
    setLegend(WString("<h3>{1}/{2}/{3}</h3>").arg(inputName).arg(outputName).arg(sinkName));
  }

  if ( (currentPage = getCurrentPage()) == specifySinkProps ) {
    specifySinkProps->setSinkProps(sink);
  }
  else if ( currentPage == confirmDelete ) {
    if ( outputName.empty() ) {
      confirmDelete->setWarningMessage(WString("This sink is not added yet"));
      confirmDelete->showButtons(false);
    }
    else {
      confirmDelete->setWarningMessage(
          WString("Are you sure to delete sink {1}/{2}/{3}?").arg(inputName).arg(outputName).arg(sinkName));
      confirmDelete->showButtons(true);
    }
  }

  showSaveChanges(currentPage == specifySinkProps);
}


bool SinkProps::getChanges(MsmSink * sink)
{
  WWidget * currentPage = getCurrentPage();
  if ( currentPage == specifySinkProps ) {
    return specifySinkProps->getChanges(sink);
  }
  return false;
}


void SinkProps::onSaveChanges()
{
  if ( getChanges(sink) ) {

    const WString oldName = sinkName;
    const char * newName;

    if ( !(newName = sink->getName()) || !*newName ) {
      WMessageBox::show("No Sink Name Specified", "Sink Name is mandatory", Wt::Ok);
    }
    else if ( oldName.empty() ) {    // add new input
      if ( msmAddSink(*sink) && msmGetSink(inputName, outputName, sinkName = sink->getName(), sink) ) {
        sinkAdded_.emit(inputName, outputName, sinkName);
      }
    }
    else {    // edit existing input
      if ( msmSetSink(inputName, outputName, oldName, *sink)
          && msmGetSink(inputName, outputName, sinkName = sink->getName(), sink) ) {
        sinkChanged_.emit(inputName, outputName, oldName, sink);
      }
    }

    updateControls();
  }
}


void SinkProps::onDeleteConfirmed(bool force)
{
  const WString inputName = this->inputName;
  const WString outputName = this->outputName;
  const WString sinkName = this->sinkName;

  if ( !outputName.empty() && msmRemoveSink(*sink, force) ) {
    sinkDeleted_.emit(inputName, outputName, sinkName);
    WMessageBox::show("Removed", WString("Sink {1}/{2}/{3} was removed").arg(inputName).arg(outputName).arg(sinkName),
        Wt::Ok);
    setCurrentPage(previousPage ? previousPage : specifySinkProps);
    updateControls();
  }
}

void SinkProps::onDeleteCancelled()
{
  setCurrentPage(previousPage ? previousPage : specifySinkProps);
  updateControls();
}
