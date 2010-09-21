// Light weight thread class interface.
// Copyright(C) 2009 LineKong.com
// @file thread.h
// @author: Benjamin
// @date: 2009-05-02

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>

#define	Warning	printf

#include "thread.h"

//////////////////////////////////////////////////////////////////////////
// class WaitCond members

WaitCond::WaitCond()
{
	init();

	pthread_condattr_t cattr;
	pthread_condattr_init( &cattr );

	int ret = pthread_cond_init( &_cond, &cattr );

	pthread_condattr_destroy( &cattr );

	if (ret) Warning("WaitCond::WaitCond: event init failure %s", strerror( ret ));
}

WaitCond::~WaitCond()
{
	int ret = pthread_cond_destroy( &_cond );

	if (ret)
	{
		pthread_cond_broadcast( &_cond );
	}

	cleanup();

}

void WaitCond::init()
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init( &attr );
	pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_DEFAULT );

	int ret = pthread_mutex_init( &_mutex, &attr );

	pthread_mutexattr_destroy( & attr );

	if (ret) Warning("WaitCond::init: mutex init failure: %s", strerror(ret));
}

void WaitCond::cleanup()
{
	int ret = pthread_mutex_destroy( &_mutex );

	if (ret) Warning("WaitCond::cleanup: mutex destroy failure: %s", strerror(ret));
}

void WaitCond::lock()
{
	int ret = pthread_mutex_lock( &_mutex );

	if (ret) Warning("WaitCond::lock: mutex lock failure: %s", strerror(ret));
}

void WaitCond::unlock()
{
	int ret = pthread_mutex_unlock( &_mutex );

	if (ret) Warning("WaitCond::unlock: mutex unlock failure: %s", strerror(ret));
}

void WaitCond::wakeOne()
{
	lock();
	int ret = pthread_cond_signal( &_cond );

	if (ret) Warning("WaitCond::wakeOne: wake error: %s",strerror(ret));

	unlock();
}

void WaitCond::wakeAll()
{
	lock();
	int ret = pthread_cond_broadcast( &_cond );
	
	if (ret) Warning("WaitCond::wakeAll: wake error: %s",strerror(ret));

	unlock();
}

bool WaitCond::wait(unsigned long time)
{
	lock();
	int ret;
	if (time != ULONG_MAX)
	{
		struct timeval tv;
		gettimeofday(&tv, 0);
		
		timespec ti;
		ti.tv_sec = tv.tv_sec + (time / 1000);
		ti.tv_nsec = (tv.tv_usec * 1000) + (time % 1000) * 1000000;
		
		ret = pthread_cond_timedwait( &_cond, &_mutex, &ti );
	}
	else
	{
		ret = pthread_cond_wait( &_cond, &_mutex );
	}
	unlock();
	
	if ( ret && ret != ETIMEDOUT )
	{
	    Warning("WaitCond::wait: wait error:%s",strerror(ret));
	}

	return (ret == 0);
}

//////////////////////////////////////////////////////////////////////////
// class Thread members

void* start_thread(void* p);


void Thread::start()
{
	if (_running)
	{
		return;
	}

	_running  = true;
	_finished = false;

#ifndef NO_DEBUG
	Warning("create thread\n");
#endif
	pthread_attr_t  attr;
	
	pthread_attr_init( &attr );
	pthread_attr_setinheritsched( &attr, PTHREAD_INHERIT_SCHED );
	pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );
	
	int ret = pthread_create( &_thr_id, &attr, start_thread, this);
	
	pthread_attr_destroy( &attr );
	
	if (ret)
	{
		_running  = false;
		_finished = false;
	}
}

bool Thread::wait(unsigned int time/* = ULONG_MAX */)
{
	if ( _finished || !_running )
	{
		return true;
	}

	return _thr_done.wait(time);
}

bool Thread::running() const
{
	return _running;
}

bool Thread::finished() const
{
	return _finished;
}

void Thread::sleep( unsigned long secs )
{
    ::sleep(secs);
}

void Thread::msleep( unsigned long msecs )
{
    ::usleep(msecs * 1000);
}

void Thread::usleep( unsigned long usecs )
{
    if ( usecs >= 1000000 )
	::sleep( usecs / 1000000 );
    ::usleep( usecs % 1000000 );
}

void Thread::interRun()
{
	run();
	Warning("over\n");

	_running  = false;
	_finished = true;

	_thr_done.wakeAll();
}

void* start_thread(void* p)
{
	((Thread*)p)->interRun();
	return 0;
}
