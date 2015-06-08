/*
 * LeftPane.cc
 *
 *  Created on: Mar 28, 2015
 *      Author: amyznikov
 */

#include "msmwctl.h"
#include "LeftPane.h"


using std::string;
using std::vector;


static const char AddInput_Text[] = "+ add input..";
static const char AddOutput_Text[] = "+ add output..";
static const char AddSink_Text[] = "+ add sink..";

enum NodeType {
  NodeType_Unknown,
  NodeType_StreamsRoot,

  NodeType_Input,
  NodeType_AddInput,

  NodeType_Output,
  NodeType_AddOutput,

  NodeType_Sink,
  NodeType_AddSink,
};

struct TreeNode : public WTreeNode {
  NodeType type_;
public:
  TreeNode(const WString & name, NodeType type) : WTreeNode(name), type_(type)  {}
  NodeType getType() const { return type_;}
};



static WTreeNode * findNode(WTreeNode * parent, const WString & label)
{
  const std::vector<WTreeNode *>& childs = parent->childNodes();
  for ( std::vector<WTreeNode *>::const_iterator ii = childs.begin(); ii != childs.end(); ++ii ) {
    if ( (*ii)->label()->text() == label ) {
      return *ii;
    }
  }
  return NULL;
}

static WTreeNode * guessNextSelection(WTreeNode * parent, const WTreeNode * removedNode)
{
  WTreeNode * nextSelection = NULL;
  const std::vector<WTreeNode *>& childs = parent->childNodes();
  std::vector<WTreeNode *>::const_iterator ii = std::find(childs.begin(), childs.end(), removedNode);
  if ( ii > childs.begin() ) {
    nextSelection = *--ii;
  }
  else if ( ii < childs.end() - 1) {
    nextSelection = *++ii;
  }
  return nextSelection;
}

LeftPane::LeftPane(WContainerWidget * parent)
    : Base(parent)
{
  WStackedWidget * wrap;

  addStyleClass("leftpane");

  vbox = new WVBoxLayout(this);
  vbox->setContentsMargins(0, 0, 0, 0);

  vbox->addWidget(reloadStreams = new WPushButton("Reload streams..."), 0, Wt::AlignLeft | Wt::AlignTop);
  vbox->addSpacing(WLength(1, WLength::FontEx));
//  vbox->addWidget(streamsTreeView = new WTree(), 1, Wt::AlignTop);
//  streamsTreeView->addStyleClass("stree");

  vbox->addWidget(wrap = new WStackedWidget(), 100, Wt::AlignTop);
  wrap->addStyleClass("stree");
  wrap->addWidget(streamsTreeView = new WTree());
  wrap->setCurrentIndex(0);

  reloadStreams->clicked().connect(this, &LeftPane::onReloadClicked);

  streamsTreeView->setSelectable(true);
  streamsTreeView->setSelectionMode(Wt::SingleSelection);
  streamsTreeView->itemSelectionChanged().connect(this, &LeftPane::onTreeViewSelectionChanged);

  //setMinimumSize(WLength::Auto, WLength(100,WLength::FontEx));

  populateTree();
}


Signal<> & LeftPane::addInput() {
  return addInput_;
}

Signal<WString> & LeftPane::editInput() {
  return editInput_;
}

Signal<WString> & LeftPane::addOutput() {
  return addOutput_;
}

Signal<WString,WString> & LeftPane::editOutput() {
  return editOutput_;
}

Signal<WString,WString> & LeftPane::addSink() {
  return addSink_;
}

Signal<WString,WString,WString> & LeftPane::editSink() {
  return editSink_;
}

Signal<> & LeftPane::nothingSelected() {
  return nothingSelected_;
}

Signal<> & LeftPane::streamsRootSelected() {
  return streamsRootSelected_;
}

void LeftPane::onReloadClicked() {
  //setMsmAddress(addrsEdit->text().toUTF8());
  populateTree();
}

