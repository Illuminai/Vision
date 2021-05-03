#include "example/RaytracingExampleWindow.h"

#include <cmrc/cmrc.hpp>
#include "vulkan/shader/ShaderModule.h"

CMRC_DECLARE(shaders);

RaytracingExampleWindow::RaytracingExampleWindow() {
    createRenderPass();

    framebuffers = swapchain->createFramebuffers(renderPass);

    auto fs = cmrc::shaders::get_filesystem();


    // Fragment Shader
    auto fragmentShaderResource = fs.open("shaders/rasterization/basic.frag");

    std::vector<uint32_t> fragmentShaderSource{fragmentShaderResource.begin(), fragmentShaderResource.end()};

    vulkan::ShaderModule fragmentShaderModule{context, fragmentShaderSource};

    VkPipelineShaderStageCreateInfo fragmentShaderStageCreateInfo{};
    fragmentShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentShaderStageCreateInfo.module = fragmentShaderModule.getVkShaderModule();
    fragmentShaderStageCreateInfo.pName = "main";

    // Vertex Shader
    auto vertexShaderResource = fs.open("shaders/rasterization/basic.vert");

    std::vector<uint32_t> vertexShaderSource{vertexShaderResource.begin(), vertexShaderResource.end()};

    vulkan::ShaderModule vertexShaderModule{context, vertexShaderSource};

    VkPipelineShaderStageCreateInfo vertexShaderStageCreateInfo{};
    vertexShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertexShaderStageCreateInfo.module = vertexShaderModule.getVkShaderModule();
    vertexShaderStageCreateInfo.pName = "main";

    // GENERAL STUFF IDK

    VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo{};
    vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputStateCreateInfo.vertexBindingDescriptionCount = 0;
    vertexInputStateCreateInfo.vertexAttributeDescriptionCount = 0;

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {};
    inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

    VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo{};
    rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;


    rasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
    //discards all primitives before the rasterization stage if enabled which we don't want
    rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;

    // We are just going to leave polygonMode as editable input, to be able to toggle between wireframe and solid drawing.
    rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizationStateCreateInfo.lineWidth = 1.0f;
    //no backface cull
    rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_NONE;
    rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    //no depth bias
    rasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
    rasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
    rasterizationStateCreateInfo.depthBiasClamp = 0.0f;
    rasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f;


    VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo{};
    multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
    multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampleStateCreateInfo.minSampleShading = 1.0f;
    multisampleStateCreateInfo.pSampleMask = nullptr;
    multisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
    multisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                          VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;


    // TODO: FINISH PIPELINE CREATION

    /*VkPipelineViewportStateCreateInfo viewportStateCreateInfo{};
    viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportStateCreateInfo.viewportCount = 1;
    viewportStateCreateInfo.pViewports = &_viewport;
    viewportStateCreateInfo.scissorCount = 1;
    viewportStateCreateInfo.pScissors = &_scissor;*/

}

RaytracingExampleWindow::~RaytracingExampleWindow() {
    vkDeviceWaitIdle(context->getDevice().getVkDevice());
    for (auto framebuffer : framebuffers) {
        vkDestroyFramebuffer(context->getDevice().getVkDevice(), framebuffer, nullptr);
    }

    vkDestroyRenderPass(context->getDevice().getVkDevice(), renderPass, nullptr);
}


void RaytracingExampleWindow::onRender(vulkan::SyncObject syncObject, uint32_t imageIndex) {
    vulkan::checkError(vkResetCommandPool(context->getDevice().getVkDevice(),
                                          commandPools[imageIndex].getVkCommandPool(), 0),
                       "Command buffer reset");

    VkCommandBufferBeginInfo commandBufferBeginInfo = {};
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferBeginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vulkan::checkError(vkBeginCommandBuffer(commandBuffers[imageIndex],
                                            &commandBufferBeginInfo),
                       "Command buffer begin");

    VkRenderPassBeginInfo renderPassBeginInfo = {};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.renderPass = renderPass;
    renderPassBeginInfo.framebuffer = framebuffers[imageIndex];
    renderPassBeginInfo.renderArea.extent = swapchain->extent;
    //renderPassBeginInfo.clearValueCount = 0;
    //renderPassBeginInfo.pClearValues = &wd->ClearValue;
    vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    //ImGui_ImplVulkan_RenderDrawData(draw_data, commandBuffers[imageIndex]);

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
                       "Command buffer end");

    vkResetFences(context->getDevice().getVkDevice(), 1, &syncObject.fence);

    vulkan::checkError(vkQueueSubmit(context->getDevice().getGraphicsQueue(), 1, &info, syncObject.fence),
                       "Queue submit");
}

void RaytracingExampleWindow::onSwapchainRebuild() {
    createRenderPass();
    for (auto framebuffer : framebuffers) {
        vkDestroyFramebuffer(context->getDevice().getVkDevice(), framebuffer, nullptr);
    }
    framebuffers = swapchain->createFramebuffers(renderPass);
}

void RaytracingExampleWindow::createRenderPass() {
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


