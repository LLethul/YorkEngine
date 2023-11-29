#ifndef MODEL_H_
#define MODEL_H_

#include "vulkan/vulkan_core.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <vector>
#include "Device.h"

class Model {
     public:
  struct Vertex {
    glm::vec2 position;
    glm::vec3 color;

    static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
  };

  Model(VKDevice &device, const std::vector<Vertex> &vertices);
  ~Model();

  Model(const Model &) = delete;
  Model &operator=(const Model &) = delete;

  void bind(VkCommandBuffer commandBuffer);
  void draw(VkCommandBuffer commandBuffer);

 private:
  void createVertexBuffers(const std::vector<Vertex> &vertices);

  VKDevice &device;
  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  uint32_t vertexCount;
};

#endif // MODEL_H_