// OpenGL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <gl/glut.h>
#include <gl/glu.h>

using namespace std;


void reshape(int w, int h)
{
	//
}

void display(void)
{
	//Clear all pixels
	glClear(GL_COLOR_BUFFER_BIT);
	//draw white polygon (rectangle) with corners at
	// (0.25, 0.25, 0.0) and (0.75, 0.75, 0.0)
	
    glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex3f(0.25, 0.25, 0.0);
	glVertex3f(0.75, 0.25, 0.0);
	glVertex3f(0.75, 0.75, 0.0);
	glVertex3f(0.25, 0.75, 0.0);
	glEnd();

	// Don't wait start processing buffered OpenGL routines
	glFlush();
}

void init(void)
{
	//select clearing (background) color
	glClearColor(0.0, 2.0, 0.0, 0.0);

	//initialize viewing values 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0,1.0);
	

}

void KeyPressed(unsigned char key, int x, int y)
{
	if (key == 'y')
		glClearColor(1.0, 3.0, 0.0, 0.0);
	else if (key == 'b')
		glClearColor(0.0, 0.0, 0.0, 0.0);
	glutPostRedisplay(); //display callback function
	
}


int main(int argc, char** argv)
{

	//Initialise GLUT with command-line parameters. 
	glutInit(&argc, argv);

	//Set Display Mode
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	//Set the window size
	glutInitWindowSize(500, 500);

	//Set the window position
	glutInitWindowPosition(100, 100);

	//Create the window
	glutCreateWindow("Hello OpenGL =)");

	glutKeyboardFunc(KeyPressed);

	//Call init (initialise GLUT)
	init();


	glutReshapeFunc(reshape);

	//Call "display" function
	glutDisplayFunc(display);

	
	//Enter the GLUT event loop
	glutMainLoop();

	return 0;
}
