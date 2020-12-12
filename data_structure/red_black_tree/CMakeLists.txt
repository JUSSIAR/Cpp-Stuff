cmake_minimum_required(VERSION 3.16)

project(Red_Black_Tree VERSION 0.0.1)

set(SOURCE redBlackTree.cpp)

add_executable(redBlackTree ${SOURCE})

set_target_properties(
	redBlackTree PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
)
