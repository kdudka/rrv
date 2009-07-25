/*
 * Copyright (C) 2007 TODO
 *
 * This file is part of rrv (Radiosity Renderer and Visualizer).
 *
 * rrv is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * rrv is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with rrv.  If not, see <http://www.gnu.org/licenses/>.
 */

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
