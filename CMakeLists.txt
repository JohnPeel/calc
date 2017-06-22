cmake_minimum_required(VERSION 3.7)
project(calc)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES calc.cpp Expression.cpp Expression.h Integer.cpp Integer.h Float.cpp Float.h Utility.h Addition.cpp Addition.h Subtraction.h Variable.cpp Variable.h Multiplication.cpp Multiplication.h Division.cpp Division.h Method.cpp Method.h Utility.cpp Exponentiation.cpp Exponentiation.h Logarithm.cpp Logarithm.h Prime.cpp Prime.h)
add_executable(calc ${SOURCE_FILES})