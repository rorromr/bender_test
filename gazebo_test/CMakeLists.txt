cmake_minimum_required(VERSION 2.8.3)
project(gazebo_test)

find_package(catkin REQUIRED COMPONENTS
  gazebo_ros
  gazebo_msgs
  rosconsole
  roscpp
  rospy
  sensor_msgs
  std_msgs
  tf
)

# Depend on system install of Gazebo and SDFormat
find_package(gazebo REQUIRED)
find_package(Boost REQUIRED COMPONENTS thread)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GAZEBO_CXX_FLAGS}")

include_directories(
  include
  ${Boost_INCLUDE_DIRS}
  ${catkin_INCLUDE_DIRS}
  ${GAZEBO_INCLUDE_DIRS}
)

link_directories(
  ${GAZEBO_LIBRARY_DIRS}
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES gazebo_ros_quad
  CATKIN_DEPENDS gazebo_msgs rosconsole roscpp rospy sensor_msgs std_msgs tf
  DEPENDS gazebo SDF
)

add_library(gazebo_ros_quad src/gazebo_ros_quad.cpp)
target_link_libraries(gazebo_ros_quad ${GAZEBO_LIBRARIES} ${catkin_LIBRARIES} ${Boost_LIBRARIES})

