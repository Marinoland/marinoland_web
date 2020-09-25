cmake_minimum_required(VERSION 2.8)
project (web)
add_definitions(-std=c++11)

set(JSONDIR ${CMAKE_CURRENT_SOURCE_DIR}/marinoland_json)
set(OPENSSLDIR ${CMAKE_CURRENT_SOURCE_DIR}/openssl)
set(BOOSTDIR ${CMAKE_CURRENT_SOURCE_DIR}/boost)

execute_process( COMMAND ./Configure WORKING_DIRECTORY ${OPENSSLDIR} )
execute_process( COMMAND make WORKING_DIRECTORY ${OPENSSLDIR} )

execute_process( COMMAND ./bootstrap.sh WORKING_DIRECTORY ${BOOSTDIR} )
execute_process( COMMAND ./b2 WORKING_DIRECTORY ${BOOSTDIR} )

execute_process( COMMAND cmake . WORKING_DIRECTORY ${JSONDIR} )
execute_process( COMMAND cmake --build . --config Release WORKING_DIRECTORY ${JSONDIR} )

add_library(web STATIC
    HttpClient.cpp
    HttpsClient.cpp
    RestClient.cpp
    RestWebResponse.cpp
    WebClient.cpp
    WebResponse.cpp)

include_directories(
    ${OPENSSLDIR}
    ${BOOSTDIR}
    ${JSONDIR})
link_directories(
    ${JSONDIR}
    ${OPENSSLDIR}
    ${BOOSTDIR}/stage/lib)
add_executable(testweb testweb.cpp)
target_link_libraries(testweb LINK_PUBLIC web json pthread boost_system boost_filesystem boost_date_time ssl crypto)
