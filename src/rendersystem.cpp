#include "rendersystem.h"
#include "application.h"
#include <iostream>
#include <iomanip>
#include <cstring>

using namespace CGO;

RenderSystem::RenderSystem() {

}

RenderSystem::~RenderSystem() {

}

void RenderSystem::initialize() {
    // Query required extensions
    std::cout << std::setw(32) << std::left << ">> Querying required extensions..";
    std::vector<const char*> reqExtensions = App.accWindowSystem().requestDisplayExtensions();
    if (ENABLE_VALIDATION_LAYERS) {
	reqExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    std::cout << "\t[Done]" << std::endl;
        
    // Query avaliable extensions
    std::cout << std::setw(32) << std::left << ">> Requesting extensions.. ";
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
    // Check for required in avaliable extensions
    std::vector<std::string> missingExtensions;
    for (size_t i = 0; i < reqExtensions.size(); i++) {
	bool found = false;
	for (const auto& ext : extensions) {
	    if (strcmp(reqExtensions[i], ext.extensionName) == 0) {
		found = true;
		break;
	    }
	}
	if (!found) 
	    missingExtensions.push_back(std::string(reqExtensions[i]));
    }
    if (missingExtensions.size() > 0) {
	std::cout << "! Failed to locate following required extensions:" << std::endl;
	for (const auto &item : missingExtensions)
	    std::cout << "! " << item << std::endl;
	throw std::runtime_error("Missing required extensions.");
    }
    std::cout << "\t[Done]" << std::endl;

    // Request Validation Layer Support
    if (ENABLE_VALIDATION_LAYERS) {
	std::cout << std::setw(32) << std::left << ">> Requesting validation layers..";
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> avaliableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, avaliableLayers.data());
	for (unsigned int i = 0; i < VALIDATION_LAYER_LIST.size(); i++) {
	    bool found = false;
	    for (const auto &layerProps : avaliableLayers) {
		if (strcmp(VALIDATION_LAYER_LIST[i], layerProps.layerName) == 0) {
		    found = true;
		    break;
		}
	    }
	    if (!found) 
		throw std::runtime_error("Requested validation layers not avaliable.");
	}
	std::cout << "\t[Done]" << std::endl;
    }
    
    // Create Vulkan Instance
    std::cout << std::setw(32) << std::left << ">> Creating instance..";
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "CGO";
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_1;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = (uint32_t)reqExtensions.size();
    createInfo.ppEnabledExtensionNames = reqExtensions.data();
    if (ENABLE_VALIDATION_LAYERS) {
	createInfo.ppEnabledLayerNames = VALIDATION_LAYER_LIST.data();
	createInfo.enabledLayerCount = (uint32_t)VALIDATION_LAYER_LIST.size();
    }
    else {
	createInfo.enabledLayerCount = 0;
    }

    if (vkCreateInstance(&createInfo, nullptr, &m_vkInstance) != VK_SUCCESS) {
	throw std::runtime_error("Failed to create Vulkan instance.");
    }
    std::cout << "\t[Done]" << std::endl;

    // Setup Debug Messenger
    if (ENABLE_VALIDATION_LAYERS) {
	std::cout << std::setw(32) << std::left << ">> Setting up debug messenger..";
	VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo = {};
	debugMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debugMessengerCreateInfo.messageSeverity =
	    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
	    VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
	    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
	    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	debugMessengerCreateInfo.messageType =
	    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
	    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
	    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	debugMessengerCreateInfo.pfnUserCallback = RenderSystem::cb_vkDebug;
	debugMessengerCreateInfo.pUserData = nullptr; 

	auto vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)
	    vkGetInstanceProcAddr(m_vkInstance,"vkCreateDebugUtilsMessengerEXT");
     
	if (vkCreateDebugUtilsMessengerEXT == nullptr ||
	    vkCreateDebugUtilsMessengerEXT(m_vkInstance, &debugMessengerCreateInfo,
					   nullptr, &m_vkDebugMessenger) != VK_SUCCESS) {
	     vkDestroyInstance(m_vkInstance, nullptr);
	     throw std::runtime_error("Failed to set up debug messenger.");
	}
	std::cout << "\t[Done]" << std::endl;
    }
    
    // List Info
    // std::cout << std::endl;
    // std::cout << "> Listing avaliable extensions:" << std::endl;
    // std::cout << "> #############################" << std::endl;
    // for (const auto& ext : extensions) 
    // 	std::cout << "> " << ext.extensionName << std::endl;
    // std::cout << "> #############################" << std::endl << std::endl;
}

void RenderSystem::shutdown() {
    if (ENABLE_VALIDATION_LAYERS) {
	auto vkDestroyDebugUtilsMessengerEXT =
	    ((PFN_vkDestroyDebugUtilsMessengerEXT)
	     vkGetInstanceProcAddr(m_vkInstance, "vkDestroyDebugUtilsMessengerEXT"));
	if(vkDestroyDebugUtilsMessengerEXT != nullptr)
	    vkDestroyDebugUtilsMessengerEXT(m_vkInstance, m_vkDebugMessenger, nullptr);
    }
    vkDestroyInstance(m_vkInstance, nullptr);
}

// Callbacks
VKAPI_ATTR VkBool32 VKAPI_CALL RenderSystem::cb_vkDebug(
	           VkDebugUtilsMessageSeverityFlagBitsEXT msgLevel,
		   VkDebugUtilsMessageTypeFlagsEXT msgType,
		   const VkDebugUtilsMessengerCallbackDataEXT* p_cbData,
		   void* p_data) {
    std::cerr << "% RenderSystem::Validation - " << p_cbData->pMessage << std::endl;
    return VK_FALSE;
}


