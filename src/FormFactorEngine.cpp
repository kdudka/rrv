#include "FormFactorEngine.h"
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

#include <math.h>
#ifndef M_PI
	#define M_PI		3.14159265358979323846
#endif

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

	float S = 256.0f*256.0f+4*256.0f*128.0f;

	map<unsigned,double>::iterator iter;
	for( iter = ffmap->begin(); iter != ffmap->end(); iter++ ) {
		if(iter->first != 0xffffff)
		{
			// this condition is due tu not offscreen rendering!!!
			if(static_cast<unsigned>(patchSet.count()) > iter->first)
				cacheLine->addPatch(iter->first, 2.0*iter->second/S);
		}
	}

	delete ffmap;
}

map<unsigned,double> *FormFactorEngine::getFF()
{
	map<unsigned,double> *ffmap = new map<unsigned,double>();

	unsigned resW = FormFactorEngine::EDGE_LENGTH;
	unsigned resH = FormFactorEngine::EDGE_LENGTH;

	int size=resW*resH*3;

	unsigned char *screen = new unsigned char[size];

	unsigned w, h, clr;

	unsigned char r,g,b;

	glReadPixels(0, 0, resW, resH, GL_BGR, GL_UNSIGNED_BYTE, screen); 

	for(w=128;w<(128+512);w++)
	for(h=128;h<(128+512);h++)
	{ 
		b = screen[ 3*(w*resH+h) +0];
		g = screen[ 3*(w*resH+h) +1];
		r = screen[ 3*(w*resH+h) +2];
		clr = ((unsigned)r)+((unsigned)g<<8)+((unsigned)b<<16);

		//screen[ 3*(w*resH+h) +0] = (unsigned char)(b*ffcoefs[w-128][h-128]);
		//screen[ 3*(w*resH+h) +1] = (unsigned char)(g*ffcoefs[w-128][h-128]);
		//screen[ 3*(w*resH+h) +2] = (unsigned char)(r*ffcoefs[w-128][h-128]);

		((*ffmap)[clr]) += /*1*/ffcoefs[w-128][h-128];
	} 

	//glDrawPixels(resW, resH, GL_BGR, GL_UNSIGNED_BYTE, screen);

	delete[] screen;

	return ffmap;
}

/**
 * @param  patchIterator
 */
FormFactorEngine::FormFactorEngine (PatchRandomAccessEnumerator *patchEnumerator):
	patchEnumerator_(patchEnumerator)
{
	createGLWindow();

	ffcoefs = new double*[EDGE_2];
	for(int i=0; i<EDGE_2; i++)
	{
		ffcoefs[i] = new double[EDGE_2];
		for(int j=0; j<EDGE_2; j++)
		{
			unsigned tw = -EDGE_1 + i;
			unsigned th = -EDGE_1 + j;
			unsigned R = EDGE_2;
			double cw = cos( M_PI * tw/(double)R );
			double ch = cos( M_PI * th/(double)R );
			ffcoefs[i][j] = cw*ch;
		}
	}
}

