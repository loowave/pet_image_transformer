cmake_minimum_required(VERSION 3.24)
project(untitled1)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled1 main.cpp tmatrix.h filter.cpp filter.h application.cpp application.h parcer.h RGB.h pipeline.cpp pipeline.h files_processing.cpp files_processing.h)
