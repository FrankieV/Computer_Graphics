#include "stdafx.h"
#include <cmath>
#include "include\glew.h"
#include "GL/freeglut.h"
#include <iostream>
#include <ctime>
#include "tga.h"
#include <vector>
#include "SDL.h"
#include "SDL_audio.h"
#include "SDL_timer.h"
#include "include\SDL_mixer.h"
#include "include\SDL_ttf.h"
#include "SDL_video.h"
#include <sstream>

using namespace std;

void loadAudio(string);
void Spawn();
void VerificaHealth();
void spostaPavimento(int);
void initText(string,float,float );

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
bool fullscreen = false;
int health_status = 100;
bool button_d = false;
bool sprofondato = false;

Mix_Music *music = NULL; /* SDL_Mixer*/
int num_rand_y = 0.0; int num_rand_x = 0.0;
int num_rand_z = 3.0, x_cube = 55.0;
float y = -2.0f;

GLuint elephant;
float dragonhantrot;
char ch = '1';

GLfloat fogColor[4] = { 1.0f, 0.1f, 0.0f, 0.0f };
GLfloat fogColor2[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; 
GLfloat fogColor3[4] = { 0.0f, 0.3f, 0.4f, 0.f };
int vector_x[100], vector_y[100];
GLfloat height_cube = 40.0;

int floor_x, floor_z;
int parete_x = 110;
int time_remaining = 0;

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

void drawDragon()
{
	
	glPushMatrix();
	glTranslatef(85.0, 2.0, 89.0);
	glRotatef(180,0.0, 1.0, 0.0);
	glColor3f(1.0, 0.0, 0.0);
	glScalef(0.03, 0.03, 0.03);
	glRotatef(dragonhantrot, 0, 1, 0);
	glCallList(elephant);
	glPopMatrix();
	dragonhantrot = dragonhantrot + 0.6;
	if (dragonhantrot>360)dragonhantrot = dragonhantrot - 360;
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
	num_rand_x = (rand() % (int)50+7);
	num_rand_y = (rand() % (int)90);

		
	if ((abs(num_rand_x - (int)x) <= 3 && abs(num_rand_y - (int)z) <= 3))
		{
			cout << "COLPITO" << endl;
			health_status -= 5;
			loadAudio("male.wav");
			Mix_PlayMusic(music,-1);
			glFogfv(GL_FOG_COLOR, fogColor);
			glutTimerFunc(500, stopMusic, 222);
		}

		VerificaHealth();

		glutPostRedisplay();
		glutTimerFunc(90, ParticelleImpazzite, 123);
}

void spostaParete(int pid)
{
	if (parete_x > 90)
	{
		parete_x -= 5;
		
	}
	else
	{
		parete_x = 115;
	}

	glutTimerFunc(300, spostaParete, 123);
	glutPostRedisplay();

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
		cout << "COLPITO CUBO" << health_status << endl;
		loadAudio("male.wav");
		Mix_PlayMusic(music, -1);
		health_status--;
		glFogfv(GL_FOG_COLOR, fogColor);
		if (z < 90 )
		z += 3;
		glutTimerFunc(1500, stopMusic, 222);
	}

	VerificaHealth();

	glutPostRedisplay();
	glutTimerFunc(5, spostaCubo, 111);
}

void spostaPavimento(int pid)
{
	floor_x = rand() % 14 + 78;
	floor_z = rand() % 90;

	if ((abs(floor_x - (int)x) <= 2 && abs(floor_z - (int)z) <= 2))
	{
		cout << "CADUTO NEL VUOTO";
		loadAudio("male.wav");
		Mix_PlayMusic(music, -1);
		health_status -= 20;
		if (health_status > 1)
		{
			y += 200;
			x += 200;
		}
		glFogfv(GL_FOG_COLOR, fogColor);
		glutTimerFunc(1500, stopMusic, 222);
		sprofondato = true;
	}

	VerificaHealth();

	glutTimerFunc(100, spostaPavimento, 333);
	glutPostRedisplay();
}

void tempo_rimanente(int pid)
{
	if (time_remaining < 60)
	{
		time_remaining++;
	}
	else
		Spawn();

	glutTimerFunc(1000, tempo_rimanente, 345);
}

