#ifndef APPLICATION_H
#define APPLICATION_H

#include <vector>
#include "windowsystem.h"
#include "rendersystem.h"

const long SPF = 1.0 / 60.0;  // Seconds per Frame = 1/FPS

namespace CGO {
    class Application {
    public:
	~Application();
	static Application& getInstance() {
	    static Application instance;
	    return instance;
	}
	
	void run();

	//Accessors
	WindowSystem& accWindowSystem();
	RenderSystem& accRenderSystem();
      
    private:
	Application();
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;
	
	void initialize();
	void update();
	void shutdown();

	WindowSystem m_windowSys;
	RenderSystem m_renderSys;
    };
}
#define App Application::getInstance()
#endif // APPLICATION_H
