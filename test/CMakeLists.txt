cmake_minimum_required(VERSION 3.16.0)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

if(WIN32)
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

FetchContent_MakeAvailable (googletest)

enable_testing()

file(GLOB srcs ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(unit_tests ${srcs})
target_link_libraries(unit_tests PUBLIC pubsub)
target_link_libraries(unit_tests PUBLIC GTest::gtest_main)
target_link_libraries(unit_tests PUBLIC gmock)

include(GoogleTest)
gtest_discover_tests(unit_tests)
