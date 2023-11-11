#include "Shader.h"

#include <fstream>
#include "globals.h"

std::vector<char> readShaderFile(std::string filename) {
	// Open stream from given file
	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	// Check if file stream successfully opened
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file " + filename + "!");
	}

	// Get current read position and use it to resize file buffer
	size_t fileSize = (size_t)file.tellg();
	std::vector<char> fileBuffer(fileSize);

	// Move read posisiton (seek to) the start of the file
	file.seekg(0);

	// read the file data into the buffer (stream "filesize" in total)
	file.read(fileBuffer.data(), fileSize);

	// close stream
	file.close();

	return fileBuffer;
}

Shader::Shader(VkDevice device, std::string filename) : mLogicalDevice(device) {
	CreateShaderModule(filename);
	CreateShaderCI();
}

Shader::~Shader() {
	vkDestroyShaderModule(mLogicalDevice, mShaderModule, nullptr); std::cout << "Success: Shader module destroyed." << std::endl;
}

VkShaderModule Shader::GetShaderModule() {
	return mShaderModule;
}

VkPipelineShaderStageCreateInfo Shader::GetShaderCI() {
	return mShaderCI;
}

void Shader::CreateShaderModule(std::string filename) {
	mShaderCode = readShaderFile(filename);

	size_t firstPeriod = filename.find_first_of(".", 1);
	size_t lastPeriod = filename.find_last_of(".") - 1;

	std::string extension = filename.substr(firstPeriod + 1, lastPeriod - firstPeriod);

	if (extension == "vert") {
		mShaderStage = VK_SHADER_STAGE_VERTEX_BIT;
	} else if (extension == "tesc") {
		mShaderStage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
	} else if (extension == "tese") {
		mShaderStage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
	} else if (extension == "geom") {
		mShaderStage = VK_SHADER_STAGE_GEOMETRY_BIT;
	} else if (extension == "frag") {
		mShaderStage = VK_SHADER_STAGE_FRAGMENT_BIT;
	} else if (extension == "comp") {
		mShaderStage = VK_SHADER_STAGE_COMPUTE_BIT;
	} else {
		throw std::runtime_error("Failed to create Shader Module! Invalid file extension.");
	}

	VkShaderModuleCreateInfo shaderModuleCI = {
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.codeSize = mShaderCode.size(),
		.pCode = reinterpret_cast<const uint32_t*>(mShaderCode.data())
	};

	VkResult result = vkCreateShaderModule(mLogicalDevice, &shaderModuleCI, nullptr, &mShaderModule);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Shader module created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create Shader Module! Error Code: " + NT_CHECK_RESULT(result));
	}
}

void Shader::CreateShaderCI() {
	mShaderCI = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.stage = mShaderStage,
		.module = mShaderModule,
		.pName = "main",
		.pSpecializationInfo = nullptr
	};
}