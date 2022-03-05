cmake_minimum_required(VERSION 3.21)
project(Brown_Coursera)

set(CMAKE_CXX_STANDARD 17)

add_executable(Brown_Coursera main.cpp PriorityCollection.h test_runner.h profile.h hash_set.h node.h database.h xml.h xml.cpp json.h json.cpp stats_aggregator.h stats_aggregator.cpp stats_aggregator_test.cpp game_object.h geo2d.h geo2d.cpp comment_server.h)