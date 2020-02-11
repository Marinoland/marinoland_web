cmake_minimum_required(VERSION 2.8)
project (web)


set(JSONDIR ${CMAKE_CURRENT_SOURCE_DIR}/marinoland_json)
set(JSONLIBDIR ${JSONDIR})
add_definitions(-std=c++11)

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
    $ENV{BOOST_INCLUDEDIR}
    ${JSONDIR}
    /usr/include)
link_directories(${JSONLIBDIR} /usr/local/lib $ENV{BOOST_LIBRARYDIR})
add_executable(test test.cpp)
target_link_libraries(test LINK_PUBLIC web json pthread boost_system boost_filesystem boost_date_time ssl crypto)
