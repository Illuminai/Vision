add_subdirectory(glfw)
add_subdirectory(imgui)
add_subdirectory(imgui_impl)
add_subdirectory(implot)

target_link_libraries(${PROJECT_NAME} PUBLIC
        glfw
        imgui
        imgui_impl
        implot)

# Vulkan
find_package(Vulkan REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Vulkan::Vulkan)