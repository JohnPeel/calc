cmake_minimum_required(VERSION 3.0)
project(calc)

set(CMAKE_CXX_STANDARD 11)

set(CATCH_DIR ${CMAKE_CURRENT_SOURCE_DIR}/catch)
add_library(catch INTERFACE)
target_include_directories(catch INTERFACE ${CATCH_DIR}/single_include)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CATCH_DIR}/contrib")
enable_testing(true)
include(ParseAndAddCatchTests)

set(SOURCE_FILES Expression.cpp Expression.h Integer.cpp Integer.h Float.cpp Float.h Utility.h Addition.cpp Addition.h Subtraction.h Variable.cpp Variable.h Multiplication.cpp Multiplication.h Division.cpp Division.h Method.cpp Method.h Utility.cpp Exponentiation.cpp Exponentiation.h Logarithm.cpp Logarithm.h Prime.cpp Prime.h Parser.cpp Parser.h ShuntingYard.cpp ShuntingYard.h)

add_library(library OBJECT ${SOURCE_FILES})

add_executable(calc calc.cpp $<TARGET_OBJECTS:library>)

add_executable(tests tests.cpp $<TARGET_OBJECTS:library>)
target_link_libraries(tests catch)
ParseAndAddCatchTests(tests)