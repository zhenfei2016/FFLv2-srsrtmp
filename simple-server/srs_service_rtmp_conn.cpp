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

#include "srs_service_rtmp_conn.hpp"

//#include <unistd.h>
using namespace std;

#include <srs_protocol_kbps.hpp>
#include <srs_rtmp_stack.hpp>
#include "srs_service_io.hpp"
#include <srs_protocol_amf0.hpp>
#include <srs_protocol_utility.hpp>
#include "srs_service_utility.hpp"

SrsBasicRtmpClient::SrsBasicRtmpClient(string u, int64_t ctm, int64_t stm)
{
    kbps = new SrsKbps();
    
    url = u;
    connect_timeout = ctm;
    stream_timeout = stm;
    
    req = new SrsRequest();
    srs_parse_rtmp_url(url, req->tcUrl, req->stream);
    srs_discovery_tc_url(req->tcUrl, req->schema, req->host, req->vhost, req->app, req->port, req->param);
    
    transport = NULL;
    client = NULL;
    
    stream_id = 0;
}

SrsBasicRtmpClient::~SrsBasicRtmpClient()
{
    close();
    srs_freep(kbps);
}

int SrsBasicRtmpClient::connect()
{
    int ret = ERROR_SUCCESS;
    
    close();
    
    transport = new SrsTcpClient(req->host, req->port, connect_timeout);
    client = new SrsRtmpClient(transport);
    kbps->set_io(transport, transport);
    
    if ((ret = transport->connect()) != ERROR_SUCCESS) {
        close();
        return ret;
    }
    
    client->set_recv_timeout(stream_timeout);
    client->set_send_timeout(stream_timeout);
    
    // connect to vhost/app
    if ((ret = client->handshake()) != ERROR_SUCCESS) {
        srs_error("sdk: handshake with server failed. ret=%d", ret);
        return ret;
    }
    if ((ret = connect_app()) != ERROR_SUCCESS) {
        srs_error("sdk: connect with server failed. ret=%d", ret);
        return ret;
    }
    if ((ret = client->create_stream(stream_id)) != ERROR_SUCCESS) {
        srs_error("sdk: connect with server failed, stream_id=%d. ret=%d", stream_id, ret);
        return ret;
    }
    
    return ret;
}

void SrsBasicRtmpClient::close()
{
    kbps->set_io(NULL, NULL);
    srs_freep(client);
    srs_freep(transport);
}

int SrsBasicRtmpClient::connect_app()
{
    return do_connect_app(srs_get_public_internet_address(), false);
}

int SrsBasicRtmpClient::do_connect_app(string local_ip, bool debug)
{
    int ret = ERROR_SUCCESS;
    
    // args of request takes the srs info.
    if (req->args == NULL) {
        req->args = SrsAmf0Any::object();
    }
    
    // notify server the edge identity,
    // @see https://github.com/ossrs/srs/issues/147
    SrsAmf0Object* data = req->args;
    data->set("srs_sig", SrsAmf0Any::str(RTMP_SIG_SRS_KEY));
    data->set("srs_server", SrsAmf0Any::str(RTMP_SIG_SRS_SERVER));
    data->set("srs_license", SrsAmf0Any::str(RTMP_SIG_SRS_LICENSE));
    data->set("srs_role", SrsAmf0Any::str(RTMP_SIG_SRS_ROLE));
    data->set("srs_url", SrsAmf0Any::str(RTMP_SIG_SRS_URL));
    data->set("srs_version", SrsAmf0Any::str(RTMP_SIG_SRS_VERSION));
    data->set("srs_site", SrsAmf0Any::str(RTMP_SIG_SRS_WEB));
    data->set("srs_email", SrsAmf0Any::str(RTMP_SIG_SRS_EMAIL));
    data->set("srs_copyright", SrsAmf0Any::str(RTMP_SIG_SRS_COPYRIGHT));
    data->set("srs_primary", SrsAmf0Any::str(RTMP_SIG_SRS_PRIMARY));
    data->set("srs_authors", SrsAmf0Any::str(RTMP_SIG_SRS_AUTHROS));
    // for edge to directly get the id of client.
    data->set("srs_pid", SrsAmf0Any::number(getpid()));
    data->set("srs_id", SrsAmf0Any::number(_srs_context->get_id()));
    
    // local ip of edge
    data->set("srs_server_ip", SrsAmf0Any::str(local_ip.c_str()));
    
    // generate the tcUrl
    std::string param = "";
    std::string target_vhost = req->vhost;
    std::string tc_url = srs_generate_tc_url(req->host, req->vhost, req->app, req->port, param);
    
    // replace the tcUrl in request,
    // which will replace the tc_url in client.connect_app().
    req->tcUrl = tc_url;
    
    // upnode server identity will show in the connect_app of client.
    // @see https://github.com/ossrs/srs/issues/160
    // the debug_srs_upnode is config in vhost and default to true.
    if ((ret = client->connect_app(req->app, tc_url, req, debug, NULL)) != ERROR_SUCCESS) {
        srs_error("sdk: connect with server failed, tcUrl=%s, dsu=%d. ret=%d",
                  tc_url.c_str(), debug, ret);
        return ret;
    }
    
    return ret;
}

