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

#ifndef GLUTWINDOW_H
#define GLUTWINDOW_H

/**
 * @file Visualizer.h
 * @brief Class Visualizer - scene visualization
 * @author xhefka00
 * @date 2007-11-17
 */

#if defined(__WIN32__) || defined(_WIN32) || defined(__CYGWIN__) 
	#include <windows.h>
#else
#endif

#include <GL/glut.h>

#ifndef BUILDING_DOX
class TriangleSetExt;
#endif

/**
 * @brief Scene visualization
 */
class Visualizer{
	public:
		/**
		 * @attention This method ends using exit() function and never returns!
		 * @brief Initiate interactive scene visualization
		 * @param title Title of visualizer's window
		 * @param te Filled patch container with computed patch and vertex colors.
		 */
		static void visualize(const char *title, TriangleSetExt *te);	/* visualize of scene in GLUT window */
		
		/**
		 * Using TGA image format to save screenshot.
		 * @attention This method ends using exit() function and never returns!
		 * @brief Take screenshot from default camera position and save to file.
		 * @param fileName Name of file to save screenshot to. Existing file will be replaced.
		 * @param te Filled patch container with computed patch and vertex colors.
		 */
		static void takeScreenshot(char *fileName, TriangleSetExt *te);		/* do screenshot of scene to file .tga */
	
	private:
		typedef enum { INTERPOLATE_RADIOSITY, INTERPOLATE_RADIOSITY_RAW, RADIOSITY_LAST, RADIOSITY, REFLECTIVITY, EMISSION, REFLECT_EMISS } TColorInput;
	
		static TriangleSetExt * patchSequenceEnumerator_;
		static TColorInput colorInput;
	
		static GLuint scene_;
	
		static int   xnew, ynew, znew;                 	/* actual position */
		static int   xold, yold, zold;                 	/* old position */
		static int   xx1, yy1, zz1;                   	/* mouse position*/
		static int   mouseState;                     	/* mouse button state */
		static int   xshift, yshift;                  	/* shifting in space*/
	
		static float fov;                              	/* field of view */
		static float near_plane;                      	/* trim plain */
		static float far_plane;                       	/* farther trim plain */
		static float point_size;                      	/* size of point */
		static float line_width;                      	/* width of line */
		static int   WindowWidth;                     	/* width and height of window */
		static int   WindowHeight;
		static int   ObjectType;                      	/* type of paint object */
		static int   Solid;                             /* fill or wireframe model */
		
		static bool doScreenshot;
		static const char * screenshotFileName;
	
		// callbacks
		static void onDisplay(void);
		static void onReshape(int w, int h);
		static void onKeyboard(unsigned char key, int x, int y);
		static void onSpecial(int key, int, int);
		static void onMouseClick(int button, int state, int x, int y);
		static void onMouseMotion(int x, int y);
	
		static void start(TriangleSetExt * patchEnumerator, TColorInput colorInput);
	
		Visualizer();
	
		static void createWindow(const char * title);						
		static void saveScreenshot(const char * filename);
		static void createCallList(void);
};

#endif
