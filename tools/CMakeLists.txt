include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    "../include/"
)

file(GLOB CXX_FILES "*.cc")

set(CXX_FILES
    ${CXX_FILES}
)

target_link_libraries(${PROJECT_NAME})
add_executable(bounded_queue_perf bounded_queue_perf.cc)
