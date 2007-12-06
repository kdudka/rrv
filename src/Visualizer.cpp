#include "Visualizer.h"
#include "Triangle.h"
#include "TriangleSetExt.h"

#include <stdlib.h>
#include <fstream>
#include <iostream>
// #include <unistd.h>

#if defined(__WIN32__) || defined(_WIN32) || defined(__CYGWIN__) 
	#include <GL/glext.h>
#else
#endif

TriangleSetExt * Visualizer::patchSequenceEnumerator_;
Visualizer::TColorInput Visualizer::colorInput;

GLuint Visualizer::scene_;

int   Visualizer::xnew, Visualizer::ynew, Visualizer::znew;                  /* actual position */
int   Visualizer::xold, Visualizer::yold, Visualizer::zold;                  /* old position */
int   Visualizer::xx1, Visualizer::yy1, Visualizer::zz1;                     /* mouse position*/
int   Visualizer::mouseState;                        /* mouse button state */
int   Visualizer::xshift, Visualizer::yshift;                    /* shifting in space*/

float Visualizer::fov;                               /* field of view */
float Visualizer::near_plane;                        /* trim plain */
float Visualizer::far_plane;                         /* farther trim plain */
float Visualizer::line_width;                        /* width of line */
int   Visualizer::WindowWidth=700;                       /* width and height of window */
int   Visualizer::WindowHeight=700;
int   Visualizer::ObjectType;                        /* type of paint object */
int   Visualizer::Solid;                             /* fill or wireframe model */

bool  Visualizer::doScreenshot=false;
const char* Visualizer::screenshotFileName="screenshot.tga";

/**
 * This method is call on redisplay of winodow
 */
void Visualizer::onDisplay()
{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* clear color buffer */
        glMatrixMode(GL_MODELVIEW);                   /* will be change of model matrix */
        glLoadIdentity();                             /* load unit matrix */

        glTranslatef(xshift, yshift, -50.0f);         /* shift of model farther from camera */
        glTranslatef(xshift, yshift, znew);
        glRotatef(ynew, 1.0, 0.0, 0.0);               /* rotation of object according mouser cursor */
        glRotatef(xnew, 0.0, 1.0, 0.0);

        glCallList(scene_);                            /* draw my scene */

        glFlush();                                    /* do all directions */
        glutSwapBuffers();                            /* swap of buffer */
	
	if(doScreenshot)
	{
//		sleep(1); // FIXME:remove this line 
		saveScreenshot(screenshotFileName);
		exit (0);
	}
}


/**
 * Configuration system of coordinates according window size
 */
void Visualizer::onReshape(int w, int h)
{
        glViewport(0, 0, w, h);                       /* visibility area */
        glMatrixMode(GL_PROJECTION);                  /* projection matrix */
        glLoadIdentity();                             /* load matrix projection */
        gluPerspective(fov, (double)w/(double)h, near_plane, far_plane);
        glMatrixMode(GL_MODELVIEW);                   /* model matrix */
        WindowWidth = w;                              /* save window size */
        WindowHeight = h;
}


/**
 * This method is call on press of key
 */
