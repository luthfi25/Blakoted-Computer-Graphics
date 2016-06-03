//JANCUK

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <GL/glut.h>
#include "glm.h"

#define BASESIZE 100.0
#define BASERES 12
#ifndef M_PI
#define M_PI 3.1415926535
#endif

#define MAINCAM 0
#define OBJ1CAM 1
#define OBJ2CAM 2
#define OBJ3CAM 3

#define PACMAN 0
#define SQUMAN 1
#define TRIMAN 2

//#pragma comment( linker, "/entry:\"mainCRTStartup\"" )  // set the entry point to be main()
//window
static int WIDTH = 800;
static int HEIGHT = 600;

//light
static GLfloat lightpos[4][4] = {
	{ 2.3, 0.0, 3.0, 1.0 },
	{ 2.3, 0.0, 3.0, 1.0 },
	{ 2.3, 0.0, 3.0, 1.0 },
	{ 2.3, 0.0, 3.0, 1.0 } };

static GLfloat lightdir[4][3] = {
	{ -2.3, 0.0, -3.0 },
	{ -2.3, 0.0, -3.0 },
	{ -2.3, 0.0, -3.0 },
	{ -2.3, 0.0, -3.0 } };

//object def
static GLuint basedlist;
GLMmodel* pachead = NULL;
GLMmodel* pacbtm = NULL;
GLMmodel* squhead = NULL;
GLMmodel* squbtm = NULL;

//object pos
static GLfloat obj1pos[3], obj2pos[3], obj3pos[3];
static GLfloat thetaobj1[2] = { 0.0, 0.0 };
static GLfloat thetaobj2 = 80;
static int obj1dir = 1;

//perspective view
static float obs[3] = { 10.0, 0.0, 2.0 };
static float dir[3];
static float alpha = -90.0;
static float beta = 90.0;
static float v = 0.0;

//global
bool isAnimated = false;
bool isWireView = false;

GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_10;

///INIT DRAW OBJECT
void pacman(void)
{
	if (!pachead) {
		if (isWireView) pachead = glmReadOBJ("data/wire_round_top.obj"); else pachead = glmReadOBJ("data/round_top.obj");
		if (!pachead) exit(0);
		glmUnitize(pachead);
		glmFacetNormals(pachead);
		glmVertexNormals(pachead, 90.0);
	}
	if (!pacbtm) {
		if (isWireView) pacbtm = glmReadOBJ("data/wire_round_btm.obj"); else pacbtm = glmReadOBJ("data/round_btm.obj");
		if (!pacbtm) exit(0);
		glmUnitize(pacbtm);
		glmFacetNormals(pacbtm);
		glmVertexNormals(pacbtm, 90.0);
	}
	glPushMatrix();
	glTranslatef(obj1pos[0], obj1pos[1], obj1pos[2]);
	glRotatef(thetaobj1[1], 0.0, 0.0, 1.0);
	glRotatef(thetaobj1[0], 1.0, 0.0, 0.0);
	glmDraw(pachead, GLM_SMOOTH | GLM_MATERIAL);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.5);
	glRotatef(-thetaobj1[0], 1.0, 0.0, 0.0);
	glmDraw(pacbtm, GLM_SMOOTH | GLM_MATERIAL);
	glPopMatrix();
}

void squman(void)
{

	if (!squhead) {
		if (isWireView) squhead = glmReadOBJ("data/wire_square_top.obj"); else squhead = glmReadOBJ("data/square_top.obj");
		if (!squhead) exit(0);
		glmUnitize(squhead);
		glmFacetNormals(squhead);
		glmVertexNormals(squhead, 90.0);
	}
	if (!squbtm) {
		if (isWireView) squbtm = glmReadOBJ("data/wire_square_btm.obj"); else squbtm = glmReadOBJ("data/square_btm.obj");
		if (!squbtm) exit(0);
		glmUnitize(squbtm);
		glmFacetNormals(squbtm);
		glmVertexNormals(squbtm, 90.0);
	}
	glPushMatrix();
	glTranslatef(obj2pos[0], obj2pos[1], obj2pos[2]);
	glRotatef(thetaobj2, 0.0, 0.0, 1.0);

	glmDraw(squhead, GLM_SMOOTH  | GLM_MATERIAL);
	glTranslatef(0.0, -0.2, -0.6);
	glRotatef(180, 0.0, 0.0, 1.0);

	glmDraw(squbtm, GLM_SMOOTH | GLM_MATERIAL);
	glPopMatrix();
}

