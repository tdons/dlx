project(sudslv C)

set(SOURCE_FILES
        src/main.c)

add_executable(sudslv ${SOURCE_FILES})

target_link_libraries(sudslv sdlx)
