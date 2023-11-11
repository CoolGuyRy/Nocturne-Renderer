#ifndef SHADER_WRAPPER_H
#define SHADER_WRAPPER_H

#include <vulkan/vulkan.h>
#include <string>
#include <vector>

/*
	Shader will read file name and determine the type of shader.
	Then it will create a shader module and a shader stage create info.
	Notes:
		- I just noticed that I have the shaderc library in my project.
		Already have an implementation of my own so for now leave it alone.
		But worth taking a look at in the future.
	Assumptions:
		- All files it reads end in .spv
		- Entry point for each shader will be called "main"
*/

std::vector<char> readShaderFile(std::string);

class Shader {
public:
	Shader(VkDevice, std::string);
	~Shader();

	VkShaderModule GetShaderModule();
	VkPipelineShaderStageCreateInfo GetShaderCI();
private:
	void CreateShaderModule(std::string);
	void CreateShaderCI();

	VkShaderModule mShaderModule;
	VkShaderStageFlagBits mShaderStage;
	VkPipelineShaderStageCreateInfo mShaderCI;
	std::vector<char> mShaderCode;

	VkDevice mLogicalDevice;
};

#endif