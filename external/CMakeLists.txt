add_subdirectory(cmrc)
add_subdirectory(glfw)
add_subdirectory(freetype)
add_subdirectory(imgui)
add_subdirectory(imgui_impl)
add_subdirectory(implot)

target_link_libraries(${PROJECT_NAME} PUBLIC
        glfw
        freetype
        imgui
        imgui_impl
        implot
        )

# Vulkan
find_package(Vulkan REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Vulkan::Vulkan)