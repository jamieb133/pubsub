cmake_minimum_required(VERSION 3.16.0)

project(pubsub)

file(GLOB srcs ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

if(UNIX)
    file(GLOB os_srcs ${CMAKE_CURRENT_SOURCE_DIR}/Linux/src/*.cpp)
elseif(APPLE)
    file(GLOB os_srcs ${CMAKE_CURRENT_SOURCE_DIR}/Mac/src/*.cpp)
elseif(WIN32)
    file(GLOB os_srcs ${CMAKE_CURRENT_SOURCE_DIR}/Windows/src/*.cpp)
endif()

list(APPEND srcs ${os_srcs})

message(srcs ${srcs})
add_library(pubsub STATIC ${srcs})

target_include_directories(pubsub PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/include)
target_compile_features(pubsub PRIVATE cxx_std_17)

add_subdirectory(test)
