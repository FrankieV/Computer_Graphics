#include "stdafx.h"
#include <GL/glut.h>
#include <gl/freeglut.h>
#include "stdafx.h"
#include <gl/GLU.h>
#include <gl/freeglut.h>
#include <random>
#include <ctime>

void CreaParticelle();

GLfloat l = 60.0; GLint num_particelle = 50; GLfloat x = 2.0, y = 1.0; GLfloat around_x = 1.0; GLfloat light_rotate = 1.0;
GLfloat num_rand_y = 0.0; GLfloat num_rand_x = 0.0; bool espandi_particelle = true;
GLfloat vector_x[100], vector_y[100];

GLfloat	 lightPos[] = { 0.0f, 0.0f, 75.0f, 1.0f };
GLfloat  specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat  specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat  ambientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat  spotDir[] = { 0.0f, 0.0f, -1.0f };

void init(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	CreaParticelle();
	
	//glEnable(GL_DEPTH);
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	glMateriali(GL_FRONT, GL_SHININESS,10);

	glEnable(GL_LIGHT0);


	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void CreaParticelle()
{
	for (int i = 0; i < 100; i++)
	{
		num_rand_x = (rand() % (int)l / 2);
		vector_x[i] = num_rand_x;
		num_rand_y = (rand() % (int)l / 2);
		vector_y[i] = num_rand_y;
	}
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glRotatef(around_x, 0.0, 1.0,0.0);
	gluLookAt(1.0,1.0, 0.0, 0.0, 4.0, 0.0, 0.0, 0.0, 1.0);
	glColor4f(1.0, 1.0, 1.0,0.3);
	glutWireCube(2*l);
	glColor4f(1.0, 1.0, 0.0, 0.2f);
	glutWireCube(l);
	glPopMatrix();

	glPushMatrix();

	//glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir);


	// Draw a red cone to enclose the light source
	glColor3ub(255, 0, 0);

	// Translate origin to move the cone out to where the light
	// is positioned.
	glPushMatrix();
	glRotatef(light_rotate, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, 70.0);
	glutSolidCone(4.0f, 6.0f, 15, 15);
	glPopMatrix();

	// Draw a smaller displaced sphere to denote the light bulb
	// Save the lighting state variables
	glPushAttrib(GL_LIGHTING_BIT);

	// Turn off lighting and specify a bright yellow sphere
	glDisable(GL_LIGHTING);
	glColor3ub(255, 255, 0);
	glRotatef(light_rotate, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, 70.0);
	
	glutSolidSphere(3.0f, 15, 15);

	// Restore lighting state variables
	glPopAttrib();
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		glColor4f(0.0f, 0.1f, 0.1f, 0.5);
		glPushMatrix();
		glTranslatef(vector_x[i], vector_y[i], 0.0);
		glutSolidSphere(1, 20, 20);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-vector_x[i], -vector_y[i], 0.0);
		glutSolidSphere(1, 20, 20);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-vector_x[i], vector_y[i], 0.0);
		glutSolidSphere(1, 20, 20);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(vector_x[i], -vector_y[i], 0.0);
		glutSolidSphere(1, 20, 20);
		glPopMatrix();
	}
	glEnd();
	glPopMatrix();
	glutSwapBuffers();

	
}

void reshape(int w, int h)
{
	GLfloat fAspect;

	// Prevent a divide by zero
	if (h == 0)
		h = 1;

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	fAspect = (GLfloat)w / (GLfloat)h;
	gluPerspective(30.0f, fAspect, 1.0f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -400.0f);
}

void EspandiParticelle()
{
	for (int i = 0; i < 100; i++)
	{
		num_rand_x = (rand() % (int)l);
		vector_x[i] = num_rand_x;
		num_rand_y = (rand() % (int)l);
		vector_y[i] = num_rand_y;
		
	}
	
	glutPostRedisplay();
	//glutTimerFunc(200, EspandiParticelle, 123);
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == 'a')
	{
		around_x -= 5 % 360;
		glutPostRedisplay();
	}
	else if (key == 'b')
	{
		light_rotate += 5 % 360;
		glutPostRedisplay();
	}
	else if (key == 'x')
	{
		if (espandi_particelle)
		{
			//glutTimerFunc(50, EspandiParticelle, 123);
			glutIdleFunc(EspandiParticelle);
			espandi_particelle = false;
		}
		else
		{
			glutIdleFunc(NULL);
			espandi_particelle = true;
		}
	}
}


int main(int argc, char** argv)
{
	srand(time(0));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL Test");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
