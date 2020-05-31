add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(imgui)
add_subdirectory(imgui_impl)

target_link_libraries(${PROJECT_NAME} PUBLIC
        glad
        glfw
        imgui
        imgui_impl)

# Vulkan
find_package(Vulkan REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Vulkan::Vulkan)