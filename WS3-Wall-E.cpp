/* Worksheet 2 Grafkom						*/
/* Modified figure.c by E. Angel from Scele	*/

/* Mgs. M. Rizqi Fadhlurrahman				*/
/* 1306464543								*/

#include <stdio.h>
#include <math.h>
#include <glut.h>

//#define EYE_RADIUS 0.23
//#define EYE_HEIGHT 1.0
//#define UPPER_NECK_RADIUS 0.15
//#define UPPER_NECK_HEIGHT 0.5
//#define LOWER_NECK_RADIUS 0.16
//#define LOWER_NECK_HEIGHT 0.75
//#define BODY_RADIUS 1.2
//#define BODY_HEIGHT 1.5
//#define UPPER_ARM_RADIUS 0.18
//#define UPPER_ARM_HEIGHT 1.5
//#define LOWER_ARM_RADIUS 0.22
//#define LOWER_ARM_HEIGHT 0.4
//#define WHEEL_RADIUS 0.6
//#define WHEEL_HEIGHT 0.5

#define EYE_RADIUS 0.069
#define EYE_HEIGHT 0.3
#define UPPER_NECK_RADIUS 0.045
#define UPPER_NECK_HEIGHT 0.15
#define LOWER_NECK_RADIUS 0.048
#define LOWER_NECK_HEIGHT 0.225
#define BODY_RADIUS 0.36
#define BODY_HEIGHT 0.45
#define UPPER_ARM_RADIUS 0.054
#define UPPER_ARM_HEIGHT 0.45
#define LOWER_ARM_RADIUS 0.066
#define LOWER_ARM_HEIGHT 0.12
#define WHEEL_RADIUS 0.18
#define WHEEL_HEIGHT 0.15

#define BASESIZE 10.0

#define BASERES 12
#define TEAPOTRES 3

#ifndef M_PI
#define M_PI 3.1415926535
#endif

typedef float point[3];
bool animate = false;
int animationState = 0;

static float obs[3] = { 7.0, 0.0, 1.0 };
static float dir[3];
static float v = 0.0;
static float alpha = -90.0;
static float beta = 90.0;

static int matz = 1;
static int oflight = 1;

static GLfloat baseshadow1[4][4];
static GLfloat lightpos1[4] = { 3, 0.0, 1.0, 1.0 };
static GLfloat lightdir1[3] = { -3, 0.0, 1.0 };
static GLfloat lightalpha1 = 0.0;
static GLfloat baseshadow2[4][4];
static GLfloat lightpos2[4] = { 1.0, 1.7, 2.0 };
static GLfloat lightdir2[3] = { -2.3, 0.0, -3.0 };
static GLfloat lightalpha2 = 0.0;

static GLuint teapotdlist, basedlist, lightdlist;

static float lamb[4] = { 0.0, 0.0, 0.0, 1 };
static float ldiff[4] = { 1, 1, 1, 1 };
static float lspec[4] = { 1, 1, 1, 1 };

//Gold Material Value
static float matamb1[4] = { 0.24725, 0.1995, 0.0745, 1 };
static float matdiff1[4] = { 0.75164, 0.60648, 0.22648, 1 };
static float matspec1[4] = { 0.628281, 0.555802, 0.366065, 1 };

//Ruby Material Value
static float matamb2[4] = { 0.1745, 0.01175, 0.01175, 1 };
static float matdiff2[4] = { 0.61424, 0.04136, 0.04136, 1 };
static float matspec2[4] = { 0.727811, 0.626959, 0.626959, 1 };

//Silver Material Value
static float matamb3[4] = { 0.19225, 0.19225, 0.19225, 1 };
static float matdiff3[4] = { 0.50754, 0.50754, 0.50754, 1 };
static float matspec3[4] = { 0.508273, 0.508273, 0.508273, 1 };

GLfloat vertices[] =
{ -10.0, -10.0, -10.0, 10.0, -10.0, -10.0,
10.0, 10.0, -10.0, -10.0, 10.0, -10.0,
-10.0, -10.0, 10.0, 10.0, -10.0, 10.0,
10.0, 10.0, 10.0, -10.0, 10.0, 10.0 };