void Visualizer::onKeyboard(unsigned char key, int, int)
{
  	if (key>='A' && key<='Z') key+='a'-'A';       /* convert upper char on lower */

  	switch (key) {                                /* press key */
    		case '1' :                                  	/* show vertex */
      			glPolygonMode(GL_FRONT, GL_POINT);
      			glPolygonMode(GL_BACK, GL_POINT);
      			glutPostRedisplay();
      			break;
    		case '2' :                                  	/* show wireframe */
      			glPolygonMode(GL_FRONT, GL_LINE);
      			glPolygonMode(GL_BACK, GL_LINE);
      			glutPostRedisplay();
     			break;
    		case '3' :                                  	/* show fill polygons */
      			glPolygonMode(GL_FRONT, GL_FILL);
      			glPolygonMode(GL_BACK, GL_FILL);
      			glutPostRedisplay();
      			break;
    		case '5' :                                  	/* decrease vision angle */
      			if (fov>0) fov--;
      			onReshape(WindowWidth, WindowHeight);
      			glutPostRedisplay();
     			break;
    		case '6' :
      			if (fov<180) fov++;                     /* increase vision angle */
      			onReshape(WindowWidth, WindowHeight);
      			glutPostRedisplay();
      			break;
    		case '7' :                                  	/* zoom in trim plain */
      			if (line_width > 0) line_width -= 0.5;	
		  	glLineWidth(line_width);                
		  	glPointSize(line_width);                
      			onReshape(WindowWidth, WindowHeight);
      			glutPostRedisplay();
      			break;
    		case '8' :                                  	/* zoom out trim plain */
      			if (line_width < 10) line_width += 0.5;
		  	glLineWidth(line_width);                
		  	glPointSize(line_width);                
      			onReshape(WindowWidth, WindowHeight);
      			glutPostRedisplay();
      			break;
		case 's' :				    	/* print to bitmap - screenshot */
			saveScreenshot(screenshotFileName);
			break;
    		case 27 :                                   	/* Escape */
    		case 'q' :
      			exit(0);                                /* exit */
      			break;
    		case 'm' :
			colorInput = INTERPOLATE_RADIOSITY;	/* interpolate radiosity */
			createCallList();
			glutPostRedisplay();
			break;
    		case 'b' :
			colorInput = INTERPOLATE_RADIOSITY_RAW;	/* interpolate radiosity */
			createCallList();
			glutPostRedisplay();
			break;
    		case 'v' :
			colorInput = RADIOSITY;	/* radiosity */
			createCallList();
			glutPostRedisplay();
			break;
    		case 'n' :
			colorInput = RADIOSITY_LAST;	/* radiosity */
			createCallList();
			glutPostRedisplay();
			break;
		case 'z' :
			colorInput = REFLECTIVITY;	/* reflectivity + emission */
			createCallList();
			glutPostRedisplay();
			break;
		case 'x' :
			colorInput = EMISSION;	/* reflectivity */
			createCallList();
			glutPostRedisplay();
			break;
		case 'c' :
			colorInput = REFLECT_EMISS;	/* emission */
			createCallList();
			glutPostRedisplay();
			break;
   		case 'f' :
      			glutFullScreen();                      	/* fullscrean */
      			break;
    		case 'w' :
      			glutReshapeWindow(WindowWidth, WindowHeight);              /* go back to window */
      			break;
    		default:
      			break;
  	}
}


/**
 * Method is call on press special key
 */
void Visualizer::onSpecial(int key, int, int)
{
        switch(key)
        {
                case GLUT_KEY_UP:
                        if (yshift>-50) yshift--;                       /* shift up */
                        onReshape(WindowWidth, WindowHeight);
                        glutPostRedisplay();
                        break;
                case GLUT_KEY_DOWN:
                        if (yshift<50) yshift++;                       /* shift down */
                        onReshape(WindowWidth, WindowHeight);
                        glutPostRedisplay();
                        break;
                case GLUT_KEY_LEFT:
                        if (xshift<50) xshift++;                       /* shift left */
                        onReshape(WindowWidth, WindowHeight);
                        glutPostRedisplay();
                        break;
                case GLUT_KEY_RIGHT:
                        if (xshift>-50) xshift--;                       /* shift right */
                        onReshape(WindowWidth, WindowHeight);
                        glutPostRedisplay();
                        break;
        }	
}


/**
 * Method is call on press mouse button
 */