void loadAudio(string audio)
{
	Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
	music = Mix_LoadMUS(audio.c_str());
	
	if (music == NULL)
		fprintf(stderr, "Non trovo il file WAV: %s\n", Mix_GetError());
	
	//Verifico il corretto caricamento della musica
	if( music == NULL ) { return; }

}

void Spawn()
{
	health_status = 100;
	x = 5.0;
	z = 2.0;
	music = Mix_LoadMUS("lose.wav");
	Mix_PlayMusic(music, -1);
	sprofondato = false;
	glutTimerFunc(2000, stopMusic, 2);
	time_remaining = 0;
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
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 95.0);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
}

void initText(char* mystring, float x, float y)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); // save
	glLoadIdentity();// and clear
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glRasterPos2f(x, y); // center of screen. (-1,0) is center left.
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	char buf[300];

	sprintf(buf, mystring, health_status);

	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)buf);
	
	glEnable(GL_DEPTH_TEST); // Turn depth testing back on

	glMatrixMode(GL_PROJECTION);
	glTranslatef(10.0f, 20.0f, 0.0f);
	glPopMatrix(); 
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glutPostRedisplay();
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

	texture = new GLuint[7];
	glGenTextures(7, texture);

	loadTexture(texture[0], "oct4.tga");
	loadTexture(texture[1], "stone3_2.tga");
	loadTexture(texture[2], "sky.tga");
	loadTexture(texture[3], "hell.tga");
	loadTexture(texture[4], "blood.tga");
	loadTexture(texture[5], "muro2.tga");
	loadTexture(texture[6], "blackfloor.tga");

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);          // Faccio un Clear del colore della nebbia

	glFogi(GL_FOG_MODE, GL_EXP);        // Imposto la modalità di nebbia
	glFogfv(GL_FOG_COLOR, fogColor);            // Imposto il colore della nebbia
	glFogf(GL_FOG_DENSITY, 0.01f);              // Densità della nebbia
	glHint(GL_FOG_HINT, GL_DONT_CARE);         
	glFogf(GL_FOG_START, 1.0f);            
	glFogf(GL_FOG_END, 5.0f);               
	glEnable(GL_FOG);

	loadAudio("male.wav");
	loadObj("Dragon.obj");

	initLight();
	glColor3f(0, 1, 0);

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

bool parete_1(char direzione)
{
	if (direzione == 'w')
	{
		if ((int)x == 48 || (int)x == 52)
		{
			if ((int)z < 87)
				return false;
		}

		return true;
	}
	else if (direzione == 's')
	{
		//if
	}
}

bool parete_2(char direzione)
{
	if (direzione == 'w')
	{
		if ((int)x == 73 || (int)x == 76)
		{
				if ((int)z > 1)
					return false;
		}

	return true;
    }
	else if (direzione == 's')
	{
		if ((int)x - (int)lx * fraction == 74)
			return false;
		 if ((int)x - (int)lx * fraction == 74 && (int) z <= 1)
			return true;
	}

	return true;
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
		if ((x + lx * fraction < 93 && x + lx * fraction >= -3) && (z + lz * fraction >= -3 && z + lz * fraction <= 93) && parete_1(key) && parete_2(key))
		{
			x += lx * fraction;
			z += lz * fraction;		
		}
		cout << "Actual pos: " << x << " " << y << " " << z << " " << lx << " " << lz << endl;
	}

	if (key == 'o')
	{
		if (!fullscreen)
		{
			glutFullScreen();
			fullscreen = true;
		}
		else
		{
			glutPositionWindow(0, 0);
			glutReshapeWindow(1200, 900);
			fullscreen = false;
		}
	}
	if (key == 's')
	{
		if ((x - lx * fraction <= 93 && x - lx * fraction >= -4) && (z - lz * fraction > -3 && z - lz * fraction < 93) && parete_1(key) && parete_2(key))
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
		health_status -= 5;
		loadAudio("male.wav");
		Mix_PlayMusic(music, -1);
		glFogfv(GL_FOG_COLOR, fogColor);
		glutTimerFunc(500, stopMusic, 222);
	}

	VerificaHealth();

	glutPostRedisplay();
	glutTimerFunc(20, cube, 223);
}

