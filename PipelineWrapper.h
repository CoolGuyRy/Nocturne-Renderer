#ifndef PIPELINE_WRAPPER_H
#define PIPELINE_WRAPPER_H

#include <string>
#include <vector>
#include <vulkan/vulkan.h>

class Context;
class ShaderWrapper;
class DescriptorSetLayoutWrapper;

/*

	TODO: Implement CreateComputePipeline() function

*/

class PipelineWrapper {
public:
	PipelineWrapper(Context*, std::vector<DescriptorSetLayoutWrapper*>);
	~PipelineWrapper();

	VkPipeline GetPipeline();
	VkPipelineLayout GetPipelineLayout();
private:
	void CreateComputePipeline();
	void CreateGenericGraphicsPipeline();
	void CreateDepthGraphicsPipeline();

	VkPipeline mPipeline;
	VkPipelineLayout mPipelineLayout;

	Context* mContext;
	std::vector<DescriptorSetLayoutWrapper*> mDescriptorSetLayouts;
};

#endif
