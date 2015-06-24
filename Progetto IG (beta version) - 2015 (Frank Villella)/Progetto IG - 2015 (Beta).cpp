#include "stdafx.h"
#include <cmath>
#include "GL/freeglut.h"
#include <iostream>
#include <ctime>
#include "tga.h"
#include <vector>
#include "SDL_audio.h"
#include "SDL_timer.h"
#include "include\SDL_mixer.h"
#include "include\SDL_ttf.h"

using namespace std;

void loadAudio();

// angolo di rotazione della camera
float angle = 0.10;
// direzione della camera
float lx = 0.0f, lz = 1.0f;
// XZ position of the camera
float x =5.0f, z = 3.0f;
float fraction = 0.5f;
float deltaAngle = 0.0f;
int xOrigin = -1;
GLuint* texture;
unsigned char* data;
bool stanza_1 = true;
int rand_c_x;
int rand_c_y;
bool prima_volta = true;
bool primo_spostamento = true;

Mix_Music *music = NULL; /* SDL_Mixer*/
int num_rand_y = 0.0; int num_rand_x = 0.0;
int num_rand_z = 3.0, x_cube = 55.0;
float y = -2.0f;

GLuint elephant;
float elephantrot;
char ch = '1';


GLfloat fogColor[4] = { 1.0f, 0.1f, 0.0f, 0.0f };
GLfloat fogColor2[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; 
GLfloat fogColor3[4] = { 0.0f, 0.3f, 0.4f, 0.5f };
int vector_x[100], vector_y[100];
GLfloat height_cube = 40.0;

void loadObj(char *fname)
{
	FILE *fp;
	int read;
	GLfloat x, y, z;
	char ch;
	elephant = glGenLists(1);
	fp = fopen(fname, "r");
	if (!fp)
	{
		printf("Non posso aprire il file %s\n", fname);
		exit(1);
	}
	glPointSize(10.0);
	glNewList(elephant, GL_COMPILE);
	{
		glPushMatrix();
		glBegin(GL_POINTS);
		while (!(feof(fp)))
		{
			read = fscanf(fp, "%c %f %f %f", &ch, &x, &y, &z);
			if (read == 4 && ch == 'v')
			{
				glVertex3f(x, y, z);
			}
		}
		glEnd();
	}
	glPopMatrix();
	glEndList();
	fclose(fp);
}

void drawElephant()
{
	
	glPushMatrix();
	glTranslatef(0.0, 6.0, 10.0);
	glColor3f(1.0, 1.0, 1.0);
	glScalef(0.1, 0.1, 0.1);
	glRotatef(elephantrot, 0, 1, 0);
	glCallList(elephant);
	glPopMatrix();
	//elephantrot = elephantrot + 0.6;
	//if (elephantrot>360)elephantrot = elephantrot - 360;
}

void loadTexture(GLuint texture, const char* filename)
{
	
	data = (unsigned char *)malloc(512 * 512 * 3);
	FILE* f;
	fopen_s(&f, filename, "rb");
	fread(data, 512 * 512 * 3, 1, f);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);

}

void stopMusic(int pid)
{
	Mix_HaltMusic();
	
}

void ParticelleImpazzite(int v)
{
	num_rand_x = (rand() % (int)50);
	num_rand_y = (rand() % (int)90);
	
	for (int i = 0; i < 100; i++)
	{	
		vector_x[i] = num_rand_x;
		vector_y[i] = num_rand_y;
	}
		
		if ((abs(num_rand_x - (int)x) <= 4 && abs(num_rand_y - (int)z) <= 4))
		{
			cout << "COLPITO" << endl;
			Mix_PlayMusic(music,-1);
			glFogfv(GL_FOG_COLOR, fogColor);
			glutTimerFunc(500, stopMusic, 222);
		}

		glutPostRedisplay();
		glutTimerFunc(90, ParticelleImpazzite, 123);
}

void spostaCubo(int pid )
{
	if (primo_spostamento)
	{
		x_cube = (rand() % (int)17 + 54);
		primo_spostamento = false;
	}

	if (num_rand_z <= 90)
		num_rand_z++;
	else
	{
		num_rand_z = 5.0;
		primo_spostamento = true;
	}

	if ((abs(x_cube- (int)x) <= 3 && abs(num_rand_z- (int)z) <= 3))
	{
		cout << "COLPITO CUBO" << endl;
		Mix_PlayMusic(music, -1);
		glFogfv(GL_FOG_COLOR, fogColor);
		if (z < 90 )
		z += 3;
		glutTimerFunc(600, stopMusic, 222);
	}

	glutPostRedisplay();
	glutTimerFunc(5, spostaCubo, 111);
}