int SrsBasicRtmpClient::publish()
{
    int ret = ERROR_SUCCESS;
    
    // publish.
    if ((ret = client->publish(req->stream, stream_id)) != ERROR_SUCCESS) {
        srs_error("sdk: publish failed, stream=%s, stream_id=%d. ret=%d",
                  req->stream.c_str(), stream_id, ret);
        return ret;
    }
    
    return ret;
}

int SrsBasicRtmpClient::play()
{
    int ret = ERROR_SUCCESS;
    
    if ((ret = client->play(req->stream, stream_id)) != ERROR_SUCCESS) {
        srs_error("connect with server failed, stream=%s, stream_id=%d. ret=%d",
                  req->stream.c_str(), stream_id, ret);
        return ret;
    }
    
    return ret;
}

void SrsBasicRtmpClient::kbps_sample(const char* label, int64_t age)
{
    kbps->sample();
    
    int sr = kbps->get_send_kbps();
    int sr30s = kbps->get_send_kbps_30s();
    int sr5m = kbps->get_send_kbps_5m();
    int rr = kbps->get_recv_kbps();
    int rr30s = kbps->get_recv_kbps_30s();
    int rr5m = kbps->get_recv_kbps_5m();
    
    srs_trace("<- %s time=%" PRId64 ", okbps=%d,%d,%d, ikbps=%d,%d,%d", label, age, sr, sr30s, sr5m, rr, rr30s, rr5m);
}

void SrsBasicRtmpClient::kbps_sample(const char* label, int64_t age, int msgs)
{
    kbps->sample();
    
    int sr = kbps->get_send_kbps();
    int sr30s = kbps->get_send_kbps_30s();
    int sr5m = kbps->get_send_kbps_5m();
    int rr = kbps->get_recv_kbps();
    int rr30s = kbps->get_recv_kbps_30s();
    int rr5m = kbps->get_recv_kbps_5m();
    
    srs_trace("<- %s time=%" PRId64 ", msgs=%d, okbps=%d,%d,%d, ikbps=%d,%d,%d", label, age, msgs, sr, sr30s, sr5m, rr, rr30s, rr5m);
}

int SrsBasicRtmpClient::sid()
{
    return stream_id;
}

int SrsBasicRtmpClient::recv_message(SrsCommonMessage** pmsg)
{
    return client->recv_message(pmsg);
}

int SrsBasicRtmpClient::decode_message(SrsCommonMessage* msg, SrsPacket** ppacket)
{
    return client->decode_message(msg, ppacket);
}

int SrsBasicRtmpClient::send_and_free_messages(SrsSharedPtrMessage** msgs, int nb_msgs)
{
    return client->send_and_free_messages(msgs, nb_msgs, stream_id);
}

int SrsBasicRtmpClient::send_and_free_message(SrsSharedPtrMessage* msg)
{
    return client->send_and_free_message(msg, stream_id);
}

void SrsBasicRtmpClient::set_recv_timeout(int64_t timeout)
{
    transport->set_recv_timeout(timeout);
}

