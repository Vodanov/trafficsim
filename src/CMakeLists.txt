file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(trafficSim ${SRC_FILES})
target_link_libraries(trafficSim raylib)