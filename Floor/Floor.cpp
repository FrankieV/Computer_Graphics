#include "stdafx.h"
#include <cmath>
#include "GL/freeglut.h"

// angle of rotation for the camera direction
float angle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = 30.0f, z = 4.0f;
float p = 0.0;


float y = 0.0f;

//GLfloat  ambientLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };

GLfloat	 lightPos[] = { 0.0f, 20.0f, 75.0f, 1.0f };
GLfloat  specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat  specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat  ambientLight[] = { 0.4f, 0.4f, 0.4f, 1.0f };
GLfloat  spotDir[] = { 0.0f, 0.0f, -1.0f };


void init()
{
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	
	// Enable this light in particular
	glEnable(GL_LIGHT0);

	// Enable color tracking
	glEnable(GL_COLOR_MATERIAL);

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}


void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, (GLfloat)w / (GLfloat)h, 1.0, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	gluLookAt(0.0, -50.0, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glRotatef(-45.0, 1.0, 0.0, 0.0);
	
}

void keyboard(unsigned char key, int w, int h)
{
	float fraction = 0.1f;

	if (key == 'd')
	{
		x += 1.0;
	}
	if (key == 'w')
	{
		y += 1.0;
	}
	if (key == 'p')
		p += 1.0;
	if (key == 'o')
		z+=1.0;
	if (key == 's')
		y -= 1.0;
	if (key == 'a')
		x -= 1.0;
	
	glutPostRedisplay();

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);	
	glClear(GL_DEPTH_BUFFER_BIT);
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };

	//glLoadIdentity();
	glPushMatrix();

	gluLookAt(x, y, z,
		x + lx, y, z + lz,
		0.0f, 1.0f, 1.0f);
	
	glPushMatrix();
	//glRotatef(p, 0.0, 0.0, 1.0);
	
//	glDisable(GL_LIGHTING);
	for (int i = 0; i < 10; i++)
	{
		glPushMatrix();
		
		for (int j = 0; j < 5; j++)
		{
			
			if (i % 2 == 0)
			{
				glColor4fv(white);
				glTranslatef(10.0, 0.0, 0.0);
				glRectf(-5.0, 5.0, 5.0, -5.0);

				glColor4f(0.0, 0.0, 1.0, 1.0);
				glTranslatef(10.0, 0.0, 0.0);
				glRectf(-5.0, 5.0, 5.0, -5.0);

			}
			else
			{
				glColor4f(0.0, 0.0, 1.0, 1.0);
				glTranslatef(10.0, 0.0, 0.0);
				glRectf(-5.0, 5.0, 5.0, -5.0);

				glColor4fv(white);
				glTranslatef(10.0, 0.0, 0.0);
				glRectf(-5.0, 5.0, 5.0, -5.0);

			}
					
		}
		glPopMatrix();
		glTranslatef(0.0, -10.0, 0.0);	
	}


	glPushMatrix();
	glTranslatef(20.0, 20.0, 2.5);
	glColor3f(1.0, 0.0, 0.0);
	glutSolidCube(5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0, 0.0, 1.0);
	glTranslatef(40.0, 20.0, 20.0);
	glutSolidSphere(2.0, 50, 60);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);
	glTranslatef(30.0, 40.0, -0.5);
	glutSolidSphere(4.0, 50, 60);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Floor");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}


//alpha += delta_alpha
//cos(2.0 * M_PI * aplha / 360.0)
//sin(2.0 * M_PI * aplha / 360.0)
//
//MP_I = pi greco(costante).