/*
 * Copyright (C) 2007 David Barina <DaBler@gmail.com>
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

#ifndef FORMFACTORENGINE_H
#define FORMFACTORENGINE_H

/**
 * @file FormFactorEngine.h
 * @brief Class FormFactorEngine - compute form factors.
 * @author xbarin02
 * @date 2007-11-17
 */

#include <map>
#include <vector>

#if defined(__WIN32__) || defined(_WIN32) || defined(__CYGWIN__)
#	include <windows.h>
#else
#	include <X11/Xlib.h>
#endif

#include <GL/glu.h>
#include "Triangle.h"

using namespace std;

#ifndef BUILDING_DOX
class Vector;
class PatchRandomAccessEnumerator;
class PatchCacheLine;
#endif

/**
 * @brief Compute all form factors for any destination patch.
 */
class FormFactorEngine {
    public:

        /**
         * @brief Constructor, create window, OpenGL context and pre-compute some temp variables.
         * @param patchEnumerator
         */
        FormFactorEngine (PatchRandomAccessEnumerator *patchEnumerator);

        /**
         * @brief Compute form factors for destination patch destPatch and store these factors into cache line.
         * @param destPatch Index of destination patch.
         * @param cacheLine Pointer to target cache line.
         */
        void fillCacheLine(int destPatch, PatchCacheLine *cacheLine);

    private:

        PatchRandomAccessEnumerator *patchEnumerator_;

#if defined(__WIN32__) || defined(_WIN32) || defined(__CYGWIN__)
#else
        Display *dpy;
        Window win;
#endif
        void createGLWindow();

        void drawScene();
        void renderViewport(const GLint x, const GLint y, const Vertex &c, const Vertex &at, const Vector &up);
        void renderFullScene(int dest);
        map<unsigned,double> *getFF();

    public:
        /**
         * @brief Destructor, close window, free memory.
         */
        ~FormFactorEngine();

        static const int EDGE_1 = 256;	 ///< size (in pixels) of hemi-cube edge
        static const int EDGE_2 = 2*EDGE_1;	///< EDGE_1 * 2 (size of important area in hemicube)
        static const int EDGE_LENGTH = 3*EDGE_1;	 ///< size (pixels) of render viewport

    private:
        double **ffcoefs;
};

#endif // FORMFACTORENGINE_H
