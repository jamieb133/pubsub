cmake_minimum_required(VERSION 3.16.0)

project(pubsub)

file(GLOB srcs ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
set(incs "")

if(UNIX OR APPLE)
    file(GLOB os_srcs ${CMAKE_CURRENT_SOURCE_DIR}/src/unix/*.cpp)
    list(APPEND incs ${CMAKE_CURRENT_SOURCE_DIR}/src/unix)
elseif(WIN32)
    file(GLOB os_srcs ${CMAKE_CURRENT_SOURCE_DIR}/src/windows/*.cpp)
    list(APPEND incs ${CMAKE_CURRENT_SOURCE_DIR}/src/windows)
endif()

list(APPEND srcs ${os_srcs})
list(APPEND incs ${CMAKE_CURRENT_SOURCE_DIR}/src/include)

message(srcs ${srcs})
add_library(pubsub STATIC ${srcs})

target_include_directories(pubsub PUBLIC ${incs})
target_compile_features(pubsub PRIVATE cxx_std_17)

add_subdirectory(test)
add_subdirectory(example)
