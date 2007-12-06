#include "Scene.h"
#include "TriangleSetExt.h"
#include "Visualizer.h"
#include "VisualizeArguments.h"

#include <string>

#ifndef NDEBUG
#	include <iostream>
#endif

namespace {
	TriangleSetExt *globalTriangleSet= 0;
	
	// Destroy global triangle set after exit() call
	void destroyTriangleSet() {
#ifndef NDEBUG
		std::cout << ">>> Freeing memory ... " << std::flush;
#endif
		delete globalTriangleSet;
#ifndef NDEBUG
		std::cout << "ok" << std::endl;
#endif
	}
}

int main(int argc, char *argv[]) {
	VisualizeArguments va;
	va.parseArguments( argc, argv);

	{
		Scene scene;
		scene.load(va.getFileIn());
		::globalTriangleSet = scene.dumpForVis();
		
		// Leaving this block should "release" Scene object from stack
	}
	
	// Clean-up after exit() call
	std::atexit(destroyTriangleSet);	

	if (va.getScreenshot())
		// Create screenshot
		Visualizer::takeScreenshot(va.getScreenshotFile(), ::globalTriangleSet);
	
	else
		// Initiate visualizer
		Visualizer::visualize(
				(std::string("rrv-visualize - ") + va.getFileIn()).c_str(),
				::globalTriangleSet);
	
	// NOTE: This line is never executed
	return 0;
}
