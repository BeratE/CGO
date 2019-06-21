#include "application.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <chrono>
#include <thread>
#include <algorithm>

using namespace CGO;

Application::Application() {
}

Application::~Application() {
}

void Application::run() {
    initialize();
    update();
    shutdown();
}

void Application::initialize() {
    // Configure Display Window and Surface
    std::cout << "> Initializing GLFW..";
    if (!glfwInit()) {
	throw std::runtime_error("Failed to initialize GLFW.");
    }
    std::cout << "\t\t\t[Done]" << std::endl;
  
    // Create Window
    std::cout << "> Creating Window..";
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    mp_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CGO", NULL, NULL);
    if (!mp_window) {
	glfwTerminate();
	throw std::runtime_error("Failed to create window.");
    }
    std::cout << "\t\t\t[Done]" << std::endl;

    // Set GLFW Callbacks
    std::cout << "> Setting GLFW Callbacks..";
    glfwSetErrorCallback(Application::cb_glfwError);
    glfwSetKeyCallback(mp_window, Application::cb_glfwKey);
    std::cout << "\t\t[Done]" << std::endl;
  
    // Query required extensions
    uint32_t glfwExtensionCount;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    if (glfwExtensions == NULL) {
	glfwDestroyWindow(mp_window);
	glfwTerminate();
	throw std::runtime_error("Failed to query instance extensions");
    }
  
    // Query avaliable extensions
    std::cout << "> Quering Extensions.. ";
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
    // Check for required in avaliable extensions
    std::vector<std::string> missingExtensions;
    for (uint32_t i = 0; i < glfwExtensionCount; i++) {
	bool found = false;
	for (const auto& ext : extensions) {
	    if (strcmp(glfwExtensions[i], ext.extensionName) == 0) {
		found = true;
		break;
	    }
	}
	if (!found) 
	    missingExtensions.push_back(std::string(glfwExtensions[i]));
    }
    if (missingExtensions.size() > 0) {
	std::cout << "> Failed to locate following required Extensions:" << std::endl;
	for (const auto &item : missingExtensions)
	    std::cout << "> " << item << std::endl;
	throw std::runtime_error("Missing required extensions.");
    }
    std::cout << "\t\t\t[Done]" << std::endl;
    std::cout << "> Avaliable Extensions:" << std::endl;
    std::cout << "> #####################" << std::endl;
    for (const auto& ext : extensions) {
	std::cout << "> " << ext.extensionName << std::endl;
    }
    std::cout << "> #####################" << std::endl;
  
    // Create Vulkan Instance
    std::cout << "> Creating Instance..";
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
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;

    if (vkCreateInstance(&createInfo, nullptr, &m_vkInstance) != VK_SUCCESS) {
	glfwDestroyWindow(mp_window);
	glfwTerminate();
	throw std::runtime_error("Failed to create Vulkan instance.");
    }
    std::cout << "\t\t\t[Done]" << std::endl;
}

void Application::update() {
    long curr_t = 0;
    long delta_t = 0;
    while(!glfwWindowShouldClose(mp_window)) {
	curr_t = glfwGetTime();




    	glfwPollEvents();
	// Sleep for fixed frame rate
	delta_t = glfwGetTime() - curr_t;
	std::this_thread::sleep_for(std::chrono::seconds(SPF - delta_t));
    }
}

void Application::shutdown() {
    vkDestroyInstance(m_vkInstance, nullptr);
    glfwDestroyWindow(mp_window);
    glfwTerminate();
}

// Callbacks
void Application::cb_glfwError(int error, const char* description) {
    std::cerr << "# Error; GLFW - " << description << std::endl;
}

void Application::cb_glfwKey(GLFWwindow *p_window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	glfwSetWindowShouldClose(p_window, GLFW_TRUE);
}
