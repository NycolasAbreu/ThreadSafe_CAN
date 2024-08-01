add_executable(
    send_receive_frames
    send_receive_frames.cpp
)

target_link_libraries(
    send_receive_frames
    PRIVATE
    can_lib
    observer_lib
)

target_include_directories(
    send_receive_frames
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)
