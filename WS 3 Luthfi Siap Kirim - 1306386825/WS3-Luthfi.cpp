/*
* WORKSHEET 3 GRAFIKA KOMPUTER
Muhammad Luthfi - 1306386825
Bersama Mgs. M. Rizqi Fadhlurrahman
*/

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <glut.h>

GLuint texture; //variable to hold texture pointer

static int WIDTH = 640;
static int HEIGHT = 480;

static GLint T0 = 0;

//Gold Material Value
static float matamb1[4] = { 0.24725, 0.1995, 0.0745, 1 };
static float matdiff1[4] = { 0.75164, 0.60648, 0.22648, 1 };
static float matspec1[4] = { 0.628281, 0.555802, 0.366065, 1 };

//Ruby Material Value
static float matamb2[4] = { 0.1745, 0.01175, 0.01175, 1 };
static float matdiff2[4] = { 0.61424, 0.04136, 0.04136, 1 };
static float matspec2[4] = { 0.727811, 0.626959, 0.626959, 1 };

//Emerald Material Value
static float matamb3[4] = { 0.0215, 0.1745, 0.0215, 1 };
static float matdiff3[4] = { 0.07568, 0.61424, 0.07568, 1 };
static float matspec3[4] = { 0.633, 0.727811, 0.633, 1 };

#define TORSO_LENGTH 1.25
#define TORSO_RADIUS 0.375
#define UPPER_ARM_LENGTH 0.75
#define LOWER_ARM_LENGTH 0.5
#define ARM_RADIUS 0.125
#define NULL 0

#define BASESIZE 10.0

#define BASERES 12
#define TEAPOTRES 3

#ifndef M_PI
#define M_PI 3.1415926535
#endif

static float obs[3] = { 5.0,0.0,1.0 };
static float dir[3];
static float v = 0.0;
static float alpha = -90.0;
static float beta = 90.0;

static GLfloat baseshadow[4][4];
static GLfloat baseshadow2[4][4];
static GLfloat lightpos[4] = { 2.3,0.0,0.75,1.0 };
static GLfloat lightdir[3] = { -2.3,0.0,0.0 };
static GLfloat lightpos2[4] = { 1.0,1.7,2.0 };
static GLfloat lightdir2[3] = { -2.3,0.0,-3.0 };
static GLfloat lightalpha = 0.0;
static GLfloat lightalpha2 = 0.0;

static GLuint teapotdlist, basedlist, lightdlist;
GLUquadricObj *t, *ua1, *la1, *ua2, *la2, *ua3, *la3, *ua4, *la4, *te;
static GLfloat theta[9] = { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 };

bool lightActive = true;
bool textureActive = false;
int mat = 2;

/*****************begin texture code*****************/

/* Load texture from RAW 256x256 image */
GLuint LoadTextureRAW(const char * filename, int wrap)
{
	int width, height;
	BYTE * data;
	FILE * file;

	// open texture data
	file = fopen(filename, "rb");
	if (file == NULL) { return 0; }

	// allocate buffer
	width = 256;
	height = 256;
	data = (unsigned char *)malloc(width * height * 3);

	// read texture data
	fread(data, width * height * 3, 1, file);
	fclose(file);

	// allocate a texture name
	glGenTextures(1, &texture);

	// select our current texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// select modulate to mix texture with color for shading
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// when texture area is small, bilinear filter the closest MIP map
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_NEAREST);
	// when texture area is large, bilinear filter the first MIP map
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// if wrap is true, the texture wraps over at the edges (repeat)
	//       ... false, the texture ends at the edges (clamp)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
		wrap ? GL_REPEAT : GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
		wrap ? GL_REPEAT : GL_CLAMP);

	// build our texture MIP maps
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width,
		height, GL_RGB, GL_UNSIGNED_BYTE, data);

	// free buffer
	free(data);

	return texture;

}

void FreeTexture(GLuint texture)
{
	glDeleteTextures(1, &texture);
}

