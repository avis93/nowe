cmake_minimum_required(VERSION 3.9.3)

project (MyProject)

set(OpenCV_DIR "C:/opencv/opencvInstall")

find_package(OpenCV    3.3.0    REQUIRED)

include_directories(
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(MyProject
  TestApp1.cpp
)

target_link_libraries(MyProject ${OpenCV_LIBS})