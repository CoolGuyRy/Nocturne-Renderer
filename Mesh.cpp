#include "Mesh.h"
#include "globals.h"
#include "PhysicalDeviceWrapper.h"
#include "LogicalDeviceWrapper.h"
#include "CommandPoolWrapper.h"
#include "BufferWrapper.h"

Mesh::Mesh(PhysicalDeviceWrapper* pDevice, LogicalDeviceWrapper* lDevice, CommandPoolWrapper* tPool, std::vector<Vertex>* vertices) : mPhysicalDevice(pDevice), mLogicalDevice(lDevice), mTransferCommandPool(tPool) {
	CreateVertexBuffer(vertices);
}

Mesh::Mesh(PhysicalDeviceWrapper* pDevice, LogicalDeviceWrapper* lDevice, CommandPoolWrapper* tPool, std::vector<Vertex>* vertices, std::vector<uint32_t>* indices) : mPhysicalDevice(pDevice), mLogicalDevice(lDevice), mTransferCommandPool(tPool) {
	CreateVertexBuffer(vertices);
	CreateIndexBuffer(indices);
}

Mesh::~Mesh() {
	delete mIndexBuffer;
	delete mVertexBuffer;
}

int Mesh::GetVertexCount() {
	return mVertexCount;
}

int Mesh::GetIndexCount() {
	return mIndexCount;
}

BufferWrapper* Mesh::GetVertexBuffer() {
	return mVertexBuffer;
}

BufferWrapper* Mesh::GetIndexBuffer() {
	return mIndexBuffer;
}

void Mesh::CreateVertexBuffer(std::vector<Vertex>* vertices) {
	mVertexCount = (int)vertices->size();

	VkDeviceSize bufferSize = sizeof(Vertex) * vertices->size();

	BufferWrapper* stagingBuffer = new BufferWrapper(mPhysicalDevice, mLogicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	stagingBuffer->MapBufferMemory(vertices->data(), bufferSize);

	mVertexBuffer = new BufferWrapper(mPhysicalDevice, mLogicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	CopyBuffer(mLogicalDevice, mTransferCommandPool, stagingBuffer, mVertexBuffer, bufferSize);

	delete stagingBuffer;
}

void Mesh::CreateIndexBuffer(std::vector<uint32_t>* indices) {
	mIndexCount = (int)indices->size();

	VkDeviceSize bufferSize = sizeof(uint32_t) * indices->size();

	BufferWrapper* stagingBuffer = new BufferWrapper(mPhysicalDevice, mLogicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	stagingBuffer->MapBufferMemory(indices->data(), bufferSize);

	mIndexBuffer = new BufferWrapper(mPhysicalDevice, mLogicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	CopyBuffer(mLogicalDevice, mTransferCommandPool, stagingBuffer, mIndexBuffer, bufferSize);

	delete stagingBuffer;
}