//cube index
GLubyte cubeIndices[] = { 0, 3, 2, 1, 2, 3, 7, 6, 0, 4, 7, 3, 1, 2, 6, 5, 4, 5, 6, 7, 0, 1, 5, 4 };

typedef struct treenode
{
	GLfloat m[16];
	void(*f)();
	struct treenode *sibling;
	struct treenode *child;
}treenode;

typedef treenode* t_ptr;

float theta[11] = { 0.0, 0.0, 140.0, 40.0, -10.0,0.0,0.0,0.0,0.0,0.0,0.0 };
float bodyTranslate = 0.0;
int angle = 2;

GLUquadricObj *le, *re, *un, *ln, *b, *rua, *lua, *rla, *lla, *rw, *lw, *room;

treenode re_node, le_node, un_node, ln_node, b_node, rua_node, lua_node, rla_node, lla_node, rw_node, lw_node;

void traverse(treenode* root)
{
	if (root == 0) return;
	glPushMatrix();
	glMultMatrixf(root->m);
	root->f();
	if (root->child != 0) traverse(root->child);
	glPopMatrix();
	if (root->sibling != 0) traverse(root->sibling);
}

void rightEye() {
	glPushMatrix();
	glTranslatef(-0.75*EYE_RADIUS, 0.0, 0.0);
	glRotatef(-45.0, 0.0, 0.0, 1.0);
	gluCylinder(re, EYE_RADIUS, EYE_RADIUS, EYE_HEIGHT, 8, 16);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, EYE_HEIGHT);
	gluDisk(re, 0.0, EYE_RADIUS, 8, 16);
	glPopMatrix();
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	gluDisk(re, 0.0, EYE_RADIUS, 8, 16);
	glPopMatrix();
}

void leftEye() {
	glPushMatrix();
	glTranslatef(0.75*EYE_RADIUS, 0.0, 0.0);
	glRotatef(-45.0, 0.0, 0.0, 1.0);
	gluCylinder(le, EYE_RADIUS, EYE_RADIUS, EYE_HEIGHT, 8, 16);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, EYE_HEIGHT);
	gluDisk(le, 0.0, EYE_RADIUS, 8, 16);
	glPopMatrix();
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	gluDisk(le, 0.0, EYE_RADIUS, 8, 16);
	glPopMatrix();
}

void upperNeck() {
	glPushMatrix();
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glRotatef(-45.0, 0.0, 0.0, 1.0);
	gluCylinder(un, UPPER_NECK_RADIUS, UPPER_NECK_RADIUS, UPPER_NECK_HEIGHT, 4, 16);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, UPPER_NECK_HEIGHT);
	gluDisk(un, 0.0, UPPER_NECK_RADIUS, 4, 16);
	glPopMatrix();
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	gluDisk(un, 0.0, UPPER_NECK_RADIUS, 4, 16);
	glPopMatrix();
}

void lowerNeck() {
	glPushMatrix();
	glTranslatef(0.0, 0.3*BODY_HEIGHT, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glRotatef(45.0, 0.0, 0.0, 1.0);
	gluCylinder(ln, LOWER_NECK_RADIUS, LOWER_NECK_RADIUS, LOWER_NECK_HEIGHT, 4, 16);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, LOWER_NECK_HEIGHT);
	gluDisk(ln, 0.0, LOWER_NECK_RADIUS, 4, 16);
	glPopMatrix();
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	gluDisk(ln, 0.0, LOWER_NECK_RADIUS, 4, 16);
	glPopMatrix();
}

void body() {
	glPushMatrix();
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glRotatef(45.0, 0.0, 0.0, 1.0);
	gluCylinder(b, BODY_RADIUS, BODY_RADIUS, BODY_HEIGHT, 4, 16);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, BODY_HEIGHT);
	gluDisk(b, 0.0, BODY_RADIUS, 4, 16);
	glPopMatrix();
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	gluDisk(b, 0.3, BODY_RADIUS, 4, 16);
	glPopMatrix();
}

