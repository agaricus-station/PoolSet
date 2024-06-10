cmake_minimum_required(VERSION 3.21)
project(thread_pool)

set(CMAKE_CXX_STANDARD 17)
set(ROOT_DIR ${CMAKE_SOURCE_DIR})

# add header files
include_directories(include DIR_INC)

# add source files
add_executable(thread_pool src/task.cc src/thread_pool.cc example/test.cc)

target_link_libraries(thread_pool libpthread.a)