void setTexture(const char * file)
{
	texture = LoadTextureRAW(file, TRUE);
	if (!texture) printf("Texture File not found !");

	glEnable(GL_TEXTURE_2D); //enable texture
	glBindTexture(GL_TEXTURE_2D, texture); //binding texture

										   //auto generation texture coordinates
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	/* setup texture parameters, see and try texture tutorial */
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

/********************end texture code************************/
/********************begin object code**********************/

typedef struct treenode
{
	GLfloat m[16];
	void(*f)();
	struct treenode *sibling;
	struct treenode *child;
}treenode;

treenode t_node, ua1_node, la1_node, ua2_node, la2_node, ua3_node, la3_node, ua4_node, la4_node;

//Deklarasi method-method untuk menggambar masing-masing bagian hirarki
void torso()
{
	if(textureActive)
		setTexture("burung.raw");

	glPushMatrix();
	glScalef(TORSO_RADIUS, TORSO_LENGTH, TORSO_RADIUS);
	gluSphere(t, 1.0, 10, 10);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void upper_arm_1()
{
	if (textureActive)
		setTexture("makara.raw");

	glPushMatrix();
	glRotatef(-45.0, 0.0, 1.0, 0.0);
	gluCylinder(ua1, ARM_RADIUS, ARM_RADIUS, UPPER_ARM_LENGTH, 10, 10);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void lower_arm_1()
{
	if (textureActive)
		setTexture("makara.raw");

	glPushMatrix();
	glRotatef(-135.0, 0.0, 1.0, 0.0);
	gluCylinder(la1, ARM_RADIUS, ARM_RADIUS, LOWER_ARM_LENGTH, 10, 10);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void upper_arm_2()
{
	if (textureActive)
		setTexture("makara.raw");

	glPushMatrix();
	glRotatef(45.0, 0.0, 1.0, 0.0);
	gluCylinder(ua2, ARM_RADIUS, ARM_RADIUS, UPPER_ARM_LENGTH, 10, 10);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void lower_arm_2()
{
	if (textureActive)
		setTexture("makara.raw");

	glPushMatrix();
	glRotatef(135.0, 0.0, 1.0, 0.0);
	gluCylinder(la2, ARM_RADIUS, ARM_RADIUS, LOWER_ARM_LENGTH, 10, 10);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void upper_arm_3()
{
	if (textureActive)
		setTexture("texture.raw");

	glPushMatrix();
	glRotatef(-45.0, 0.0, 1.0, 0.0);
	gluCylinder(ua3, ARM_RADIUS, ARM_RADIUS, UPPER_ARM_LENGTH, 10, 10);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void lower_arm_3()
{
	if (textureActive)
		setTexture("texture.raw");

	glPushMatrix();
	glRotatef(-135.0, 0.0, 1.0, 0.0);
	gluCylinder(la3, ARM_RADIUS, ARM_RADIUS, LOWER_ARM_LENGTH, 10, 10);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void upper_arm_4()
{
	if (textureActive)
		setTexture("texture.raw");

	glPushMatrix();
	glRotatef(45.0, 0.0, 1.0, 0.0);
	gluCylinder(ua4, ARM_RADIUS, ARM_RADIUS, UPPER_ARM_LENGTH, 10, 10);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void lower_arm_4()
{
	if (textureActive)
		setTexture("texture.raw");

	glPushMatrix();
	glRotatef(135.0, 0.0, 1.0, 0.0);
	gluCylinder(la4, ARM_RADIUS, ARM_RADIUS, LOWER_ARM_LENGTH, 10, 10);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

//Method traverse tree
void traverse(treenode *root)
{
	if (root == 0) return;
	glPushMatrix();
	glMultMatrixf(root->m);
	root->f();
	if (root->child != 0) traverse(root->child);
	glPopMatrix();
	if (root->sibling != 0) traverse(root->sibling);
}

/*********************end object code***********************/

/******************** begin shadow code ********************/

/* Taken from the projshadow.c - by Tom McReynolds, SGI */
/* modified by Dadan Hardianto, Fasilkom UI */


enum {
	X, Y, Z, W
};
enum {
	A, B, C, D
};

/* create a matrix that will project the desired shadow */
void shadowmatrix(GLfloat shadowMat[4][4],
	GLfloat groundplane[4], GLfloat lightpos[4])
{
	GLfloat dot;

	/* find dot product between light position vector and ground plane normal */
	dot = groundplane[X] * lightpos[X] +
		groundplane[Y] * lightpos[Y] +
		groundplane[Z] * lightpos[Z] +
		groundplane[W] * lightpos[W];

	shadowMat[0][0] = dot - lightpos[X] * groundplane[X];
	shadowMat[1][0] = 0.f - lightpos[X] * groundplane[Y];
	shadowMat[2][0] = 0.f - lightpos[X] * groundplane[Z];
	shadowMat[3][0] = 0.f - lightpos[X] * groundplane[W];

	shadowMat[X][1] = 0.f - lightpos[Y] * groundplane[X];
	shadowMat[1][1] = dot - lightpos[Y] * groundplane[Y];
	shadowMat[2][1] = 0.f - lightpos[Y] * groundplane[Z];
	shadowMat[3][1] = 0.f - lightpos[Y] * groundplane[W];

	shadowMat[X][2] = 0.f - lightpos[Z] * groundplane[X];
	shadowMat[1][2] = 0.f - lightpos[Z] * groundplane[Y];
	shadowMat[2][2] = dot - lightpos[Z] * groundplane[Z];
	shadowMat[3][2] = 0.f - lightpos[Z] * groundplane[W];

	shadowMat[X][3] = 0.f - lightpos[W] * groundplane[X];
	shadowMat[1][3] = 0.f - lightpos[W] * groundplane[Y];
	shadowMat[2][3] = 0.f - lightpos[W] * groundplane[Z];
	shadowMat[3][3] = dot - lightpos[W] * groundplane[W];

}

/* find the plane equation given 3 points */
void findplane(GLfloat plane[4],
	GLfloat v0[3], GLfloat v1[3], GLfloat v2[3])
{
	GLfloat vec0[3], vec1[3];

	/* need 2 vectors to find cross product */
	vec0[X] = v1[X] - v0[X];
	vec0[Y] = v1[Y] - v0[Y];
	vec0[Z] = v1[Z] - v0[Z];

	vec1[X] = v2[X] - v0[X];
	vec1[Y] = v2[Y] - v0[Y];
	vec1[Z] = v2[Z] - v0[Z];

	/* find cross product to get A, B, and C of plane equation */
	plane[A] = vec0[Y] * vec1[Z] - vec0[Z] * vec1[Y];
	plane[B] = -(vec0[X] * vec1[Z] - vec0[Z] * vec1[X]);
	plane[C] = vec0[X] * vec1[Y] - vec0[Y] * vec1[X];

	plane[D] = -(plane[A] * v0[X] + plane[B] * v0[Y] + plane[C] * v0[Z]);
}

/******************** end shadow code ********************/


static void calcposobs(void) //calculate viewer position
{
	dir[0] = sin(alpha*M_PI / 180.0);
	dir[1] = cos(alpha*M_PI / 180.0)*sin(beta*M_PI / 180.0);
	dir[2] = cos(beta*M_PI / 180.0);

	obs[0] += v*dir[0];
	obs[1] += v*dir[1];
	obs[2] += v*dir[2];
}

static void reshape(int w, int h)
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


static void drawbase(void)
{
	GLfloat amb[4] = { 1, .5, 0.2, 1 };
	GLfloat diff[4] = { 1, .4, 0.2, 1 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
	glPushMatrix();
	glTranslatef(0.0, 0.0, 1.5);
	glRotatef(45.0, 1.0, 0.0, 0.0);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	glCullFace(GL_FRONT);
	gluCylinder(te, 4.0, 4.0, 4.0, 4, 10);
	glPopMatrix();

	GLfloat amb1[4] = { 0.5, .25, 0.1, 0.5 };
	GLfloat diff1[4] = { 0.5, .2, 0.1, 0.5 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff1);

	glPushMatrix();
	glTranslatef(-4.0, 0.0, 1.5);
	glRotatef(45.0, 1.0, 0.0, 0.0);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	glCullFace(GL_FRONT);
	gluDisk(te, 0.0, 4.0, 4, 10);
	glPopMatrix();

	GLfloat amb2[4] = { 0.19225, 0.19225, 0.19225, 1 };
	GLfloat diff2[4] = { 0.50754, 0.50754, 0.50754, 1 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff2);

	glPushMatrix();
	glTranslatef(0.0, 2.0, 2.0);
	glRotatef(45.0, 1.0, 0.0, 0.0);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	glCullFace(GL_FRONT);
	gluDisk(te, 0.0, 0.125, 20, 20);
	glPopMatrix();
}

static void drawteapot(void)
{
	if (mat == 1)
	{
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 51.2);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matdiff1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matspec1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matamb1);

	}
	else if (mat == 2)
	{
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 76.8);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matamb2);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matdiff2);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matspec2);
	}
	else if (mat == 3)
	{
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 76.8);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matamb3);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matdiff3);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matspec3);
	}

	glPushMatrix();
	glRotatef(lightalpha, 0.0, 0.0, 1.0);
	glMultMatrixf((GLfloat *)baseshadow);
	glRotatef(-lightalpha, 0.0, 0.0, 1.0);

	glTranslatef(-4.0, 0.0, 1.5);
	glRotatef(90.0, 0.0, 0.0, 1.0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glColor3f(0.0, 0.0, 0.0);

	if (lightActive)
		glCallList(teapotdlist); //draw main shadow

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glPopMatrix();

	glPushMatrix();
	glRotatef(lightalpha2, 0.0, 0.0, 1.0);
	glMultMatrixf((GLfloat *)baseshadow2);
	glRotatef(-lightalpha2, 0.0, 0.0, 1.0);

	glTranslatef(1.0, 0.0, 1.0);
	glRotatef(-45.0, 0.0, 0.0, 1.0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glColor3f(0.0, 0.0, 0.0);

	if (lightActive)
		glCallList(teapotdlist); //draw floor shadow

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.5);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	traverse(&t_node);
	glPopMatrix();
}

static void drawlight1(void)
{
	//light ws 3
	glPushMatrix();
	glRotatef(lightalpha, 0.0, 0.0, 1.0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightdir);
	glPopMatrix();

	//light ws 2 (little sphere at 0.0,1.0,1.0)
	glPushMatrix();
	glLightfv(GL_LIGHT1, GL_POSITION, lightpos2);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lightdir2);
	glPopMatrix();
}

static void drawlight2(void)
{
	glPushMatrix();
	glRotatef(lightalpha, 0.0, 0.0, 1.0);
	glTranslatef(lightpos[0], lightpos[1], lightpos[2]);
	glCallList(lightdlist);
	glPopMatrix();

	if(textureActive)
		lightalpha += 0.5;
	else
		lightalpha += 0.1;
}

static void draw(void)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_LIGHTING);

	glShadeModel(GL_SMOOTH);

	glPushMatrix();

	calcposobs();

	gluLookAt(obs[0], obs[1], obs[2],
		obs[0] + dir[0], obs[1] + dir[1], obs[2] + dir[2],
		0.0, 0.0, 1.0);

	drawlight1();
	glCallList(basedlist);
	drawteapot();

	if (lightActive)
		drawlight2();

	glPopMatrix();

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-0.5, 639.5, -0.5, 479.5, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	reshape(WIDTH, HEIGHT);
	glFlush();
	glutSwapBuffers();
}


