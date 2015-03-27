#include "stdafx.h"
#include <GL/glut.h>
#include <gl/freeglut.h>

static int year = 0, day = 0, satellite = 0;

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (1.0, 1.0, 1.0);
   glPushMatrix();
   glutWireSphere(1.0, 30, 50);   /* draw sun */
   glRotatef ((GLfloat) year, 0.0, 1.0, 0.0);
   glTranslatef (2.0, 0.0, 0.0);
   glRotatef ((GLfloat) day, 0.0, 1.0, 0.0);
   glutWireSphere(0.2, 10, 8);    /* draw smaller planet */
   glColor3f(0.0, 0.0, 2.0);
   glRotatef((GLfloat)satellite, 0.0, 0.0, 1.0); // why?
   glTranslatef(0.5, 0.0,0.0);
   glutWireSphere(0.05, 40, 20);
   glPopMatrix();
  
   glPushMatrix();
   glRotatef((GLfloat)year, 0.0, 1.0, 0.0);
   glColor3f(0.0, 1.0, 1.0);
   glTranslatef(0.0, -2.0, 0.0);
   glutWireSphere(0.4, 10, 15);
   glPopMatrix();

   glutSwapBuffers();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case 'd':
         day = (day + 10) % 360;
         glutPostRedisplay();
         break;
	  case 's':
		  satellite = (satellite + 5) % 360; // why?
		  glutPostRedisplay();
		  break;
      case 'D':
         day = (day - 10) % 360;
         glutPostRedisplay();
         break;
      case 'y':
         year = (year + 5) % 360;
		 day = (day + 5) % 360;
         glutPostRedisplay();
         break;
      case 'Y':
         year = (year - 5) % 360;
         glutPostRedisplay();
         break;
	  case 'b':
		day = (day - 10) % 360;
		year = (year + 5) % 360;
		glutPostRedisplay();
		break;
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("OpenGL Test");
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
