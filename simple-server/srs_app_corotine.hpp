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

#ifndef SRS_APP_ST_HPP
#define SRS_APP_ST_HPP

#include <srs_core.hpp>


#include <string>
#include <thread/FFL_Thread.hpp>
class ISrsCoroutineHandler;

/**
 * Each ST-coroutine must implements this interface,
 * to do the cycle job and handle some events.
 *
 * Thread do a job then terminated normally, it's a SrsOneCycleThread:
 *      class SrsOneCycleThread : public ISrsCoroutineHandler {
 *          public: SrsCoroutine trd;
 *          public: virtual srs_error_t cycle() {
 *              // Do something, then return this cycle and thread terminated normally.
 *          }
 *      };
 *
 * Thread has its inside loop, such as the RTMP receive thread:
 *      class SrsReceiveThread : public ISrsCoroutineHandler {
 *          public: SrsCoroutine* trd;
 *          public: virtual srs_error_t cycle() {
 *              while (true) {
 *                  // Check whether thread interrupted.
 *                  if ((err = trd->pull()) != srs_success) {
 *                      return err;
 *                  }
 *                  // Do something, such as st_read() packets, it'll be wakeup
 *                  // when user stop or interrupt the thread.
 *              }
 *          }
 *      };
 */
class ISrsCoroutineHandler
{
public:
    ISrsCoroutineHandler();
    virtual ~ISrsCoroutineHandler();
public:
    /**
     * Do the work. The ST-coroutine will terminated normally if it returned.
     * @remark If the cycle has its own loop, it must check the thread pull.
     */
    virtual srs_error_t cycle() = 0;
};

/**
 * The corotine object.
 */
class SrsCoroutine
{
public:
    SrsCoroutine();
    virtual ~SrsCoroutine();
public:
    virtual srs_error_t start() = 0;
    virtual void stop() = 0;
    virtual void interrupt() = 0;
    // @return a copy of error, which should be freed by user.
    //      NULL if not terminated and user should pull again.
    virtual srs_error_t pull() = 0;
    virtual int cid() = 0;
};

/**
 * An empty coroutine, user can default to this object before create any real coroutine.
 * @see https://github.com/ossrs/srs/pull/908
 */
class SrsDummyCoroutine : public SrsCoroutine
{
public:
    SrsDummyCoroutine();
    virtual ~SrsDummyCoroutine();
public:
    virtual srs_error_t start();
    virtual void stop();
    virtual void interrupt();
    virtual srs_error_t pull();
    virtual int cid();
};


class CoroutineThread;
class SrsThreadCoroutine : public SrsCoroutine
{
public:
	SrsThreadCoroutine(std::string name,ISrsCoroutineHandler* handler);
	virtual ~SrsThreadCoroutine();
public:
	virtual srs_error_t start() ;
	virtual void stop();
	virtual void interrupt();
	// @return a copy of error, which should be freed by user.
	//      NULL if not terminated and user should pull again.
	virtual srs_error_t pull();
	virtual int cid() ;
private:
	FFL::sp<CoroutineThread> mThread;
	ISrsCoroutineHandler* m_handler;

	int m_stop;
};
#endif

