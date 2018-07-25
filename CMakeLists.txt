cmake_minimum_required(VERSION 3.4.1)

PROJECT(rtmp)

INCLUDE_DIRECTORIES(../install/FFLv2-lib/include/  ./include/core/  ./include/kernel/   ./include/protocol/ 
  )
LINK_DIRECTORIES( ../install/FFLv2-lib/libs/
 )

SET(LOCAL_SRC_FILES "")
FUNCTION(ADD_SRC_FILE arg)    
    SET(LOCAL_SRC_FILES "${LOCAL_SRC_FILES}${LOCAL_SRC_DIR}${ARGV0};" PARENT_SCOPE)
ENDFUNCTION()

if (MSVC)
    add_definitions("/wd4819")
    set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
    foreach(CompilerFlag ${CompilerFlags})
        string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach()
endif(MSVC)

SET(LOCAL_SRC_DIR ./include/core/)
ADD_SRC_FILE(srs_core.hpp)
ADD_SRC_FILE(srs_core_window.hpp)
ADD_SRC_FILE(srs_core_autofree.hpp)

SET(LOCAL_SRC_DIR ./src/core/)
ADD_SRC_FILE(srs_core.cpp)
ADD_SRC_FILE(srs_core_window.cpp)
ADD_SRC_FILE(srs_core_autofree.cpp)

SET(LOCAL_SRC_DIR ./src/kernel/)
ADD_SRC_FILE(srs_kernel_error.cpp)
ADD_SRC_FILE(srs_kernel_log.cpp)
ADD_SRC_FILE(srs_kernel_consts.cpp)


ADD_SRC_FILE(srs_kernel_io.cpp)
ADD_SRC_FILE(srs_kernel_file.cpp)
ADD_SRC_FILE(srs_kernel_stream.cpp)

ADD_SRC_FILE(srs_kernel_flv.cpp)
ADD_SRC_FILE(srs_kernel_utility.cpp)
ADD_SRC_FILE(srs_kernel_buffer.cpp)

ADD_SRC_FILE(srs_kernel_aac.cpp)
ADD_SRC_FILE(srs_kernel_codec.cpp)
ADD_SRC_FILE(srs_kernel_log.cpp)

# public
ADD_SRC_FILE(srs_kernel_mp4.cpp)

SET(LOCAL_SRC_DIR ./include/kernel/)
ADD_SRC_FILE(srs_kernel_error.hpp)
ADD_SRC_FILE(srs_kernel_log.hpp)
ADD_SRC_FILE(srs_kernel_consts.hpp)

ADD_SRC_FILE(srs_kernel_io.hpp)
ADD_SRC_FILE(srs_kernel_file.hpp)
ADD_SRC_FILE(srs_kernel_stream.hpp)


ADD_SRC_FILE(srs_kernel_flv.hpp)
ADD_SRC_FILE(srs_kernel_utility.hpp)
ADD_SRC_FILE(srs_kernel_buffer.hpp)

ADD_SRC_FILE(srs_kernel_aac.hpp)
ADD_SRC_FILE(srs_kernel_codec.hpp)
ADD_SRC_FILE(srs_kernel_log.hpp)

# public
ADD_SRC_FILE(srs_kernel_mp4.hpp)



SET(LOCAL_SRC_DIR ./include/protocol/)
ADD_SRC_FILE(srs_protocol_io.hpp)
ADD_SRC_FILE(srs_protocol_stream.hpp)

ADD_SRC_FILE(srs_protocol_utility.hpp)
ADD_SRC_FILE(srs_protocol_amf0.hpp)

ADD_SRC_FILE(srs_rtmp_msg_array.hpp)
ADD_SRC_FILE(srs_rtmp_handshake.hpp)
ADD_SRC_FILE(srs_rtmp_stack.hpp)
ADD_SRC_FILE(srs_protocol_kbps.hpp)
ADD_SRC_FILE(srs_protocol_json.hpp)
ADD_SRC_FILE(srs_protocol_format.hpp)

# public
ADD_SRC_FILE(srs_raw_avc.hpp)

SET(LOCAL_SRC_DIR ./src/protocol/)
ADD_SRC_FILE(srs_protocol_io.cpp)
ADD_SRC_FILE(srs_protocol_stream.cpp)

ADD_SRC_FILE(srs_protocol_utility.cpp)
ADD_SRC_FILE(srs_protocol_amf0.cpp)

ADD_SRC_FILE(srs_rtmp_msg_array.cpp)
ADD_SRC_FILE(srs_rtmp_handshake.cpp)
ADD_SRC_FILE(srs_rtmp_stack.cpp)
ADD_SRC_FILE(srs_protocol_kbps.cpp)
ADD_SRC_FILE(srs_protocol_json.cpp)
ADD_SRC_FILE(srs_protocol_format.cpp)

# public
ADD_SRC_FILE(srs_raw_avc.cpp)



add_library( rtmp STATIC ${LOCAL_SRC_FILES})
TARGET_LINK_LIBRARIES(rtmp FFL )


#INCLUDE_DIRECTORIES(../FFLv2-lib/include/  ./include/core/  ./include/kernel/   ./include/protocol/ ./include/app/  ./include/service/) 
SET(LOCAL_SRC_FILES )

SET(LOCAL_SRC_DIR ./include/service/)
ADD_SRC_FILE(srs_service_conn.hpp)
ADD_SRC_FILE(srs_service_http_client.hpp)
ADD_SRC_FILE(srs_service_http_conn.hpp)
ADD_SRC_FILE(srs_service_log.hpp)
ADD_SRC_FILE(srs_service_st.hpp)
ADD_SRC_FILE(srs_service_utility.hpp)

