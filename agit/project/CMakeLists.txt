cmake_minimum_required(VERSION 3.9.3)

project(Game)

set(OpenCV_DIR "C:/opencv/opencvInstall")

find_package(OpenCV 3.3.0 REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Game
     cameralib.h
     game.h
     libgame.h
	 
	 Main.cpp
 )

target_link_libraries( Game ${OpenCV_LIBS} )