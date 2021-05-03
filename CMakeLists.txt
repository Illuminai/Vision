cmake_minimum_required(VERSION 3.16)
project(Vision)

set(CMAKE_CXX_STANDARD 20)

include_directories("include")

#FILE(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
#add_executable(${PROJECT_NAME} ${SOURCES})

add_executable(${PROJECT_NAME}
        # perfectly done
        src/main.cpp
        src/vulkan/core/SharedContext.cpp
        src/vulkan/core/Instance.cpp
        src/vulkan/core/Surface.cpp
        src/vulkan/core/PhysicalDevice.cpp
        src/vulkan/core/Device.cpp
        src/vulkan/debug/DebugMessenger.cpp
        src/vulkan/debug/ErrorCheck.cpp
        src/vulkan/core/CommandPool.cpp
        src/vulkan/core/Context.cpp
        src/vulkan/core/InFlightFrames.cpp
        src/vulkan/core/Swapchain.cpp


        #TODO: Vulkan
        src/vulkan/shader/ShaderModule.cpp

        #TODO: Windowing
        src/windowing/WindowManager.cpp
        src/windowing/VulkanWindow.cpp
        src/windowing/ImGuiWindow.cpp

        #TODO: Example
        src/example/ImGuiExampleWindow.cpp
        src/example/RaytracingExampleWindow.cpp)


set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
        LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/lib
        ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/lib)

# Dependency Management
add_subdirectory(external)
add_subdirectory(assets)