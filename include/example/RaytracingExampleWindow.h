#ifndef VISION_RAYTRACINGEXAMPLEWINDOW_H
#define VISION_RAYTRACINGEXAMPLEWINDOW_H

#include "windowing/VulkanWindow.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <tiny_obj_loader.h>
#include <iostream>

struct AllocatedBuffer {
    VkBuffer buffer;
    VmaAllocation allocation;
};

struct VertexInputDescription {

    std::vector<VkVertexInputBindingDescription> bindings;
    std::vector<VkVertexInputAttributeDescription> attributes;

    VkPipelineVertexInputStateCreateFlags flags = 0;
};

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;

    static VertexInputDescription get_vertex_description();
};

struct Mesh {
    std::vector<Vertex> vertices;
    AllocatedBuffer vertexBuffer;

    bool loadFromObj();
};

struct MeshPushConstants {
    glm::vec4 data;
    glm::mat4 render_matrix;
};

struct AllocatedImage {
    VkImage _image;
    VmaAllocation _allocation;
};


class RaytracingExampleWindow : public windowing::VulkanWindow {
public:
    RaytracingExampleWindow();

    ~RaytracingExampleWindow();

    void onRender(vulkan::SyncObject syncObject, uint32_t imageIndex) override;

    void onSwapchainRebuild() override;

private:
    VkRenderPass renderPass{VK_NULL_HANDLE};

    VkPipeline pipeline{VK_NULL_HANDLE};
    VkPipelineLayout layout{VK_NULL_HANDLE};

    std::vector<VkFramebuffer> framebuffers;

    Mesh triangleMesh;
    Mesh cubeMesh;

    int frameNumber{1};

    void createRenderPass();

    void createPipeline();

    void loadMesh();

    void uploadMesh(Mesh& mesh);

};

#endif //VISION_RAYTRACINGEXAMPLEWINDOW_H
