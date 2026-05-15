cmake_minimum_required(VERSION 4.2)
project(coffee_dialer)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(external/glfw)

add_executable(coffee_dialer
        src/main.cpp

        external/imgui/imgui.cpp
        external/imgui/imgui_draw.cpp
        external/imgui/imgui_tables.cpp
        external/imgui/imgui_widgets.cpp

        external/imgui/backends/imgui_impl_glfw.cpp
        external/imgui/backends/imgui_impl_opengl3.cpp
        external/sqlite/sqlite3.h
        external/sqlite/sqlite3.c
        src/notes.cpp
        src/db.cpp
        src/model.h
        src/db.h
)

target_include_directories(coffee_dialer PRIVATE
        external/imgui
        external/imgui/backends
        external/glfw/include
)

target_link_libraries(coffee_dialer PRIVATE glfw)

if(APPLE)
    target_link_libraries(coffee_dialer PRIVATE "-framework OpenGL")
endif()