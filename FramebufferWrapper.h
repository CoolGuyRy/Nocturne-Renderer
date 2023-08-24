#ifndef FRAME_BUFFER_WRAPPER_H
#define FRAME_BUFFER_WRAPPER_H

#include <vector>
#include <vulkan/vulkan.h>

class Context;
class ImageViewWrapper;

/*

	

*/

class FramebufferWrapper {
public:
	FramebufferWrapper(Context*, int);
	FramebufferWrapper(Context*, int, ImageViewWrapper*);
	~FramebufferWrapper();

	VkFramebuffer GetFramebuffer();
private:
	void CreateFramebuffer(int);
	void CreateFramebuffer(int, ImageViewWrapper*);

	VkFramebuffer mFramebuffer;

	Context* mContext;
};

#endif