void VerificaHealth()
{
	if (health_status < 1)
	{
		glFogfv(GL_FOG_COLOR, fogColor);
		Spawn();
	}

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
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(-50.0f, -50.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(50.0f, -50.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(50.0f, 50.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); 
	glVertex3f(-50.0f, 50.0f, 0.0f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTranslatef(75.0, -55.0, 50.0);
	glRotatef(-90, 0.0, 1.0, 0.0);

	glBegin(GL_QUADS);
	glColor4f(1.0, 1.0, 1.0, 0.0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-50.0f, -50.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(50.0f, -50.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(50.0f, 50.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); 
	glVertex3f(-50.0f, 50.0f, 0.0f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTranslatef(45.0, -95.0, 50.0);
	glRotatef(90, 1.0, 0.0, 0.0);

	glBegin(GL_QUADS);
	glColor4f(1.0, 1.0, 1.0, 0.0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-50.0f, -50.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(50.0f, -50.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(50.0f, 50.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); 
	glVertex3f(-50.0f, 50.0f, 0.0f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTranslatef(50.0, -35.0, 50.0);
	glRotatef(90, 0.0, 1.0, 0.0);

	glBegin(GL_QUADS);
	glColor4f(1.0, 1.0, 1.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(-50.0f, -50.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(50.0f, -50.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(50.0f, 50.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); 
	glVertex3f(-50.0f, 50.0f, 0.0f);
	glEnd();
	glPopMatrix();



	glPushMatrix();
	
	glTranslatef(95.0, -45.0, 50.0);
	glRotatef(90, 0.0, 1.0, 0.0);

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);
	glColor4f(1.0, 1.0, 1.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(-50.0f, -50.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(50.0f, -50.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(50.0f, 50.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); 
	glVertex3f(-50.0f, 50.0f, 0.0f);
	glEnd();
	glPopMatrix();
	
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTranslatef(45.0, 5.0, 50.0);
	glRotatef(90, 1.0, 0.0, 0.0);

	glBegin(GL_QUADS);
	glColor4f(1.0, 1.0, 1.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(-50.0f, -50.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(50.0f, -50.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(50.0f, 50.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
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
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(-10.0f, -10.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(10.0f, -10.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(10.0f, 10.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); 
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
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(-30.0f, -10.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(10.0f, -10.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(10.0f, 10.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); 
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
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(-5.0f, -5.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f(5.0f, -5.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f(5.0f, 5.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); 
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
	glColor3f(0.0, 0.0, 1.0);
	glTranslatef(rand_c_x, rand_c_y, 2.0);
	glutSolidSphere(1.0, 20, 30);
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glTranslatef(rand_c_x, rand_c_y, height_cube);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);

	
	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(2.0f, 2.0f, -2.0f);    
	glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(-2.0f, 2.0f, -2.0f);    
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(-2.0f, 2.0f, 2.0f);    
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(2.0f, 2.0f, 2.0f);  

	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(2.0f, -2.0f, 2.0f);    
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, -2.0f, 2.0f);   
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.0f, -2.0f, -2.0f);    
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(2.0f, -2.0f, -2.0f);    

	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(2.0f, 2.0f, 2.0f);    
	glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(-2.0f, 2.0f, 2.0f);   
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(-2.0f, -2.0f, 2.0f);    
	glTexCoord2f(0.0f, 1.0f); 
	glVertex3f(2.0f, -2.0f, 2.0f);   

	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(2.0f, -2.0f, -2.0f);    
	glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(-2.0f, -2.0f, -2.0f);   
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(-2.0f, 2.0f, -2.0f);   
	glTexCoord2f(0.0f, 1.0f); 
	glVertex3f(2.0f, 2.0f, -2.0f);    

	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(-2.0f, 2.0f, 2.0f);   
	glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(-2.0f, 2.0f, -2.0f);    
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.0f, -2.0f, -2.0f);   
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.0f, -2.0f, 2.0f);    

	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(2.0f, 2.0f, -2.0f);  
	glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(2.0f, 2.0f, 2.0f);    
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(2.0f, -2.0f, 2.0f);  
	glTexCoord2f(0.0f, 1.0f); 
	glVertex3f(2.0f, -2.0f, -2.0f);    
	glEnd();           

	
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
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(2.0f, 2.0f, -2.0f);    
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, -2.0f);   
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.0f, 2.0f, 2.0f);    
	glTexCoord2f(0.0f, 1.0f); 
	glVertex3f(2.0f, 2.0f, 2.0f);    

	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(2.0f, -2.0f, 2.0f);    
	glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(-2.0f, -2.0f, 2.0f);    
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(-2.0f, -2.0f, -2.0f);   
	glTexCoord2f(0.0f, 1.0f); 
	glVertex3f(2.0f, -2.0f, -2.0f);    

	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(2.0f, 2.0f, 2.0f);    
	glTexCoord2f(1.0f, 0.0f); /*  */
	glVertex3f(-2.0f, 2.0f, 2.0f);   
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(-2.0f, -2.0f, 2.0f);    
	glTexCoord2f(0.0f, 1.0f); 
	glVertex3f(2.0f, -2.0f, 2.0f);    

	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(2.0f, -2.0f, -2.0f);    
	glTexCoord2f(1.0f, 0.0f); /*  */
	glVertex3f(-2.0f, -2.0f, -2.0f);    // Top Left Of The Quad (Back)
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(-2.0f, 2.0f, -2.0f);    // Bottom Left Of The Quad (Back)
	glTexCoord2f(0.0f, 1.0f); 
	glVertex3f(2.0f, 2.0f, -2.0f);    // Bottom Right Of The Quad (Back)

	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(-2.0f, 2.0f, 2.0f);    // Top Right Of The Quad (Left)
	glTexCoord2f(1.0f, 0.0f); /*  */
	glVertex3f(-2.0f, 2.0f, -2.0f);    // Top Left Of The Quad (Left)
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(-2.0f, -2.0f, -2.0f);    // Bottom Left Of The Quad (Left)
	glTexCoord2f(0.0f, 1.0f); 
	glVertex3f(-2.0f, -2.0f, 2.0f);    // Bottom Right Of The Quad (Left)

	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(2.0f, 2.0f, -2.0f);    // Top Right Of The Quad (Right)
	glTexCoord2f(1.0f, 0.0f); /*  */
	glVertex3f(2.0f, 2.0f, 2.0f);    // Top Left Of The Quad (Right)
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(2.0f, -2.0f, 2.0f);    // Bottom Left Of The Quad (Right)
	glTexCoord2f(0.0f, 1.0f); 
	glVertex3f(2.0f, -2.0f, -2.0f);    // Bottom Right Of The Quad (Right)
	glEnd();            // End Drawing The Cube 
	glPopMatrix();


	/* STANZA 3 - VUOTO + PARETI IN MOVIMENTO */
	glPushMatrix();
	glTranslatef(parete_x, 2.0, 50);

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, texture[5]);

	glBegin(GL_QUADS);
	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(10.0f, 10.0f, -10.0f);    
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-10.0f, 10.0f, -10.0f);   
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-10.0f, 10.0f, 10.0f);    
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(10.0f, 10.0f, 10.0f);  

	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(10.0f, -10.0f, 10.0f);    
	glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(-10.0f, -10.0f, 10.0f);    
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(-10.0f, -10.0f, -10.0f);   
	glTexCoord2f(0.0f, 1.0f); 
	glVertex3f(10.0f, -10.0f, -10.0f);    

	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(10.0f, 10.0f, 10.0f);    
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-10.0f, 10.0f, 10.0f);   
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(-10.0f, -10.0f, 10.0f);    
	glTexCoord2f(0.0f, 1.0f); 
	glVertex3f(10.0f, -10.0f, 10.0f);    

	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(10.0f, -10.0f, -10.0f);    
	glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(-10.0f, -10.0f, -10.0f);  
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(-10.0f, 10.0f, -10.0f);    
	glTexCoord2f(0.0f, 1.0f); 
	glVertex3f(10.0f, 10.0f, -10.0f);    

	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(-10.0f, 10.0f, 10.0f);    
	glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(-10.0f, 10.0f, -10.0f);    
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(-10.0f, -10.0f, -10.0f);   
	glTexCoord2f(0.0f, 1.0f); 
	glVertex3f(-10.0f, -10.0f, 10.0f);   

	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(10.0f, 10.0f, -10.0f);    
	glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(10.0f, 10.0f, 10.0f);    
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(10.0f, -10.0f, 10.0f);    
	glTexCoord2f(0.0f, 1.0f); 
	glVertex3f(10.0f, -10.0f, -10.0f);   
	glEnd();           

	glPopMatrix();

	glPushMatrix();
	glTranslatef(floor_x, 0.01, floor_z);
	glRotatef(90, 1.0, 0.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glBegin(GL_QUADS);
	glColor4f(1.0, 1.0, 1.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(-4.0f, -4.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(4.0f, -4.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(4.0f, 4.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, 4.0f, 0.0f);
	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE);
	glPopMatrix();

}


void printTime(int number, float x, float y)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); // save
	glLoadIdentity();// and clear
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	char* buf;
	buf = new char[100];

	sprintf(buf,"Time: %d", time_remaining);

	glRasterPos2f(x, y); // center of screen. (-1,0) is center left.
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(const unsigned char*)buf);

	glEnable(GL_DEPTH_TEST); 

	glMatrixMode(GL_PROJECTION);
	glTranslatef(10.0f, 20.0f, 0.0f);
	glPopMatrix(); 
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glutPostRedisplay();
}

void Win()
{
	loadAudio("win.wav");
	Mix_PlayMusic(music, -1);
	glutTimerFunc(1000, stopMusic, 222);
	health_status = 100;
	x = 5.0;
	z = 2.0;
	sprofondato = false;
	time_remaining = 0;

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);	
	glClear(GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glTranslatef(0.0, y, 0.0);
	gluLookAt(x,1.0, z,
		x + lx, 1.0f, z + lz,
		0.0f, 1.0f, 0.0f);

	disegnaStanza();
	glEnable(GL_FOG);
	
	if (x >= 50 && x <= 76)
	{
		glFogfv(GL_FOG_COLOR, fogColor2);
		stanza_1 = false;
	}
	else if (x < 49)
	{
		glFogfv(GL_FOG_COLOR, fogColor2);
		stanza_1 = true;
	}
	if (x >= 76)
	{
			glFogfv(GL_FOG_COLOR, fogColor3);
			stanza_1 = false;
	}

	drawDragon();

	initText("Health Status: %d", -0.9, 0.8);
	printTime(time_remaining, -0.9, 0.7);

	if (sprofondato)
	{
		initText("Premi O per risalire!",0,0);
	}

	if (x >= 85 && z >= 85)
	{
		Win();
	}

	glDisable(GL_TEXTURE);

	glutSwapBuffers();
	
}

void  gamepad(unsigned int buttonMask, int xg, int yg, int zg){

	if (buttonMask & GLUT_JOYSTICK_BUTTON_A) {
		printf("button A is pressed ");
		
	}
	if (buttonMask & GLUT_JOYSTICK_BUTTON_B) {
		if (sprofondato)
		{
			sprofondato = false;
			x = 85.0;
			y = -2.0;
			z = 0.0;
			glutPostRedisplay();
		}
	}
	if (buttonMask & GLUT_JOYSTICK_BUTTON_C) {
		printf("button C is pressed ");
	}

	if (buttonMask & GLUT_JOYSTICK_BUTTON_D) {
		if (!button_d)
			button_d = true;
	}

	if ( xg > 0)
	{
		angle += 0.05f;
		lx = sin(angle);
		lz = -cos(angle);
	}

	if (xg < 0 && xg != -7)
	{
		angle -= 0.05f;
		lx = sin(angle);
		lz = -cos(angle);
	}

	if (yg > 0)
	{
		keyboard('s', GLUT_WINDOW_WIDTH, GLUT_WINDOW_HEIGHT);

	}
	else if (yg < 0 && yg != -7)
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
	glutCreateWindow("IN HELL");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutJoystickFunc(gamepad, 20);
	glutTimerFunc(20, ParticelleImpazzite, 123);
	glutTimerFunc(20, cube, 223);
	glutTimerFunc(20, spostaCubo, 111);
	glutTimerFunc(20, spostaPavimento, 333);
	glutTimerFunc(20, spostaParete, 123);
	glutTimerFunc(0, tempo_rimanente, 345);
	glutMainLoop();
	
	return 0;
}