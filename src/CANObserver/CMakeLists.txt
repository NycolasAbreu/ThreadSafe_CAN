add_library(
    observer_lib
    CanMessageObserverManager.cpp
    SimpleMessageObserver.cpp
)

target_include_directories(
    observer_lib
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    observer_lib
    PRIVATE
    can_lib
)