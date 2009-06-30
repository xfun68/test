// Light weight thread class interface.
// Copyright(C) 2009 LineKong.com
// @file thread.h
// @author: Benjamin
// @date: 2009-05-02

#ifndef _UOS_THREAD_H
#define _UOS_THREAD_H 

#include <pthread.h>
#ifndef ULONG_MAX
#	define ULONG_MAX 0xFFFFFFFF
#endif

class WaitCond
{
public:
        WaitCond();
        ~WaitCond();
        void    wakeOne();
        void    wakeAll();
        bool    wait(unsigned long time);

private:
        void    lock();
        void    unlock();
        void    init();
        void    cleanup();

private:
        pthread_cond_t  _cond;
        pthread_mutex_t _mutex;

};

class Thread
{
public:
	Thread():_thr_id(0),_running(false),_finished(false){}
	virtual ~Thread(){ _thr_id = 0; }

	void start();

	bool wait(unsigned int time = ULONG_MAX);

	bool running() const;
	bool finished() const;

protected:
	virtual void run() = 0;

	static void sleep( unsigned long );
	static void msleep( unsigned long );
	static void usleep( unsigned long );

private:
	pthread_t	_thr_id;
	WaitCond	_thr_done;

	bool	_running;
	bool	_finished;
	
public:
	void interRun();
};

#endif//_HYFTHREAD_H
