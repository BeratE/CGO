#include "windowsystem.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

using namespace CGO;

WindowSystem::WindowSystem() {
}

WindowSystem::~WindowSystem() {
}

void WindowSystem::initialize() {
    std::cout << "> Window system initialization.." << std::endl;
    // Configure Display Window and Surface
    std::cout << std::setw(32) << std::left << ">> Initializing GLFW..";
    if (!glfwInit())
	throw std::runtime_error("Failed to initialize GLFW.");
    std::cout << "\t[Done]" << std::endl;
    
    // Create Window
    std::cout << std::setw(32) << std::left << ">> Creating display window..";
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    mp_window = glfwCreateWindow(INIT_WIN_WIDTH, INIT_WIN_HEIGHT, "CGO", NULL, NULL);
    if (!mp_window) {
	glfwTerminate();
	throw std::runtime_error("Failed to create display window.");
    }
    std::cout << "\t[Done]" << std::endl;
 
    // Set GLFW Callbacks
    std::cout << std::setw(32) << std::left << ">> Registering window callbacks..";
    glfwSetKeyCallback(mp_window, WindowSystem::cb_keyEvent);
#ifndef NDEBUG
    glfwSetErrorCallback(WindowSystem::cb_error);
#endif
    std::cout << "\t[Done]" << std::endl;
 }

void WindowSystem::shutdown() {
    std::cout << "> Window system shutdown.." << std::endl;
    glfwDestroyWindow(mp_window);
    glfwTerminate();
}


// Public Interface
void WindowSystem::update() {
    glfwPollEvents();
}

bool WindowSystem::isAlive() {
    return !glfwWindowShouldClose(mp_window);
}

std::vector<const char*> WindowSystem::requestDisplayExtensions() {
    uint32_t glfwExtensionCount;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    if (glfwExtensions == NULL) {
	shutdown();
	throw std::runtime_error("Failed to query instance extensions");
    }
    return std::vector<const char*>(glfwExtensions, glfwExtensions+glfwExtensionCount);
}

// Callbacks
void WindowSystem::cb_keyEvent(GLFWwindow *p_window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	glfwSetWindowShouldClose(p_window, GLFW_TRUE);
}

void WindowSystem::cb_error(int error, const char* description) {
    std::cerr << "# Error; Window System:: " << description << std::endl;
}
