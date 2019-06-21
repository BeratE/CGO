#ifndef APPLICATION_H
#define APPLICATION_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

const long SPF = 1.0 / 60.0;  // Seconds per Frame = 1/FPS
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

namespace CGO {
    class Application {
    public:
	Application();
	~Application();

	void run();
    
	// Callbacks
	static void cb_glfwError(int error, const char* description);
	static void cb_glfwKey(GLFWwindow *p_window, int key, int scancode, int action, int mods);

    private:
	void initialize();
	void update();
	void shutdown();

	VkInstance m_vkInstance;
	GLFWwindow *mp_window;
    };
}

#endif // APPLICATION_H
