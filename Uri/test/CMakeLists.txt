# CMakeLists.txt for UriTests

cmake_minimum_required(VERSION 3.8)
set(This UriTests)

set(Sources
	src/UriTests.cpp
)

add_executable(${This} ${Sources})
set_target_properties(${This} PROPERTIES
	FOLDER Tests
)

target_link_libraries(${This} PUBLIC
	gtest_main
	Uri
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_test(
	NAME ${This}
	COMMAND ${This}
)
