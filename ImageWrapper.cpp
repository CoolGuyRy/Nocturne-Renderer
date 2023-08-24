#include "ImageWrapper.h"
#include "globals.h"
#include "Context.h"
#include "PhysicalDeviceWrapper.h"
#include "LogicalDeviceWrapper.h"
#include "CommandPoolWrapper.h"
#include "CommandBufferWrapper.h"
#include "BufferWrapper.h"

ImageWrapper::ImageWrapper(Context* context, CommandPoolWrapper* gPool, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags useFlags, VkMemoryPropertyFlags propFlags) : mContext(context), mGraphicsCommandPool(gPool) {
	CreateImage(width, height, format, tiling, useFlags, propFlags);
}

ImageWrapper::ImageWrapper(Context* context, CommandPoolWrapper* gPool, std::string filename) : mContext(context), mGraphicsCommandPool(gPool) {
	CreateTextureImage(filename);
}

ImageWrapper::~ImageWrapper() {
	vkDestroyImage(mContext->mLogicalDevice->GetLogicalDevice(), mImage, nullptr);
	vkFreeMemory(mContext->mLogicalDevice->GetLogicalDevice(), mImageMemory, nullptr);
}

VkImage ImageWrapper::GetImage() {
	return mImage;
}

void ImageWrapper::CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags useFlags, VkMemoryPropertyFlags propFlags) {
	VkImageCreateInfo imageCI = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.imageType = VK_IMAGE_TYPE_2D,
		.format = format,
		.extent = {
			.width = width,
			.height = height,
			.depth = 1
		},
		.mipLevels = 1,
		.arrayLayers = 1,
		.samples = VK_SAMPLE_COUNT_1_BIT,
		.tiling = tiling,
		.usage = useFlags,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.queueFamilyIndexCount = 0,
		.pQueueFamilyIndices = nullptr,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
	};

	VkResult result = vkCreateImage(mContext->mLogicalDevice->GetLogicalDevice(), &imageCI, nullptr, &mImage);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Image created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create Image! Error Code: " + NT_CHECK_RESULT(result));
	}

	VkMemoryRequirements memoryRequirements;
	vkGetImageMemoryRequirements(mContext->mLogicalDevice->GetLogicalDevice(), mImage, &memoryRequirements);

	VkMemoryAllocateInfo memoryAI = {
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.pNext = nullptr,
		.allocationSize = memoryRequirements.size,
		.memoryTypeIndex = FindMemoryTypeIndex(mContext->mPhysicalDevice->GetPhysicalDevice(), memoryRequirements.memoryTypeBits, propFlags)
	};

	result = vkAllocateMemory(mContext->mLogicalDevice->GetLogicalDevice(), &memoryAI, nullptr, &mImageMemory);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Image Memory allocated." << std::endl;
	} else {
		throw std::runtime_error("Failed to allocate image memory! Error Code: " + NT_CHECK_RESULT(result));
	}

	vkBindImageMemory(mContext->mLogicalDevice->GetLogicalDevice(), mImage, mImageMemory, 0);
}

