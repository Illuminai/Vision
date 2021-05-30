add_library(vma
        src/Common.cpp
        src/SparseBindingTest.cpp
        src/Tests.cpp
        src/VmaUsage.cpp
        src/VulkanSample.cpp)

target_include_directories(vma PUBLIC include)

list(APPEND libraries Vulkan::Vulkan)

target_link_libraries(vma PUBLIC ${libraries})