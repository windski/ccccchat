cmake_minimum_required(VERSION 3.13)
project(server)

find_package(Threads)
set(CMAKE_C_STANDARD 11)

set(SOURCE_FILE
    poll.c
    queue.c
    server.c
    service.c
    socket.c
    thread_pool.c
    utils.c
)

add_subdirectory(third_party)
add_executable(server ${SOURCE_FILE})
target_link_libraries(server log ${CMAKE_THREAD_LIBS_INIT})

