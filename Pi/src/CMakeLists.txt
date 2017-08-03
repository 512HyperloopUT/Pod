cmake_minimum_required(VERSION 2.6)
project(podex)
set(CMAKE_CXX_STANDARD 11)

find_package(Boost COMPONENTS system REQUIRED)

include_directories(include/ ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE podex_src "cpp/*")

add_executable(podex ${podex_src})

target_link_libraries(podex LINK_PUBLIC ${Boost_LIBRARIES})