FormFactorEngine::~FormFactorEngine()
{
#if defined(__WIN32__) || defined(_WIN32) || defined(__CYGWIN__) 
#else
	XCloseDisplay(dpy);
#endif
	for(int i=0; i<EDGE_2; i++)
		delete[] ffcoefs[i];
	delete[] ffcoefs;
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

class Vector;
inline float operator* (const Vector &u, const Vector &v);

#ifndef NDEBUG
inline std::ostream& operator<< (std::ostream &out, const Vector &vect);
#endif
	
class Vector {
	public:
		// tohle by mohlo byt pozdeji private
		float dx,dy,dz;

	public:
		static Vector fromTriangle(Triangle &t) {
			return Vector(
					t.vertex[0],
		 			t.vertex[1],
					t.vertex[2]
					);
		}
		
		static float cos(const Vector &u, const Vector &v) {
			return (u*v)/(u.size()*v.size());
		}
		
		float& operator[](int i) {
			if(0==i)
				return dx;
			if(1==i)
				return dy;
			if(2==i)
				return dz;
			return(dx);
		}
		
		const float& operator[](int i) const {
			if(0==i)
				return dx;
			if(1==i)
				return dy;
			if(2==i)
				return dz;
			return(dx);
		}
		
		Vector(const float dx, const float dy, const float dz)
		{
			this->dx = dx;
			this->dy = dy;
			this->dz = dz;
		}

		Vector(const Vertex &a, const Vertex &b) {
			dx = a.x - b.x;
			dy = a.y - b.y;
			dz = a.z - b.z;
		}

		Vector(const Vector &u, const Vector &v) { // normalovy
			dx = u[1]*v[2] - v[1]*u[2];
			dy = u[2]*v[0] - v[2]*u[0];
			dz = u[0]*v[1] - v[0]*u[1];
		}
		
		Vector(const Vertex &a, const Vertex &b, const Vertex &c) {
			Vector u(b,a);
			Vector v(c,a);
			dx = u[1]*v[2] - v[1]*u[2];
			dy = u[2]*v[0] - v[2]*u[0];
			dz = u[0]*v[1] - v[0]*u[1];
		}
		
		float size() const {
			return sqrtf( dx*dx + dy*dy + dz*dz );
		}
/*
		Vertex operator+ (const Vertex &v) {
			return Vertex(v.x+dx, v.y+dy, v.z+dz);
		}
*/
};

inline float operator* (const Vector &u, const Vector &v) {
	return u[0]*v[0] + u[1]*v[1] + u[2]*v[2];
}
#ifndef NDEBUG
inline std::ostream& operator<< (std::ostream &out, const Vector &vect) {
	out << "vect(" <<
			vect[0] << ", " <<
			vect[1] << ", " <<
			vect[2] << ")";
	return out;
}
#endif

inline Vertex operator+ (const Vertex &v, const Vector &n) {
	return Vertex(v.x+n.dx, v.y+n.dy, v.z+n.dz);
}

void FormFactorEngine::drawScene()
{
	PatchRandomAccessEnumerator &patchSet = *patchEnumerator_;
	glBegin(GL_TRIANGLES);
	for(unsigned i=0;i< static_cast<unsigned>(patchSet.count());i++)
	{
		Triangle &t = patchSet[i];
		glColor3ub((i)&0xff, (i>>8)&0xff,(i>>16)&0xff);
		for(int j=0;j<3;j++)
			glVertex3f(t.vertex[j].x,t.vertex[j].y,t.vertex[j].z);
	}
	glEnd();
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
	glViewport(x,y, 256,256);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, (double)EDGE_LENGTH/(double)EDGE_LENGTH, 1e-3, 50);
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

	// center of this triangle
	Vertex c = Triangle::centerOf(t0);
	
	// normal vector and inverse normal vector of this triangle
	Vector norm = Vector::fromTriangle(t0);
	Vector norm_m(-norm.dx,-norm.dy,-norm.dz);
	
	Vector side(norm, Vector(1,2,3));
	if(0 == side.size())
	    side = Vector(norm, Vector(1,1,1)); // a neudelat Vector::operator= ??
	Vector side_m(-side.dx,-side.dy,-side.dz);
	
	// side vectors
	Vector vctD(/*t0.vertex[0], t0.vertex[1]*/side);
	Vector vctC(/*t0.vertex[1], t0.vertex[0]*/side_m);
	Vector vctA( vctD, norm );
	Vector vctB( vctC, norm );
	
	// points for directions of camera (top and 4 side)
	Vertex at = c+norm;
	Vertex atA = c+vctA;
	Vertex atB = c+vctB;
	Vertex atC = c+vctC;
	Vertex atD = c+vctD;
	
	// top
	renderViewport(256, 256, c, at, vctA);
	
	// 1. side
	renderViewport(256, 512, c, atA, norm_m);
	
	// opposite side
	renderViewport(256, 0, c, atB, norm);
	
	// left side
	renderViewport(0, 256, c, atC, vctA);
	
	// right side
	renderViewport(512, 256, c, atD, vctA);
	
	// render
	glFlush();

	// only due to doublebuffering
#if defined(__WIN32__) || defined(_WIN32) || defined(__CYGWIN__) 
#else
	glXSwapBuffers( dpy, win );
#endif
}
