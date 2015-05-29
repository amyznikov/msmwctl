/*
 * msm.cc
 *
 *  Created on: Mar 28, 2015
 *      Author: amyznikov
 */

#include "msmwctl.h"
#include <pthread.h>

static const std::string msm_address = "localhost:7002";
static std::string media_address;


static std::vector<msm_event_listener*> event_listeners;
static pthread_t msm_event_listener_thread_id;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;




//void setMsmAddress(const std::string & address)
//{
//  msm_address = address;
//}

const std::string & getMsmAddress()
{
  return msm_address;
}

void setMediaServerAddress(const std::string & address)
{
  media_address = address;
}

const std::string & getMediaServerAddress()
{
  return media_address;
}


double qp2q(double qp)
{
  double q = 100 * (1 - qp / 59);
  if ( q < 0 ) {
    q = 0;
  }
  else if ( q > 100 ) {
    q = 100;
  }
  return q;
}

double q2qp(double q)
{
  double qp = (100 - q) * 59 / 100;
  if ( qp < 1 ) {
    qp = 1;
  }
  else if ( qp > 59 ) {
    qp = 59;
  }
  return qp;
}

static int msm_event_listener_callback(msm_connection * msm, void * opaque, const struct msm_event * event)
{
  (void)(msm);
  (void)(opaque);

  pthread_mutex_lock(&mtx);

  for ( std::vector<msm_event_listener*>::iterator ii = event_listeners.begin(); ii != event_listeners.end(); ++ii ) {
    (*ii)->on_msm_event(event);
  }

  pthread_mutex_unlock(&mtx);
  return 0;
}

static void * msm_event_listener_thread(void * arg)
{
  (void)(arg);

  msm_connection * msm = NULL;

  while ( 42 ) {
    if ( msm_connection_status(msm = msm_connection_open(msm_address.c_str())) == msm_connection_established ) {
      msmctl_listen_events(msm, NULL, msm_event_listener_callback, NULL);
    }
    msm_connection_close(msm);
    sleep(1);
  }

  pthread_detach(pthread_self());
  return 0;
}


void msmRegisterEventListener(struct msm_event_listener * listener)
{
  const int lock = msm_event_listener_thread_id != pthread_self();
  if ( lock ) {
    pthread_mutex_lock(&mtx);
  }

  event_listeners.push_back(listener);

  if ( !msm_event_listener_thread_id ) {
    int status = pthread_create(&msm_event_listener_thread_id, NULL, msm_event_listener_thread, 0);
    if ( status ) {
      fprintf(stderr, "FATAL: pthread_create(msm_event_listener_thread) fails: %s\n", strerror(errno));
    }
  }

  if ( lock ) {
    pthread_mutex_unlock(&mtx);
  }
}

void msmUnRegisterEventListener(struct msm_event_listener * listener)
{
  const int lock = msm_event_listener_thread_id != pthread_self();
  if ( lock ) {
    pthread_mutex_lock(&mtx);
  }

  std::vector<msm_event_listener*>::iterator ii = std::find(event_listeners.begin(), event_listeners.end(), listener);
  if ( ii != event_listeners.end() ) {
    event_listeners.erase(ii);
    if ( event_listeners.empty() ) {
      // FIXME: stop listen
    }
  }

  if ( lock ) {
    pthread_mutex_unlock(&mtx);
  }
}
