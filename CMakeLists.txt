cmake_minimum_required(VERSION 2.8.3)
project(rosbag_controller)

find_package(catkin REQUIRED)

find_package(catkin REQUIRED COMPONENTS roscpp)

catkin_package(
   INCLUDE_DIRS include
)

include_directories(include)

set(CMAKE_CXX_FLAGS "-std=c++11 -O3 ${OTHERS_DEFINE}")

add_executable(rosbag_controller_node 
							 src/rosbag_controller_node.cpp
							 src/spawner.cpp
							)

target_link_libraries(rosbag_controller_node
  ${catkin_LIBRARIES}
)