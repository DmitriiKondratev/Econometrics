cmake_minimum_required(VERSION 3.10)
project(Generator)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

find_package(GSL REQUIRED)
link_libraries(GSL::gsl GSL::gslcblas)

add_executable(main
        src/main.cpp
        src/Generator.cpp
        include/Generator.h
        src/Parser.cpp
        include/Parser.h
)