SET(LOCAL_SRC_DIR ./src/service/)
ADD_SRC_FILE(srs_service_conn.cpp)
ADD_SRC_FILE(srs_service_http_client.cpp)
ADD_SRC_FILE(srs_service_http_conn.cpp)
ADD_SRC_FILE(srs_service_log.cpp)
ADD_SRC_FILE(srs_service_st.cpp)
ADD_SRC_FILE(srs_service_utility.cpp)

SET(LOCAL_SRC_DIR ./include/app/)
ADD_SRC_FILE(srs_lib_simple_socket.hpp)
ADD_SRC_FILE(srs_app_conn.hpp)
ADD_SRC_FILE(srs_app_reload.hpp)
ADD_SRC_FILE(srs_app_server.hpp)
ADD_SRC_FILE(srs_app_st.hpp)

ADD_SRC_FILE(srs_app_rtmp_conn.hpp)


ADD_SRC_FILE(srs_app_config.hpp)
#ADD_SRC_FILE(srs_app_bandwidth.hpp)
#ADD_SRC_FILE(srs_app_refer.hpp)
#ADD_SRC_FILE(srs_app_security.hpp)
ADD_SRC_FILE(srs_app_source.hpp)
ADD_SRC_FILE(srs_app_utility.hpp)
ADD_SRC_FILE(srs_app_pithy_print.hpp)
ADD_SRC_FILE(srs_app_recv_thread.hpp)
ADD_SRC_FILE(srs_app_thread.hpp)

SET(LOCAL_SRC_DIR ./src/app/)
ADD_SRC_FILE(srs_lib_simple_socket.cpp)

ADD_SRC_FILE(srs_app_rtmp_conn.cpp)

ADD_SRC_FILE(main.cpp)

ADD_SRC_FILE(srs_app_conn.cpp)
ADD_SRC_FILE(srs_app_reload.cpp)
ADD_SRC_FILE(srs_app_server.cpp)
ADD_SRC_FILE(srs_app_st.cpp)

ADD_SRC_FILE(srs_app_config.cpp)
#ADD_SRC_FILE(srs_app_bandwidth.cpp)
#ADD_SRC_FILE(srs_app_refer.cpp)
#ADD_SRC_FILE(srs_app_security.cpp)
ADD_SRC_FILE(srs_app_source.cpp)
ADD_SRC_FILE(srs_app_utility.cpp)
ADD_SRC_FILE(srs_app_pithy_print.cpp)
ADD_SRC_FILE(srs_app_recv_thread.cpp)
ADD_SRC_FILE(srs_app_thread.cpp)

#ADD_EXECUTABLE( server ${LOCAL_SRC_FILES})
#TARGET_LINK_LIBRARIES(server rtmp FFL )


SET(LOCAL_SRC_FILES )
INCLUDE_DIRECTORIES(../install/FFLv2-lib/include/  ./include/core/  ./include/kernel/   ./include/protocol/  ) 
SET(LOCAL_SRC_DIR ./simple-server/)

ADD_SRC_FILE(srs_socket_util.hpp)

#ADD_SRC_FILE(srs_lib_simple_socket.hpp)
#ADD_SRC_FILE(srs_lib_simple_socket.cpp)

ADD_SRC_FILE(srs_service_io.hpp)
ADD_SRC_FILE(srs_service_io.cpp)

ADD_SRC_FILE(srs_app_corotine.hpp)
ADD_SRC_FILE(srs_app_corotine.cpp)

ADD_SRC_FILE(srs_service_conn.hpp)
ADD_SRC_FILE(srs_service_conn.cpp)

ADD_SRC_FILE(srs_app_conn.hpp)
ADD_SRC_FILE(srs_app_conn.cpp)

ADD_SRC_FILE(srs_app_source.hpp)
ADD_SRC_FILE(srs_app_source.cpp)

ADD_SRC_FILE(srs_app_rtmp_conn.hpp)
ADD_SRC_FILE(srs_app_rtmp_conn.cpp)


ADD_SRC_FILE(srs_app_config.hpp)
ADD_SRC_FILE(srs_app_config.cpp)

ADD_SRC_FILE(srs_app_reload.hpp)

ADD_SRC_FILE(srs_app_statistic.hpp)
ADD_SRC_FILE(srs_app_statistic.cpp)

ADD_SRC_FILE(srs_service_utility.hpp)
ADD_SRC_FILE(srs_service_utility.cpp)

ADD_SRC_FILE(srs_service_rtmp_conn.hpp)
ADD_SRC_FILE(srs_service_rtmp_conn.cpp)

ADD_SRC_FILE(srs_app_recv_thread.hpp)
ADD_SRC_FILE(srs_app_recv_thread.cpp)

#ADD_SRC_FILE(srs_app_utility.hpp)
#ADD_SRC_FILE(srs_app_utility.cpp)

ADD_SRC_FILE(simpleserver.cpp)

ADD_SRC_FILE(srs_app_reload.hpp)
ADD_SRC_FILE(srs_app_reload.cpp)

#  Êä³öÂ·¾¶ bin
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

ADD_EXECUTABLE(rtmp_server ${LOCAL_SRC_FILES})
TARGET_LINK_LIBRARIES(rtmp_server rtmp FFL )

#ADD_SUBDIRECTORY(example)