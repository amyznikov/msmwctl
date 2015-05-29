/*
 * MainForm.cc
 *
 *  Created on: Mar 27, 2015
 *      Author: amyznikov
 */

#include "msmwctl.h"
#include "MainForm.h"
#include "LeftPane.h"
#include "RightPane.h"

MainForm::MainForm(WContainerWidget * parent)
    : Base(parent)
{
  //decorationStyle().setBorder(WBorder(WBorder::Solid, WBorder::Thin, Wt::red));
  hbox = new WHBoxLayout(this);
  hbox->setContentsMargins(0,0,0,0);
  hbox->addWidget(leftPane = new LeftPane());
  hbox->addWidget(rightPane = new RightPane());
  hbox->setResizable(0, true, WLength(25, WLength::Percentage));

  leftPane->addInput().connect(rightPane, &RightPane::AddInput);
  leftPane->editInput().connect(rightPane, &RightPane::EditInput);
  leftPane->addOutput().connect(rightPane, &RightPane::AddOutput);
  leftPane->editOutput().connect(rightPane, &RightPane::EditOutput);
  leftPane->addSink().connect(rightPane, &RightPane::AddSink);
  leftPane->editSink().connect(rightPane, &RightPane::EditSink);
  leftPane->streamsRootSelected().connect(rightPane, &RightPane::showStreams);
  leftPane->nothingSelected().connect(rightPane, &RightPane::showNothingSelected);

  rightPane->inputAdded().connect(leftPane, &LeftPane::onInputAdded);
  rightPane->inputDeleted().connect(leftPane, &LeftPane::onInputDeleted);
  rightPane->inputChanged().connect(leftPane, &LeftPane::onInputChanged);

  rightPane->outputAdded().connect(leftPane, &LeftPane::onOutputAdded);
  rightPane->outputDeleted().connect(leftPane, &LeftPane::onOutputDeleted);
  rightPane->outputChanged().connect(leftPane, &LeftPane::onOutputChanged);

  rightPane->sinkAdded().connect(leftPane, &LeftPane::onSinkAdded);
  rightPane->sinkDeleted().connect(leftPane, &LeftPane::onSinkDeleted);
  rightPane->sinkChanged().connect(leftPane, &LeftPane::onSinkChanged);
}


