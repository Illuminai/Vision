#include "windowing/ImGuiWindow.h"


namespace windowing {


    ImGuiWindow::ImGuiWindow() {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        if (!glfwVulkanSupported()) {
            throw std::runtime_error("Vulkan not supported!");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);

        std::vector<std::tuple<const char *, bool>> instanceExtensions = {
                //std::make_tuple("TESTTEST", false), Extension not optional and not available. will crash.
                std::make_tuple("OOF", true) // Extension optional
        };
        uint32_t extensions_count = 0;
        const char **extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
        std::vector<const char *> glfwEx(extensions, extensions + extensions_count);

        for (auto ex : glfwEx) {
            instanceExtensions.emplace_back(ex, false);
        }

        const std::vector<const char *> validationLayers = {
                "VK_LAYER_KHRONOS_validation"
        };

        std::vector<std::tuple<const char *, bool>> deviceExtensions = {
                //std::make_tuple(VK_KHR_SWAPCHAIN_EXTENSION_NAME, false),
                //std::make_tuple("TESTTEST", false), Extension not optional and not available. will crash.
                std::make_tuple("OOF", true) // Extension optional
        };


        context = std::make_shared<vulkan::Context>(instanceExtensions,
                                                    validationLayers,
                                                    deviceExtensions,
                                                    window);



        // CREATE DESCRIPTOR POOL
        VkDescriptorPoolSize pool_sizes[] = {
                {VK_DESCRIPTOR_TYPE_SAMPLER,                1000},
                {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
                {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,          1000},
                {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,          1000},
                {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,   1000},
                {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,   1000},
                {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         1000},
                {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         1000},
                {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
                {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
                {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,       1000}
        };
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
        pool_info.poolSizeCount = (uint32_t) IM_ARRAYSIZE(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;
        vulkan::checkError(
                vkCreateDescriptorPool(context->getDevice().getVkDevice(),
                                       &pool_info, nullptr, &descriptorPool),
                "ImGui descriptor pool creation");

        // CREATE CONTEXT
        imguiContext = ImGui::CreateContext();
        ImGui::SetCurrentContext(imguiContext);

        ImGui::StyleColorsDark();

        ImGuiIO &io = ImGui::GetIO();
        (void) io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        // INIT WINDOW DATA
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        imguiWindowData.Surface = context->getSurface()->getVkSurface();
        auto details = vulkan::SwapchainSupportDetails(context->getDevice().getVkPhysicalDevice(),
                                                       context->getSurface()->getVkSurface());
        imguiWindowData.SurfaceFormat = details.getOptimalSurfaceFormat();
        imguiWindowData.PresentMode = details.getOptimalPresentMode();
        ImGui_ImplVulkanH_CreateOrResizeWindow(context->getInstance().getVkInstance(),
                                               context->getDevice().getVkPhysicalDevice(),
                                               context->getDevice().getVkDevice(), &imguiWindowData,
                                               context->getDevice().getGraphicsQueueFamily(),
                                               nullptr, w, h, details.getOptimalImageCount());

        // INIT
        ImGui_ImplGlfw_InitForVulkan(window, true);
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = context->getInstance().getVkInstance();
        init_info.PhysicalDevice = context->getDevice().getVkPhysicalDevice();
        init_info.Device = context->getDevice().getVkDevice();
        init_info.QueueFamily = context->getDevice().getGraphicsQueueFamily();
        init_info.Queue = context->getDevice().getGraphicsQueue();
        init_info.DescriptorPool = descriptorPool;
        init_info.MinImageCount = details.getOptimalImageCount();
        init_info.ImageCount = details.getOptimalImageCount();
        init_info.CheckVkResultFn = [](VkResult result) { vulkan::checkError(result, "Internal ImGui operation"); };
        ImGui_ImplVulkan_Init(&init_info, /* TODO: OWN RENDER PASS*/ imguiWindowData.RenderPass);

        // UPLOAD FONTS
        context->executeTransient([](VkCommandBuffer commandBuffer) {
            return ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
        });
        ImGui_ImplVulkan_DestroyFontUploadObjects();

        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    }


    ImGuiWindow::~ImGuiWindow() {
        imguiWindowData.Surface = VK_NULL_HANDLE;
        ImGui_ImplVulkanH_DestroyWindow(context->getInstance().getVkInstance(),
                                        context->getDevice().getVkDevice(), &imguiWindowData, nullptr);

        vkDestroyDescriptorPool(context->getDevice().getVkDevice(),
                                descriptorPool, nullptr);

        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext(imguiContext);

        context.reset();

        glfwDestroyWindow(window);

    }


    void ImGuiWindow::renderFrame(ImDrawData *draw_data) {
        VkSemaphore imageAvailableSemaphore = imguiWindowData.FrameSemaphores[imguiWindowData.SemaphoreIndex].ImageAcquiredSemaphore;
        VkSemaphore renderFinishedSemaphore = imguiWindowData.FrameSemaphores[imguiWindowData.SemaphoreIndex].RenderCompleteSemaphore;
        VkResult err = vkAcquireNextImageKHR(context->getDevice().getVkDevice(), imguiWindowData.Swapchain, UINT64_MAX,
                                             imageAvailableSemaphore, VK_NULL_HANDLE, &imguiWindowData.FrameIndex);
        if (err == VK_ERROR_OUT_OF_DATE_KHR) {
            rebuildSwapchain = true;
            return;
        }
        vulkan::checkError(err, "ImGui render frame acquire next image");

        ImGui_ImplVulkanH_Frame *fd = &imguiWindowData.Frames[imguiWindowData.FrameIndex];

        vulkan::checkError(vkWaitForFences(context->getDevice().getVkDevice(), 1, &fd->Fence, VK_TRUE,
                                           UINT64_MAX),
                           "ImGui render frame wait for fences");

        vulkan::checkError(vkResetFences(context->getDevice().getVkDevice(), 1, &fd->Fence),
                           "ImGui render frame reset fences");

        vulkan::checkError(vkResetCommandPool(context->getDevice().getVkDevice(), fd->CommandPool, 0),
                           "ImGui render frame reset command pool");

        VkCommandBufferBeginInfo commandBufferBeginInfo = {};
        commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        vulkan::checkError(vkBeginCommandBuffer(fd->CommandBuffer, &commandBufferBeginInfo),
                           "ImGui render frame begin command buffer");

        VkRenderPassBeginInfo renderPassBeginInfo = {};
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.renderPass = imguiWindowData.RenderPass;
        renderPassBeginInfo.framebuffer = fd->Framebuffer;
        renderPassBeginInfo.renderArea.extent.width = imguiWindowData.Width;
        renderPassBeginInfo.renderArea.extent.height = imguiWindowData.Height;
        renderPassBeginInfo.clearValueCount = 1;
        renderPassBeginInfo.pClearValues = &imguiWindowData.ClearValue;
        vkCmdBeginRenderPass(fd->CommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);

        vkCmdEndRenderPass(fd->CommandBuffer);

        VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &imageAvailableSemaphore;
        submitInfo.pWaitDstStageMask = &wait_stage;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &fd->CommandBuffer;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &renderFinishedSemaphore;

        vulkan::checkError(vkEndCommandBuffer(fd->CommandBuffer),
                           "ImGui render frame end command buffer");

        vulkan::checkError(vkQueueSubmit(context->getDevice().getGraphicsQueue(), 1, &submitInfo, fd->Fence),
                           "ImGui render frame queue submit");

    }

    void ImGuiWindow::renderImGui() {
        if (rebuildSwapchain) {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            if (width > 0 && height > 0) {
                ImGui_ImplVulkan_SetMinImageCount(3);
                ImGui_ImplVulkanH_CreateOrResizeWindow(context->getInstance().getVkInstance(),
                                                       context->getDevice().getVkPhysicalDevice(),
                                                       context->getDevice().getVkDevice(), &imguiWindowData,
                                                       context->getDevice().getGraphicsQueueFamily(),
                                                       nullptr, width, height, 2);
                imguiWindowData.FrameIndex = 0;
                rebuildSwapchain = false;
            }
        }

        ImGui::SetCurrentContext(imguiContext);

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // DRAW STUFF
        // replace with ::onRenderUI();
        ImGui::ShowDemoWindow();

        ImGui::Render();
        ImDrawData *main_draw_data = ImGui::GetDrawData();
        const bool main_is_minimized = (main_draw_data->DisplaySize.x <= 0.0f
                                        || main_draw_data->DisplaySize.y <= 0.0f);

        if (!main_is_minimized)
            renderFrame(main_draw_data);

        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        presentFrame();
    }

    void ImGuiWindow::loopWindow() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            renderImGui();
        }
    }

    void ImGuiWindow::presentFrame() {
        if (rebuildSwapchain) {
            return;
        }

        VkSemaphore render_complete_semaphore = imguiWindowData.FrameSemaphores[imguiWindowData.SemaphoreIndex].RenderCompleteSemaphore;
        VkPresentInfoKHR info = {};
        info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = &render_complete_semaphore;
        info.swapchainCount = 1;
        info.pSwapchains = &imguiWindowData.Swapchain;
        info.pImageIndices = &imguiWindowData.FrameIndex;
        VkResult err = vkQueuePresentKHR(context->getDevice().getGraphicsQueue(), &info);
        if (err == VK_ERROR_OUT_OF_DATE_KHR) {
            rebuildSwapchain = true;
            return;
        }
        vulkan::checkError(err, "ImGui present frame queue present");
        imguiWindowData.SemaphoreIndex = (imguiWindowData.SemaphoreIndex + 1) % imguiWindowData.ImageCount;
    }

    void ImGuiWindow::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
        static_cast<ImGuiWindow*>(glfwGetWindowUserPointer(window))->renderImGui();
    }

}