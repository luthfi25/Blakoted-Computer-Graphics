/*
* Teapot with shadow
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glut.h>

static int WIDTH = 640;
static int HEIGHT = 480;

static GLint T0 = 0;

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
static GLfloat lightpos[4] = { 2.3,0.0,0.75,1.0 };
static GLfloat lightdir[3] = { -2.3,0.0,0.0 };
static GLfloat lightalpha = 0.0;

static int bfcull = 1;

static GLuint teapotdlist, basedlist, lightdlist;
GLUquadricObj *t, *ua1, *la1, *ua2, *la2, *ua3, *la3, *ua4, *la4, *te;
static GLfloat theta[9] = { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 };

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
	glPushMatrix();
	glScalef(TORSO_RADIUS, TORSO_LENGTH, TORSO_RADIUS);
	gluSphere(t, 1.0, 10, 10);
	glPopMatrix();
}

void upper_arm_1()
{
	glPushMatrix();
	glRotatef(-45.0, 0.0, 1.0, 0.0);
	gluCylinder(ua1, ARM_RADIUS, ARM_RADIUS, UPPER_ARM_LENGTH, 10, 10);
	glPopMatrix();
}

void lower_arm_1()
{
	glPushMatrix();
	glRotatef(-135.0, 0.0, 1.0, 0.0);
	gluCylinder(la1, ARM_RADIUS, ARM_RADIUS, LOWER_ARM_LENGTH, 10, 10);
	glPopMatrix();
}

void upper_arm_2()
{
	glPushMatrix();
	glRotatef(45.0, 0.0, 1.0, 0.0);
	gluCylinder(ua2, ARM_RADIUS, ARM_RADIUS, UPPER_ARM_LENGTH, 10, 10);
	glPopMatrix();
}

void lower_arm_2()
{
	glPushMatrix();
	glRotatef(135.0, 0.0, 1.0, 0.0);
	gluCylinder(la2, ARM_RADIUS, ARM_RADIUS, LOWER_ARM_LENGTH, 10, 10);
	glPopMatrix();
}

void upper_arm_3()
{
	glPushMatrix();
	glRotatef(-45.0, 0.0, 1.0, 0.0);
	gluCylinder(ua3, ARM_RADIUS, ARM_RADIUS, UPPER_ARM_LENGTH, 10, 10);
	glPopMatrix();
}

void lower_arm_3()
{
	glPushMatrix();
	glRotatef(-135.0, 0.0, 1.0, 0.0);
	gluCylinder(la3, ARM_RADIUS, ARM_RADIUS, LOWER_ARM_LENGTH, 10, 10);
	glPopMatrix();
}

void upper_arm_4()
{
	glPushMatrix();
	glRotatef(45.0, 0.0, 1.0, 0.0);
	gluCylinder(ua4, ARM_RADIUS, ARM_RADIUS, UPPER_ARM_LENGTH, 10, 10);
	glPopMatrix();
}

void lower_arm_4()
{
	glPushMatrix();
	glRotatef(135.0, 0.0, 1.0, 0.0);
	gluCylinder(la4, ARM_RADIUS, ARM_RADIUS, LOWER_ARM_LENGTH, 10, 10);
	glPopMatrix();
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

static void special(int k, int x, int y) //change viewer angle position
{
	switch (k) {
	case GLUT_KEY_LEFT:
		alpha -= 2.0;
		break;
	case GLUT_KEY_RIGHT:
		alpha += 2.0;
		break;
	case GLUT_KEY_DOWN:
		beta -= 2.0;
		break;
	case GLUT_KEY_UP:
		beta += 2.0;
		break;
	}
}

static void key(unsigned char k, int x, int y)
{
	switch (k) {
	case 27:
		exit(0); //exit program
		break;

		/* a or z to change viewer depth position */
	case 'a':
		v += 0.005;
		break;
	case 'z':
		v -= 0.005;
		break;

	case 'b': //enable/disable culling
		if (bfcull) {
			glDisable(GL_CULL_FACE);
			bfcull = 0;
		}
		else {
			glEnable(GL_CULL_FACE);
			bfcull = 1;
		}
		break;
	}
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
	static const GLfloat amb[4] = { 1, .5, 0.2, 1 };
	static const GLfloat diff[4] = { 1, .4, 0.2, 1 };
	int i, j;
	float x, y, dx, dy;


	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
	dx = BASESIZE / BASERES;
	dy = -BASESIZE / BASERES;
	for (y = BASESIZE / 2.0, j = 0; j<BASERES; y += dy, j++) {
		glCullFace(GL_FRONT);
		glBegin(GL_QUAD_STRIP);
		glColor3f(1.0, 1.0, 1.0);
		glNormal3f(0.0, 0.0, 1.0);
		for (x = -BASESIZE / 2.0, i = 0; i<BASERES; x += dx, i++) {
			glVertex3f(x - 5.0, -5.0, y);
			glVertex3f(x - 5.0, -5.0, y + dy);
			//glVertex3f(x, 5.0, y);
			//glVertex3f(x, 5.0, y + dy);
			printf("%f,%f,%f\n",x,y,dy);
		}
		glEnd();
	}
}

