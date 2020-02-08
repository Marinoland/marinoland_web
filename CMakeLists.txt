cmake_minimum_required(VERSION 2.8)
project (web)


if(WIN32) 
    set(CONFIGURE_SCRIPT configure.bat)
    set(JSON_NAME libjson.dll)
else()
    set(CONFIGURE_SCRIPT ./configure.sh)
    set(JSON_NAME libjson.so)
	add_definitions(-std=c++11)
endif()

set(JSONDIR ${CMAKE_CURRENT_SOURCE_DIR}/marinoland_json)

execute_process( COMMAND chmod +x ${CONFIGURE_SCRIPT} WORKING_DIRECTORY ${JSONDIR} )
execute_process( COMMAND ${CONFIGURE_SCRIPT} WORKING_DIRECTORY ${JSONDIR} )
execute_process( COMMAND make json WORKING_DIRECTORY ${JSONDIR} )

include_directories(${BOOST_INCLUDEDIR} ${JSONDIR} /usr/include)
add_library(web STATIC
    HttpClient.cpp
    HttpsClient.cpp
    RestClient.cpp
    RestWebResponse.cpp
    WebClient.cpp
    WebResponse.cpp)

link_directories(${JSONDIR} /usr/local/lib ${BOOST_LIBRARYDIR})
add_executable(test test.cpp)
target_link_libraries(test LINK_PUBLIC web json pthread boost_system boost_filesystem ssl crypto)
