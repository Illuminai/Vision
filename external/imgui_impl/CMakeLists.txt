add_library(imgui_impl
        src/imgui_impl_glfw.cpp
        src/imgui_impl_vulkan.cpp)

target_include_directories(imgui_impl PUBLIC include)

find_package(Vulkan REQUIRED)
target_link_libraries(imgui PUBLIC Vulkan::Vulkan)

target_link_libraries(imgui_impl PUBLIC imgui)