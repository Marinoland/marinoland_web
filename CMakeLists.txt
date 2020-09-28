cmake_minimum_required(VERSION 2.8)
project (web)

set(JSONDIR ${CMAKE_CURRENT_SOURCE_DIR}/marinoland_json)
set(OPENSSLDIR ${CMAKE_CURRENT_SOURCE_DIR}/openssl)
set(BOOSTDIR ${CMAKE_CURRENT_SOURCE_DIR}/boost)
IF (WIN32)
    set(SLASH \\)
    set(SH_EXT bat)
    set(OPENSSL_INCLUDEDIR ${OPENSSLDIR}/include)
    set(JSON_OUTDIR ${JSONDIR}/Release)
    set(LIBS )
    set(BOOST_LIBVER -vc142-mt-x64-1_75)
    add_definitions(-D_WIN32_WINNT=0x0601)
ELSE()
    set(SLASH /)
    set(SH_EXT sh)
    set(OPENSSL_INCLUDEDIR ${OPENSSLDIR})
    set(JSON_OUTDIR ${JSONDIR})
    set(LIBS pthread)
    set(BOOST_LIBVER )
    add_definitions(-std=c++11)
ENDIF()

IF (WIN32)
    execute_process( COMMAND perl Configure VC-WIN64A WORKING_DIRECTORY ${OPENSSLDIR} )
    execute_process( COMMAND nmake WORKING_DIRECTORY ${OPENSSLDIR} )
ELSE()
    execute_process( COMMAND ./Configure WORKING_DIRECTORY ${OPENSSLDIR} )
    execute_process( COMMAND make WORKING_DIRECTORY ${OPENSSLDIR} )
ENDIF()

execute_process( COMMAND .${SLASH}bootstrap.${SH_EXT} WORKING_DIRECTORY ${BOOSTDIR} )
execute_process( COMMAND .${SLASH}b2 WORKING_DIRECTORY ${BOOSTDIR} )

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
    libboost_system${BOOST_LIBVER} libboost_filesystem${BOOST_LIBVER} libboost_date_time${BOOST_LIBVER}
    libssl libcrypto)

IF (WIN32)
    file( COPY ${OPENSSLDIR}/libssl-3.dll DESTINATION Release/ )
    file( COPY ${OPENSSLDIR}/libcrypto-3.dll DESTINATION Release/ )
ENDIF()


