cmake_minimum_required(VERSION 2.8.3)
project(node1)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  message_generation
  hms_client
)

## Generate added messages and services with any dependencies listed here
generate_messages(
   DEPENDENCIES
   std_msgs
)

catkin_package(
  CATKIN_DEPENDS message_runtime
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

add_executable(node1 src/node1.cpp)
add_dependencies(node1 ${catkin_EXPORTED_TARGETS})
add_dependencies(node1 hms_client_generate_messages_cpp)
target_link_libraries(node1 ${catkin_LIBRARIES} )
