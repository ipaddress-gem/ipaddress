cmake_minimum_required(VERSION 2.8.7)
project (ipaddressTest)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ipaddressTest_SOURCE_DIR}/${CMAKE_BUILD_TYPE})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -Wall -Wextra -DTEST")
if (${CMAKE_COMPILER_IS_GNUCXX})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3 -g")


#set(CMAKE_SHARED_LINKER_FLAGS="-lprofiler -ltcmalloc")


enable_testing()

find_package(Boost 1.54.0 COMPONENTS system regex coroutine context iostreams thread REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

find_package(Threads REQUIRED)

#add_executable(test_crunchy, test_crunchy.cpp)
#add_test(test_crunchy test_crunchy)

add_executable(test_option test_option.cpp)
add_test(test_option test_option)

add_executable(test_result test_result.cpp)
add_test(test_result test_result)