void rightUpperArm() {
	glPushMatrix();
	glTranslatef(-0.81*BODY_RADIUS, -0.15*BODY_HEIGHT, 0.0);
	glRotatef(45.0, 0.0, 0.0, 1.0);
	gluCylinder(rua, UPPER_ARM_RADIUS, UPPER_ARM_RADIUS, UPPER_ARM_HEIGHT, 4, 16);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, UPPER_ARM_HEIGHT);
	gluDisk(rua, 0.0, UPPER_ARM_RADIUS, 4, 16);
	glPopMatrix();
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	gluDisk(rua, 0.0, UPPER_ARM_RADIUS, 4, 16);
	glPopMatrix();
}

void leftUpperArm() {
	glPushMatrix();
	glTranslatef(0.81*BODY_RADIUS, -0.15*BODY_HEIGHT, 0.0);
	glRotatef(45.0, 0.0, 0.0, 1.0);
	gluCylinder(lua, UPPER_ARM_RADIUS, UPPER_ARM_RADIUS, UPPER_ARM_HEIGHT, 4, 16);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, UPPER_ARM_HEIGHT);
	gluDisk(lua, 0.0, UPPER_ARM_RADIUS, 4, 16);
	glPopMatrix();
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	gluDisk(lua, 0.0, UPPER_ARM_RADIUS, 4, 16);
	glPopMatrix();
}

void rightLowerArm() {
	glPushMatrix();
	glRotatef(45.0, 0.0, 0.0, 1.0);
	gluCylinder(rla, LOWER_ARM_RADIUS, LOWER_ARM_RADIUS, LOWER_ARM_HEIGHT, 4, 16);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, LOWER_ARM_HEIGHT);
	gluDisk(rla, 0.0, LOWER_ARM_RADIUS, 4, 16);
	glPopMatrix();
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	gluDisk(rla, 0.0, LOWER_ARM_RADIUS, 4, 16);
	glPopMatrix();
}

void leftLowerArm() {
	glPushMatrix();
	glRotatef(45.0, 0.0, 0.0, 1.0);
	gluCylinder(lla, LOWER_ARM_RADIUS, LOWER_ARM_RADIUS, LOWER_ARM_HEIGHT, 4, 16);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, LOWER_ARM_HEIGHT);
	gluDisk(lla, 0.0, LOWER_ARM_RADIUS, 4, 16);
	glPopMatrix();
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	gluDisk(lla, 0.0, LOWER_ARM_RADIUS, 4, 16);
	glPopMatrix();
}

void rightWheel() {
	glPushMatrix();
	glRotatef(90.0, 0.0, 1.0, 0.0);
	gluCylinder(rw, WHEEL_RADIUS, WHEEL_RADIUS, WHEEL_HEIGHT, 10, 16);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, WHEEL_HEIGHT);
	gluDisk(rw, 0.0, WHEEL_RADIUS, 10, 16);
	glPopMatrix();
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	gluDisk(rw, 0.0, WHEEL_RADIUS, 10, 16);
	glPopMatrix();
}

void leftWheel() {
	glPushMatrix();
	glRotatef(90.0, 0.0, 1.0, 0.0);
	gluCylinder(rw, WHEEL_RADIUS, WHEEL_RADIUS, WHEEL_HEIGHT, 10, 16);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, WHEEL_HEIGHT);
	gluDisk(rw, 0.0, WHEEL_RADIUS, 10, 16);
	glPopMatrix();
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	gluDisk(rw, 0.0, WHEEL_RADIUS, 10, 16);
	glPopMatrix();
}