static void drawteapot(void)
{
	static const GLfloat amb[4] = { 0.2, 0.2, 0.2, 1 };
	static const GLfloat diff[4] = { 0.8, 0.3, 0.5, 1 };
	static float xrot = 0.0;
	static float zrot = 0.0;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);

	glPushMatrix();
	glRotatef(lightalpha, 0.0, 0.0, 1.0);
	glMultMatrixf((GLfloat *)baseshadow);
	glRotatef(-lightalpha, 0.0, 0.0, 1.0);

	glTranslatef(0.0, 0.0, 1.375);
	glRotatef(xrot, 1.0, 0.0, 0.0);
	glRotatef(zrot, 0.0, 0.0, 1.0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glColor3f(0.0, 0.0, 0.0);
	glCallList(teapotdlist); //draw shadow

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glPopMatrix();

	glPushMatrix();
	glRotatef(90.0,1.0,0.0,0.0);
	glTranslatef(-0.5, 1.5, 0.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	traverse(&t_node);
	glPopMatrix();

	//xrot += 0.5;
	//zrot += 0.25;
}

static void drawlight1(void)
{
	glPushMatrix();
	glRotatef(lightalpha, 0.0, 0.0, 1.0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightdir);
	glPopMatrix();
}

static void drawlight2(void)
{
	glPushMatrix();
	glRotatef(lightalpha, 0.0, 0.0, 1.0);
	glTranslatef(lightpos[0], lightpos[1], lightpos[2]);
	glCallList(lightdlist);
	glPopMatrix();
	lightalpha += 0.25;
}

static void drawtembok()
{
	glPushMatrix();
	static const GLfloat amb[4] = { 1, .5, 0.2, 1 };
	static const GLfloat diff[4] = { 1, .4, 0.2, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glRotatef(45.0, 0.0, 0.0, 1.0);
	gluCylinder(te,7.5,7.5,7.5,4,100);
	glPopMatrix();
}


static void draw(void)
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_LIGHTING);

	glShadeModel(GL_SMOOTH);

	glPushMatrix();

	calcposobs();

	gluLookAt(obs[0], obs[1], obs[2],
		obs[0] + dir[0], obs[1] + dir[1], obs[2] + dir[2],
		0.0, 0.0, 1.0);

	//drawtembok();
	drawlight1();
	glCallList(basedlist);
	drawteapot();
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
	float lspec[4] = { 1.0, 1.0, 1.0, 1.0 };

	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 70.0);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 20.0);
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

static void initdlists(void)
{
	GLUquadricObj *lcone, *lbase;
	GLfloat plane[4];
	GLfloat v0[3] = { 0.0,0.0,0.0 };
	GLfloat v1[3] = { 1.0,0.0,0.0 };
	GLfloat v2[3] = { 0.0,1.0,0.0 };

	findplane(plane, v0, v1, v2);
	shadowmatrix(baseshadow, plane, lightpos);

	teapotdlist = glGenLists(1);
	glNewList(teapotdlist, GL_COMPILE);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glCullFace(GL_FRONT);
	//glutSolidTeapot(0.75);
	traverse(&t_node);
	//glutSolidCube(0.75 );
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

int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Shadowed Teapot");

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
	glutKeyboardFunc(key);
	glutSpecialFunc(special);
	glutIdleFunc(draw);

	glutMainLoop();

	return 0;
}
