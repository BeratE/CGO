#include "application.h"
#include <iostream>
#include <stdexcept>
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
    std::cout << "> Application initialization.." << std::endl;
    m_windowSys.initialize();
    m_renderSys.initialize();
}

void Application::update() {
    long curr_t = 0;
    long delta_t = 0;
    while(m_windowSys.isAlive()) {
	curr_t = glfwGetTime();


	m_windowSys.update();	
	// Sleep for fixed frame rate
	delta_t = glfwGetTime() - curr_t;
	std::this_thread::sleep_for(std::chrono::seconds(SPF - delta_t));
    }
}

void Application::shutdown() {
    std::cout << "> Application shutdown.." << std::endl;
    m_renderSys.shutdown();
    m_windowSys.shutdown();
}

// Accessors
WindowSystem& Application::accWindowSystem() {
    return m_windowSys;
}

RenderSystem& Application::accRenderSystem() {
    return m_renderSys;
}
