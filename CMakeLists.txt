cmake_minimum_required(VERSION 2.8)
project (web)

execute_process(COMMAND git submodule init)
execute_process(COMMAND git submodule update --recursive)

add_subdirectory(marinoland_json ${CMAKE_BINARY_DIR}/json)
add_subdirectory(marinoland_openssl ${CMAKE_BINARY_DIR}/ext/openssl)
add_subdirectory(marinoland_boost ${CMAKE_BINARY_DIR}/ext/boost)

set(OPENSSLDIR ${CMAKE_BINARY_DIR}/ext/openssl)

IF (WIN32)
    set(SLASH \\)
    set(SH_EXT bat)
    set(LIBS )
    set(LIBPREFIX lib)
    set(SSLSUFFIX _static)
	string(REPLACE v vc toolset ${CMAKE_VS_PLATFORM_TOOLSET})
    set(JSONDIR ${CMAKE_BINARY_DIR}/json)
    add_definitions(-D_WIN32_WINNT=0x0601)
ELSE()
    set(SLASH /)
    set(SH_EXT sh)
    set(LIBPREFIX )
    set(SSLSUFFIX )
    set(JSONDIR ${CMAKE_BINARY_DIR}/json)
    add_definitions(-std=c++11)
    find_package(Threads REQUIRED)
    set(LIBS dl Threads::Threads)
ENDIF()

add_library(web STATIC
    src/Authorization.cpp
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
    ${CMAKE_CURRENT_SOURCE_DIR}/marinoland_openssl/ext/include
)
link_directories(
    ${JSONDIR}
    ${OPENSSLDIR}
)
add_executable(testweb test/testweb.cpp)
target_link_libraries(testweb LINK_PUBLIC
    web
    json
    ${LIBPREFIX}ssl${SSLSUFFIX}
    ${LIBPREFIX}crypto${SSLSUFFIX}
    ${LIBS}
)

if(WIN32)
    add_custom_command(TARGET web POST_BUILD
	    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/rootcerts/ "${CMAKE_BINARY_DIR}/$<CONFIG>/rootcerts" 
    )
else()
	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/rootcerts DESTINATION ${CMAKE_BINARY_DIR} )
endif()
