#ifndef PIPELINE_WRAPPER_H
#define PIPELINE_WRAPPER_H

#include <string>
#include <vector>
#include <vulkan/vulkan.h>

class LogicalDeviceWrapper;
class ShaderWrapper;
class RenderPassWrapper;
class DescriptorSetLayoutWrapper;

/*

	TODO: Implement CreateComputePipeline() function

*/

class PipelineWrapper {
public:
	PipelineWrapper(LogicalDeviceWrapper*, RenderPassWrapper*, DescriptorSetLayoutWrapper*);
	~PipelineWrapper();

	VkPipeline GetPipeline();
	VkPipelineLayout GetPipelineLayout();
private:
	void CreateComputePipeline();
	void CreateGenericGraphicsPipeline();
	void CreateDepthGraphicsPipeline();

	VkPipeline mPipeline;
	VkPipelineLayout mPipelineLayout;

	LogicalDeviceWrapper* mLogicalDevice;
	RenderPassWrapper* mRenderPass;
	DescriptorSetLayoutWrapper* mDescriptorSetLayout;
};

#endif
