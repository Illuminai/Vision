#include "example/RaytracingExampleWindow.h"

#include <cmrc/cmrc.hpp>
#include "vulkan/shader/ShaderModule.h"
#include <shaderc/shaderc.hpp>
#include <glm/gtx/transform.hpp>

CMRC_DECLARE(shaders);

RaytracingExampleWindow::RaytracingExampleWindow() {
    createRenderPass();

    framebuffers = swapchain->createFramebuffers(renderPass);

    createPipeline();

    loadMesh();
}

RaytracingExampleWindow::~RaytracingExampleWindow() {
    vkDeviceWaitIdle(context->getDevice().getVkDevice());

    vkDestroyPipelineLayout(context->getDevice().getVkDevice(), layout, nullptr);
    vkDestroyPipeline(context->getDevice().getVkDevice(), pipeline, nullptr);

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


    vkCmdBindPipeline(commandBuffers[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);


    glm::vec3 camPos = {0.f, 0.f, -10.f};

    glm::mat4 view = glm::translate(glm::mat4(1.f), camPos);
    //camera projection
    glm::mat4 projection = glm::perspective(glm::radians(70.f), 1700.f / 900.f, 0.1f, 200.0f);
    projection[1][1] *= -1;
    //model rotation
    frameNumber += 1;
    glm::mat4 model = glm::rotate(glm::mat4{1.0f}, glm::radians(frameNumber * 0.4f), glm::vec3(0, 1, 0));

    //calculate final mesh matrix
    glm::mat4 mesh_matrix = projection * view * model;

    MeshPushConstants constants{};
    constants.render_matrix = mesh_matrix;

    //upload the matrix to the GPU via pushconstants
    vkCmdPushConstants(commandBuffers[imageIndex], layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(MeshPushConstants),
                       &constants);

    VkDeviceSize offset = 0;
    vkCmdBindVertexBuffers(commandBuffers[imageIndex], 0, 1, &cubeMesh.vertexBuffer.buffer, &offset);

    vkCmdDraw(commandBuffers[imageIndex], cubeMesh.vertices.size(), 1, 0, 0);


    //vkCmdBindVertexBuffers(commandBuffers[imageIndex], 0, 1, &triangleMesh.vertexBuffer.buffer, &offset);

    //vkCmdDraw(commandBuffers[imageIndex], triangleMesh.vertices.size(), 1, 0, 0);

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
    createPipeline();
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

void RaytracingExampleWindow::createPipeline() {

    if (pipeline != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(context->getDevice().getVkDevice(), layout, nullptr);
        vkDestroyPipeline(context->getDevice().getVkDevice(), pipeline, nullptr);
    }

    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);

    auto fs = cmrc::shaders::get_filesystem();

    shaderc::Compiler compiler;
    shaderc::CompileOptions options;
    options.SetOptimizationLevel(shaderc_optimization_level_size);

    // Fragment Shader
    auto fragmentShaderResource = fs.open("shaders/rasterization/basic.frag");

    std::string fragmentShaderSource{fragmentShaderResource.begin(), fragmentShaderResource.end()};

    shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(
            fragmentShaderSource, shaderc_glsl_fragment_shader, "t", options);
    if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
        std::cerr << result.GetErrorMessage();
    }

    std::vector<uint32_t> fragmentSpirv{result.cbegin(), result.cend()};

    vulkan::ShaderModule fragmentShaderModule{context, fragmentSpirv};

    VkPipelineShaderStageCreateInfo fragmentShaderStageCreateInfo{};
    fragmentShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentShaderStageCreateInfo.module = fragmentShaderModule.getVkShaderModule();
    fragmentShaderStageCreateInfo.pName = "main";

    // Vertex Shader
    auto vertexShaderResource = fs.open("shaders/rasterization/basic.vert");

    std::string vertexShaderSource{vertexShaderResource.begin(), vertexShaderResource.end()};

    result = compiler.CompileGlslToSpv(
            vertexShaderSource, shaderc_glsl_vertex_shader, "tv", options);
    if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
        std::cerr << result.GetErrorMessage();
    }

    std::vector<uint32_t> vertexSpirv{result.cbegin(), result.cend()};

    vulkan::ShaderModule vertexShaderModule{context, vertexSpirv};

    VkPipelineShaderStageCreateInfo vertexShaderStageCreateInfo{};
    vertexShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertexShaderStageCreateInfo.module = vertexShaderModule.getVkShaderModule();
    vertexShaderStageCreateInfo.pName = "main";

    std::vector<VkPipelineShaderStageCreateInfo> shaderStages{vertexShaderStageCreateInfo,
                                                              fragmentShaderStageCreateInfo};



    // GENERAL STUFF IDK
    VertexInputDescription vertexDescription = Vertex::get_vertex_description();

    VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo{};
    vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputStateCreateInfo.vertexBindingDescriptionCount = vertexDescription.bindings.size();
    vertexInputStateCreateInfo.pVertexBindingDescriptions = vertexDescription.bindings.data();

    vertexInputStateCreateInfo.vertexAttributeDescriptionCount = vertexDescription.attributes.size();
    vertexInputStateCreateInfo.pVertexAttributeDescriptions = vertexDescription.attributes.data();

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

    // JUST STUFF

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = width;
    viewport.height = height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;


    // TODO: FINISH PIPELINE CREATION

    VkPipelineViewportStateCreateInfo viewportStateCreateInfo{};
    viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportStateCreateInfo.viewportCount = 1;
    viewportStateCreateInfo.pViewports = &viewport;
    viewportStateCreateInfo.scissorCount = 1;
    viewportStateCreateInfo.pScissors = &scissor;

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.pNext = nullptr;

    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;

    // PIPELINE LAYOUT

    VkPipelineLayoutCreateInfo layoutCreateInfo{};
    layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutCreateInfo.pNext = nullptr;

    layoutCreateInfo.flags = 0;
    layoutCreateInfo.setLayoutCount = 0;
    layoutCreateInfo.pSetLayouts = nullptr;

    VkPushConstantRange push_constant;
    //this push constant range starts at the beginning
    push_constant.offset = 0;
    //this push constant range takes up the size of a MeshPushConstants struct
    push_constant.size = sizeof(MeshPushConstants);
    push_constant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    layoutCreateInfo.pushConstantRangeCount = 1;
    layoutCreateInfo.pPushConstantRanges = &push_constant;

    vulkan::checkError(vkCreatePipelineLayout(context->getDevice().getVkDevice(), &layoutCreateInfo,
                                              nullptr, &layout),
                       "Pipeline layout creation");


    // PIPELINE

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = nullptr;

    pipelineInfo.stageCount = shaderStages.size();
    pipelineInfo.pStages = shaderStages.data();
    pipelineInfo.pVertexInputState = &vertexInputStateCreateInfo;
    pipelineInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
    pipelineInfo.pViewportState = &viewportStateCreateInfo;
    pipelineInfo.pRasterizationState = &rasterizationStateCreateInfo;
    pipelineInfo.pMultisampleState = &multisampleStateCreateInfo;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.layout = layout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    vulkan::checkError(vkCreateGraphicsPipelines(context->getDevice().getVkDevice(), VK_NULL_HANDLE, 1, &pipelineInfo,
                                                 nullptr, &pipeline),
                       "Graphics pipeline creation");

    vertexShaderModule.destroy();
    fragmentShaderModule.destroy();
}

