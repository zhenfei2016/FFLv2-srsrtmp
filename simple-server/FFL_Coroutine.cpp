/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  FFL_Coroutine.cpp
*  Created by zhufeifei(34008081@qq.com) on 2018/03/04
*  https://github.com/zhenfei2016/FFLv2-lib.git
*
*
*/

#include <thread/FFL_Coroutine.hpp>
#include <thread/FFL_Thread.hpp>

namespace FFL {
	

	ThreadCoroutine::ThreadCoroutine(ICoroutineHandler* h):mHandler(h)
	{
		mThread=new Coroutine(mHandler);
	}
	ThreadCoroutine::~ThreadCoroutine()
	{

	}

	status_t ThreadCoroutine::start()
	{	
		return mThread->run();
	}
	void ThreadCoroutine::stop()
	{		
	    mThread->requestExitAndWait();
	}

	void ThreadCoroutine::interrupt()
	{		
		mThread->requestExit();
	}

	status_t ThreadCoroutine::pull()
	{
		return mThread->exitPending()?FFL_ERROR_FAILED:FFL_NO_ERROR;

	}

	int ThreadCoroutine::cid()
	{
		return 0;
	}

};



