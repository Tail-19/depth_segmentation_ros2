cmake_minimum_required(VERSION 3.8)
project(depth_segmentation_ros2)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(image_transport REQUIRED)
find_package(OpenCV REQUIRED)


find_package(OpenMP)
if(OpenMP_CXX_FOUND)
  message(STATUS "OpenMP support found, enabling OpenMP flags.")
  set(OpenMP_FLAGS ${OpenMP_CXX_FLAGS})
  set(OpenMP_LIBS OpenMP::OpenMP_CXX)
else()
  message(STATUS "OpenMP support not found, building without OpenMP support.")
  set(OpenMP_FLAGS "")
  set(OpenMP_LIBS "")
endif()


if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()


add_library(${PROJECT_NAME} SHARED
  src/depth_segmentation_ros2.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenMP_LIBS})
target_compile_options(${PROJECT_NAME} PRIVATE ${OpenMP_FLAGS})
ament_target_dependencies(${PROJECT_NAME} ${OpenCV_LIBS})


add_executable(${PROJECT_NAME}_node
  src/depth_segmentation_node_ros2.cpp
)
target_link_libraries(${PROJECT_NAME}_node PRIVATE ${PROJECT_NAME})

ament_target_dependencies(${PROJECT_NAME}_node
  rclcpp
  std_msgs
  sensor_msgs
  image_transport
  OpenCV
)

# Test
find_package(ament_cmake_gtest REQUIRED)
ament_add_gtest(test_depth_segmentation test/test_depth_segmentation.cpp)
if(TARGET test_depth_segmentation)
  target_link_libraries(test_depth_segmentation PRIVATE ${PROJECT_NAME} pthread)
endif()


install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_node
  EXPORT export_${PROJECT_NAME}
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
