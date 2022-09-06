cmake_minimum_required(VERSION 2.8)
project (web)

execute_process(COMMAND git submodule init)
execute_process(COMMAND git submodule update)

add_subdirectory(marinoland_json ${CMAKE_CURRENT_BINARY_DIR}/json)
add_subdirectory(marinoland_openssl ${CMAKE_CURRENT_BINARY_DIR}/openssl)
add_subdirectory(marinoland_boost ${CMAKE_CURRENT_BINARY_DIR}/boost)

set(JSONDIR ${CMAKE_BINARY_DIR}/json)
set(OPENSSLDIR ${CMAKE_BINARY_DIR}/openssl/ext)

IF (WIN32)
    set(SLASH \\)
    set(SH_EXT bat)
    set(LIBS )
    set(LIBPREFIX lib)
    set(BOOST_LIBVER -vc142-mt-x64-1_75)
    add_definitions(-D_WIN32_WINNT=0x0601)
ELSE()
    set(SLASH /)
    set(SH_EXT sh)
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
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/marinoland_json/
)
link_directories(${OPENSSLDIR} ${JSONDIR})
add_executable(testweb testweb.cpp)
target_link_libraries(testweb LINK_PUBLIC web ${LIBS} ${DEP_LIBS} ${LIBPREFIX}ssl ${LIBPREFIX}crypto)

IF (WIN32)
    file( COPY ${OPENSSLDIR}/libssl-3.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release/ )
    file( COPY ${OPENSSLDIR}/libcrypto-3.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release/ )
ENDIF()
file( COPY rootcerts DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

