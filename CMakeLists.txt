cmake_minimum_required(VERSION 2.8)
project (web)

execute_process(COMMAND git submodule init)
execute_process(COMMAND git submodule update --recursive)

add_subdirectory(marinoland_json ${CMAKE_CURRENT_BINARY_DIR}/json)
add_subdirectory(marinoland_openssl ${CMAKE_CURRENT_BINARY_DIR}/ext/openssl)
add_subdirectory(marinoland_boost ${CMAKE_CURRENT_BINARY_DIR}/ext/boost)

set(JSONDIR ${CMAKE_BINARY_DIR}/json)
set(OPENSSLDIR ${CMAKE_BINARY_DIR}/ext/openssl)
set(BOOSTDIR ${CMAKE_BINARY_DIR}/ext/boost)

IF (WIN32)
    set(SLASH \\)
    set(SH_EXT bat)
    set(LIBS )
    set(LIBPREFIX lib)
    set(BOOST_LIBVER -vc143-mt-x64-1_81)
    add_definitions(-D_WIN32_WINNT=0x0601)
ELSE()
    set(SLASH /)
    set(SH_EXT sh)
    set(LIBS pthread)
    set(LIBPREFIX )
    set(LIBSUFFIX )
    set(BOOST_LIBVER )
    add_definitions(-std=c++11)
ENDIF()

add_library(web STATIC
    src/HttpClient.cpp
    src/HttpsClient.cpp
    src/RestClient.cpp
    src/RestWebResponse.cpp
    src/WebClient.cpp
    src/WebResponse.cpp)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/marinoland_json/include
    ${CMAKE_CURRENT_SOURCE_DIR}/marinoland_boost/ext/
)
link_directories(
    ${JSONDIR}
    ${OPENSSLDIR}
    ${BOOSTDIR}
)
add_executable(testweb test/testweb.cpp)
target_link_libraries(testweb LINK_PUBLIC web
    ${LIBS}
    ${LIBPREFIX}json
    ${LIBPREFIX}ssl
    ${LIBPREFIX}crypto
    ${LIBPREFIX}boost_system${BOOST_LIBVER}
    ${LIBPREFIX}boost_filesystem${BOOST_LIBVER}
    ${LIBPREFIX}boost_date_time${BOOST_LIBVER}
)

file( COPY rootcerts DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )