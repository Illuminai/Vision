add_library(imgui
        src/imgui.cpp
        src/imgui_draw.cpp
        src/imgui_widgets.cpp
        src/imgui_demo.cpp)

target_include_directories(imgui PUBLIC include)

target_link_libraries(imgui PUBLIC
        glad
        glfw)