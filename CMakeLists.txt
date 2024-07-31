cmake_minimum_required(VERSION 3.10)

# Project name
project(NycoCAN)

set(CMAKE_CXX_STANDARD 17)

add_executable(
    ${PROJECT_NAME}
    src/main.cpp
    src/CanDriver.cpp
    src/CanMessage.cpp
    src/CanInterface.cpp
    src/SimpleMessageObserver.cpp
    src/CanMessageObserverManager.cpp
)

target_link_libraries(
    ${PROJECT_NAME}
    socketcan
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    include
)