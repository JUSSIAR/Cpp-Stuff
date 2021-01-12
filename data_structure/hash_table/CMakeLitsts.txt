cmake_minimum_required(VERSION 3.16)

project(hash_table VERSION 0.0.1)

set(SOURCE hashTable.cpp)

add_executable(hashTable ${SOURCE})

set_target_properties(
	hashTable PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
)