static void initlight(void)
{
	float matamb[4] = { 0.5, 0.5, 0.5, 1.0 };
	float matdiff[4] = { 0.9, 0.2, 0.2, 1.0 };
	float matspec[4] = { 1.0,1.0,1.0,1.0 };

	float lamb[4] = { 1.5, 1.5, 1.5, 1.0 };
	float ldiff[4] = { 1.0, 1.0, 1.0, 1.0 };
	float lspec[4] = { 0.0, 0.0, 0.0, 0.0 };

	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 70.0);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 20.0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lamb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ldiff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lspec);

	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 70.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 20.0);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lamb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, ldiff);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lspec);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lamb);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
}

static void initdlists(void)
{
	GLUquadricObj *lcone, *lbase;
	GLfloat plane[4];
	GLfloat v0[3] = { -10, -10, -30 };
	GLfloat v1[3] = { -10, -10, -50 };
	GLfloat v2[3] = { -10, 300, -50 };

	findplane(plane, v0, v1, v2);
	shadowmatrix(baseshadow, plane, lightpos);

	GLfloat v02[3] = { 0, 0, 0 };
	GLfloat v12[3] = { 1, 0, 0 };
	GLfloat v22[3] = { 0, 1, 0 };

	findplane(plane, v02, v12, v22);
	shadowmatrix(baseshadow2, plane, lightpos2);

	teapotdlist = glGenLists(1);
	glNewList(teapotdlist, GL_COMPILE);
	glRotatef(90.0, 1.0, 0.0, 0.0);

	glCullFace(GL_FRONT);
	traverse(&t_node);
	glCullFace(GL_BACK);
	glEndList();

	basedlist = glGenLists(1);
	glNewList(basedlist, GL_COMPILE);
	drawbase();
	glEndList();

	lightdlist = glGenLists(1);
	glNewList(lightdlist, GL_COMPILE);
	glDisable(GL_LIGHTING);

	lcone = gluNewQuadric();
	lbase = gluNewQuadric();
	glRotatef(90.0, 0.0, 1.0, 0.0);

	glColor3f(1.0, 1.0, 1.0);
	glCullFace(GL_FRONT);
	gluDisk(lbase, 0.0, 0.2, 12.0, 1.0);
	glCullFace(GL_BACK);

	glColor3f(0.5, 0.0, 0.0);
	gluCylinder(lcone, 0.2, 0.0, 0.5, 12, 1);

	gluDeleteQuadric(lcone);
	gluDeleteQuadric(lbase);

	glEnable(GL_LIGHTING);
	glEndList();
}

