#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

namespace CGO {
    // Constants
    // Required Validation Layers
    static const std::vector<const char*> VALIDATION_LAYER_LIST = {
	"VK_LAYER_LUNARG_standard_validation"
    };
    // Required Queue Family Functionalities
    static const std::vector<VkQueueFlagBits> QUEUE_FAMILY_LIST = {
	VK_QUEUE_GRAPHICS_BIT
    };
    
#ifdef NDEBUG
    static const bool ENABLE_VALIDATION_LAYERS = false;
#else
    static const bool ENABLE_VALIDATION_LAYERS = true;
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

	// Initialization routines
	void createInstance();
	void createDebugMessenger();
	void selectPhysicalDevice();
	bool testPhysicalDevice(const VkPhysicalDevice &device);

	// Member
	VkInstance m_vkInstance;
	VkDebugUtilsMessengerEXT m_vkDebugMessenger;
	VkPhysicalDevice m_physicalDevice;
    };
}

#endif // RENDER_SYSTEM_H
