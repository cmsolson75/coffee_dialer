cmake_minimum_required(VERSION 3.24)
project(coffee_dialer LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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
        src
        external/sqlite
        external/imgui
        external/imgui/backends
        external/glfw/include
)

target_link_libraries(coffee_dialer PRIVATE glfw)

if(APPLE)
    target_compile_definitions(coffee_dialer PRIVATE
            GL_SILENCE_DEPRECATION
    )

    target_link_libraries(coffee_dialer PRIVATE
            "-framework OpenGL"
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreVideo"
    )

    set_target_properties(coffee_dialer PROPERTIES
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_BUNDLE_NAME "Coffee Dialer"
            MACOSX_BUNDLE_GUI_IDENTIFIER "com.cameronolson.coffeedialer"
            MACOSX_BUNDLE_BUNDLE_VERSION "1.0.0"
            MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0.0"
            MACOSX_BUNDLE_COPYRIGHT "Copyright © 2026 Cameron Olson"
    )
endif()