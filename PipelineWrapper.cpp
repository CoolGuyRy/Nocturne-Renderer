#include "PipelineWrapper.h"
#include "globals.h"
#include "LogicalDeviceWrapper.h"
#include "ShaderWrapper.h"
#include "RenderPassWrapper.h"

PipelineWrapper::PipelineWrapper(LogicalDeviceWrapper* lDevice, RenderPassWrapper* renderpass) : mLogicalDevice(lDevice), mRenderPass(renderpass) {
	CreateGenericGraphicsPipeline();
}

PipelineWrapper::~PipelineWrapper() {
	vkDestroyPipelineLayout(mLogicalDevice->GetLogicalDevice(), mPipelineLayout, nullptr);	std::cout << "Success: Pipeline Layout Destroyed" << std::endl;
	vkDestroyPipeline(mLogicalDevice->GetLogicalDevice(), mPipeline, nullptr);	std::cout << "Success: Pipeline Destroyed" << std::endl;
}

void PipelineWrapper::CreateComputePipeline() {
}

void PipelineWrapper::CreateGenericGraphicsPipeline() {
	// Grab the shader file locations
	std::vector<std::string> shaderFileNames = {
		".\\Resources\\Shaders\\simple.vert.spv",
		".\\Resources\\Shaders\\simple.frag.spv"
	};

	// Initialize ShaderWrapper classes
	std::vector<ShaderWrapper*> mShaders(shaderFileNames.size());
	for (size_t i = 0; i < shaderFileNames.size(); i++) {
		mShaders.at(i) = new ShaderWrapper(mLogicalDevice, shaderFileNames[i]);
	}

	// Create the shader stage create info structs
	std::vector<VkPipelineShaderStageCreateInfo> shaderStageCIs(mShaders.size());
	for (size_t i = 0; i < mShaders.size(); i++) {
		shaderStageCIs.at(i) = mShaders.at(i)->GetShaderCI();
	}

	// Create the vertex input state create info struct
	VkPipelineVertexInputStateCreateInfo vertexInputCI = {
		VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,			// sType
		nullptr,															// pNext
		0,																	// flags
		0,																	// vertexBindingDescriptionCount
		nullptr,															// pVertexBindingDescriptions
		0,																	// vertexAttributeDescriptionCount
		nullptr																// pVertexAttributeDescriptions
	};

	// Create the input assembly state create info struct
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyCI = {
		VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,		// sType
		nullptr,															// pNext
		0,																	// flags
		VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,								// topology
		VK_FALSE															// primitiveRestartEnable
	};

	// Create the viewport state create info struct
	VkViewport viewport = {
		0.0f,																// x
		0.0f,																// y
		(float)WINDOW_WIDTH,												// width
		(float)WINDOW_HEIGHT,												// height
		0.0f,																// minDepth
		1.0f																// maxDepth
	};
	VkRect2D scissor {
		{0, 0 },
		{ WINDOW_WIDTH, WINDOW_HEIGHT }
	};
	VkPipelineViewportStateCreateInfo viewportCI = {
		VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,				// sType
		nullptr,															// pNext
		0,																	// flags
		1,																	// viewportCount
		&viewport,															// pViewports
		1,																	// scissorCount
		&scissor															// pScissors
	};

	// Create the rasterization state create info struct
	VkPipelineRasterizationStateCreateInfo rasterizerCI = {
		VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,			// sType
		nullptr,															// pNext
		0,																	// flags
		VK_FALSE,															// depthClampEnable
		VK_FALSE,															// rasterizerDiscardEnable
		VK_POLYGON_MODE_FILL,												// polygonMode
		VK_CULL_MODE_BACK_BIT,												// cullMode
		VK_FRONT_FACE_CLOCKWISE,											// frontFace
		VK_FALSE,															// depthBiasEnable
		0.0f,																// depthBiasConstantFactor
		0.0f,																// depthBiasClamp
		0.0f,																// depthBiasSlopeFactor
		1.0f																// lineWidth
	};

	// Create the multisample state create info struct
	VkPipelineMultisampleStateCreateInfo multiSamplingCI = {
		VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,			// sType
		nullptr,															// pNext
		0,																	// flags
		VK_SAMPLE_COUNT_1_BIT,												// rasterizationSamples
		VK_FALSE,															// sampleShadingEnable
		1.0f,																// minSampleShading
		nullptr,															// pSampleMask
		VK_FALSE,															// alphaToCoverageEnable
		VK_FALSE															// alphaToOneEnable
	};

	// Create the depth stencil state create info struct
	// TODO: Implement later

	// Create the color blend attachment state struct
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {
		VK_TRUE,															// blendEnable
		VK_BLEND_FACTOR_SRC_ALPHA,											// srcColorBlendFactor
		VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,								// dstColorBlendFactor
		VK_BLEND_OP_ADD,													// colorBlendOp
		VK_BLEND_FACTOR_ONE,												// srcAlphaBlendFactor
		VK_BLEND_FACTOR_ZERO,												// dstAlphaBlendFactor
		VK_BLEND_OP_ADD,													// alphaBlendOp
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |				// colorWriteMask
		VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
	};
	VkPipelineColorBlendStateCreateInfo colorBlendCI = {
		VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,			// sType
		nullptr,															// pNext
		0,																	// flags
		VK_FALSE,															// logicOpEnable
		VK_LOGIC_OP_COPY,													// logicOp
		1,																	// attachmentCount
		&colorBlendAttachment,												// pAttachments
		{ 0.0f, 0.0f, 0.0f, 0.0f }											// blendConstants
	};

	// Create the pipeline layout create info struct
	VkPipelineLayoutCreateInfo pipelineLayoutCI = {
		VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,						// sType
		nullptr,															// pNext
		0,																	// flags
		0,																	// setLayoutCount
		nullptr,															// pSetLayouts
		0,																	// pushConstantRangeCount
		nullptr																// pPushConstantRanges
	};

	// Create the pipeline layout
	VkResult result = vkCreatePipelineLayout(mLogicalDevice->GetLogicalDevice(), &pipelineLayoutCI, nullptr, &mPipelineLayout);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Pipeline layout created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create a pipeline layout! Error Code: " + NT_CHECK_RESULT(result));
	}

	// Create the graphics pipeline create info struct
	VkGraphicsPipelineCreateInfo graphicsPipelineCI = {
		VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,					// sType
		nullptr,															// pNext
		0,																	// flags
		(uint32_t)shaderStageCIs.size(),									// stageCount
		shaderStageCIs.data(),												// pStages
		&vertexInputCI,														// pVertexInputState
		&inputAssemblyCI,													// pInputAssemblyState
		nullptr,															// pTessellationState
		&viewportCI,														// pViewportState
		&rasterizerCI,														// pRasterizationState
		&multiSamplingCI,													// pMultisampleState
		nullptr,															// pDepthStencilState
		&colorBlendCI,														// pColorBlendState
		nullptr,															// pDynamicState
		mPipelineLayout,													// layout
		mRenderPass->GetRenderPass(),										// renderPass
		0,																	// subpass						TODO: Figure out how to calculate instead of hard-code
		VK_NULL_HANDLE,														// basePipelineHandle
		-1																	// basePipelineIndex
	};

	result = vkCreateGraphicsPipelines(mLogicalDevice->GetLogicalDevice(), VK_NULL_HANDLE, 1, &graphicsPipelineCI, nullptr, &mPipeline);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Graphics pipeline created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create graphics pipeline! Error Code: " + NT_CHECK_RESULT(result));
	}

	// Destroy Shader Modules, since graphics pipeline has been created
	for (size_t i = 0; i < mShaders.size(); i++) {
		vkDestroyShaderModule(mLogicalDevice->GetLogicalDevice(), mShaders.at(i)->GetShaderModule(), nullptr);	std::cout << "Success: Shader module destroyed." << std::endl;
	}
}