static void drawbase(void)
{
	static const GLfloat amb[4] = { 1, .5, 0.2, 1 };
	static const GLfloat diff[4] = { 1, .4, 0.2, 1 };
	int i, j;
	float x, y, dx, dy;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
	dx = BASESIZE / BASERES;
	dy = -BASESIZE / BASERES;
	for (y = BASESIZE / 2.0, j = 0; j<BASERES; y += dy, j++) {
		glBegin(GL_QUAD_STRIP);
		glColor3f(1.0, 1.0, 1.0);
		glNormal3f(0.0, 0.0, 1.0);
		for (x = -BASESIZE / 2.0, i = 0; i<BASERES; x += dx, i++) {
			glVertex3f(x, y, 0.0);
			glVertex3f(x, y + dy, 0.0);
		}
		glEnd();
	}
}

static void initdlist(void){
	basedlist = glGenLists(1);
	glNewList(basedlist, GL_COMPILE);
	drawbase();
	glEndList();
}

///LIGHT
static void initlight(void)
{
	float matamb[4] = { 0.5, 0.5, 0.5, 1.0 };
	float matdiff[4] = { 0.9, 0.2, 0.2, 1.0 };
	float matspec[4] = { 1.0, 1.0, 1.0, 1.0 };

	float lamb[4] = { 1.5, 1.5, 1.5, 1.0 };
	float ldiff[4] = { 1.0, 1.0, 1.0, 1.0 };
	float lspec[4] = { 1.0, 1.0, 1.0, 1.0 };

	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 70.0);
	//glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 20.0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lamb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ldiff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lspec);

	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 15.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matdiff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matspec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matamb);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lamb);
	glEnable(GL_LIGHT0);
}

static void drawlight(int i)
{
	glPushMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos[i]);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightdir[i]);
	glPopMatrix();
}


///DISPLAY PROCESS

static void animation(void)
{
	//Pacman animation
	//change
	if (thetaobj1[0] > 50.0 || thetaobj1[0] < 0.0) obj1dir = obj1dir * -1;
	//anim
	thetaobj1[0] += 0.3 * obj1dir;

}


static void calcposobs(void) //calculate viewer position
{
	dir[0] = sin(alpha*M_PI / 180.0);
	dir[1] = cos(alpha*M_PI / 180.0)*sin(beta*M_PI / 180.0);
	dir[2] = cos(beta*M_PI / 180.0);

	obs[0] += v*dir[0];
	obs[1] += v*dir[1];
	obs[2] += v*dir[2];
}

void screen_reshape(int w, int h)
{
	WIDTH = w;
	HEIGHT = h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, w / (float)h, 0.2, 40.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, w, h);
}

void screen_display(void)
{
	if (isAnimated) animation();
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);

	glPushMatrix();
	calcposobs();
	gluLookAt(obs[0], obs[1], obs[2], obs[0] + dir[0], obs[1] + dir[1], obs[2] + dir[2], 0.0, 0.0, 1.0);

	drawlight(1);
	glCallList(basedlist);

	glPushMatrix();
	glTranslatef(obj1pos[0], obj1pos[1], obj1pos[2]);
	pacman();
	glPopMatrix();

	glTranslatef(2.0, 0.0, 0.0);

	glPushMatrix();
	glTranslatef(obj2pos[0], obj2pos[1], obj2pos[2]);
	pacman();
	glPopMatrix();

	glPopMatrix();

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-0.5, 639.5, -0.5, 479.5, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	screen_reshape(WIDTH, HEIGHT);

	glFlush();
	glutSwapBuffers();
}

//INPUT INTERRUPT
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case ' ':
		isAnimated = !isAnimated;
		break;
	}

}

///INITIALIZATION
void special1(int key, int x, int y)
{
}

void init(void)
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	initlight();
	initdlist();
	glClearColor(0.25, 0.25, 0.25, 1.0);

	obj1pos[0] = 0.0;
	obj1pos[1] = 0.0;
	obj1pos[2] = 1.5;

	obj2pos[0] = 0.0;
	obj2pos[1] = -2.0;
	obj2pos[2] = 1.5;

	obj3pos[0] = 0.0;
	obj3pos[1] = 2.0;
	obj3pos[2] = 2.0;
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

	glutInitWindowPosition(50, 50);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("KEREN COY");

	init();

	glutReshapeFunc(screen_reshape);
	glutDisplayFunc(screen_display);

	glutSpecialFunc(special1);
	glutKeyboardFunc(keyboard);

	glutIdleFunc(screen_display);

	glutMainLoop();
	return 0;
}
