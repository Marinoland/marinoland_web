cmake_minimum_required(VERSION 2.8)
project (web)

if(NOT DEFINED DEP_ROOT)
	set(DEP_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
	if(NOT EXISTS ./depend)
		execute_process(COMMAND git clone git@github.com:Marinoland/marinoland_depend.git depend)
		execute_process(COMMAND cmake . WORKING_DIRECTORY depend)
	endif()
endif()

set(JSONDIR ${DEP_ROOT}/depend/json)
set(OPENSSLDIR ${DEP_ROOT}/depend/openssl/ext)
set(BOOSTDIR ${DEP_ROOT}/depend/boost/ext)
set(OPENSSL_INCLUDEDIR ${OPENSSLDIR}/include)
IF (WIN32)
    set(SLASH \\)
    set(SH_EXT bat)
    set(JSON_OUTDIR ${JSONDIR}/Release)
    set(LIBS )
    set(LIBPREFIX lib)
    set(BOOST_LIBVER -vc142-mt-x64-1_75)
    add_definitions(-D_WIN32_WINNT=0x0601)
ELSE()
    set(SLASH /)
    set(SH_EXT sh)
    set(JSON_OUTDIR ${JSONDIR})
    set(LIBS pthread)
    set(LIBPREFIX )
    set(BOOST_LIBVER )
    add_definitions(-std=c++11)
ENDIF()

add_library(web STATIC
    HttpClient.cpp
    HttpsClient.cpp
    RestClient.cpp
    RestWebResponse.cpp
    WebClient.cpp
    WebResponse.cpp)

include_directories(
    ${OPENSSL_INCLUDEDIR}
    ${BOOSTDIR}
    ${BOOSTDIR}/libs/beast/include
    ${JSONDIR})
link_directories(
    ${JSON_OUTDIR}
    ${OPENSSLDIR}
    ${BOOSTDIR}/stage/lib)
add_executable(testweb testweb.cpp)
target_link_libraries(testweb LINK_PUBLIC web json ${LIBS}
	${LIBPREFIX}boost_system${BOOST_LIBVER} ${LIBPREFIX}boost_filesystem${BOOST_LIBVER}
	${LIBPREFIX}boost_date_time${BOOST_LIBVER} ${LIBPREFIX}ssl ${LIBPREFIX}crypto)

IF (WIN32)
    file( COPY ${OPENSSLDIR}/libssl-3.dll DESTINATION Release/ )
    file( COPY ${OPENSSLDIR}/libcrypto-3.dll DESTINATION Release/ )
ENDIF()


