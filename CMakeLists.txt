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
file(WRITE depend.libs web)
file(WRITE depend.includedirs ${CMAKE_CURRENT_SOURCE_DIR})
IF (WIN32)
    set(SLASH \\)
    set(SH_EXT bat)
    set(LIBS )
    set(LIBPREFIX lib)
    set(BOOST_LIBVER -vc142-mt-x64-1_75)
    add_definitions(-D_WIN32_WINNT=0x0601)
    file(WRITE depend.linkdirs ${CMAKE_CURRENT_SOURCE_DIR}/Release)
ELSE()
    set(SLASH /)
    set(SH_EXT sh)
    set(LIBS pthread)
    set(LIBPREFIX )
    set(BOOST_LIBVER )
    add_definitions(-std=c++11)
    file(WRITE depend.linkdirs ${CMAKE_CURRENT_SOURCE_DIR})
ENDIF()

add_library(web STATIC
    HttpClient.cpp
    HttpsClient.cpp
    RestClient.cpp
    RestWebResponse.cpp
    WebClient.cpp
    WebResponse.cpp)

file(STRINGS ${DEP_ROOT}/depend/depend.includedirs DEP_INC_DIRS)
message(${DEP_INC_DIRS})
include_directories(
    ${DEP_INC_DIRS})
file(STRINGS ${DEP_ROOT}/depend/depend.linkdirs DEP_LINK_DIRS)
link_directories(
    ${OPENSSLDIR}
    ${DEP_LINK_DIRS})
add_executable(testweb testweb.cpp)
file(STRINGS ${DEP_ROOT}/depend/depend.libs DEP_LIBS)
target_link_libraries(testweb LINK_PUBLIC web ${LIBS} ${DEP_LIBS} ${LIBPREFIX}ssl ${LIBPREFIX}crypto)

IF (WIN32)
    file( COPY ${OPENSSLDIR}/libssl-3.dll DESTINATION ${DEL_ROOT}/Release/ )
    file( COPY ${OPENSSLDIR}/libcrypto-3.dll DESTINATION ${DEL_ROOT}/Release/ )
ENDIF()