void buildRoom() {
	glPushMatrix();
	//glRotatef(45.0, 0.0, 0.0, 1.0);
	////glTranslatef(0.0f, 0.0f, -0.2f);
	//gluCylinder(room, 1.5, 1.5, 1.5, 4, 16);
	//glPushMatrix();
	//glTranslatef(0.0f, 0.0f, 1.5);
	////gluDisk(rw, 0.0, 2.0, 4, 16);
	//glPopMatrix();
	//glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	//gluDisk(rw, 0.0, 1.5, 4, 16);
	glutSolidCube(5);
	glPopMatrix();
}


static void drawWallE(void)
{
	static const GLfloat amb[4] = { 0.2, 0.2, 0.2, 1 };
	static const GLfloat diff[4] = { 0.8, 0.3, 0.5, 1 };
	static float xrot = 0.0;
	static float zrot = 0.0;
	if (matz == 1)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matdiff1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matspec1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matamb1);

	}
	else if (matz == 2)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matamb2);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matdiff2);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matspec2);
	}
	else
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matamb3);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matdiff3);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matspec3);
	}
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 70.0);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 20.0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lamb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ldiff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lspec);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lamb);

	glPushMatrix();
	glRotatef(lightalpha1, 0.0, 0.0, 1.0);
	glMultMatrixf((GLfloat *)baseshadow1);
	glRotatef(-lightalpha1, 0.0, 0.0, 1.0);

	glTranslatef(0.0, 0.0, 1.0);
	//glRotatef(xrot, 1.0, 0.0, 0.0);
	glRotatef(zrot, 0.0, 0.0, 1.0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glColor3f(0.0, 0.0, 0.0);
	if (oflight)
	{
		glCallList(teapotdlist);
		//draw shadow
	}


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, 1.0);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	//glRotatef(zrot,0.0,0.0,1.0);

	glCallList(teapotdlist);
	glPopMatrix();
}

static void drawWallE2(void)
{
	static const GLfloat amb[4] = { 0.2, 0.2, 0.2, 1 };
	static const GLfloat diff[4] = { 0.8, 0.3, 0.5, 1 };
	static float xrot = 0.0;
	static float zrot = 0.0;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);

	glPushMatrix();
	glRotatef(lightalpha2, 0.0, 0.0, 1.0);
	glMultMatrixf((GLfloat *)baseshadow2);
	glRotatef(-lightalpha2, 0.0, 0.0, 1.0);

	glTranslatef(0.0, 0.0, 1.0);
	glRotatef(xrot, 1.0, 0.0, 0.0);
	glRotatef(zrot, 0.0, 0.0, 1.0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glColor3f(0.0, 0.0, 0.0);
	if (oflight)
	{
		glCallList(teapotdlist);
		//draw shadow
	}


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, 1.0);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	//glRotatef(zrot,0.0,0.0,1.0);

	glCallList(teapotdlist);
	glPopMatrix();
}

/* Taken from the projshadow.c - by Tom McReynolds, SGI */
/* modified by Dadan Hardianto, Fasilkom UI */

enum {
	X, Y, Z, W
};
enum {
	A, B, C, D
};

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

static void calcposobs(void) //calculate viewer position
{
	dir[0] = sin(alpha*M_PI / 180.0);
	dir[1] = cos(alpha*M_PI / 180.0)*sin(beta*M_PI / 180.0);
	dir[2] = cos(beta*M_PI / 180.0);

	obs[0] += v*dir[0];
	obs[1] += v*dir[1];
	obs[2] += v*dir[2];
}

static void drawlight1(void)
{
	glPushMatrix();
	glRotatef(lightalpha1, 0.0, 0.0, 1.0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos1);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightdir1);
	glPopMatrix();
}

static void drawlight2(void)
{
	glPushMatrix();
	glRotatef(lightalpha2, 0.0, 0.0, 1.0);
	glTranslatef(lightpos2[0], lightpos2[1], lightpos2[2]);
	glCallList(lightdlist);
	glPopMatrix();
	if (animate)
	{
		lightalpha2 += 0.1;
	}
}

void display(void) {
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
	drawWallE();
	drawWallE2();
	drawlight2();
	//glTranslatef(0.0f, 0.0f, -2.5);
	//buildRoom();
	//traverse(&b_node);

	glPopMatrix();

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);

	glFlush();
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 's':
		glLoadIdentity();
		gluLookAt(0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		glutPostRedisplay();
		break;
	}
}

