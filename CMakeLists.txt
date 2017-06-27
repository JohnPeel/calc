cmake_minimum_required(VERSION 3.0)
project(calc)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(catch)
include_directories(${CATCH_INCLUDE_DIR} ${COMMON_INCLUDES})
enable_testing(true)

set(SOURCE_FILES Expression.cpp Expression.h Integer.cpp Integer.h Float.cpp Float.h Utility.h Addition.cpp Addition.h Subtraction.h Variable.cpp Variable.h Multiplication.cpp Multiplication.h Division.cpp Division.h Method.cpp Method.h Utility.cpp Exponentiation.cpp Exponentiation.h Logarithm.cpp Logarithm.h Prime.cpp Prime.h Parser.cpp Parser.h ShuntingYard.cpp ShuntingYard.h)

add_executable(calc calc.cpp ${SOURCE_FILES})
add_executable(tests tests.cpp ${SOURCE_FILES})

add_dependencies(tests catch)