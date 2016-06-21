cmake_minimum_required(VERSION 2.8)

project(wow_fishing_bot)

find_package(OpenCV REQUIRED)
link_libraries(${OpenCV_LIBS})

add_executable(main main.cpp blob_tracker.cpp)