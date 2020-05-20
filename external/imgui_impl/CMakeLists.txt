add_library(imgui_impl
        src/imgui_impl_glfw.cpp
        src/imgui_impl_opengl3.cpp)

target_include_directories(imgui_impl PUBLIC include)

target_link_libraries(imgui_impl PUBLIC imgui)