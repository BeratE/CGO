#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

namespace CGO {
    const std::vector<const char*> VALIDATION_LAYER_LIST = {
	"VK_LAYER_LUNARG_standard_validation"
    };

#ifdef NDEBUG
    const bool ENABLE_VALIDATION_LAYERS = false;
#else
    const bool ENABLE_VALIDATION_LAYERS = true;
#endif
    
    class RenderSystem {
	friend class Application;
    public:
	~RenderSystem();
	
	// Callbacks
	static VKAPI_ATTR VkBool32 VKAPI_CALL cb_vkDebug(
	           VkDebugUtilsMessageSeverityFlagBitsEXT msgLevel,
		   VkDebugUtilsMessageTypeFlagsEXT msgType,
		   const VkDebugUtilsMessengerCallbackDataEXT* p_cbData,
		   void* p_data);

    private:
	RenderSystem();
	RenderSystem(const RenderSystem&) = delete;
	RenderSystem& operator=(const RenderSystem&) = delete;

	void initialize();
	void shutdown();

	VkInstance m_vkInstance;
	VkDebugUtilsMessengerEXT m_vkDebugMessenger;
    };
}

#endif // RENDER_SYSTEM_H
