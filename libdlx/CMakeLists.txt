project(libdlx LANGUAGES C VERSION 1.0)

# Note: all symbols are currently exported.

set(SOURCE_FILES
    src/matrix.c
    src/matrix_query.c
    src/dlx_dance.c
    src/dlx_iterative_solver.c
    src/dlx_recursive_solver.c)

add_library(dlx SHARED ${SOURCE_FILES})
add_library(sdlx STATIC ${SOURCE_FILES})

target_include_directories(dlx PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

target_include_directories(sdlx PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

# Documentation

find_package(Doxygen)
if(DOXYGEN_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)


# Tests

set(TEST ${CMAKE_CURRENT_SOURCE_DIR}/test/run.py ${CMAKE_CURRENT_SOURCE_DIR}/test/cases)

enable_testing()
add_test(empty ${TEST}/empty)
add_test(1x1-1 ${TEST}/1x1-1)
add_test(2x2-1 ${TEST}/2x2-1)
add_test(2x2-2 ${TEST}/2x2-2)
add_test(2x2-3 ${TEST}/2x2-3)
add_test(2x2-4 ${TEST}/2x2-4)
add_test(3x3-1 ${TEST}/3x3-1)
add_test(3x3-2 ${TEST}/3x3-2)
add_test(latin-square-all-1x1 ${TEST}/latin-square-all-1x1)
add_test(latin-square-all-2x2 ${TEST}/latin-square-all-2x2)
add_test(latin-square-all-3x3 ${TEST}/latin-square-all-3x3)
add_test(latin-square-all-4x4 ${TEST}/latin-square-all-4x4)
add_test(sudoku-all-4x4 ${TEST}/sudoku-all-4x4)
add_test(queens-2 ${TEST}/queens-2)
add_test(queens-3 ${TEST}/queens-3)
add_test(queens-4 ${TEST}/queens-4)
add_test(queens-5 ${TEST}/queens-5)
add_test(queens-6 ${TEST}/queens-6)
add_test(queens-7 ${TEST}/queens-7)
add_test(queens-8 ${TEST}/queens-8)
add_test(queens-9 ${TEST}/queens-9)
add_test(queens-10 ${TEST}/queens-10)
add_test(queens-11 ${TEST}/queens-11)
add_test(queens-12 ${TEST}/queens-12)

