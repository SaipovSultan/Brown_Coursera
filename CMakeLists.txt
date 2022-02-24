cmake_minimum_required(VERSION 3.22)
project(Brown_Coursera)

set(CMAKE_CXX_STANDARD 17)

add_executable(Brown_Coursera main.cpp PriorityCollection.h test_runner.h profile.h hash_set.h node.h database.h)