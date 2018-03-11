cmake_minimum_required(VERSION 3.5)
project(dlx C)

# Output binaries at top level.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic -march=native -Wall -Wextra -O3 -std=c99")

if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
	set(CMAKE_C_FLAGS "-O2 --memory-init-file 0 --closure 1")
endif()

enable_testing()
add_subdirectory(libdlx)
add_subdirectory(sudoku/sudgen)
add_subdirectory(sudoku/sudslv)
