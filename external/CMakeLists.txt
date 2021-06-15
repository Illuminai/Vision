list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

add_subdirectory(cmrc)
add_subdirectory(glfw)
add_subdirectory(freetype)
add_subdirectory(imgui)
add_subdirectory(imgui_impl)
add_subdirectory(implot)
add_subdirectory(stb)
add_subdirectory(glm)
add_subdirectory(tinyobjloader)
add_subdirectory(iconfont)

target_link_libraries(${PROJECT_NAME} PUBLIC
        glfw
        freetype
        imgui
        imgui_impl
        implot
        stb
        glm
        tinyobjloader
        iconfont
        )

# Vulkan
find_package(Vulkan REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Vulkan::Vulkan)

find_package(Shaderc REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Shaderc::Shaderc)

add_subdirectory(vma)
target_link_libraries(${PROJECT_NAME} PUBLIC vma)