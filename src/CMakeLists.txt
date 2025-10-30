file(GLOB SRC_FILES *.cpp)
add_executable(trafficSim ${SRC_FILES})
target_link_libraries(trafficSim raylib)