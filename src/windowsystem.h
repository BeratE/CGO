#ifndef WINDOW_SYSTEM_H
#define WINDOW_SYSTEM_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

namespace CGO {
    const int INIT_WIN_WIDTH = 800;
    const int INIT_WIN_HEIGHT = 600;
 
    class WindowSystem {
	friend class Application;
    public:
	~WindowSystem();

	// Public Interface
	void update();
	bool isAlive();
	std::vector<const char*> requestDisplayExtensions();
	
	// Callbacks
	static void cb_error(int error, const char* description);
	static void cb_keyEvent(GLFWwindow *p_window, int key, int scancode, int action, int mods);

    private:
	WindowSystem();
	WindowSystem(const WindowSystem&) = delete;
	WindowSystem& operator=(const WindowSystem&) = delete;

	void initialize();
	void shutdown();

	GLFWwindow *mp_window;
    };
}

#endif
