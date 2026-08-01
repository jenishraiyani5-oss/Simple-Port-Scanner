cmake_minimum_required(VERSION 3.15)
project(simplePortScanner)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS program_options)

# Includes directory search path
include_directories(src)

# Add executable using only .cpp source files
add_executable(simplePortScanner 
    main.cpp
    src/PortScanner.cpp
)

target_link_libraries(simplePortScanner
    Boost::program_options
)