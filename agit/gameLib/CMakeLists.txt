cmake_minimum_required(VERSION 3.9.3)

project (GameLib)
set(OpenCV_DIR "C:/opencv/opencvInstall")

find_package(OpenCV    3.3.0    REQUIRED)

add_library(GameLib STATIC
  game.h
  libgame.h
  cameralib.h	
)