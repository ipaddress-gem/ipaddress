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

add_executable(test_rle test_rle.cpp ../src/rle.cpp)
add_test(test_rle test_rle)

add_executable(test_crunchy test_crunchy.cpp ../src/crunchy.cpp)
add_test(test_crunchy test_crunchy)

add_executable(test_prefix32 test_prefix32.cpp ../src/crunchy.cpp)
add_test(test_prefix32 test_prefix32)
target_link_libraries(test_prefix32 ${Boost_LIBRARIES})

add_executable(test_prefix128 test_prefix128.cpp ../src/crunchy.cpp)
add_test(test_prefix128 test_prefix128)
target_link_libraries(test_prefix128 ${Boost_LIBRARIES})


add_executable(test_ipv6_unspec test_ipv6_unspec.cpp ../src/crunchy.cpp)
add_test(test_ipv6_unspec test_ipv6_unspec)
target_link_libraries(test_ipv6_unspec ${Boost_LIBRARIES})

add_executable(test_ipv6_mapped test_ipv6_mapped.cpp ../src/crunchy.cpp)
add_test(test_ipv6_mapped test_ipv6_mapped)
target_link_libraries(test_ipv6_mapped ${Boost_LIBRARIES})

add_executable(test_ipv6_loopback test_ipv6_loopback.cpp ../src/crunchy.cpp ../src/rle.cpp ../src/ipaddress.cpp ../src/ipv4.cpp)
add_test(test_ipv6_loopback test_ipv6_loopback)
target_link_libraries(test_ipv6_loopback ${Boost_LIBRARIES})

add_executable(test_ipv6 test_ipv6.cpp ../src/crunchy.cpp)
add_test(test_ipv6 test_ipv6)
target_link_libraries(test_ipv6 ${Boost_LIBRARIES})

add_executable(test_ipv4 test_ipv4.cpp ../src/crunchy.cpp ../src/rle.cpp ../src/ipaddress.cpp ../src/ipv4.cpp)
add_test(test_ipv4 test_ipv4)
target_link_libraries(test_ipv4 ${Boost_LIBRARIES})


add_executable(test_ipaddress test_ipaddress.cpp ../src/crunchy.cpp ../src/ipaddress.cpp ../src/ipv4.cpp ../src/rle.cpp)
add_test(test_ipaddress test_ipaddress)
target_link_libraries(test_ipaddress ${Boost_LIBRARIES})
