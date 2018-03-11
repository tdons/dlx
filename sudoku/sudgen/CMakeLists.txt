project(sudgen C)

set(SOURCE_FILES
        src/fisher_yates.c
        src/main.c)

add_executable(sudgen ${SOURCE_FILES})

target_link_libraries(sudgen sdlx)
