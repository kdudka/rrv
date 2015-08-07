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

#include "FormFactorEngine.h"
#include "FormFactorView.h"
#include "TriangleSet.h"
#include "PatchRandomAccessEnumerator.h"
#include "PatchCacheLine.h"

#include <GL/glu.h>

#if defined(__WIN32__) || defined(_WIN32) || defined(__CYGWIN__)
	#include <windows.h>
	#include <wingdi.h>
	#include <GL/glext.h>
#else
	#include <X11/Xlib.h>
	#include <GL/glx.h>
#endif

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

using namespace std;

/**
 * @param destPatch Index of destination patch.
 * @param cacheLine Pointer to target cache line.
 */
void FormFactorEngine::fillCacheLine(int destPatch, PatchCacheLine *cacheLine)
{
    PatchRandomAccessEnumerator &patchSet = *patchEnumerator_;

    renderFullScene(destPatch);
    map<unsigned,double> *ffmap = getFF();

    map<unsigned,double>::iterator iter;
    for( iter = ffmap->begin(); iter != ffmap->end(); iter++ ) {
        if(iter->first != 0xffffff)
        {
            // this condition is due tu not offscreen rendering!!!
            if(static_cast<unsigned>(patchSet.count()) > iter->first)
                cacheLine->addPatch(iter->first, iter->second);
        }
    }

    delete ffmap;
}

map<unsigned,double> *FormFactorEngine::getFF()
{
    map<unsigned,double> *ffmap = new map<unsigned,double>();

    unsigned EDGE_1_2 = hemicube_.edge() / 2;
    unsigned EDGE_2 = hemicube_.edge() * 2;

    unsigned resW = EDGE_2;
    unsigned resH = resW;

    int size=resW*resH*3;

    unsigned char *screen = new unsigned char[size];

    unsigned w, h, clr;

    unsigned char r,g,b;

    glReadPixels(EDGE_1_2, EDGE_1_2, resW, resH, GL_BGR, GL_UNSIGNED_BYTE, screen);

    for(w=0;w<EDGE_2;w++)
        for(h=0;h<EDGE_2;h++)
        {
            b = screen[ 3*(w*resH+h) +0];
            g = screen[ 3*(w*resH+h) +1];
            r = screen[ 3*(w*resH+h) +2];
            clr = ((unsigned)r)+((unsigned)g<<8)+((unsigned)b<<16);

            //screen[ 3*(w*resH+h) +0] = (unsigned char)(b*ffcoefs[w-128][h-128]);
            //screen[ 3*(w*resH+h) +1] = (unsigned char)(g*ffcoefs[w-128][h-128]);
            //screen[ 3*(w*resH+h) +2] = (unsigned char)(r*ffcoefs[w-128][h-128]);

            ((*ffmap)[clr]) += hemicube_.ff(w,h);
        }

    //glDrawPixels(resW, resH, GL_BGR, GL_UNSIGNED_BYTE, screen);

    delete[] screen;

    return ffmap;
}

/**
 * @param  patchIterator
 */
FormFactorEngine::FormFactorEngine (PatchRandomAccessEnumerator *patchEnumerator, const FormFactorHemicube &hemicube):
    patchEnumerator_(patchEnumerator), hemicube_(hemicube)
{
    createGLWindow();
    uploadScene();
}

FormFactorEngine::~FormFactorEngine()
{
#if defined(__WIN32__) || defined(_WIN32) || defined(__CYGWIN__)
#else
    XCloseDisplay(dpy);
#endif
}

#if defined(__WIN32__) || defined(_WIN32) || defined(__CYGWIN__)
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
#else
#endif

void FormFactorEngine::createGLWindow()
{
#if defined(__WIN32__) || defined(_WIN32) || defined(__CYGWIN__)
    HINSTANCE hInstance = NULL;

    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc= (WNDPROC)WndProc;
    wcex.cbClsExtra= 0;
    wcex.cbWndExtra= 0;
    wcex.hInstance= hInstance;
    wcex.hIcon= 0;
    wcex.hCursor= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground= (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName= 0;
    wcex.lpszClassName= "RadiosityRenderWindowClass";
    wcex.hIconSm= 0;

    RegisterClassEx(&wcex);

    HWND hWnd = CreateWindow("RadiosityRenderWindowClass", "Radiosity render window",
            WS_POPUP, 0, 0, EDGE_LENGTH, EDGE_LENGTH, NULL, NULL, hInstance, NULL);

    HDC hDC = GetDC( hWnd );
    HGLRC  hglrc;

    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;
    ZeroMemory( &pfd, sizeof( pfd ) );
    pfd.nSize = sizeof( pfd );
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    iFormat = ChoosePixelFormat( hDC, &pfd );
    SetPixelFormat( hDC, iFormat, &pfd );

    hglrc = wglCreateContext (hDC);
    wglMakeCurrent (hDC, hglrc);

    //ShowWindow(hWnd, SW_SHOW);
    //UpdateWindow(hWnd);
#else
    int attributeList[]={
        GLX_RGBA, GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 16,
        None};

    dpy = XOpenDisplay(getenv("DISPLAY"));
    if(!dpy)
    {
        cerr << "Cannot open display!" << endl;
        exit(-1);
    }

    int screen;
    screen = DefaultScreen(dpy);

    XVisualInfo *vi;
    vi = glXChooseVisual(dpy, screen, attributeList);
    if(!vi)
    {
        cerr << "Cannot find visual with desired attributes!" << endl;
        exit(-1);
    }

    GLXContext cx;
    cx = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    if(!cx)
    {
        cerr << "Cannot create context!" << endl;
        exit(-1);
    }

    int EDGE_LENGTH = 3 * hemicube_.edge();
    win = XCreateSimpleWindow(dpy, RootWindow(dpy, vi->screen), 0, 0, EDGE_LENGTH,  EDGE_LENGTH, 0, 0, 0);

    XFree(vi);

    XMapWindow(dpy, win);

    XStoreName(dpy, win, "Computing form factors...");

    XSync(dpy,false);

    XEvent ev;
    memset(&ev,0,sizeof(ev));
    ev.type=ClientMessage;
    ev.xclient.type = ClientMessage;
    ev.xclient.message_type = XInternAtom(dpy, "_NET_WM_STATE", 0);
    ev.xclient.display=dpy;
    ev.xclient.window=win;
    ev.xclient.format=32;
    ev.xclient.data.l[0]= 1;
    ev.xclient.data.l[1]=XInternAtom(dpy, "_NET_WM_STATE_STAYS_ON_TOP", 0);
    XLockDisplay(dpy);
    XSendEvent(dpy, XDefaultRootWindow(dpy), 0, SubstructureRedirectMask | SubstructureNotifyMask, &ev);

    ev.xclient.data.l[1]=XInternAtom(dpy, "_NET_WM_STATE_ABOVE", 0);
    XSendEvent(dpy, XDefaultRootWindow(dpy), 0, SubstructureRedirectMask | SubstructureNotifyMask, &ev);
    XUnlockDisplay(dpy);

    XSync(dpy,false);

    glXMakeCurrent(dpy, win, cx);
#endif

    glewInit();

    // this should be in some event ;-)
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, EDGE_LENGTH, EDGE_LENGTH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, (double)EDGE_LENGTH/(double)EDGE_LENGTH, 1e-3, 50);
    glMatrixMode(GL_MODELVIEW);
}

