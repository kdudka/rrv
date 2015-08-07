/*
 * Copyright (C) 2007 David Barina <DaBler@gmail.com>
 * Copyright (C) 2015 Claude Heiland-Allen <claude@mathr.co.uk>
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

#if defined(__WIN32__) || defined(_WIN32) || defined(__CYGWIN__)
#	include <windows.h>
#else
#	include <X11/Xlib.h>
#endif

#include <GL/glew.h>
#include <GL/glu.h>
#include "Triangle.h"
#include "FormFactorHemicube.h"
#include "LinearAlgebra.h"

#include <map>

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
         * @brief Constructor, create window and OpenGL context.
         * @param patchEnumerator
         * @param hemicube Per-pixel form factors.
         */
        FormFactorEngine (PatchRandomAccessEnumerator *patchEnumerator, const FormFactorHemicube &hemicube);

        /**
         * @brief Enqueue calculation of form factors for destination patch destPatch.
         * @note  fillCacheLine() must be called later with the same destPatch to avoid leaking memory.
         * @param destPatch Index of destination patch.
         */
        void prepare(int destPatch);

        /**
         * @brief Compute form factors for destination patch destPatch and store these factors into cache line.
         * @note  prepare() must have been called before with the same destPatch
         * @param destPatch Index of destination patch.
         * @param cacheLine Pointer to target cache line.
         */
        void fillCacheLine(int destPatch, PatchCacheLine *cacheLine);

    private:

        PatchRandomAccessEnumerator *patchEnumerator_;
        const FormFactorHemicube &hemicube_;
        GLuint vbo_;
        std::map<int, unsigned char *> screens;
        std::map<int, GLuint> pbos;
        DenseVector<float> *ffvec;

#if defined(__WIN32__) || defined(_WIN32) || defined(__CYGWIN__)
#else
        Display *dpy;
        Window win;
#endif
        void createGLWindow();
        void uploadScene();
        void drawScene();
        void renderViewport(const GLint x, const GLint y, const Vertex &c, const Vertex &at, const Vector &up);
        void renderFullScene(int dest);
        void getFF(int destPatch);

    public:
        /**
         * @brief Destructor, close window, free memory.
         */
        ~FormFactorEngine();
};

#endif // FORMFACTORENGINE_H
