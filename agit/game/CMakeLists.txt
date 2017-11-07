cmake_minimum_required(VERSION 3.9.3)

project (MyProject)

set(GameLib_DIR "C:/Projekt/gameLib")

include_directories(
  ${GameLib_INCLUDE_DIRS}
)

add_executable(MyProject
  main.cpp
)

target_link_libraries(MyProject ${GameLib_LIBS})