void idle() {
	glLoadIdentity();
	glutPostRedisplay();
}

void mouse(int btn, int state, int x, int y) {
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		lightalpha1 -= 10;
	}
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		lightalpha1 += 10;
	}
	glutPostRedisplay();
}

void menu(int id) {
	if (id == 0) {
		if (animate) {
			animate = false;
		}
		else {
			animate = true;
		}
	}
	else if (id == 1)
	{
		matz = 1;
	}
	else if (id == 2)
	{
		matz = 2;
	}
	else if (id == 3)
	{
		matz = 3;
	}
	else if (id == 11) {
		exit(0);
	}
}

void reshape(int w, int h) {
	GLdouble aspect_ratio;
	if (0 >= w || 0 >= h)
	{
		return;
	}
	glViewport(0, 0, w, h);
	aspect_ratio = (GLdouble)w / (GLdouble)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-3.0 * aspect_ratio, 3.0 * aspect_ratio, -3.0, 3.0, -10.0, 10.0);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//gluLookAt(0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	gluPerspective(60.0f, aspect_ratio, 0.1f, 256.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void myinit() {
	glClearColor(0.85, 0.85, 0.85, 1.0);
	glClearDepth(1.0f);

	le = gluNewQuadric();
	gluQuadricDrawStyle(le, GLU_FILL);
	re = gluNewQuadric();
	gluQuadricDrawStyle(re, GLU_FILL);
	un = gluNewQuadric();
	gluQuadricDrawStyle(un, GLU_FILL);
	ln = gluNewQuadric();
	gluQuadricDrawStyle(ln, GLU_FILL);
	b = gluNewQuadric();
	gluQuadricDrawStyle(b, GLU_FILL);
	rua = gluNewQuadric();
	gluQuadricDrawStyle(rua, GLU_FILL);
	lua = gluNewQuadric();
	gluQuadricDrawStyle(lua, GLU_FILL);
	rla = gluNewQuadric();
	gluQuadricDrawStyle(rla, GLU_FILL);
	lla = gluNewQuadric();
	gluQuadricDrawStyle(lla, GLU_FILL);
	rw = gluNewQuadric();
	gluQuadricDrawStyle(rw, GLU_FILL);
	lw = gluNewQuadric();
	gluQuadricDrawStyle(lw, GLU_FILL);
	room = gluNewQuadric();
	gluQuadricDrawStyle(room, GLU_FILL);

	glLoadIdentity();
	glRotatef(theta[4], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, b_node.m);
	b_node.f = body;
	b_node.sibling = 0;
	b_node.child = &ln_node;

	glLoadIdentity();
	glRotatef(theta[3], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, ln_node.m);
	ln_node.f = lowerNeck;
	ln_node.sibling = &rua_node;
	ln_node.child = &un_node;

	glLoadIdentity();
	glTranslatef(0.0, 0.5*LOWER_NECK_HEIGHT, 0.0);
	glRotatef(theta[2], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, un_node.m);
	un_node.f = upperNeck;
	un_node.sibling = 0;
	un_node.child = &re_node;

	glLoadIdentity();
	glTranslatef(-0.81*UPPER_NECK_RADIUS, -UPPER_NECK_HEIGHT, -0.65*EYE_HEIGHT);
	glRotatef(-theta[1], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, re_node.m);
	re_node.f = rightEye;
	re_node.sibling = &le_node;
	re_node.child = 0;

	glLoadIdentity();
	glTranslatef(0.81*UPPER_NECK_RADIUS, -UPPER_NECK_HEIGHT, -0.65*EYE_HEIGHT);
	glRotatef(theta[1], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, le_node.m);
	le_node.f = leftEye;
	le_node.sibling = 0;
	le_node.child = 0;

	glLoadIdentity();
	glRotatef(theta[5], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, rua_node.m);
	rua_node.f = rightUpperArm;
	rua_node.sibling = &lua_node;
	rua_node.child = &rla_node;

	glLoadIdentity();
	glTranslatef(-0.81*BODY_RADIUS, -0.15*BODY_HEIGHT, 0.9*UPPER_ARM_HEIGHT);
	glRotatef(theta[7], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, rla_node.m);
	rla_node.f = rightLowerArm;
	rla_node.sibling = 0;
	rla_node.child = 0;

	glLoadIdentity();
	glRotatef(theta[6], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lua_node.m);
	lua_node.f = leftUpperArm;
	lua_node.sibling = &lw_node;
	lua_node.child = &lla_node;

	glLoadIdentity();
	glTranslatef(0.81*BODY_RADIUS, -0.15*BODY_HEIGHT, 0.9*UPPER_ARM_HEIGHT);
	glRotatef(theta[8], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lla_node.m);
	lla_node.f = leftLowerArm;
	lla_node.sibling = 0;
	lla_node.child = 0;

	glLoadIdentity();
	glTranslatef(0.7*BODY_RADIUS, -0.9*BODY_HEIGHT, 0.0);
	glRotatef(theta[10], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lw_node.m);
	lw_node.f = leftWheel;
	lw_node.sibling = &rw_node;
	lw_node.child = 0;

	glLoadIdentity();
	glTranslatef(-0.7*BODY_RADIUS - WHEEL_HEIGHT, -0.9*BODY_HEIGHT, 0.0);
	glRotatef(theta[9], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, rw_node.m);
	rw_node.f = rightWheel;
	rw_node.sibling = 0;
	rw_node.child = 0;
}

