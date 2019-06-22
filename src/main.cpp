#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include "application.h"
#include <functional>
#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[]) {
    int exitCode = EXIT_SUCCESS;
    try {
	CGO::App.run();
    } catch (const std::exception &e) {
	std::cerr << std::endl;
	std::cerr << "# Critical Error; " << e.what() << std::endl;
	std::cerr << "# Terminating the application" << std::endl;
	exitCode = EXIT_FAILURE;
    }

    return exitCode;
}
