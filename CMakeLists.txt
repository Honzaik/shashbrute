cmake_minimum_required(VERSION 3.10)
project(hashbrute)

set(CMAKE_CXX_STANDARD 11)

add_executable(hashbrute main.cpp)

include_directories("${CMAKE_SOURCE_DIR}/cryptopp")
target_link_libraries(hashbrute ${CMAKE_SOURCE_DIR}/libcryptopp.a)