/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2013-2017 OSSRS(winlin)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "srs_app_corotine.hpp"


#include <string>
using namespace std;

#include <srs_kernel_error.hpp>
#include <srs_kernel_log.hpp>

#include <FFL.h>
#include <thread/FFL_Thread.hpp>

ISrsCoroutineHandler::ISrsCoroutineHandler()
{
}

ISrsCoroutineHandler::~ISrsCoroutineHandler()
{
}

SrsCoroutine::SrsCoroutine()
{
}

SrsCoroutine::~SrsCoroutine()
{
}

SrsDummyCoroutine::SrsDummyCoroutine()
{
}

SrsDummyCoroutine::~SrsDummyCoroutine()
{
}

srs_error_t SrsDummyCoroutine::start()
{
    return srs_error_new(ERROR_THREAD_DUMMY, "dummy coroutine");
}

void SrsDummyCoroutine::stop()
{
}

void SrsDummyCoroutine::interrupt()
{
}

srs_error_t SrsDummyCoroutine::pull()
{
    return srs_error_new(ERROR_THREAD_DUMMY, "dummy pull");
}

int SrsDummyCoroutine::cid()
{
    return 0;
}


class CoroutineThread : public  FFL::Thread
{
	friend class ThreadCoroutine;
public:
	CoroutineThread(ISrsCoroutineHandler* handler) :mHandler(handler)
	{
	}

	virtual bool threadLoop()
	{
		do {

		} while (mHandler->cycle() !=NULL);

		return false;
	}
private:
	ISrsCoroutineHandler* mHandler;
};

SrsThreadCoroutine::SrsThreadCoroutine(std::string name, ISrsCoroutineHandler* handler)
{
	m_handler = handler;	
	mThread = new CoroutineThread(handler);
	m_stop = 0;
}

SrsThreadCoroutine::~SrsThreadCoroutine()
{
}

srs_error_t SrsThreadCoroutine::start()
{	
	mThread->run();
	return srs_success;
}

void SrsThreadCoroutine::stop()
{
	m_stop = 1;
	mThread->requestExitAndWait();
}

void SrsThreadCoroutine::interrupt()
{
	m_stop = 1;
	mThread->requestExit();
}

srs_error_t SrsThreadCoroutine::pull()
{
	//return srs_error_new(ERROR_THREAD_DUMMY, "dummy pull");
	//
	//return mThread->exitPending() ? FFL_ERROR_FAILED : FFL_NO_ERROR; 
	return m_stop==0?srs_success: srs_error_new(ERROR_THREAD_DUMMY, "dummy pull");
}

int SrsThreadCoroutine::cid()
{
	return 0;
}