static void drawbase(void)
{
	/*static const GLfloat amb[4] = { 1, .5, 0.2, 1 };
	static const GLfloat diff[4] = { 1, .4, 0.2, 1 };
	int i, j;
	float x, y, dx, dy;


	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
	dx = BASESIZE / BASERES;
	dy = -BASESIZE / BASERES;
	for (y = BASESIZE / 2.0, j = 0;j<BASERES;y += dy, j++) {
		glBegin(GL_QUAD_STRIP);
		glColor3f(1.0, 1.0, 1.0);
		glNormal3f(0.0, 0.0, 1.0);
		for (x = -BASESIZE / 2.0, i = 0;i<BASERES;x += dx, i++) {
			glVertex3f(x, y, 0.0);
			glVertex3f(x, y + dy, 0.0);
		}
		glEnd();
	}*/
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 1.0f);
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cubeIndices);
	glPopMatrix();
}

static void initlight(void)
{

	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 15.0);
	glEnable(GL_LIGHT0);

	float ldiff2[4] = { 0.0, 0.0, 0.0, 0.0 };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, ldiff2);
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
	shadowmatrix(baseshadow1, plane, lightpos1);

	GLfloat v02[3] = { 0, 0, 0 };
	GLfloat v12[3] = { 1, 0, 0 };
	GLfloat v22[3] = { 0, 1, 0 };

	findplane(plane, v02, v12, v22);
	shadowmatrix(baseshadow2, plane, lightpos2);

	teapotdlist = glGenLists(1);
	glNewList(teapotdlist, GL_COMPILE);
	glRotatef(90.0, 1.0, 0.0, 0.0);

	glPushMatrix();
	glTranslatef(1.0, 1.7, 2.0);
	glutSolidSphere(0.05, 20, 20);
	glPopMatrix();

	glCullFace(GL_BACK);
	traverse(&b_node);
	glCullFace(GL_FRONT);

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

void main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("WS2 - Wall-E");
	myinit();
	calcposobs();
	initlight();
	initdlists();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	glutCreateMenu(menu);
	glutAddMenuEntry("Toggle Animasi Lampu", 0);
	glutAddMenuEntry("Material 1", 1);
	glutAddMenuEntry("Material 2", 2);
	glutAddMenuEntry("Material 3", 3);
	glutAddMenuEntry("Quit", 11);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);

	glutMainLoop();
}