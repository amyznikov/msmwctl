/*
 * RightPane.cc
 *
 *  Created on: Mar 28, 2015
 *      Author: amyznikov
 */
#include "msmwctl.h"
#include "RightPane.h"
#include "StreamListView.h"
#include "InputProps.h"
#include "OutputProps.h"
#include "SinkProps.h"
#include "ServerLogViewer.h"

RightPane::RightPane(WContainerWidget * parent)
    : Base(parent)
{
  addStyleClass("rightpane");

  vbox = new WVBoxLayout(this);
  vbox->setContentsMargins(0, 0, 0, 0);
  vbox->addWidget(stack = new WStackedWidget());
  stack->addWidget(streamListView = new StreamListView());
  stack->addWidget(inputProps = new InputProps());
  stack->addWidget(outputProps = new OutputProps());
  stack->addWidget(sinkProps = new SinkProps());
  stack->setCurrentIndex(-1);
}

void RightPane::AddInput()
{
  inputProps->editInput("");
  stack->setCurrentWidget(inputProps);
}

void RightPane::EditInput(const WString & inputName)
{
  inputProps->editInput(inputName);
  stack->setCurrentWidget(inputProps);
}

void RightPane::AddOutput(const WString & inputName)
{
  outputProps->editOutput(inputName, "");
  stack->setCurrentWidget(outputProps);
}

void RightPane::EditOutput(const WString & inputName, const WString & outputName)
{
  outputProps->editOutput(inputName, outputName);
  stack->setCurrentWidget(outputProps);
}

void RightPane::AddSink(const WString & inputName, const WString & outputName)
{
  sinkProps->editSink(inputName, outputName, "");
  stack->setCurrentWidget(sinkProps);
}

void RightPane::EditSink(const WString & inputName, const WString & outputName, const WString & sinkName)
{
  sinkProps->editSink(inputName, outputName, sinkName);
  stack->setCurrentWidget(sinkProps);
}

void RightPane::showStreams()
{
  stack->setCurrentWidget(streamListView);
  streamListView->refreshStreams();
}

void RightPane::showNothingSelected()
{
  // stack->setCurrentWidget(nullptr);
  stack->setCurrentIndex(-1);
}


Signal<WString> & RightPane::inputAdded() {
  return inputProps->inputAdded();
}

Signal<WString> & RightPane::inputDeleted() {
  return inputProps->inputDeleted();
}

Signal<WString,const MsmInput*> & RightPane::inputChanged() {
  return inputProps->inputChanged();
}

Signal<WString,WString> & RightPane::outputAdded() {
  return outputProps->outputAdded();
}

Signal<WString, WString> & RightPane::outputDeleted() {
  return outputProps->outputDeleted();
}

Signal<WString, WString, const MsmOutput*> & RightPane::outputChanged() {
  return outputProps->outputChanged();
}

Signal<WString, WString, WString> & RightPane::sinkAdded() {
  return sinkProps->sinkAdded();
}

Signal<WString, WString, WString> & RightPane::sinkDeleted() {
  return sinkProps->sinkDeleted();
}

Signal<WString, WString, WString, const MsmSink*> & RightPane::sinkChanged() {
  return sinkProps->sinkChanged();
}