void loadAudio()
{
	Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
	music = Mix_LoadMUS("male.wav");
	
	if (music == NULL)
		fprintf(stderr, "Unable to load WAV file: %s\n", Mix_GetError());
	
	//Verifico il corretto caricamento della musica
	if( music == NULL ) { return; }
	
}

void initLight()
{
	GLfloat mat_specular[] = { 0.3, 1.0, 0.3, 1.0 };
	GLfloat mat_shininess[] = { 30.0 };
	GLfloat light_position[] = { 10.0, 10.0, 5.0, 1.0 };
	GLfloat spotDir[] = { 3.0, 4.0, 3.0 };

	glClearColor(0.5, 0.5, 0.5, 0.0);
	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT0, GL_SPECULAR, mat_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	// Definig spotlight attributes
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 95.0);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);

}

void init()
{
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);


	glEnable(GL_LIGHT0);

	// Abilito il color tracking
	glEnable(GL_COLOR_MATERIAL);

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/* INIZIALIZZO LE TEXTURE */

	texture = new GLuint[5];
	glGenTextures(5, texture);

	loadTexture(texture[0], "oct4.tga");
	loadTexture(texture[1], "stone3_2.tga");
	loadTexture(texture[2], "sky.tga");
	loadTexture(texture[3], "hell.tga");
	loadTexture(texture[4], "blood.tga");

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);          // Faccio un Clear del colore della nebbia

	glFogi(GL_FOG_MODE, GL_EXP);        // Imposto la modalità di nebbia
	glFogfv(GL_FOG_COLOR, fogColor);            // Imposto il colore della nebbia
	glFogf(GL_FOG_DENSITY, 0.02f);              // Densità della nebbia
	glHint(GL_FOG_HINT, GL_DONT_CARE);         
	glFogf(GL_FOG_START, 1.0f);            
	glFogf(GL_FOG_END, 5.0f);               
	glEnable(GL_FOG);

	loadAudio();
	loadObj("Elephant/elephant.obj");

	initLight();

}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, (GLfloat)w / (GLfloat)h, 1.0, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int w, int h)
{
	
	if (key == 'd')
	{
		angle += 0.10f;
		lx = sin(angle);
		lz = -cos(angle);
	}
	if (key == 'w')
	{
		if ((x + lx * fraction < 93 && x + lx * fraction >= -3) && (z + lz * fraction >= -3 && z + lz * fraction <= 93))
		{
			x += lx * fraction;
			z += lz * fraction;		
		}
		cout << "Actual pos: " << x << " " << y << " " << z << " " << lx << " " << lz << endl;
	}

	if (key == 'o')
	{
		y -= 1.0;
		glDisable(GL_FOG);
	}
	if (key == 's')
	{
		if ((x - lx * fraction <= 93 && x - lx * fraction >= -4) && (z - lz * fraction > -3 && z - lz * fraction < 93))
		{
			x -= lx * fraction;
			z -= lz * fraction;
		}
	}
	if (key == 'a')
	{
		angle -= 0.10f;
		lx = sin(angle);
		lz = -cos(angle);
	}
	
	glutPostRedisplay();

}



void cube(int pid)
{
	if (prima_volta)
	{
		rand_c_x = (rand() % (int)47);
		rand_c_y = (rand() % (int)90);
		prima_volta = false;
	}
	if (height_cube >= 0)
		height_cube-=5;
	else
	{
		height_cube = 40.0;
		prima_volta = true;
	}

	if ((abs(rand_c_x - (int)x) <= 2 && abs(rand_c_y - (int)z) <= 2))
	{
		cout << "COLPITO CUBO" << endl;
		Mix_PlayMusic(music, -1);
		glFogfv(GL_FOG_COLOR, fogColor);
		glutTimerFunc(500, stopMusic, 222);
	}

	glutPostRedisplay();
	glutTimerFunc(50, cube, 223);
}



