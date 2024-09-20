cmake_minimum_required(VERSION 3.8)
project(depth_segmentation_ros2)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_definitions(-std=c++14)

# find dependencies
find_package(ament_cmake REQUIRED)

find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)

# LLVM is required for OpenMP support
if(APPLE)
  set(LLVM_PATH "/usr/local/opt/llvm")
  find_package(LLVM HINTS ${LLVM_PATH}/lib/cmake/llvm)
  if (LLVM_FOUND)
    message(STATUS "Building with LLVM clang and omp support.")
    set(CMAKE_C_COMPILER ${LLVM_TOOLS_BINARY_DIR}/clang)
    set(CMAKE_CXX_COMPILER ${LLVM_TOOLS_BINARY_DIR}/clang++)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp -L${LLVM_PATH}/lib -Wl,-rpath,${LLVM_PATH}/lib")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -L${LLVM_PATH}/lib -Wl,-rpath,${LLVM_PATH}/lib")
  else()
    message(STATUS "LLVM not found, building without omp support.")
  endif()
else()
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
    set(OpenMP_FLAGS -fopenmp)
    set(OpenMP_LIBS omp)
    add_compile_definitions(-Wno-unknown-pragmas -fopenmp)
  elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    set(OpenMP_FLAGS -fopenmp)
    set(OpenMP_LIBS gomp)
    add_compile_definitions(-Wno-unknown-pragmas -fopenmp)
  else()
    message(WARNING "Unrecognized C++ compiler: ${CMAKE_CXX_COMPILER} ID: ${CMAKE_CXX_COMPILER_ID}. Compiling without openMP support.")
  endif()
endif()

#default settings
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()


add_library(${PROJECT_NAME} SHARED
  src/depth_segmentation.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenMP_LIBS})
target_compile_options(${PROJECT_NAME} PRIVATE ${OpenMP_FLAGS})


add_executable(${PROJECT_NAME}_node
  src/depth_segmentation_node.cpp
)
target_link_libraries(${PROJECT_NAME}_node PRIVATE ${PROJECT_NAME})

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
