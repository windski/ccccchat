cmake_minimum_required(VERSION 3.13)
project(server)

find_package(Threads)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/sanitizers-cmake/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_C_STANDARD 11)
find_package(Sanitizers)

set(SOURCE_FILE
    poll.cc
    server.cc
    service.cc
    socket.cc
    thread_pool.cc
    utils.cc
)

add_compile_options(-Wall -Werror)
add_subdirectory(third_party)
add_executable(server ${SOURCE_FILE})
add_sanitizers(server)
target_link_libraries(server log ${CMAKE_THREAD_LIBS_INIT})

