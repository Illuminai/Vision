#include "windowing/ImGuiWindow.h"


namespace windowing {

    ImGuiWindow::ImGuiWindow() {

        // CREATE DESCRIPTOR POOLS
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
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        // CREATE RENDER PASS
        createRenderPass();

        // FRAMEBUFFERS
        framebuffers = swapchain->createFramebuffers(renderPass);

        // INIT
        ImGui_ImplGlfw_InitForVulkan(window, true);
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = context->getInstance().getVkInstance();
        init_info.PhysicalDevice = context->getDevice().getVkPhysicalDevice();
        init_info.Device = context->getDevice().getVkDevice();
        init_info.QueueFamily = context->getDevice().getGraphicsQueueFamily();
        init_info.Queue = context->getDevice().getGraphicsQueue();
        init_info.DescriptorPool = descriptorPool;
        init_info.MinImageCount = swapchain->imageCount;
        init_info.ImageCount = swapchain->imageCount;
        init_info.CheckVkResultFn = [](VkResult result) { vulkan::checkError(result, "Internal ImGui operation"); };
        ImGui_ImplVulkan_Init(&init_info, renderPass);


        // UPLOAD FONTS
        ImFont* font2 = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 13.0f);

        context->executeTransient([](VkCommandBuffer commandBuffer) {
            return ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
        });
        ImGui_ImplVulkan_DestroyFontUploadObjects();

        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    }

    ImGuiWindow::~ImGuiWindow() {

    }

    void ImGuiWindow::createRenderPass() {
        vkDestroyRenderPass(context->getDevice().getVkDevice(), renderPass, nullptr);

        VkAttachmentDescription attachment = {};
        attachment.format = swapchain->format.format;
        attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        attachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        VkAttachmentReference color_attachment = {};
        color_attachment.attachment = 0;
        color_attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &color_attachment;
        VkSubpassDependency dependency = {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        VkRenderPassCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        info.attachmentCount = 1;
        info.pAttachments = &attachment;
        info.subpassCount = 1;
        info.pSubpasses = &subpass;
        info.dependencyCount = 1;
        info.pDependencies = &dependency;
        vulkan::checkError(vkCreateRenderPass(context->getDevice().getVkDevice(), &info, nullptr, &renderPass),
                           "Render pass creation");
    }

    void ImGuiWindow::onRender(vulkan::SyncObject syncObject, uint32_t imageIndex) {
        ImGui::SetCurrentContext(imguiContext);

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        onImGuiFrameRender();

        ImGui::Render();
        ImDrawData *main_draw_data = ImGui::GetDrawData();

        renderImGuiFrame(syncObject, imageIndex, main_draw_data);

        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

    void ImGuiWindow::onSwapchainRebuild() {
        createRenderPass();
        framebuffers = swapchain->createFramebuffers(renderPass);
    }

    void ImGuiWindow::renderImGuiFrame(vulkan::SyncObject syncObject, uint32_t imageIndex, ImDrawData *draw_data) {
        vulkan::checkError(vkResetCommandPool(context->getDevice().getVkDevice(),
                                              commandPools[imageIndex].getVkCommandPool(), 0),
                           "d");

        VkCommandBufferBeginInfo commandBufferBeginInfo = {};
        commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        vulkan::checkError(vkBeginCommandBuffer(commandBuffers[imageIndex],
                                                &commandBufferBeginInfo),
                           "f");

        VkRenderPassBeginInfo renderPassBeginInfo = {};
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.renderPass = renderPass;
        renderPassBeginInfo.framebuffer = framebuffers[imageIndex];
        renderPassBeginInfo.renderArea.extent = swapchain->extent;
        //renderPassBeginInfo.clearValueCount = 0;
        //renderPassBeginInfo.pClearValues = &wd->ClearValue;
        vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        ImGui_ImplVulkan_RenderDrawData(draw_data, commandBuffers[imageIndex]);

        vkCmdEndRenderPass(commandBuffers[imageIndex]);

        VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkSubmitInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = &syncObject.imageAvailableSemaphore;
        info.pWaitDstStageMask = &wait_stage;
        info.commandBufferCount = 1;
        info.pCommandBuffers = &commandBuffers[imageIndex];
        info.signalSemaphoreCount = 1;
        info.pSignalSemaphores = &syncObject.renderFinishedSemaphore;

        vulkan::checkError(vkEndCommandBuffer(commandBuffers[imageIndex]),
                           "g");

        vkResetFences(context->getDevice().getVkDevice(), 1, &syncObject.fence);

        vulkan::checkError(vkQueueSubmit(context->getDevice().getGraphicsQueue(), 1, &info, syncObject.fence),
                           "h");
    }
}