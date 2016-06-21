cmake_minimum_required(VERSION 2.8)

project(wow_fishing_bot)

find_package(OpenCV REQUIRED)
link_libraries(${OpenCV_LIBS})
find_package(X11 REQUIRED)
link_libraries(X11)

add_executable(main main.cpp blob_tracker.cpp)