cmake_minimum_required(VERSION 2.8)
project (web)


execute_process( COMMAND git clone https://github.com/Marinoland/marinoland_json.git $ENV{HOME}/.marinoland/json )
execute_process( COMMAND git pull WORKING_DIRECTORY $ENV{HOME}/.marinoland/json )
execute_process( COMMAND cmake . WORKING_DIRECTORY $ENV{HOME}/.marinoland/json )
execute_process( COMMAND make json WORKING_DIRECTORY $ENV{HOME}/.marinoland/json )


include_directories(/usr/include $ENV{HOME}/.marinoland/json)
add_definitions(-std=c++11)
add_library(web SHARED
    HttpClient.cpp
    HttpsClient.cpp
    RestClient.cpp
    RestWebResponse.cpp
    WebClient.cpp
    WebResponse.cpp)

link_directories($ENV{HOME}/.marinoland/json /usr/local/lib)
add_executable(sample sample.cpp)
target_link_libraries(sample LINK_PUBLIC web json pthread boost_filesystem boost_system ssl crypto)