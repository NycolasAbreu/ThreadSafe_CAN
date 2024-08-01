add_library(
    can_lib
    CanDriver.cpp
    CanInterface.cpp
    CanMessage.cpp
)

target_include_directories(
    can_lib
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    can_lib
    PRIVATE
    socketcan
    observer_lib
)
