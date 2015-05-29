/*
 * msm.h
 *
 *  Created on: Mar 28, 2015
 *      Author: amyznikov
 */

#ifndef __msm_h__
#define __msm_h__

#include <msmctl.h>
#include <string>

using namespace Wt;

//void setMsmAddress(const std::string & address);
const std::string & getMsmAddress();


void setMediaServerAddress(const std::string & address);
const std::string & getMediaServerAddress();



double qp2q(double q);
double q2qp(double qp);



inline bool openMsmConnection(MsmConnection * c) {
  const std::string & address = getMsmAddress();
  if ( !c->open(address.c_str()) ) {
    WMessageBox::show(WString("Failed to connect {1}").arg(address),WString("{1}").arg(c->error_message()), Wt::Ok);
    return false;
  }
  return true;
}



template<class _Cont>
bool msmLoadInputs(_Cont * cont) {
  MsmConnection c;
  bool Ok = openMsmConnection(&c);
  if ( Ok && !(Ok = MSM::loadInputs(c, cont)) ) {
    WMessageBox::show(WString("MSM::loadInputs() fails:"), WString("{1}").arg(c.error_message()), Wt::Ok);
  }
  return Ok;
}



template<class _Cont>
bool msmLoadOutputs(_Cont * cont, const WString & inputName = WString::Empty) {
  MsmConnection c;
  bool Ok = openMsmConnection(&c);
  if ( Ok && !(Ok = MSM::loadOutputs(c, cont, inputName.empty() ? NULL : inputName.toUTF8().c_str())) ) {
    WMessageBox::show(WString("MSM::loadOutputs() fails:"), WString("{1}").arg(c.error_message()), Wt::Ok);
  }
  return Ok;
}



template<class _Cont>
bool msmLoadSinks(_Cont * cont, const WString & ownedBy = WString::Empty) {
  MsmConnection c;
  bool Ok = openMsmConnection(&c);
  if ( Ok && !(Ok = MSM::loadSinks(c, cont, ownedBy.empty() ? NULL : ownedBy.toUTF8().c_str())) ) {
    WMessageBox::show(WString("MSM::loadSinks({1}) fails:").arg(ownedBy), WString("{1}").arg(c.error_message()),
        Wt::Ok);
  }
  return Ok;
}

template<class _Cont>
bool msmLoadSessions(_Cont * cont, const WString & ownedBy = WString::Empty) {
  MsmConnection c;
  bool Ok = openMsmConnection(&c);
  if ( Ok && !(Ok = MSM::loadSessions(c, cont, ownedBy.empty() ? NULL : ownedBy.toUTF8().c_str())) ) {
    WMessageBox::show(WString("MSM::loadSessions({1}) fails:").arg(ownedBy), WString("{1}").arg(c.error_message()),
        Wt::Ok);
  }
  return Ok;
}


template<class _Cont>
bool msmLoadObjects(_Cont * cont, const WString & prefix) {
  MsmConnection c;
  bool Ok = openMsmConnection(&c);
  if ( Ok && !(Ok = MSM::getObjectNames(c, prefix.toUTF8().c_str(), cont)) ) {
    WMessageBox::show(WString("getObjectNames() fails:"), WString("{1}").arg(c.error_message()), Wt::Ok);
  }
  return Ok;
}



inline bool msmAddInput(const MsmInput & input) {
  MsmConnection c;
  bool Ok = openMsmConnection(&c);
  if ( Ok && !(Ok = MSM::addInput(c, NULL, input)) ) {
    WMessageBox::show(WString("addInput('{1}') fails:").arg(input.getName()),
        WString("{1}").arg(c.error_message()), Wt::Ok);
  }
  return Ok;
}


inline bool msmGetInput(const WString & name, MsmInput * input ) {
  MsmConnection c;
  bool Ok = openMsmConnection(&c);
  if ( Ok && !(Ok = MSM::getInput(c, name.toUTF8().c_str(), input)) ) {
    WMessageBox::show(WString("getInput({1}) fails:").arg(name), WString("{1}").arg(c.error_message()),Wt::Ok);
  }
  return Ok;
}


inline bool msmSetInput(const WString & name, const MsmInput & input) {
  MsmConnection c;
  bool Ok = openMsmConnection(&c);
  if ( Ok && !(Ok = MSM::setInput(c, name.toUTF8().c_str(), input, true)) ) {
    WMessageBox::show(WString("setInput({1}) fails:").arg(name), WString("{1}").arg(c.error_message()), Wt::Ok);
  }
  return Ok;
}

inline bool msmRemoveInput(const WString & name, bool force) {
  MsmConnection c;
  bool Ok = openMsmConnection(&c);
  if ( Ok && !(Ok = MSM::removeInput(c, name.toUTF8().c_str(), force)) ) {
    WMessageBox::show(WString("removeInput({1}) fails:").arg(name), WString("{1}").arg(c.error_message()), Wt::Ok);
  }
  return Ok;
}

