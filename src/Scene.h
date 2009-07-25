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

#ifndef SCENE_H
#define SCENE_H

/**
 * @file Scene.h
 * @brief Class Scene - high-level interface to scene represenatation.
 * @author xdudka00, xfilak01
 * @date 2007-11-18
 */

#include "EntitySet.h"

#include <string>

#ifndef NDEBUG
#	ifndef BUILDING_DOX
#		include <iostream>
#	endif
#endif

class RadiosityRenderer;

/**
 * @brief High-level interface to scene representation.
 */
class Scene {
public:
		/**
		 * @brief Load scene from XML file
		 * @param fileName Name of file to load.
		 * @note Current DTD should be at http://dudka.cz/dtd/scene.dtd
		 */
		void load(std::string fileName);
		
		/**
		 * This is a way of acceleration - you get useful output after 1st step.
		 * @brief Set initial radiosity for light sources.
		 */
		void applyEmission();

		/**
		 * Recursive division of triangles until they are smaller as given size.
		 * @brief Divide scene triangles to patches.
		 * @param size Maximum acceptable size of patch.
		 */
		void divide (float size);
		
		/**
		 * @brief @return Return instance of RadiosityRenderer object allocated on the heap.
		 * @param stepCount Number of radiosity computation steps.
		 * @param formFactorTreshold Pair of patches with smaller form factor than formFactorTreshold will be ignored.
		 * @param maxCacheSize Maximum size of patch cache (in bytes).
		 * @note Maximum cache size is raw size (estimated). The real cache size can be greater.
		 */
		RadiosityRenderer* createRadiosityRenderer(int stepCount, float formFactorTreshold, long maxCacheSize);

		/**
		 * @brief Save scene to XML file.
		 * @param fileName Name of file to save scene to.
		 */
		void save(std::string fileName);


		/**
		 * This method is used for scene visualization.
		 * @brief @return Return instance of TriangleSetExt object containing scene triangles.
		 */
		TriangleSetExt* dumpForVis();


	private:
		EntitySet entitySet_;
		
#ifndef NDEBUG
		friend std::ostream& operator<< (std::ostream &out, Scene& scene);
#endif
};

#ifndef NDEBUG
/**
 * @brief Dump all scene patches to output stream in human readable format.
 * @note This function will not be built if NDEBUG macro is defined.
 * @param out Output stream to write to.
 * @param scene Reference to Scene object to dump.
 * @return Return output stream given as parameter.
 */
inline std::ostream& operator<< (std::ostream &out, Scene& scene) {
	out << scene.entitySet_;
	return out;
}
#endif

#endif // SCENE_H