void disegnaStanza()
{
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	glPushMatrix();
	
	glRotatef(-90, 1.0, 0.0, 0.0);

	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	

	/* DISEGNO LE PARETI*/
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTranslatef(-5.0, -45.0, 50.0);
	glRotatef(-90, 0.0, 1.0, 0.0);

	glBegin(GL_QUADS);
	glColor4f(1.0, 1.0, 1.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); /* lower left corner of image */
	glVertex3f(-50.0f, -50.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); /* lower right corner of image */
	glVertex3f(50.0f, -50.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); /* upper right corner of image */
	glVertex3f(50.0f, 50.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); /* upper left corner of image */
	glVertex3f(-50.0f, 50.0f, 0.0f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTranslatef(75.0, -55.0, 50.0);
	glRotatef(-90, 0.0, 1.0, 0.0);

	glBegin(GL_QUADS);
	glColor4f(1.0, 1.0, 1.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); /* lower left corner of image */
	glVertex3f(-50.0f, -50.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); /* lower right corner of image */
	glVertex3f(50.0f, -50.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); /* upper right corner of image */
	glVertex3f(50.0f, 50.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); /* upper left corner of image */
	glVertex3f(-50.0f, 50.0f, 0.0f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTranslatef(45.0, -95.0, 50.0);
	glRotatef(90, 1.0, 0.0, 0.0);

	glBegin(GL_QUADS);
	glColor4f(1.0, 1.0, 1.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); /* lower left corner of image */
	glVertex3f(-50.0f, -50.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); /* lower right corner of image */
	glVertex3f(50.0f, -50.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); /* upper right corner of image */
	glVertex3f(50.0f, 50.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); /* upper left corner of image */
	glVertex3f(-50.0f, 50.0f, 0.0f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTranslatef(50.0, -35.0, 50.0);
	glRotatef(90, 0.0, 1.0, 0.0);

	glBegin(GL_QUADS);
	glColor4f(1.0, 1.0, 1.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); /* lower left corner of image */
	glVertex3f(-50.0f, -50.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); /* lower right corner of image */
	glVertex3f(50.0f, -50.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); /* upper right corner of image */
	glVertex3f(50.0f, 50.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); /* upper left corner of image */
	glVertex3f(-50.0f, 50.0f, 0.0f);
	glEnd();
	glPopMatrix();



	glPushMatrix();
	
	glTranslatef(95.0, -45.0, 50.0);
	glRotatef(90, 0.0, 1.0, 0.0);

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);
	glColor4f(1.0, 1.0, 1.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); /* lower left corner of image */
	glVertex3f(-50.0f, -50.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); /* lower right corner of image */
	glVertex3f(50.0f, -50.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); /* upper right corner of image */
	glVertex3f(50.0f, 50.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); /* upper left corner of image */
	glVertex3f(-50.0f, 50.0f, 0.0f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTranslatef(45.0, 5.0, 50.0);
	glRotatef(90, 1.0, 0.0, 0.0);


	glBegin(GL_QUADS);
	glColor4f(1.0, 1.0, 1.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); /* lower left corner of image */
	glVertex3f(-50.0f, -50.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); /* lower right corner of image */
	glVertex3f(50.0f, -50.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); /* upper right corner of image */
	glVertex3f(50.0f, 50.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); /* upper left corner of image */
	glVertex3f(-50.0f, 50.0f, 0.0f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTranslatef(-25.0, -10.0, 50.0);
	glRotatef(90, 1.0, 0.0, 0.0);

	glBegin(GL_QUADS);
	glColor4f(1.0, 1.0, 1.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); /* lower left corner of image */
	glVertex3f(-50.0f, -50.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); /* lower right corner of image */
	glVertex3f(50.0f, -50.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); /* upper right corner of image */
	glVertex3f(50.0f, 50.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); /* upper left corner of image */
	glVertex3f(-50.0f, 50.0f, 0.0f);
	glEnd();
	glPopMatrix();

	/* DISEGNO IL PAVIMENTO */

	glPushMatrix();
	glTranslatef(49.9, -7, 12);
	glRotatef(-90, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_QUADS);
	glColor4f(1.0, 1.0, 1.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); /* lower left corner of image */
	glVertex3f(-10.0f, -10.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); /* lower right corner of image */
	glVertex3f(10.0f, -10.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); /* upper right corner of image */
	glVertex3f(10.0f, 10.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); /* upper left corner of image */
	glVertex3f(-10.0f, 10.0f, 0.0f);
	glEnd();
	glPopMatrix();
	

	glPushMatrix();
	glTranslatef(49.9, -50, 12);
	glRotatef(-90, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glBegin(GL_QUADS);
	glColor4f(1.0, 1.0, 1.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); /* lower left corner of image */
	glVertex3f(-30.0f, -10.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); /* lower right corner of image */
	glVertex3f(10.0f, -10.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); /* upper right corner of image */
	glVertex3f(10.0f, 10.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); /* upper left corner of image */
	glVertex3f(-30.0f, 10.0f, 0.0f);
	glEnd();
	glPopMatrix();

	for (int i = 0; i < 10; i++)
	{
		glPushMatrix();
		for (int j = 0; j < 10; j++)
		{
			if (j < 8)
			glBindTexture(GL_TEXTURE_2D, texture[0]);
			else
			glBindTexture(GL_TEXTURE_2D, texture[2]);

			glBegin(GL_QUADS);
			glColor4f(1.0, 1.0, 1.0,0.0);
			glTexCoord2f(0.0f, 0.0f); /* lower left corner of image */
			glVertex3f(-5.0f, -5.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); /* lower right corner of image */
			glVertex3f(5.0f, -5.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); /* upper right corner of image */
			glVertex3f(5.0f, 5.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); /* upper left corner of image */
			glVertex3f(-5.0f, 5.0f, 0.0f);
			glEnd();
			glTranslatef(10.0, 0.0, 0.0);

		}
		glPopMatrix();
		glTranslatef(0.0, -10.0, 0.0);
	}

	glDisable(GL_TEXTURE_2D);

	glEnable(GL_LIGHTING);

	glPushMatrix();
	glColor3f(0.0, 0.0, 1.0);
	glTranslatef(num_rand_x, num_rand_y,2.0);
	glutSolidSphere(1.0, 20, 30);
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glTranslatef(rand_c_x, rand_c_y, height_cube);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);
	
	
	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); /* lower left corner of image */
	glVertex3f(2.0f, 2.0f, -2.0f);    // Top Right Of The Quad (Top)
	glTexCoord2f(1.0f, 0.0f); /* lower right corner of image */
	glVertex3f(-2.0f, 2.0f, -2.0f);    // Top Left Of The Quad (Top)
	glTexCoord2f(1.0f, 1.0f); /* upper right corner of image */
	glVertex3f(-2.0f, 2.0f, 2.0f);    // Bottom Left Of The Quad (Top)
	glTexCoord2f(0.0f, 1.0f); /* upper left corner of image */
	glVertex3f(2.0f, 2.0f, 2.0f);    // Bottom Right Of The Quad (Top)

	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); /* lower left corner of image */
	glVertex3f(2.0f, -2.0f, 2.0f);    // Top Right Of The Quad (Bottom)
	glTexCoord2f(1.0f, 0.0f); /* lower right corner of image */
	glVertex3f(-2.0f, -2.0f, 2.0f);    // Top Left Of The Quad (Bottom)
	glTexCoord2f(1.0f, 1.0f); /* upper right corner of image */
	glVertex3f(-2.0f, -2.0f, -2.0f);    // Bottom Left Of The Quad (Bottom)
	glTexCoord2f(0.0f, 1.0f); /* upper left corner of image */
	glVertex3f(2.0f, -2.0f, -2.0f);    // Bottom Right Of The Quad (Bottom)
 
	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); /* lower left corner of image */
	glVertex3f(2.0f, 2.0f, 2.0f);    // Top Right Of The Quad (Front)
	glTexCoord2f(1.0f, 0.0f); /* lower right corner of image */
	glVertex3f(-2.0f, 2.0f, 2.0f);    // Top Left Of The Quad (Front)
	glTexCoord2f(1.0f, 1.0f); /* upper right corner of image */
	glVertex3f(-2.0f, -2.0f, 2.0f);    // Bottom Left Of The Quad (Front)
	glTexCoord2f(0.0f, 1.0f); /* upper left corner of image */
	glVertex3f(2.0f, -2.0f, 2.0f);    // Bottom Right Of The Quad (Front)

	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); /* lower left corner of image */
	glVertex3f(2.0f, -2.0f, -2.0f);    // Top Right Of The Quad (Back)
	glTexCoord2f(1.0f, 0.0f); /* lower right corner of image */
	glVertex3f(-2.0f, -2.0f, -2.0f);    // Top Left Of The Quad (Back)
	glTexCoord2f(1.0f, 1.0f); /* upper right corner of image */
	glVertex3f(-2.0f, 2.0f, -2.0f);    // Bottom Left Of The Quad (Back)
	glTexCoord2f(0.0f, 1.0f); /* upper left corner of image */
	glVertex3f(2.0f, 2.0f, -2.0f);    // Bottom Right Of The Quad (Back)

	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); /* lower left corner of image */
	glVertex3f(-2.0f, 2.0f, 2.0f);    // Top Right Of The Quad (Left)
	glTexCoord2f(1.0f, 0.0f); /* lower right corner of image */
	glVertex3f(-2.0f, 2.0f, -2.0f);    // Top Left Of The Quad (Left)
	glTexCoord2f(1.0f, 1.0f); /* upper right corner of image */
	glVertex3f(-2.0f, -2.0f, -2.0f);    // Bottom Left Of The Quad (Left)
	glTexCoord2f(0.0f, 1.0f); /* upper left corner of image */
	glVertex3f(-2.0f, -2.0f, 2.0f);    // Bottom Right Of The Quad (Left)

	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); /* lower left corner of image */
	glVertex3f(2.0f, 2.0f, -2.0f);    // Top Right Of The Quad (Right)
	glTexCoord2f(1.0f, 0.0f); /* lower right corner of image */
	glVertex3f(2.0f, 2.0f, 2.0f);    // Top Left Of The Quad (Right)
	glTexCoord2f(1.0f, 1.0f); /* upper right corner of image */
	glVertex3f(2.0f, -2.0f, 2.0f);    // Bottom Left Of The Quad (Right)
	glTexCoord2f(0.0f, 1.0f); /* upper left corner of image */
	glVertex3f(2.0f, -2.0f, -2.0f);    // Bottom Right Of The Quad (Right)
	glEnd();            // End Drawing The Cube 

	
	glPopMatrix();
	glPopMatrix();


	/* STANZA 2 - CUBO IN MOVIMENTO */

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glTranslatef(x_cube, 2.0, num_rand_z);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	
	glBegin(GL_QUADS);
	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); /* lower left corner of image */
	glVertex3f(2.0f, 2.0f, -2.0f);    // Top Right Of The Quad (Top)
	glTexCoord2f(1.0f, 0.0f); /* lower right corner of image */
	glVertex3f(-2.0f, 2.0f, -2.0f);    // Top Left Of The Quad (Top)
	glTexCoord2f(1.0f, 1.0f); /* upper right corner of image */
	glVertex3f(-2.0f, 2.0f, 2.0f);    // Bottom Left Of The Quad (Top)
	glTexCoord2f(0.0f, 1.0f); /* upper left corner of image */
	glVertex3f(2.0f, 2.0f, 2.0f);    // Bottom Right Of The Quad (Top)

	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); /* lower left corner of image */
	glVertex3f(2.0f, -2.0f, 2.0f);    // Top Right Of The Quad (Bottom)
	glTexCoord2f(1.0f, 0.0f); /* lower right corner of image */
	glVertex3f(-2.0f, -2.0f, 2.0f);    // Top Left Of The Quad (Bottom)
	glTexCoord2f(1.0f, 1.0f); /* upper right corner of image */
	glVertex3f(-2.0f, -2.0f, -2.0f);    // Bottom Left Of The Quad (Bottom)
	glTexCoord2f(0.0f, 1.0f); /* upper left corner of image */
	glVertex3f(2.0f, -2.0f, -2.0f);    // Bottom Right Of The Quad (Bottom)

	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); /* lower left corner of image */
	glVertex3f(2.0f, 2.0f, 2.0f);    // Top Right Of The Quad (Front)
	glTexCoord2f(1.0f, 0.0f); /* lower right corner of image */
	glVertex3f(-2.0f, 2.0f, 2.0f);    // Top Left Of The Quad (Front)
	glTexCoord2f(1.0f, 1.0f); /* upper right corner of image */
	glVertex3f(-2.0f, -2.0f, 2.0f);    // Bottom Left Of The Quad (Front)
	glTexCoord2f(0.0f, 1.0f); /* upper left corner of image */
	glVertex3f(2.0f, -2.0f, 2.0f);    // Bottom Right Of The Quad (Front)

	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); /* lower left corner of image */
	glVertex3f(2.0f, -2.0f, -2.0f);    // Top Right Of The Quad (Back)
	glTexCoord2f(1.0f, 0.0f); /* lower right corner of image */
	glVertex3f(-2.0f, -2.0f, -2.0f);    // Top Left Of The Quad (Back)
	glTexCoord2f(1.0f, 1.0f); /* upper right corner of image */
	glVertex3f(-2.0f, 2.0f, -2.0f);    // Bottom Left Of The Quad (Back)
	glTexCoord2f(0.0f, 1.0f); /* upper left corner of image */
	glVertex3f(2.0f, 2.0f, -2.0f);    // Bottom Right Of The Quad (Back)

	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); /* lower left corner of image */
	glVertex3f(-2.0f, 2.0f, 2.0f);    // Top Right Of The Quad (Left)
	glTexCoord2f(1.0f, 0.0f); /* lower right corner of image */
	glVertex3f(-2.0f, 2.0f, -2.0f);    // Top Left Of The Quad (Left)
	glTexCoord2f(1.0f, 1.0f); /* upper right corner of image */
	glVertex3f(-2.0f, -2.0f, -2.0f);    // Bottom Left Of The Quad (Left)
	glTexCoord2f(0.0f, 1.0f); /* upper left corner of image */
	glVertex3f(-2.0f, -2.0f, 2.0f);    // Bottom Right Of The Quad (Left)

	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); /* lower left corner of image */
	glVertex3f(2.0f, 2.0f, -2.0f);    // Top Right Of The Quad (Right)
	glTexCoord2f(1.0f, 0.0f); /* lower right corner of image */
	glVertex3f(2.0f, 2.0f, 2.0f);    // Top Left Of The Quad (Right)
	glTexCoord2f(1.0f, 1.0f); /* upper right corner of image */
	glVertex3f(2.0f, -2.0f, 2.0f);    // Bottom Left Of The Quad (Right)
	glTexCoord2f(0.0f, 1.0f); /* upper left corner of image */
	glVertex3f(2.0f, -2.0f, -2.0f);    // Bottom Right Of The Quad (Right)
	glEnd();            // End Drawing The Cube 
	glPopMatrix();

	glPopMatrix();

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);	
	glClear(GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glTranslatef(0.0, y, 0.0);
	gluLookAt(x, 1.0, z,
		x + lx, 1.0f, z + lz,
		0.0f, 1.0f, 0.0f);

	disegnaStanza();
	
	if (x >= 50 && x <= 76)
	{
		glFogfv(GL_FOG_COLOR, fogColor2);
		stanza_1 = false;
	}
	else if (x <= 50)
	{
		glFogfv(GL_FOG_COLOR, fogColor2);
		stanza_1 = true;
	}
	if (x >= 76)
	{
		glFogfv(GL_FOG_COLOR, fogColor3);
		stanza_1 = false;
	}

	drawElephant();
	glutSwapBuffers();
	
}

