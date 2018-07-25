#include <stdio.h>
#include <FFL.h>
#include <net/base/FFL_Net.h>
#include <net/FFL_NetServer.hpp>

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
int main()
{
	FFL_LogSetLevel(FFL_LOG_LEVEL_ALL);
	_srs_log = new ISrsLog();
	_srs_context = new ISrsThreadContext();
	_srs_config = new SrsConfig();
	
	FFL_socketInit();



	char buf[1024] = {};
	FFL_socketLocalAddr(buf, 1023);

//	FFL::TcpServer server(NULL, SRS_CONSTS_RTMP_DEFAULT_PORT);
//	server.setConnectManager(&mgr);
//	server.start();

	NetFD fd_server=-1;
	FFL_socketAnyAddrTcpServer(SRS_CONSTS_RTMP_DEFAULT_PORT,&fd_server);
	if (fd_server == -1)
	{
		printf("create serve fail.\n");
		return 0;

	}
	
	while (1) {
		
		std::string ip;
		int fd_client;
		FFL_socketAccept(fd_server, &fd_client);

		if (fd_client > 0) {

			char sz_ip[128] = {};
			FFL_socketLocalAddr(sz_ip,100);
			ip = sz_ip;
			ip = "127.0.0.1";
			SrsRtmpConn* conn = new SrsRtmpConn(0, new MySourceHandler(), fd_client, ip);
			conn->start();
		}


		FFL_sleep(1000);
	}

	FFL_socketUninit();
	return 0;

}