struct vdata {
    GLfloat v[3];
    GLubyte c[4];
};

void FormFactorEngine::uploadScene()
{
    PatchRandomAccessEnumerator &patchSet = *patchEnumerator_;
    int bytes = 3 * patchSet.count() * sizeof(vdata);
    vdata *data = (vdata *) malloc(bytes);
    for(unsigned i=0;i< static_cast<unsigned>(patchSet.count());i++)
    {
        Triangle &t = patchSet[i];
        for(int j=0;j<3;j++)
        {
            int k = 3 * i + j;
            data[k].v[0] = t.vertex[j].x;
            data[k].v[1] = t.vertex[j].y;
            data[k].v[2] = t.vertex[j].z;
            data[k].c[0] = (i)&0xff;
            data[k].c[1] = (i>>8)&0xff;
            data[k].c[2] = (i>>16)&0xff;
            data[k].c[3] = 0xff;
        }
    }
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, bytes, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    free(data);
}

void FormFactorEngine::drawScene()
{
    PatchRandomAccessEnumerator &patchSet = *patchEnumerator_;
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glVertexPointer(3, GL_FLOAT, sizeof(vdata), 0);
    glEnableClientState(GL_VERTEX_ARRAY);
#define OFFSET(n) ((void *)(((char *) 0) + (n)))
    glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(vdata), OFFSET(3 * sizeof(GLfloat)));
#undef OFFSET
    glEnableClientState(GL_COLOR_ARRAY);
    glDrawArrays(GL_TRIANGLES, 0, 3 * patchSet.count());
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/**
 * @param eye Position of camera
 * @param center Direction of camera
 * @param up Up vector of camera
 */
void gluLookAtV(const Vertex &eye, const Vertex &center, const Vector &up)
{
    gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.dx, up.dy, up.dz);
}

/**
 * @param x Offset from left
 * @param y Offset from bottom
 * @param c Position of camera
 * @param at Direction of camera
 * @param up Up vector of camera
 */
void FormFactorEngine::renderViewport(const GLint x, const GLint y, const Vertex &c, const Vertex &at, const Vector &up)
{
    glViewport(x,y, hemicube_.edge(), hemicube_.edge());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, 1, 1e-3, 50);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAtV( c, at, up );
    drawScene();
}

/**
 * @param dest Index of destination patch.
 */
void FormFactorEngine::renderFullScene(int dest)
{
    PatchRandomAccessEnumerator &patchSet = *patchEnumerator_;

    // clear window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // destination triangle
    Triangle &t0 = patchSet[dest];

    FormFactorView v(t0);

    // points for directions of camera (top and 4 side)
    Vertex at = v.c+v.norm;
    Vertex atA = v.c+v.vctA;
    Vertex atB = v.c+v.vctB;
    Vertex atC = v.c+v.vctC;
    Vertex atD = v.c+v.vctD;

    int EDGE_1 = hemicube_.edge();
    int EDGE_2 = 2 * EDGE_1;
    // top
    renderViewport(EDGE_1, EDGE_1, v.c, at, v.vctA);

    // 1. side
    renderViewport(EDGE_1, EDGE_2, v.c, atA, v.norm_m);

    // opposite side
    renderViewport(EDGE_1, 0, v.c, atB, v.norm);

    // left side
    renderViewport(0, EDGE_1, v.c, atC, v.vctA);

    // right side
    renderViewport(EDGE_2, EDGE_1, v.c, atD, v.vctA);

    // render
    glFlush();

    // only due to doublebuffering
#if defined(__WIN32__) || defined(_WIN32) || defined(__CYGWIN__)
#else
    glXSwapBuffers( dpy, win );
#endif
}