void  gamepad(unsigned int buttonMask, int x, int y, int z){

	if (buttonMask & GLUT_JOYSTICK_BUTTON_A) {
		printf("button A is pressed ");
		
	}
	if (buttonMask & GLUT_JOYSTICK_BUTTON_B) {
		printf("button B is pressed ");
	}
	if (buttonMask & GLUT_JOYSTICK_BUTTON_C) {
		printf("button C is pressed ");
	}
	if (buttonMask & GLUT_JOYSTICK_BUTTON_D) {
		printf("button D is pressed ");
	}

	if ( x > 0)
	{
		angle += 0.05f;
		lx = sin(angle);
		lz = -cos(angle);
	}

	if (x < 0 && x != -7)
	{
		angle -= 0.05f;
		lx = sin(angle);
		lz = -cos(angle);
	}

	if (y > 0)
	{
		keyboard('s', GLUT_WINDOW_WIDTH, GLUT_WINDOW_HEIGHT);

	}
	else if (y < 0 && y != -7)
	{
		keyboard('w', GLUT_WINDOW_WIDTH, GLUT_WINDOW_HEIGHT);
	}

	glutPostRedisplay();
	
}

int main(int argc, char *argv[])
{
	srand(time(0));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Floor");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutJoystickFunc(gamepad, 15);
	glutTimerFunc(20, ParticelleImpazzite, 123);
	glutTimerFunc(20, cube, 223);
	glutTimerFunc(20, spostaCubo, 111);
	glutMainLoop();
	
	return 0;
}