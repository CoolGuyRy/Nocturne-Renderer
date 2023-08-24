#ifndef RENDERER_H
#define RENDERER_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>

class Context;
class ShaderWrapper;
class PipelineWrapper;
class FramebufferWrapper;
class CommandPoolWrapper;
class CommandBufferWrapper;
class SemaphoreWrapper;
class FenceWrapper;
class BufferWrapper;
class Mesh;
class DescriptorSetLayoutWrapper;
class DescriptorPoolWrapper;
class DescriptorSetWrapper;
class ImageWrapper;
class ImageViewWrapper;
class SamplerWrapper;

struct UboViewProjection {
	glm::mat4 mProjection;
	glm::mat4 mView;
};

/*

	

*/

class Renderer {
public:
	Renderer();
	~Renderer();

	void Draw();

	void UpdateModel(int, glm::mat4);

	void UpdateCamera(glm::mat4);

	Context* GetContext();
private:
	void RecordCommands();

	void allocateDynamicBufferTransferSpace();

	std::vector<Mesh*> mMeshList;

	UboViewProjection mVP;

	int mCurrentFrame;

	VkDeviceSize mMinUniformBufferOffset;
	size_t mModelUniformAlignment;
	glm::mat4* mModelTransferSpace;

	Context* mContext;
	PipelineWrapper* mPipeline;
	std::vector<FramebufferWrapper*> mFramebuffers;
	CommandPoolWrapper* mGraphicsCommandPool;
	CommandPoolWrapper* mTransferCommandPool;
	DescriptorPoolWrapper* mDescriptorPool;
	DescriptorPoolWrapper* mTDescriptorPool;
	ImageWrapper* mDepthImage;
	ImageViewWrapper* mDepthImageView;
	ImageWrapper* mTextureImage;
	ImageViewWrapper* mTextureImageView;
	std::vector<CommandBufferWrapper*> mCommandBuffers;
	std::vector<SemaphoreWrapper*> mImageAvailableSemaphores;
	std::vector<SemaphoreWrapper*> mRenderFinishedSemaphores;
	std::vector<FenceWrapper*> mDrawFences;
	DescriptorSetLayoutWrapper* mDescriptorSetLayout;
	DescriptorSetLayoutWrapper* mTSDescriptorSetLayout;
	std::vector<BufferWrapper*> mUniformBuffers;
	std::vector<BufferWrapper*> mDynamicUniformBuffers;
	std::vector<DescriptorSetWrapper*> mDescriptorSets;
	DescriptorSetWrapper* mTextureDescriptorSet;
	SamplerWrapper* mSampler;
};
#endif
