cmake_minimum_required(VERSION 4.1.2)
project(trafficSim)
find_package(raylib)
add_subdirectory(src)
target_link_libraries(trafficSim raylib)

