cmake_minimum_required(VERSION 3.10)
project(formals)

enable_testing()

set(CMAKE_CXX_STANDARD 14)

add_executable(
        formals
        main.cpp
        src/Automata.cpp
        src/DFA.cpp)
target_include_directories(formals PUBLIC ${CMAKE_SOURCE_DIR})

add_subdirectory(${CMAKE_SOURCE_DIR}/src)
target_link_libraries(formals formals_lib)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

add_executable(
        Test
        ${CMAKE_SOURCE_DIR}/tests/main_tests.cpp
        ${CMAKE_SOURCE_DIR}/tests/tests.cpp
)
target_include_directories(Test PUBLIC ${CMAKE_SOURCE_DIR})
target_include_directories(Test PUBLIC ${GTEST_INCLUDE_DIRS})
target_link_libraries(Test formals_lib)
target_link_libraries(Test ${GTEST_LIBRARIES} Threads::Threads)
add_test(Test ${CMAKE_SOURCE_DIR}/Tested)

