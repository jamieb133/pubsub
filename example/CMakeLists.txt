cmake_minimum_required(VERSION 3.16.0)

add_executable(example_node_a node_a.cpp)
target_link_libraries(example_node_a PUBLIC pubsub)
target_compile_features(example_node_a PRIVATE cxx_std_17)