inline bool msmProbeInput(const WString & name, struct ffs ** ffs) {
  MsmConnection c;
  bool Ok = openMsmConnection(&c);
  if ( Ok && !(Ok = MSM::probeInput(c, name.toUTF8().c_str(), ffs)) ) {
    WMessageBox::show(WString("probeInput({1}) fails:").arg(name), WString("{1}").arg(c.error_message()), Wt::Ok);
  }
  return Ok;
}


inline bool msmAddOutput(const MsmOutput & output) {
  MsmConnection c;
  bool Ok = openMsmConnection(&c);
  if ( Ok && !(Ok = MSM::addOutput(c, NULL, output))) {
    WMessageBox::show(WString("addOutput('{1}') fails:").arg(output.getName()),
        WString("{1}").arg(c.error_message()), Wt::Ok);
  }
  return Ok;
}


inline bool msmGetOutput(const WString & inputName, const WString & outputName, MsmOutput * output ) {
  MsmConnection c;
  const WString name = WString("{1}/{2}").arg(inputName).arg(outputName);
  bool Ok = openMsmConnection(&c);
  if ( Ok && !(Ok = MSM::getOutput(c, name.toUTF8().c_str(), output)) ) {
    WMessageBox::show(WString("getOutput({1}) fails:").arg(name), WString("{1}").arg(c.error_message()), Wt::Ok);
  }
  return Ok;
}


inline bool msmSetOutput(const WString & inputName, const WString & outputName, const MsmOutput & output)
{
  MsmConnection c;
  const WString name = WString("{1}/{2}").arg(inputName).arg(outputName);
  bool Ok = openMsmConnection(&c);
  if ( Ok && !(Ok = MSM::setOutput(c, name.toUTF8().c_str(), output, true)) ) {
    WMessageBox::show(WString("setOutput({1}) fails:").arg(name), WString("{1}").arg(c.error_message()), Wt::Ok);
  }
  return Ok;
}

inline bool msmRemoveOutput(const MsmOutput & output, bool force) {
  MsmConnection c;
  bool Ok = openMsmConnection(&c);
  if ( Ok && !(Ok = MSM::removeOutput(c, output, force)) ) {
    WMessageBox::show(WString("removeOutput({1}/{2}) fails:").arg(output.getInputName()).arg(output.getName()),
        WString("{1}").arg(c.error_message()), Wt::Ok);
  }
  return Ok;
}



inline bool msmAddSink(const MsmSink & sink) {
  MsmConnection c;
  bool Ok = openMsmConnection(&c);
  if ( Ok && !(Ok = MSM::addSink(c, NULL, sink))) {
    WMessageBox::show(WString("addSink('{1}') fails:").arg(sink.getName()),
        WString("{1}").arg(c.error_message()), Wt::Ok);
  }
  return Ok;
}


inline bool msmGetSink(const WString & inputName, const WString & outputName, const WString & sinkName, MsmSink * sink) {
  MsmConnection c;
  const WString name = WString("{1}/{2}/{3}").arg(inputName).arg(outputName).arg(sinkName);
  bool Ok = openMsmConnection(&c);
  if ( Ok && !(Ok = MSM::getSink(c, name.toUTF8().c_str(), sink)) ) {
    WMessageBox::show(WString("MSM::getSink({1}) fails:").arg(name), WString("{1}").arg(c.error_message()), Wt::Ok);
  }
  return Ok;
}


inline bool msmSetSink(const WString & inputName, const WString & outputName, const WString & sinkName,
    const MsmSink & sink) {
  MsmConnection c;
  const WString name = WString("{1}/{2}/{3}").arg(inputName).arg(outputName).arg(sinkName);
  bool Ok = openMsmConnection(&c);
  if ( Ok && !(Ok = MSM::setSink(c, name.toUTF8().c_str(), sink, true)) ) {
    WMessageBox::show(WString("MSM::setSink({1}) fails:").arg(name), WString("{1}").arg(c.error_message()), Wt::Ok);
  }
  return Ok;
}

inline bool msmRemoveSink(const MsmSink & sink, bool force) {
  MsmConnection c;
  bool Ok = openMsmConnection(&c);
  if ( Ok && !(Ok = MSM::removeSink(c, sink, force)) ) {
    WMessageBox::show(WString("removeSink({1}/{2}/{3}) fails:")
        .arg(sink.getInputName()).arg(sink.getOutputName()).arg(sink.getName()),
        WString("{1}").arg(c.error_message()),
        Wt::Ok);
  }
  return Ok;
}


typedef /* interface */
struct msm_event_listener {
  virtual void on_msm_event(const struct msm_event * event) = 0;
} msm_event_listener;

void msmRegisterEventListener(struct msm_event_listener * listener);
void msmUnRegisterEventListener(struct msm_event_listener * listener);


#endif /* __msm_h__ */
