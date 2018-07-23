/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_Coroutine   
*  Created by zhufeifei(34008081@qq.com) on 2018/03/04 
*  https://github.com/zhenfei2016/FFLv2-lib.git
*
*
*/
#ifndef _FFL_COROUTINE_HPP_
#define _FFL_COROUTINE_HPP_

#include <FFL.h>
#include <ref/FFL_Ref.hpp>

namespace FFL {

	class ICoroutineHandler
	{
	public:
		virtual status_t loop() = 0;
	};


	class ICoroutine
	{
	public:
		virtual status_t start() = 0;
		virtual void stop() = 0;
		virtual void interrupt() = 0;

		virtual status_t pull() = 0;
		virtual int cid() = 0;
	};


	class Thread;
	class ThreadCoroutine : public ICoroutine
	{
	public:
		ThreadCoroutine(ICoroutineHandler* h);
		virtual ~ThreadCoroutine();

		virtual status_t start();
		virtual void stop();
		virtual void interrupt();

		virtual status_t pull();
		virtual int cid();
	private:
		ICoroutineHandler* mHandler;
		sp<Thread> mThread;
	};

};

#endif

