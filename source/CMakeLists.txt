cmake_minimum_required(VERSION 3.2)
project(BarChartRace VERSION 0.1 LANGUAGES CXX)

#=== Main App ===

include_directories("src"
                    "lib"
                    "include")

# set the compiling flags
set(CMAKE_CXX_FLAGS "-Wall")

add_executable(bcr
               src/bcr.cpp
               src/animation_mgr.cpp
               src/bar_chart.cpp
               include/animation_mgr.h
               include/bar_chart.h)

# Define C++11 standard
target_compile_features(bcr PUBLIC cxx_std_17)

# The end