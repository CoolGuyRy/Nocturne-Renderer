#ifndef RENDERER_H
#define RENDERER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>

class WindowWrapper;
class InstanceWrapper;
class SurfaceWrapper;
class PhysicalDeviceWrapper;
class LogicalDeviceWrapper;
class SwapchainWrapper;
class ShaderWrapper;
class RenderPassWrapper;
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

struct MVP {
	glm::mat4 mProjection;
	glm::mat4 mView;
	glm::mat4 mModel;
};

/*

	

*/

class Renderer {
public:
	Renderer(WindowWrapper*);
	~Renderer();

	void Draw();

	glm::mat4& GetModel();
private:
	void RecordCommands();

	Mesh* mFirstMesh;

	MVP mMVP;

	int mCurrentFrame;

	WindowWrapper* mWindow;
	InstanceWrapper* mInstance;
	SurfaceWrapper* mSurface;
	PhysicalDeviceWrapper* mPhysicalDevice;
	LogicalDeviceWrapper* mLogicalDevice;
	SwapchainWrapper* mSwapchain;
	RenderPassWrapper* mRenderPass;
	PipelineWrapper* mPipeline;
	std::vector<FramebufferWrapper*> mFramebuffers;
	CommandPoolWrapper* mGraphicsCommandPool;
	CommandPoolWrapper* mTransferCommandPool;
	DescriptorPoolWrapper* mDescriptorPool;
	std::vector<CommandBufferWrapper*> mCommandBuffers;
	std::vector<SemaphoreWrapper*> mImageAvailableSemaphores;
	std::vector<SemaphoreWrapper*> mRenderFinishedSemaphores;
	std::vector<FenceWrapper*> mDrawFences;
	DescriptorSetLayoutWrapper* mDescriptorSetLayout;
	std::vector<BufferWrapper*> mUniformBuffers;
	std::vector<DescriptorSetWrapper*> mDescriptorSets;
};
#endif
