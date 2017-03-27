cmake_minimum_required(VERSION 3.6)
project(Samples)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED COMPONENTS system thread)

include_directories(/usr/local/include)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(/usr/local/lib)

add_subdirectory(cpprest)