void initobj()
{
	t = gluNewQuadric();
	gluQuadricDrawStyle(t, GLU_FILL);
	ua1 = gluNewQuadric();
	gluQuadricDrawStyle(ua1, GLU_FILL);
	la1 = gluNewQuadric();
	gluQuadricDrawStyle(la1, GLU_FILL);
	ua2 = gluNewQuadric();
	gluQuadricDrawStyle(ua2, GLU_FILL);
	la2 = gluNewQuadric();
	gluQuadricDrawStyle(la2, GLU_FILL);
	ua3 = gluNewQuadric();
	gluQuadricDrawStyle(ua3, GLU_FILL);
	la3 = gluNewQuadric();
	gluQuadricDrawStyle(la3, GLU_FILL);
	ua4 = gluNewQuadric();
	gluQuadricDrawStyle(ua4, GLU_FILL);
	la4 = gluNewQuadric();
	gluQuadricDrawStyle(la4, GLU_FILL);
	te = gluNewQuadric();
	gluQuadricDrawStyle(t, GLU_FILL);

	/* Set up tree */
	glLoadIdentity();
	glRotatef(theta[0], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, t_node.m);
	t_node.f = torso;
	t_node.sibling = NULL;
	t_node.child = &ua1_node;

	glLoadIdentity();
	glTranslatef(-0.5 * TORSO_RADIUS, 0.5 * TORSO_LENGTH, 0.0);
	glRotatef(theta[1], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, ua1_node.m);
	ua1_node.f = upper_arm_1;
	ua1_node.sibling = &ua2_node;
	ua1_node.child = &la1_node;

	glLoadIdentity();
	glTranslatef(0.5 * TORSO_RADIUS, 0.5 * TORSO_LENGTH, 0.0);
	glRotatef(theta[3], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, ua2_node.m);
	ua2_node.f = upper_arm_2;
	ua2_node.sibling = &ua3_node;
	ua2_node.child = &la2_node;

	glLoadIdentity();
	glTranslatef(-0.5 * TORSO_RADIUS, -0.5 * TORSO_LENGTH, 0.0);
	glRotatef(theta[5], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, ua3_node.m);
	ua3_node.f = upper_arm_3;
	ua3_node.sibling = &ua4_node;
	ua3_node.child = &la3_node;

	glLoadIdentity();
	glTranslatef(0.5 * TORSO_RADIUS, -0.5 * TORSO_LENGTH, 0.0);
	glRotatef(theta[7], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, ua4_node.m);
	ua4_node.f = upper_arm_4;
	ua4_node.sibling = NULL;
	ua4_node.child = &la4_node;

	glLoadIdentity();
	glTranslatef(-0.75 * UPPER_ARM_LENGTH, 0.0, 0.75 * UPPER_ARM_LENGTH);
	glRotatef(theta[2], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, la1_node.m);
	la1_node.f = lower_arm_1;
	la1_node.sibling = NULL;
	la1_node.child = NULL;

	glLoadIdentity();
	glTranslatef(0.75 * UPPER_ARM_LENGTH, 0.0, 0.75 * UPPER_ARM_LENGTH);
	glRotatef(theta[4], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, la2_node.m);
	la2_node.f = lower_arm_2;
	la2_node.sibling = NULL;
	la2_node.child = NULL;

	glLoadIdentity();
	glTranslatef(-0.75 * UPPER_ARM_LENGTH, 0.0, 0.75 * UPPER_ARM_LENGTH);
	glRotatef(theta[6], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, la3_node.m);
	la3_node.f = lower_arm_3;
	la3_node.sibling = NULL;
	la3_node.child = NULL;

	glLoadIdentity();
	glTranslatef(0.75 * UPPER_ARM_LENGTH, 0.0, 0.75 * UPPER_ARM_LENGTH);
	glRotatef(theta[8], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, la4_node.m);
	la4_node.f = lower_arm_4;
	la4_node.sibling = NULL;
	la4_node.child = NULL;

	glLoadIdentity();
}