void Visualizer::onMouseClick(int button, int, int x, int y)
{
        if (button == GLUT_LEFT_BUTTON) {               /* left button activate of rotation */
                if (mouseState == GLUT_DOWN) {          /* on press */
                        mouseState = 1;                 /* set state for function motion */
                        xx1 = x;                        /* remember position of mouse cursor */
                        yy1 = y;
                } else {                                /* GLUT_UP */
                        mouseState = 0;                 /* normal state */
                        xold = xnew;                    /* remember new start */
                        yold = ynew;
                }
        }
        if (button == GLUT_RIGHT_BUTTON) {              /* right mouse button activating shift */
                if (mouseState == GLUT_DOWN) {          /* pri stlaceni */
                        mouseState = 2;                 /* nastaveni pro funkci motion */
                        zz1 = y;                        /* remember position of mouse cursor */
                }
                else {
                        mouseState = 0;
                        zold = znew;                    /* remember new start */
                }
        }
        glutPostRedisplay();                            /* redraw scene */
}

/**
 * Method is call on mouse motion
 */
void Visualizer::onMouseMotion(int x, int y)
{
        if (mouseState == 1) {                          /* state of position */
                xnew = xold+x-xx1;                      /* count new position */
                ynew = yold+y-yy1;
                glutPostRedisplay();                    /* redraw scene */
        }
        if (mouseState == 2) {                          /* shift state */
                znew = zold+y-zz1;                      /* count new position */
                glutPostRedisplay();                    /* redraw scene */
        }
}

/**
 * Method for create a window
 */
void Visualizer::createWindow(const char * title)
{	
        xnew=0; ynew=0; znew=0;                   /* actual position */
        xold=0; yold=0; zold=0;                   /* old position */
        xx1=0; yy1=0; zz1=0;                      /* mouse position*/
        mouseState=0;                             /* mouse button state */
        xshift=0; yshift=0;                       /* shifting in space*/

        xnew=0; ynew=0; znew=0;                   /* actual position */
        xold=0; yold=0; zold=0;                   /* old position */
        xx1=0; yy1=0; zz1=0;                      /* mouse position*/
        mouseState=0;                             /* mouse button state */
        xshift=0; yshift=0;                       /* shifting in space*/

 	fov=45.0;                                 /* field of view */
 	near_plane=1;                             /* trim plain */
 	far_plane=1000.0;                         /* farther trim plain */
 	line_width=1.0;                           /* width of line */
 	WindowWidth=1024;                         /* width and height of window */
 	WindowHeight=768;
 	ObjectType=0;                             /* type of paint object */
 	Solid=1;                                  /* fill or wireframe model */

	char * dummy_argv[1];
  dummy_argv[0] = const_cast<char *>("run");
	int dummy_argc = 1;

	glutInit(&dummy_argc, dummy_argv);

        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH); /* graphic mod of window */
        glutInitWindowSize(WindowWidth,WindowHeight); /* original window size */
        glutInitWindowPosition(10,10);                /* original window position */
        glutCreateWindow(title);                      /* create window */

        glutDisplayFunc(onDisplay);                   /* set function for redisplay */
        glutReshapeFunc(onReshape);                   /* set function for change of size window */
        glutKeyboardFunc(onKeyboard);                 /* set function for press key */
        glutSpecialFunc(onSpecial);
        glutMouseFunc(onMouseClick);                  /* set function for press mouse button */
        glutMotionFunc(onMouseMotion);                /* set function for mouse move */

        glClearColor(0.0, 0.0, 0.0, 0.0);             /* color for clearing of color-buffer */
        glClearDepth(1.0f);                           /* color for clearing of z-buffer */
        glEnable(GL_DEPTH_TEST);                      /* configure function for testing value in z-buffer */
        glDepthFunc(GL_LESS);                         /* select function */
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); /* improvement display */

        glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT, GL_FILL);             /* configure draw of fill polygons */
        //glCullFace(GL_FRONT);

        glLineWidth(line_width);                      /* line width */
  	glPointSize(line_width);                

        scene_ = glGenLists(1);                        /* get number of calllist */
        createCallList();                             /* initialization */

	glutMainLoop();
}


/**
 * Visualization
 */
void Visualizer::visualize(const char * setTitle, TriangleSetExt * patchSequenceEnumerator)
{
	patchSequenceEnumerator_ = patchSequenceEnumerator;
	createWindow(setTitle);
}

/**
 *  Screenshot
 */
