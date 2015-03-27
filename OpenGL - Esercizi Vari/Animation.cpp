#include "stdafx.h"
#include <iostream>
#include <gl/glut.h>
#include <gl/freeglut.h>

static GLfloat spin = 0.0;
GLfloat x1=-25.0, x2=-25.0, y3=25.0, y2=25.0;
bool continua = true;
bool antiorario = false;

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	if (!antiorario)
		glRotatef(spin, 0.0, 0.0, 3.0);
	else
		glRotatef(-spin, 0.0, 0.0, 3.0);


	glColor3f(1.0, 1.0, 1.0);
	if (x1 <= 0.0 && continua)
	{
		glRectf(x1 += 0.05, x2 += 0.05, y3 -= 0.05, y2 -= 0.05);
	}
	else
	{
		if (x1 >= -25)
		{
			glRectf(x1 -= 0.05, x2 -= 0.05, y3 += 0.05, y2 += 0.05);
			continua = false;
		}
		else
			continua = true;
	}
	glPopMatrix();
	glutSwapBuffers();
	glFlush();

}

void spinDisplay(void)
{
	spin = spin + 0.3;
	if (spin >= 360)
	{
		spin = 0.0;

		if (antiorario == false)
			antiorario = true;
		else
			antiorario = false;
	}

	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);

	if (h == 0)
		h = 1;

	glLoadIdentity();
	GLfloat aspect_ratio = (GLfloat)w / (GLfloat)h;
	if (aspect_ratio <= 1)
	{
		glOrtho(-50.0, 50.0, -50.0/aspect_ratio, 50.0/aspect_ratio, -1.0, 1.0);
	}
	else
	{
		glOrtho(-50.0*aspect_ratio, 50.0*aspect_ratio, -50.0, 50.0, -1.0, 1.0);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void Key(unsigned char key, int w, int h)
{
	if (key == 'f')
		glutFullScreen();
	else if (key == 'r')
	{
		glutPositionWindow(50, 50);
		glutReshapeWindow(400, 300);
	}
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
			if (spin < 360.0)
				glutIdleFunc(spinDisplay);
			else
			spin = 0.0;
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
			glutIdleFunc(NULL);
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(250, 250);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL Test");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(Key);
	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}