void LeftPane::onTreeViewSelectionChanged()
{
  const WTree::WTreeNodeSet & nodes = streamsTreeView->selectedNodes();
  if ( nodes.empty() ) {
    nothingSelected_.emit();
  }
  else {
    TreeNode * node = (TreeNode *) (*nodes.begin());
    switch( node->getType() )
    {
      case NodeType_StreamsRoot:
        streamsRootSelected_.emit();
        break;

      case NodeType_AddInput:
        addInput_.emit();
        break;

      case NodeType_Input:
        editInput_.emit(node->label()->text());
        break;

      case NodeType_AddOutput:
        addOutput_.emit(node->parentNode()->label()->text());
        break;

      case NodeType_Output:
        editOutput_.emit(node->parentNode()->label()->text(), node->label()->text());
        break;

      case NodeType_AddSink:
        addSink_.emit(node->parentNode()->parentNode()->label()->text(), node->parentNode()->label()->text());
        break;

      case NodeType_Sink:
        editSink_.emit(node->parentNode()->parentNode()->label()->text(), node->parentNode()->label()->text(),
            node->label()->text());
        break;

      default:
        break;
    }
  }
}

void LeftPane::clearTree()
{
  WTreeNode * node;

  streamsTreeView->hide();
  if ( (node = streamsTreeView->treeRoot()) ) {
    while ( node->childNodes().size() > 0 ) {
      node->removeChildNode(node->childNodes().back());
    }
  }
}


void LeftPane::populateTree()
{
  TreeNode * rootNode;
  TreeNode * inputNode;
  TreeNode * outputNode;
  TreeNode * sinkNode;

  clearTree();

  if ( getMsmAddress().empty() ) {
    return;
  }

  if ( !(rootNode = (TreeNode*)streamsTreeView->treeRoot()) ) {
    rootNode = new TreeNode("Streams", NodeType_StreamsRoot);
    rootNode->label()->setTextFormat(Wt::PlainText);
    streamsTreeView->setTreeRoot(rootNode);
  }


  std::vector<std::string> inputs;
  if ( msmLoadObjects(&inputs, "") ) {

    for ( std::vector<std::string>::const_iterator ii = inputs.begin(); ii != inputs.end(); ++ii ) {

      const string & inputName = *ii;
      rootNode->addChildNode(inputNode = new TreeNode(inputName, NodeType_Input));

      std::vector<std::string> outputs;
      if ( msmLoadObjects(&outputs, inputName + "/") ) {
        for ( std::vector<std::string>::const_iterator jj = outputs.begin(); jj != outputs.end(); ++jj ) {

          const string & outputName = *jj;
          inputNode->addChildNode(outputNode = new TreeNode(jj->substr(jj->find('/')+1), NodeType_Output));

          std::vector<std::string> sinks;
          if ( msmLoadObjects(&sinks, outputName + "/") ) {
            for ( std::vector<std::string>::const_iterator kk = sinks.begin(); kk != sinks.end(); ++kk ) {
              outputNode->addChildNode(sinkNode = new TreeNode(kk->substr(kk->rfind('/')+1), NodeType_Sink));
            }

            outputNode->addChildNode(sinkNode = new TreeNode(AddSink_Text, NodeType_AddSink));
          }

        }

        inputNode->addChildNode(outputNode = new TreeNode(AddOutput_Text, NodeType_AddOutput));
      }

    }
    rootNode->addChildNode(inputNode = new TreeNode(AddInput_Text, NodeType_AddInput));
  }

  streamsTreeView->show();
  rootNode->expand();
}



// insert nodes
void LeftPane::onInputAdded(const WString & inputName)
{
  WTreeNode * inputNode;
  inputNode = new TreeNode(inputName, NodeType_Input);
  inputNode->addChildNode(new TreeNode(AddOutput_Text, NodeType_AddOutput));
  streamsTreeView->treeRoot()->insertChildNode(streamsTreeView->treeRoot()->childNodes().size() - 1, inputNode);
  streamsTreeView->select(inputNode);
}

// remove node and move selection
void LeftPane::onInputDeleted(const WString & inputName)
{
  WTreeNode * root, * inputNode, * nextSelection;

  if ( !inputName.empty() && (inputNode = findNode(root = streamsTreeView->treeRoot(), inputName)) ) {

    nextSelection = guessNextSelection(root, inputNode);
    root->removeChildNode(inputNode);

    if ( nextSelection ) {
      streamsTreeView->select(nextSelection);
    }
  }
}