void Visualizer::takeScreenshot(char * filename, TriangleSetExt * patchSequenceEnumerator)
{
	doScreenshot = true;
	screenshotFileName = filename;

	patchSequenceEnumerator_ = patchSequenceEnumerator;	
	createWindow("Screenshot");
}

/**
 * Method for save Screenshot of scene to *.tga
 */
void Visualizer::saveScreenshot(const char * filename)
{
	int resolutionW = WindowWidth;
	int resolutionH = WindowHeight;

	//char filename[]="screenshot.tga"; 
	int size=resolutionW*resolutionH*4; 

	unsigned char *screen = (unsigned char*)malloc(size*sizeof(unsigned char)); 

	FILE *fout; 
	int i, j; 
	int width=resolutionW; 
	int height=resolutionH; 
	unsigned char tgaHeader[18]={0x00,
				     0x00, 
				     0x02, //truecolor 
				     0x00, 0x00, 
				     0x00, 0x00, 0x00, 
				     0x00, 0x00, 0x00, 0x00, //position
				     0x00, 0x00, 0x00, 0x00, //width and height 
				     0x20, //bpp 
				     0x20}; //orientation

	memcpy(tgaHeader+12, &width, 2); 
	memcpy(tgaHeader+14, &height, 2); 

	// reading pixels in glut window
	glReadPixels(0, 0, resolutionW, resolutionH, GL_BGRA_EXT, GL_UNSIGNED_BYTE, screen); 

	// open file
	if((fout=fopen(filename, "wb")) == NULL)
		std::cerr << "Error: Can't open file \n";
	
	// write to file
	if (fout) 
	{ 
		fwrite(tgaHeader, 18, 1, fout); 
	
		for (j=resolutionH; j>0; j--) 
		{ 
			for (i=0; i<resolutionW*4; i++) 
			{ 
			fputc(screen[((j*resolutionW-resolutionW)*4)+i], fout); 
			} 
		} 
		fclose(fout); 
	} 

	free(screen); 
	screen=NULL; 
}

/**
 * Method draw triangles in patchEnumerator to scene
 */
void Visualizer::start(TriangleSetExt *pTset, TColorInput colorInput)
{
	TriangleSetExt &tset = *pTset;

	glBegin(GL_TRIANGLES);
	for(unsigned int i=0; i<tset.count(); i++) {
		TriangleExt &te = tset[i];
		for(int i=0; i<3; i++) {
			Color &color = te.vertexColor[i];
			Color &color2 = te.vertexColorRaw[i];
			Vertex &vertex = te.vertex[i];
			
			switch (colorInput)
			{
				case INTERPOLATE_RADIOSITY:
					glColor3f(
						color.r,
						color.g,
						color.b);
					break;
				
				case INTERPOLATE_RADIOSITY_RAW:
					glColor3f(
						color2.r,
						color2.g,
						color2.b);
					break;
				
				case RADIOSITY_LAST:
					glColor3f(
						te.radiosityLast.r,
						te.radiosityLast.g,
						te.radiosityLast.b);
					break;
					
				case RADIOSITY:
					glColor3f(
						te.radiosity.r,
						te.radiosity.g,
						te.radiosity.b);
					break;

				case REFLECTIVITY:
					glColor3f(
						te.reflectivity.r, 
						te.reflectivity.g, 
						te.reflectivity.b);
					break;

				case EMISSION:
					glColor3f(
						te.emission.r, 
						te.emission.g, 
						te.emission.b);
				
					break;

				case REFLECT_EMISS:
					glColor3f(
						te.reflectivity.r + te.emission.r, 
						te.reflectivity.g + te.emission.g, 
						te.reflectivity.b + te.emission.b);

					break;
			}
			glVertex3f(
				vertex.x,
				vertex.y,
				vertex.z);
		}
	}	
  	glEnd();
}


/**
 * Method for create calllist
 */
void Visualizer::createCallList(void)
{
        glNewList(scene_,GL_COMPILE);

        start(patchSequenceEnumerator_, colorInput);

        glEndList();
}


