#include <stdio.h>
#include <FFL.h>
#include <net/base/FFL_Net.h>
#include <net/FFL_NetConnect.hpp>
#include <net/FFL_NetServer.hpp>
#include <net/FFL_NetConnectManager.hpp>

#include "srs_app_config.hpp"
#include "srs_kernel_log.hpp"
#include "srs_app_rtmp_conn.hpp"
#include "srs_app_source.hpp"

SrsConfig* _srs_config;
ISrsLog* _srs_log;
ISrsThreadContext* _srs_context;

class MySourceHandler : public ISrsSourceHandler
{
public:
	virtual int on_publish(SrsSource* s, SrsRequest* r) 
	{
		return 0;

	}
	/**
	* when stream stop publish, unmount stream.
	*/
	virtual void on_unpublish(SrsSource* s, SrsRequest* r)
	{

	}
};

//
//  一个rtmp连接
//
class RtmpConn : public FFL::NetConnect {
public:
	RtmpConn(NetFD fd,FFL::NetServer* srv) : FFL::NetConnect(fd){
	}

	virtual ~RtmpConn() {
	}
	virtual status_t onStart() {
		char ipBuf[128] = {};
		FFL_socketLocalAddr(ipBuf, 100);
		const char* ip = "127.0.0.1";
		mConn = new SrsRtmpConn(0, new MySourceHandler(), getFd(), ip);
		mConn->start();
		return FFL_OK;
	}	
	virtual void onStop() {
		
	}

	SrsRtmpConn* mConn;
};

int main()
{
	FFL_LogSetLevel(FFL_LOG_LEVEL_ALL);
	_srs_log = new ISrsLog();
	_srs_context = new ISrsThreadContext();
	_srs_config = new SrsConfig();

	//
	//  启动服务
	//
	FFL::SimpleConnectManager<RtmpConn> mgr;
	FFL::TcpServer server(NULL, SRS_CONSTS_RTMP_DEFAULT_PORT);
	server.setConnectManager(&mgr);
	server.start();

	while (1) {
		FFL_sleep(1000);
	}

	return 0;

}