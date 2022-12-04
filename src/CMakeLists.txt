cmake_minimum_required(VERSION 3.10)
project(LibraryProject)
set(SOURCE_LIB Automata.cpp DFA.cpp)
add_library(formals_lib SHARED ${SOURCE_LIB})