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
target_compile_definitions(${PROJECT_NAME} PRIVATE VK_USE_PLATFORM_WIN32_KHR)
target_link_libraries(${PROJECT_NAME} PUBLIC Vulkan::Vulkan)