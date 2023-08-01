#include "Renderer.h"

Renderer::Renderer() {
	mInstance = new InstanceWrapper();
}

Renderer::~Renderer() {
	delete mInstance;
}