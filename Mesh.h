#ifndef MESH_H
#define MESH_H

#include <vector>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

class PhysicalDeviceWrapper;
class LogicalDeviceWrapper;


struct Vertex {
	glm::vec3 mPosition;
	glm::vec3 mColor;
};

class Mesh {
public:
	Mesh(PhysicalDeviceWrapper*, LogicalDeviceWrapper*, std::vector<Vertex>*);
	~Mesh();

	int GetVertexCount();
	VkBuffer GetVertexBuffer();
private:
	void CreateVertexBuffer(std::vector<Vertex>*);
	uint32_t FindMemoryTypeIndex(uint32_t, VkMemoryPropertyFlags);

	int mVertexCount;
	VkBuffer mVertexBuffer;
	VkDeviceMemory mVertexBufferMemory;

	PhysicalDeviceWrapper* mPhysicalDevice;
	LogicalDeviceWrapper* mLogicalDevice;
};

#endif
