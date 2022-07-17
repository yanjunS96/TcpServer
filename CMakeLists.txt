cmake_minimum_required(VERSION 3.0)
project(TcpServer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-pmf-conversions -Wno-format -pthread")

include_directories(
        ./include
        ./base
)

file(GLOB SOURCES ./*.cpp ./src/*.cpp ./base/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

