#include "stdafx.h"
#include <iostream>
#include <gl\glut.h>
#include <gl\GLU.h>

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	GLfloat aspect_ratio = (GLfloat)w / (GLfloat)h;
	if (aspect_ratio <= 1)
	{
		glOrtho(-50.0, 50.0, -50.0 / aspect_ratio, 50.0 / aspect_ratio, -1.0, 1.0);
	}
	else
	{
		glOrtho(-50.0*aspect_ratio, 50.0*aspect_ratio, -50.0, 50.0, -1.0, 1.0);
	}

	

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init()
{
	//select clearing (background) color
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	////initialize viewing values 
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0.0f, 0.0f);
	// Draw a filled rectangle with current color
	glRectf(-25.0f, 25.0f, 25.0f, -25.0f);

	// Don't wait start processing buffered OpenGL routines
	glFlush();
	
}

void keyBoard(unsigned char key, int x, int y)
{
	if (key == 'd')
	{
		glRotatef(60, 0, 0, 1);
		glutPostRedisplay();
	}
	if (key == 's')
	{
		glScalef(2, 0.5, -1);
		glutPostRedisplay();
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(10, 20);
	glutInitWindowSize(800, 480);
	glutCreateWindow("Hello World");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyBoard);
	init();
	glutMainLoop();


}