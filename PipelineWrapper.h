#ifndef PIPELINE_WRAPPER_H
#define PIPELINE_WRAPPER_H

#include <string>
#include <vector>
#include <vulkan/vulkan.h>

class LogicalDeviceWrapper;
class ShaderWrapper;
class RenderPassWrapper;

/*

	TODO: Implement CreateComputePipeline() function

*/

class PipelineWrapper {
public:
	PipelineWrapper(LogicalDeviceWrapper*, RenderPassWrapper*);
	~PipelineWrapper();

	VkPipeline GetPipeline();
private:
	void CreateComputePipeline();
	void CreateGenericGraphicsPipeline();

	VkPipeline mPipeline;
	VkPipelineLayout mPipelineLayout;

	LogicalDeviceWrapper* mLogicalDevice;
	RenderPassWrapper* mRenderPass;
};

#endif