void RaytracingExampleWindow::loadMesh() {
    triangleMesh.vertices.resize(3);

    //vertex positions
    triangleMesh.vertices[0].position = {1.f, 1.f, 0.0f};
    triangleMesh.vertices[1].position = {-1.f, 1.f, 0.0f};
    triangleMesh.vertices[2].position = {0.f, -1.f, 0.0f};

    //vertex colors, all green
    triangleMesh.vertices[0].color = {0.f, 1.f, 0.0f}; //pure green
    triangleMesh.vertices[1].color = {0.f, 0.f, 1.0f}; //pure green
    triangleMesh.vertices[2].color = {0.f, 1.f, 1.0f}; //pure green

    //we don't care about the vertex normals

    cubeMesh.loadFromObj();
    uploadMesh(cubeMesh);

    uploadMesh(triangleMesh);
}

void RaytracingExampleWindow::uploadMesh(Mesh &mesh) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = mesh.vertices.size() * sizeof(Vertex);
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

    VmaAllocationCreateInfo vmaAllocationCreateInfo{};
    vmaAllocationCreateInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

    vulkan::checkError(vmaCreateBuffer(context->getAllocator(), &bufferInfo,
                                       &vmaAllocationCreateInfo,
                                       &mesh.vertexBuffer.buffer,
                                       &mesh.vertexBuffer.allocation, nullptr),
                       "VMA");

    void *data;
    vmaMapMemory(context->getAllocator(), mesh.vertexBuffer.allocation, &data);

    memcpy(data, mesh.vertices.data(), mesh.vertices.size() * sizeof(Vertex));

    vmaUnmapMemory(context->getAllocator(), mesh.vertexBuffer.allocation);
}

