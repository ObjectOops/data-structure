cmake_minimum_required(VERSION 3.12)
project(data-structure VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -Werror -Wpedantic -fsanitize=address)
add_link_options(-fsanitize=address)

add_library(data-structure INTERFACE)
target_include_directories(data-structure INTERFACE include)

include(CTest)
enable_testing()
add_subdirectory(test)
