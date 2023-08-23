#ifndef MESH_H
#define MESH_H

#include <vector>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

class PhysicalDeviceWrapper;
class LogicalDeviceWrapper;
class CommandPoolWrapper;
class BufferWrapper;


struct Vertex {
	glm::vec3 mPosition;
	glm::vec3 mColor;
	glm::vec2 mUV;
};

class Mesh {
public:
	Mesh(PhysicalDeviceWrapper*, LogicalDeviceWrapper*, CommandPoolWrapper*, std::vector<Vertex>*, std::vector<uint32_t>*);
	Mesh(PhysicalDeviceWrapper*, LogicalDeviceWrapper*, CommandPoolWrapper*, std::vector<Vertex>*, std::vector<uint32_t>*, int);
	~Mesh();

	glm::mat4 GetModel();
	void SetModel(glm::mat4);

	int GetTexID();
	int GetVertexCount();
	int GetIndexCount();
	BufferWrapper* GetVertexBuffer();
	BufferWrapper* GetIndexBuffer();
private:
	void CreateVertexBuffer(std::vector<Vertex>*);
	void CreateIndexBuffer(std::vector<uint32_t>*);

	glm::mat4 mModel;

	int mTexID;
	int mVertexCount;
	int mIndexCount;
	BufferWrapper* mVertexBuffer;
	BufferWrapper* mIndexBuffer;

	PhysicalDeviceWrapper* mPhysicalDevice;
	LogicalDeviceWrapper* mLogicalDevice;
	CommandPoolWrapper* mTransferCommandPool;
};

#endif
