cmake_minimum_required (VERSION 2.8.4)
project (ipaddress)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ipaddress_SOURCE_DIR}/${CMAKE_BUILD_TYPE})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")

if (${CMAKE_COMPILER_IS_GNUCXX})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3 -g")


#Only tested with versions 1.55 and 1.56
find_package(Boost 1.54.0 COMPONENTS system regex coroutine context thread iostreams REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

if(APPLE)
  set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl")
endif()

#include(CMakeLists-easyloggingpp.txt)

#set(SIMPLEWEB_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/3rdparty/Simple-WebSocket-Server")
#TODO: add requirement for version 1.0.1g (can it be done in one line?)
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
#include_directories(${SIMPLEWEB_INCLUDE_DIR})

find_package(Threads REQUIRED)

include_directories(.)

file(GLOB SOURCES "src/*.cpp")
#add_library(ipaddress SHARED ${SOURCES})
add_library(ipaddress STATIC ${SOURCES})
target_link_libraries(ipaddress ${Boost_LIBRARIES})


enable_testing()
add_subdirectory(test)