// check if node must be renamed
void LeftPane::onInputChanged(const WString & inputName, const MsmInput * input)
{
  if ( inputName != input->getName() ) {
    WTreeNode * inputNode = findNode(streamsTreeView->treeRoot(), inputName);
    if ( inputNode ) {
      inputNode->label()->setText(input->getName());
    }
  }
}


// insert nodes
void LeftPane::onOutputAdded(const WString & inputName, const WString & outputName)
{
  WTreeNode * inputNode, * outputNode, * sinkNode;
  if ( (inputNode = findNode(streamsTreeView->treeRoot(), inputName)) ) {
    outputNode = new TreeNode(outputName, NodeType_Output);
    inputNode->insertChildNode(inputNode->childNodes().size() - 1,outputNode);
    outputNode->addChildNode(sinkNode = new TreeNode(AddSink_Text, NodeType_AddSink));
    streamsTreeView->select(outputNode);
  }
}


// remove node and move selection
void LeftPane::onOutputDeleted(const WString & inputName, const WString & outputName)
{
  WTreeNode * inputNode, * outputNode, * nextSelection;

  if ( !inputName.empty() && (inputNode = findNode(streamsTreeView->treeRoot(), inputName)) ) {
    if ( !outputName.empty() && (outputNode = findNode(inputNode, outputName)) ) {

      nextSelection = guessNextSelection(inputNode, outputNode);
      inputNode->removeChildNode(outputNode);
      if ( nextSelection ) {
        streamsTreeView->select(nextSelection);
      }
    }
  }
}

// check if node must be renamed
void LeftPane::onOutputChanged(const WString & inputName, const WString & outputName, const MsmOutput * output)
{
  WTreeNode * inputNode, * outputNode;
  if ( outputName != output->getName() ) {
    if ( (inputNode = findNode(streamsTreeView->treeRoot(), inputName)) ) {
      if ( (outputNode = findNode(inputNode, outputName)) ) {
        outputNode->label()->setText(output->getName());
      }
    }
  }
}


// insert node
void LeftPane::onSinkAdded(const WString & inputName, const WString & outputName, const WString & sinkName)
{
  WTreeNode * inputNode, * outputNode, * sinkNode;
  if ( (inputNode = findNode(streamsTreeView->treeRoot(), inputName)) ) {
    if ( (outputNode = findNode(inputNode, outputName)) ) {
      sinkNode = new TreeNode(sinkName, NodeType_Sink);
      outputNode->insertChildNode(outputNode->childNodes().size() - 1, sinkNode);
      streamsTreeView->select(sinkNode);
    }
  }
}


// remove node and move selection
void LeftPane::onSinkDeleted(const WString & inputName, const WString & outputName, const WString & sinkName)
{
  WTreeNode * inputNode, * outputNode, * sinkNode, * nextSelection;

  if ( !inputName.empty() && (inputNode = findNode(streamsTreeView->treeRoot(), inputName)) ) {
    if ( !outputName.empty() && (outputNode = findNode(inputNode, outputName)) ) {
      if ( !sinkName.empty() && (sinkNode = findNode(outputNode, sinkName)) ) {

        nextSelection = guessNextSelection(outputNode, sinkNode);
        outputNode->removeChildNode(sinkNode);
        if ( nextSelection ) {
          streamsTreeView->select(nextSelection);
        }
      }
    }
  }
}

// check if node must be renamed
void LeftPane::onSinkChanged(WString inputName, WString outputName, WString sinkName, const MsmSink * sink)
{
  WTreeNode * inputNode, *outputNode, *sinkNode;
  if ( sinkName != sink->getName() ) {
    if ( (inputNode = findNode(streamsTreeView->treeRoot(), inputName)) ) {
      if ( (outputNode = findNode(inputNode, outputName)) ) {
        if ( (sinkNode = findNode(outputNode, sinkName)) ) {
          sinkNode->label()->setText(sink->getName());
        }
      }
    }
  }
}
