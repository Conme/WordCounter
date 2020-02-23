cmake_minimum_required(VERSION 3.10.2)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

project(WordCounter)

if (CMAKE_C_COMPILER_ID MATCHES "GNU")
	add_compile_options("$<$<CONFIG:DEBUG>:-D_DEBUG;-D_STATS;-O0;-g3;-pg;-Wall;-Wextra;-Wconversion>")
	add_compile_options("$<$<CONFIG:RELEASE>:-O3;-march=native;-Wextra;-Wconversion>")
endif()

if (CMAKE_C_COMPILER_ID MATCHES "MSVC")
	add_compile_options("$<$<CONFIG:DEBUG>:/D_DEBUG;/D_STATS;/Od;/W4>")
	add_compile_options("$<$<CONFIG:RELEASE>:/O2;/W4>")
endif()

include_directories(include)

file(GLOB SOURCES "src/*.c")

list(REMOVE_ITEM SOURCES "src/wordcount.c")

add_library(support ${SOURCES})

add_executable(WordCounter "src/wordcount.c")

target_link_libraries(WordCounter support)

add_subdirectory(${PROJECT_SOURCE_DIR}/googletest)

enable_testing()
# 
# Include the gtest library. gtest_SOURCE_DIR is available due to
# 'project(gtest)' above.
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

########################################
# Test filesset(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG
########################################
file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/tests/*.cpp)

########################################
# Unit Tests
########################################
add_executable(runUnitTests ${TEST_SRC_FILES})

########################################
# Standard linking to gtest stuff.
########################################
target_link_libraries(runUnitTests gtest gtest_main)

########################################
# Extra linking for the project.
########################################
target_link_libraries(runUnitTests support)

# This is so you can do 'make test' to see all your tests run, instead of
# manually running the executable runUnitTests to see those specific tests.
add_test(UnitTests runUnitTests)
