cmake_minimum_required(VERSION 3.10)
project(opencvtest VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)
enable_testing()

find_package(OpenCV REQUIRED COMPONENTS core highgui video videoio xobjdetect)

add_executable(opencvtest face_detection.cpp)

target_include_directories(opencvtest PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(opencvtest PRIVATE ${OpenCV_LIBS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