#define TINYOBJLOADER_IMPLEMENTATION

#include "tiny_obj_loader.h"

VertexInputDescription Vertex::get_vertex_description() {
    VertexInputDescription description;

    //we will have just 1 vertex buffer binding, with a per-vertex rate
    VkVertexInputBindingDescription mainBinding = {};
    mainBinding.binding = 0;
    mainBinding.stride = sizeof(Vertex);
    mainBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    description.bindings.push_back(mainBinding);

    //Position will be stored at Location 0
    VkVertexInputAttributeDescription positionAttribute = {};
    positionAttribute.binding = 0;
    positionAttribute.location = 0;
    positionAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    positionAttribute.offset = offsetof(Vertex, position);

    //Normal will be stored at Location 1
    VkVertexInputAttributeDescription normalAttribute = {};
    normalAttribute.binding = 0;
    normalAttribute.location = 1;
    normalAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    normalAttribute.offset = offsetof(Vertex, normal);

    //Color will be stored at Location 2
    VkVertexInputAttributeDescription colorAttribute = {};
    colorAttribute.binding = 0;
    colorAttribute.location = 2;
    colorAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    colorAttribute.offset = offsetof(Vertex, color);

    description.attributes.push_back(positionAttribute);
    description.attributes.push_back(normalAttribute);
    description.attributes.push_back(colorAttribute);
    return description;
}

bool Mesh::loadFromObj() {

    std::string inputfile = "./cube.obj";
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str());

    if (!err.empty()) {
        std::cerr << err << std::endl;
    }

    if (!ret) {
        exit(1);
    }

    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {

            //hardcode loading to triangles
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];


                Vertex new_vert;

                //vertex position
                new_vert.position.x = attrib.vertices[3 * idx.vertex_index + 0];
                new_vert.position.y= attrib.vertices[3 * idx.vertex_index + 1];
                new_vert.position.z= attrib.vertices[3 * idx.vertex_index + 2];
                //vertex normal
                if (idx.normal_index >= 0) {
                    new_vert.normal.x = attrib.normals[3*size_t(idx.normal_index)+0];
                    new_vert.normal.y = attrib.normals[3*size_t(idx.normal_index)+1];
                    new_vert.normal.z = attrib.normals[3*size_t(idx.normal_index)+2];
                    new_vert.color = new_vert.normal;
                }else{
                    new_vert.color = glm::vec3(0,1,0);
                }

                //we are setting the vertex color as the vertex normal. This is just for display purposes



                vertices.push_back(new_vert);
            }
            index_offset += fv;
        }
    }
    return true;
}