void menu(int id)
{
	if (id == 1)
	{
		if (lightActive) {
			glDisable(GL_LIGHT0);
			glDisable(GL_LIGHT1);
			lightActive = false;
		}
		else {
			glEnable(GL_LIGHT0);
			glEnable(GL_LIGHT1);
			lightActive = true;
		}
	}
	else if (id > 1 && id < 5)
	{
		mat = id;
	}
	else if (id == 5)
	{
		textureActive = !textureActive;
	}
	else if (id == 11)
		exit(0);
}

int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("WS 3 Grafkom - Muhammad Luthfi - 1306386825");

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	calcposobs();

	initlight();

	initobj();

	initdlists();

	glClearColor(0.25, 0.25, 0.25, 1.0);

	glutReshapeFunc(reshape);
	glutDisplayFunc(draw);
	glutIdleFunc(draw);

	glutCreateMenu(menu);
	glutAddMenuEntry("Hidup/Matikan Lampu", 1);
	glutAddMenuEntry("Material Gold (Default)", 2);
	glutAddMenuEntry("Material Ruby", 3);
	glutAddMenuEntry("Material Emerald", 4);
	glutAddMenuEntry("Aktif/Nonaktifkan Tekstur",5);
	glutAddMenuEntry("quit", 11);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();

	return 0;
}