void ImageWrapper::CreateTextureImage(std::string filename) {
	int width, height;
	VkDeviceSize imageSize;
	stbi_uc* imageData = LoadTextureFile(filename, &width, &height, &imageSize);

	BufferWrapper stagingBuffer(mContext, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	stagingBuffer.MapBufferMemory(imageData, imageSize);

	stbi_image_free(imageData);

	CreateImage(width, height, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	TransitionImageLayout(mContext, mGraphicsCommandPool, this, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

	CopyImageBuffer(mContext, mGraphicsCommandPool, &stagingBuffer, this, width, height);

	TransitionImageLayout(mContext, mGraphicsCommandPool, this, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

stbi_uc* LoadTextureFile(std::string filename, int* width, int* height, VkDeviceSize* imageSize) {
	int channels;

	stbi_uc* image = stbi_load(filename.c_str(), width, height, &channels, STBI_rgb_alpha);

	if (!image) {
		throw std::runtime_error("Failed to load texture file! Located at " + filename);
	}

	*imageSize = (VkDeviceSize)*width * (VkDeviceSize)*height * (VkDeviceSize)STBI_rgb_alpha;

	return image;
}

void CopyImageBuffer(Context* context, CommandPoolWrapper* tPool, BufferWrapper* srcBuffer, ImageWrapper* dstImage, uint32_t width, uint32_t height) {
	// Create Transfer Command Buffer
	CommandBufferWrapper* transferCommandBuffer = new CommandBufferWrapper(context, tPool);

	// Begin Recording Transfer Command Buffer struct
	VkCommandBufferBeginInfo commandBufferBI = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.pNext = nullptr,
		.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
		.pInheritanceInfo = nullptr
	};

	// Begin Recording Transfer Command Buffer
	VkResult result = vkBeginCommandBuffer(transferCommandBuffer->GetCommandBuffer(), &commandBufferBI);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to begin recording transfer command buffer! Error Code: " + NT_CHECK_RESULT(result));
	}

	VkBufferImageCopy imageRegion = {
		.bufferOffset = 0,
		.bufferRowLength = 0,
		.bufferImageHeight = 0,
		.imageSubresource = {
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.mipLevel = 0,
			.baseArrayLayer = 0,
			.layerCount = 1
		},
		.imageOffset = {
			.x = 0,
			.y = 0,
			.z = 0
		},
		.imageExtent = {
			.width = width,
			.height = height,
			.depth = 1
		}
	};

	vkCmdCopyBufferToImage(transferCommandBuffer->GetCommandBuffer(), srcBuffer->GetBuffer(), dstImage->GetImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageRegion);

	result = vkEndCommandBuffer(transferCommandBuffer->GetCommandBuffer());
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to end recording transfer command buffer! Error Code: " + NT_CHECK_RESULT(result));
	}

	// Submit Transfer Command Buffer struct
	VkCommandBuffer buffer = transferCommandBuffer->GetCommandBuffer();
	VkSubmitInfo submitInfo = {
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.pNext = nullptr,
		.waitSemaphoreCount = 0,
		.pWaitSemaphores = nullptr,
		.pWaitDstStageMask = nullptr,
		.commandBufferCount = 1,
		.pCommandBuffers = &buffer,
		.signalSemaphoreCount = 0,
		.pSignalSemaphores = nullptr
	};

	// Submit Transfer Command Buffer
	result = vkQueueSubmit(context->mLogicalDevice->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to submit copy command to queue! Error Code: " + NT_CHECK_RESULT(result));
	}

	// Wait for Transfer Queue to finish
	result = vkQueueWaitIdle(context->mLogicalDevice->GetGraphicsQueue());
	if (result == VK_SUCCESS) {
		std::cout << "Success: Data transferred." << std::endl;
	} else {
		throw std::runtime_error("Failed to wait for transfer queue! Error Code: " + NT_CHECK_RESULT(result));
	}

	// Free Transfer Command Buffer
	vkFreeCommandBuffers(context->mLogicalDevice->GetLogicalDevice(), tPool->GetCommandPool(), 1, &buffer);
}

void TransitionImageLayout(Context* context, CommandPoolWrapper* tPool, ImageWrapper* image, VkImageLayout oldLayout, VkImageLayout newLayout) {
	// Create Transfer Command Buffer
	CommandBufferWrapper* transferCommandBuffer = new CommandBufferWrapper(context, tPool);

	// Begin Recording Transfer Command Buffer struct
	VkCommandBufferBeginInfo commandBufferBI = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.pNext = nullptr,
		.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
		.pInheritanceInfo = nullptr
	};

	// Begin Recording Transfer Command Buffer
	VkResult result = vkBeginCommandBuffer(transferCommandBuffer->GetCommandBuffer(), &commandBufferBI);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to begin recording transfer command buffer! Error Code: " + NT_CHECK_RESULT(result));
	}

	VkImageMemoryBarrier imageMemoryBarrier = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
		.pNext = nullptr,
		.srcAccessMask = 0,
		.dstAccessMask = 0,
		.oldLayout = oldLayout,
		.newLayout = newLayout,
		.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.image = image->GetImage(),
		.subresourceRange = {
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.baseMipLevel = 0,
			.levelCount = 1,
			.baseArrayLayer = 0,
			.layerCount = 1
		}
	};

	VkPipelineStageFlags srcStage = { }, dstStage = { };

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		imageMemoryBarrier.srcAccessMask = 0;
		imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	} else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}

	vkCmdPipelineBarrier(
		transferCommandBuffer->GetCommandBuffer(),
		srcStage,
		dstStage,
		0,
		0,
		nullptr,
		0,
		nullptr,
		1,
		&imageMemoryBarrier
	);

	result = vkEndCommandBuffer(transferCommandBuffer->GetCommandBuffer());
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to end recording transfer command buffer! Error Code: " + NT_CHECK_RESULT(result));
	}

	// Submit Transfer Command Buffer struct
	VkCommandBuffer buffer = transferCommandBuffer->GetCommandBuffer();
	VkSubmitInfo submitInfo = {
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.pNext = nullptr,
		.waitSemaphoreCount = 0,
		.pWaitSemaphores = nullptr,
		.pWaitDstStageMask = nullptr,
		.commandBufferCount = 1,
		.pCommandBuffers = &buffer,
		.signalSemaphoreCount = 0,
		.pSignalSemaphores = nullptr
	};

	// Submit Transfer Command Buffer
	result = vkQueueSubmit(context->mLogicalDevice->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to submit copy command to queue! Error Code: " + NT_CHECK_RESULT(result));
	}

	// Wait for Transfer Queue to finish
	result = vkQueueWaitIdle(context->mLogicalDevice->GetGraphicsQueue());
	if (result == VK_SUCCESS) {
		std::cout << "Success: Data transferred." << std::endl;
	} else {
		throw std::runtime_error("Failed to wait for transfer queue! Error Code: " + NT_CHECK_RESULT(result));
	}

	// Free Transfer Command Buffer
	vkFreeCommandBuffers(context->mLogicalDevice->GetLogicalDevice(), tPool->GetCommandPool(), 1, &buffer);
}