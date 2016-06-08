/* Worksheet2.c   */
/* modifikasi dari figuretr.c   */

/* E. Angel, Interactive Computer Graphics */
/* A Top-Down Approach with OpenGL, Third Edition */
/* Addison-Wesley Longman, 2003 */

/* Interactive Figure Program from Chapter 8 using cylinders (quadrics) */
/* Style similar to robot program but here we must traverse tree to display */
/* Cylinders are displayed as filled and light/material properties */
/* are set as in sphere approximation program */

/* This version uses a standard left-child right-sibling */
/* tree structure */

#define NULL 0
#define TORSO_LENGTH 6.25
#define TORSO_RADIUS 1.5
#define UPPER_ARM_LENGTH 3.75
#define LOWER_ARM_LENGTH 2.5
#define ARM_RADIUS 0.5

#include <stdio.h>
#include <windows.h>
#include <glut.h>
#include <string>
#include <iostream>

//Struct untuk mencatat posisi objek.
struct Object {
	float translation[3];
	float rotation;
	std::string direction;
};
Object camera;
Object dog;
Object train;
Object officer;
Object lamp3;
Object lamp4;
Object spider;

#define ENGINE_RADIUS 3.0
#define ENGINE_HEIGHT 7.0
#define CAB_RADIUS 4.0
#define CAB_HEIGHT 9.0
#define WHEEL_RADIUS 1.0
#define SS_RADIUS 0.5
#define SS_HEIGHT 1.5

#define TORSO_HEIGHT 5.0
#define UPPER_LEG_RADIUS  0.5
#define LOWER_LEG_RADIUS  0.5
#define LOWER_LEG_HEIGHT 2.0
#define UPPER_LEG_HEIGHT 3.0
#define UPPER_LEG_RADIUS  0.5
#define TORSO_RADIUS2 1.0
#define HEAD_HEIGHT 1.5
#define HEAD_RADIUS 1.0

void head();
void body();
void left_upper_arm();
void right_upper_arm();
void left_upper_leg();
void right_upper_leg();
void myinit();

void initObjectPos();

typedef struct treenode
{
	GLfloat m[16];
	void(*f)();
	struct treenode *sibling;
	struct treenode *child;
}treenode;

//untuk animasi
static GLfloat theta[10] = { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 };
bool forwardSpider = true;
static GLfloat thetaDog[12] = { 0.0, 0.0, 0.0, 180.0, 0.0, 180.0, 0.0, 180.0, 0.0, 180.0, 0.0, 90.0 };
bool forwardDog[5];
static GLfloat thetaTrain[2] = { 0.0, 0.0 };
GLfloat smokeStack = 0;
bool topStack = true;
static GLfloat thetaOfficer[12] = { 90.0, 0.0, 0.0, 90.0, 0.0, 90.0, 0.0, 180.0, 0.0, 180.0, 0.0, 90.0 };
bool forwardOfficer[2];


//quadric spider
GLUquadricObj *t, *ua1, *la1, *ua2, *la2, *ua3, *la3, *ua4, *la4, *te;
//quadric anjing
GLUquadricObj *b, *h, *lua, *lla, *rua, *rla, *lll, *rll, *rul, *lul, *t1;
//quadric kereta
GLUquadricObj *e, *c, *lfw, *lrw, *rfw, *rrw, *ss;
//quadric officer
GLUquadricObj *t2, *h2, *lua2, *lla2, *rua2, *rla2, *lll2, *rll2, *rul2, *lul2;
//quadric meja
GLUquadricObj *ut1, *ut2, *ut3, *lt;

treenode t_node, ua1_node, la1_node, ua2_node, la2_node, ua3_node, la3_node, ua4_node, la4_node;
treenode body_node, head_node, lua_node, rua_node, lll_node, rll_node, lla_node, rla_node, rul_node, lul_node, tail_node;
treenode engine_node, cab_node, lfw_node, lrw_node, rfw_node, rrw_node, ss_node;
treenode torso_node, head2_node, lua2_node, rua2_node, lll2_node, rll2_node, lla2_node, rla2_node, rul2_node, lul2_node;

//variabel untuk rotasi kamera
float rotation1 = 0;
float rotation2 = 0;
//variabel untuk mode animasi atau interaktif
std::string mode = "interactive";
//variabel untuk rotasi lampu
GLfloat rotLamp = 0;
//variable to hold texture pointer
GLuint *texture = new GLuint[12];
//untuk menentukan pilihan view kamera
std::string view = "camera";
//untuk menentukan pilihan kontrol
std::string control = "camera";
//variabel untuk mode shading atau wireframe
std::string objmode = "shading";

GLfloat lamp_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lamp_diffuse[] = { 0.43, 0.5, 0.5, 1.0 };
GLfloat lamp_ambient[] = { 0.3, 0.3, 0.52, 1.0 };
GLfloat lamp_shininess = { 10 };

GLfloat light1_position[] = { -15.0, 10.0, -15.0, 1.0 };
GLfloat light2_position[] = { 15.0, 10.0, -15.0, 1.0 };
GLfloat light3_position[] = { lamp3.translation[0], lamp3.translation[1], lamp3.translation[2], 1.0 };
GLfloat light4_position[] = { lamp4.translation[0], lamp4.translation[1], lamp4.translation[2], 1.0 };

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
static GLfloat baseshadow[4][4];
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
GLfloat plane[4];
void findplane(GLfloat plane[4], GLfloat v0[3], GLfloat v1[3], GLfloat v2[3])
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

void traverse(treenode* root)
{
	if (root == NULL) return;
	glPushMatrix();
	glMultMatrixf(root->m);
	root->f();
	if (root->child != NULL) traverse(root->child);
	glPopMatrix();
	if (root->sibling != NULL) traverse(root->sibling);
}

//method objek anjing
void body()
{
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glPushMatrix();
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	gluCylinder(b, 1.0, 1.0, 5.0, 10, 10);
	glTranslatef(0.0, 0.0, 5.0);
	glutSolidCone(1.0, 1.67, 10, 10);
	glTranslatef(0.0, 0.0, -5.0);
	gluSphere(h, 1.0, 20, 20);
	glPopMatrix();
}

void head()
{
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glPushMatrix();
	gluSphere(h, 1.0, 20, 20);
	glPopMatrix();
}

void left_upper_arm()
{
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(lua, 0.25, 0.25, 3.0, 10, 10);
	glPopMatrix();
}

void left_lower_arm()
{
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(lla, 0.25, 0.25, 2.0, 10, 10);
	glPopMatrix();
}

void right_upper_arm()
{
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(rua, 0.25, 0.25, 3.0, 10, 10);
	glPopMatrix();
}

void right_lower_arm()
{
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(rla, 0.25, 0.25, 2.0, 10, 10);
	glPopMatrix();
}

void left_upper_leg()
{
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(lul, 0.25, 0.25, 3.0, 10, 10);
	glPopMatrix();
}

void left_lower_leg()
{
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(lll, 0.25, 0.25, 2.0, 10, 10);
	glPopMatrix();
}

void right_upper_leg()
{
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(rul, 0.25, 0.25, 3.0, 10, 10);
	glPopMatrix();
}

void right_lower_leg()
{
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(rll, 0.25, 0.25, 2.0, 10, 10);
	glPopMatrix();
}

void tail()
{
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glPushMatrix();
	gluCylinder(t1, 0.2, 0.2, 5.0, 20, 20);
	glPopMatrix();
}

//Method objek kereta
void engine() //membuat objek engine dari locomotive
{
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glPushMatrix();
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	gluCylinder(e, ENGINE_RADIUS, ENGINE_RADIUS, ENGINE_HEIGHT, 10, 10);
	glPopMatrix();
}

void cab() //membuat objek cab dari locomotive
{
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(c, CAB_RADIUS, CAB_RADIUS, CAB_HEIGHT, 10, 10);
	glPopMatrix();
}

void left_front_wheel() //membuat objek roda dari locomotive
{
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glPushMatrix();
	gluSphere(lfw, WHEEL_RADIUS, 10, 10);
	glPopMatrix();
}

void left_rear_wheel() //membuat objek roda dari locomotive
{
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glPushMatrix();
	gluSphere(lrw, WHEEL_RADIUS, 10, 10);
	glPopMatrix();
}

void right_front_wheel() //membuat objek roda dari locomotive
{
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glPushMatrix();
	gluSphere(rfw, WHEEL_RADIUS, 10, 10);
	glPopMatrix();
}

void right_rear_wheel() //membuat objek roda dari locomotive
{
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glPushMatrix();
	gluSphere(rrw, WHEEL_RADIUS, 10, 10);
	glPopMatrix();
}

void smoke_stack() //membuat objek pembuangan uap dari locomotive
{
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(ss, SS_RADIUS, SS_RADIUS + 0.5, SS_HEIGHT, 10, 10);
	glPopMatrix();
}

//method objek officer
void torso2()
{
	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glPushMatrix();
	glRotatef(0.0, 1.0, 0.0, 0.0);
	gluCylinder(t2, TORSO_RADIUS2, TORSO_RADIUS2, TORSO_HEIGHT, 10, 10);
	glPopMatrix();
}

void head2()
{
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glPushMatrix();
	glTranslatef(0.0, 0.5*HEAD_HEIGHT, 0.0);
	glScalef(HEAD_RADIUS, HEAD_HEIGHT, HEAD_RADIUS);
	gluSphere(h, 1.0, 10, 10);
	glPopMatrix();
}

void left_upper_arm2()
{
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(lua, UPPER_LEG_RADIUS, UPPER_LEG_RADIUS, UPPER_LEG_HEIGHT, 10, 10);
	glPopMatrix();
}

void left_lower_arm2()
{
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(lla, LOWER_LEG_RADIUS, LOWER_LEG_RADIUS, LOWER_LEG_HEIGHT, 10, 10);
	glPopMatrix();
}

void right_upper_arm2()
{
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(rua, UPPER_LEG_RADIUS, UPPER_LEG_RADIUS, UPPER_LEG_HEIGHT, 10, 10);
	glPopMatrix();
}

void right_lower_arm2()
{
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(rla, LOWER_LEG_RADIUS, LOWER_LEG_RADIUS, LOWER_LEG_HEIGHT, 10, 10);
	glPopMatrix();
}

void left_upper_leg2()
{
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(lul, UPPER_LEG_RADIUS, UPPER_LEG_RADIUS, UPPER_LEG_HEIGHT, 10, 10);
	glPopMatrix();
}

void left_lower_leg2()
{
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(lll, LOWER_LEG_RADIUS, LOWER_LEG_RADIUS, LOWER_LEG_HEIGHT, 10, 10);
	glPopMatrix();
}

void right_upper_leg2()
{
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(rul, UPPER_LEG_RADIUS, UPPER_LEG_RADIUS, UPPER_LEG_HEIGHT, 10, 10);
	glPopMatrix();
}

void right_lower_leg2()
{
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(rll, LOWER_LEG_RADIUS, LOWER_LEG_RADIUS, LOWER_LEG_HEIGHT, 10, 10);
	glPopMatrix();
}

//method objek spider
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

void display(void)
{
	//cari shadow matrix
	shadowmatrix(baseshadow, plane, light3_position);

	glLoadIdentity();
	//mengatur view kamera
	if (view.compare("camera") == 0)
	{
		glTranslated(camera.translation[0], camera.translation[1], camera.translation[2]);
	}
	else if (view.compare("dog") == 0)
	{
		if (dog.direction.compare("left") == 0)
		{
			glTranslated(dog.translation[2], dog.translation[1] + 11, -dog.translation[0] - 15);
			glRotated(-90, 0, 1, 0);
		}
		else if (dog.direction.compare("right") == 0)
		{
			glTranslated(-dog.translation[2], dog.translation[1] + 11, dog.translation[0] - 15);
			glRotated(90, 0, 1, 0);
		}
		else if (dog.direction.compare("up") == 0)
		{
			glTranslated(-dog.translation[0], dog.translation[1] + 11, -dog.translation[2] - 15);
			glRotated(0, 0, 1, 0);
		}
		else
		{
			glTranslated(dog.translation[0], dog.translation[1] + 11, dog.translation[2] - 15);
			glRotated(180, 0, 1, 0);
		}

	}
	else if (view.compare("train") == 0)
	{
		if (train.direction.compare("left") == 0)
		{
			glTranslated(train.translation[2], train.translation[1] + 12, -train.translation[0] - 22.5);
			glRotated(-90, 0, 1, 0);
		}
		else if (train.direction.compare("right") == 0)
		{
			glTranslated(-train.translation[2], train.translation[1] + 12, train.translation[0] - 22.5);
			glRotated(90, 0, 1, 0);
		}
		else if (train.direction.compare("up") == 0)
		{
			glTranslated(-train.translation[0], train.translation[1] + 12, -train.translation[2] - 22.5);
			glRotated(0, 0, 1, 0);
		}
		else
		{
			glTranslated(train.translation[0], train.translation[1] + 12, train.translation[2] - 22.5);
			glRotated(180, 0, 1, 0);
		}
	}
	else
	{
		if (officer.direction.compare("left") == 0)
		{
			glTranslated(officer.translation[2], officer.translation[1] + 5, -officer.translation[0] - 22.5);
			glRotated(-90, 0, 1, 0);
		}
		else if (officer.direction.compare("right") == 0)
		{
			glTranslated(-officer.translation[2], officer.translation[1] + 5, officer.translation[0] - 22.5);
			glRotated(90, 0, 1, 0);
		}
		else if (officer.direction.compare("up") == 0)
		{
			glTranslated(-officer.translation[0], officer.translation[1] + 5, -officer.translation[2] - 22.5);
			glRotated(0, 0, 1, 0);
		}
		else
		{
			glTranslated(officer.translation[0], officer.translation[1] + 5, officer.translation[2] - 22.5);
			glRotated(180, 0, 1, 0);
		}
	}

	objmode.compare("shading") == 0 ? glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) : glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_TEXTURE_2D);


	//set posisi dan gambar anjing
	glPushMatrix();
	glTranslated(dog.translation[0], dog.translation[1], dog.translation[2]);
	glRotatef(dog.rotation, 0, 1, 0);

	glRotatef(thetaDog[0], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, body_node.m);

	glLoadIdentity();
	glTranslatef(-6.5, 0.0, 0.0);
	glRotatef(thetaDog[1], 1.0, 0.0, 0.0);
	glRotatef(thetaDog[2], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, head_node.m);

	glLoadIdentity();
	glTranslatef(-4.5, 0.0, 0.5);
	glRotatef(thetaDog[3], 0.0, 0.0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lua_node.m);

	glLoadIdentity();
	glTranslatef(-4.5, 0.0, -0.5);
	glRotatef(thetaDog[5], 0.0, 0.0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, rua_node.m);

	glLoadIdentity();
	glTranslatef(-1.0, 0.0, 0.5);
	glRotatef(thetaDog[7], 0.0, 0.0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lul_node.m);

	glLoadIdentity();
	glTranslatef(-1.0, 0.0, -0.5);
	glRotatef(thetaDog[9], 0.0, 0.0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, rul_node.m);

	glLoadIdentity();
	glTranslatef(0.0, 3.0, 0.0);
	glRotatef(thetaDog[4], 0.0, 0.0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lla_node.m);

	glLoadIdentity();
	glTranslatef(0.0, 3.0, 0.0);
	glRotatef(thetaDog[6], 0.0, 0.0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, rla_node.m);

	glLoadIdentity();
	glTranslatef(0.0, 3.0, 0.0);
	glRotatef(thetaDog[8], 0.0, 0.0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lll_node.m);

	glLoadIdentity();
	glTranslatef(0.0, 3.0, 0.0);
	glRotatef(thetaDog[10], 0.0, 0.0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, rll_node.m);

	glLoadIdentity();
	glRotatef(thetaDog[11], 1.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, tail_node.m);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//gambar shadow
	glLoadIdentity();
	glMultMatrixf((GLfloat *)baseshadow);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glColor3f(0.0, 0.0, 0.0);
	traverse(&body_node);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glLoadIdentity();
	traverse(&body_node);
	glPopMatrix();

	//gambar spider
	glPushMatrix();
	glTranslated(spider.translation[0], spider.translation[1], spider.translation[2]);
	glRotatef(spider.rotation, 0, 1, 0);

	glRotatef(theta[0], 0.0, 1.0, 0.0);
	glRotatef(theta[9], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, t_node.m);

	glLoadIdentity();
	glTranslatef(-0.5 * TORSO_RADIUS, 0.5 * TORSO_LENGTH, 0.0);
	glRotatef(theta[1], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, ua1_node.m);

	glLoadIdentity();
	glTranslatef(0.5 * TORSO_RADIUS, 0.5 * TORSO_LENGTH, 0.0);
	glRotatef(theta[3], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, ua2_node.m);

	glLoadIdentity();
	glTranslatef(-0.5 * TORSO_RADIUS, -0.5 * TORSO_LENGTH, 0.0);
	glRotatef(theta[5], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, ua3_node.m);

	glLoadIdentity();
	glTranslatef(0.5 * TORSO_RADIUS, -0.5 * TORSO_LENGTH, 0.0);
	glRotatef(theta[7], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, ua4_node.m);

	glLoadIdentity();
	glTranslatef(-0.75 * UPPER_ARM_LENGTH, 0.0, 0.75 * UPPER_ARM_LENGTH);
	glRotatef(theta[2], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, la1_node.m);

	glLoadIdentity();
	glTranslatef(0.75 * UPPER_ARM_LENGTH, 0.0, 0.75 * UPPER_ARM_LENGTH);
	glRotatef(theta[4], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, la2_node.m);

	glLoadIdentity();
	glTranslatef(-0.75 * UPPER_ARM_LENGTH, 0.0, 0.75 * UPPER_ARM_LENGTH);
	glRotatef(theta[6], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, la3_node.m);

	glLoadIdentity();
	glTranslatef(0.75 * UPPER_ARM_LENGTH, 0.0, 0.75 * UPPER_ARM_LENGTH);
	glRotatef(theta[8], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, la4_node.m);

	glLoadIdentity();
	traverse(&t_node);

	glPopMatrix();

	//set posisi dan gambar train
	glPushMatrix();
	glTranslated(train.translation[0], train.translation[1], train.translation[2]);
	glRotatef(train.rotation, 0, 1, 0);

	glGetFloatv(GL_MODELVIEW_MATRIX, engine_node.m);

	glLoadIdentity();
	glTranslatef(3.5, -3, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, cab_node.m);

	glLoadIdentity();
	glTranslatef(-5, -3, 3.5);
	glRotatef(thetaTrain[0], 0.0, 0.0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lfw_node.m);

	glLoadIdentity();
	glTranslatef(0, -3, 3.5);
	glRotatef(thetaTrain[0], 0.0, 0.0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lrw_node.m);

	glLoadIdentity();
	glTranslatef(-5, -3, -3.5);
	glRotatef(thetaTrain[0], 0.0, 0.0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, rfw_node.m);

	glLoadIdentity();
	glTranslatef(0, -3, -3.5);
	glRotatef(thetaTrain[0], 0.0, 0.0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, rrw_node.m);

	glLoadIdentity();
	glTranslatef(-3.5, 3 + smokeStack, 0);
	glRotatef(thetaTrain[0], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, ss_node.m);

	glLoadIdentity();
	traverse(&engine_node);
	glPopMatrix();

	//set posisi dan gambar officer
	glPushMatrix();
	glTranslated(officer.translation[0], officer.translation[1], officer.translation[2]);
	glRotatef(officer.rotation + 90, 0, 1, 0);

	glRotatef(thetaOfficer[0], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, torso_node.m);

	glLoadIdentity();
	glTranslatef(0, 0.5*HEAD_HEIGHT, -1);
	glRotatef(thetaOfficer[1], 1.0, 0.0, 0.0);
	glRotatef(thetaOfficer[2], 0.0, 1.0, 0.0);
	glTranslatef(0.0, -0.5*HEAD_HEIGHT, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, head2_node.m);

	glLoadIdentity();
	glTranslatef(-(TORSO_RADIUS2 + UPPER_LEG_RADIUS), 0.1*UPPER_LEG_HEIGHT, TORSO_HEIGHT - 0.5);
	glRotatef(thetaOfficer[3], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lua2_node.m);

	glLoadIdentity();
	glTranslatef(TORSO_RADIUS2 + UPPER_LEG_RADIUS, 0.1*UPPER_LEG_HEIGHT, TORSO_HEIGHT - 0.5);
	glRotatef(thetaOfficer[5], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, rua2_node.m);

	glLoadIdentity();
	glTranslatef(-(TORSO_RADIUS2 + UPPER_LEG_RADIUS), 0.1*UPPER_LEG_HEIGHT, 0.5);
	glRotatef(thetaOfficer[7], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lul2_node.m);

	glLoadIdentity();
	glTranslatef(TORSO_RADIUS2 + UPPER_LEG_RADIUS, 0.1*UPPER_LEG_HEIGHT, 0.5);
	glRotatef(thetaOfficer[9], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, rul2_node.m);

	glLoadIdentity();
	glTranslatef(0.0, UPPER_LEG_HEIGHT, 0.0);
	glRotatef(thetaOfficer[4], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lla2_node.m);

	glLoadIdentity();
	glTranslatef(0.0, UPPER_LEG_HEIGHT, 0.0);
	glRotatef(thetaOfficer[6], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, rla2_node.m);

	glLoadIdentity();
	glTranslatef(0.0, UPPER_LEG_HEIGHT, 0.0);
	glRotatef(thetaOfficer[8], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lll2_node.m);

	glLoadIdentity();
	glTranslatef(0.0, UPPER_LEG_HEIGHT, 0.0);
	glRotatef(thetaOfficer[10], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, rll2_node.m);

	glLoadIdentity();
	traverse(&torso_node);
	glPopMatrix();

	//set posisi dan gambar meja
	glBindTexture(GL_TEXTURE_2D, texture[10]);
	glPushMatrix();
	glRotated(-90, 1, 0, 0);
	glTranslated(0, 0, -10);
	gluCylinder(lt, 10.0, 3.0, 10, 10, 10);
	glTranslated(0, 0, 10);
	gluCylinder(ut1, 10.0, 0, 0, 10, 10);
	gluCylinder(ut2, 10.0, 10.0, 1, 10, 10);
	glTranslated(0, 0, 1);
	gluCylinder(ut3, 10.0, 0, 0, 10, 10);
	glPopMatrix();

	//set posisi dan gambar teapot
	glBindTexture(GL_TEXTURE_2D, texture[11]);
	glPushMatrix();
	glTranslated(0, 3, 0);
	glutSolidTeapot(3.0);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//set posisi dan gambar lampu
	GLfloat light1_position[] = { -15.0, 10.0, -15.0, 1.0 };
	GLfloat light2_position[] = { 15.0, 10.0, -15.0, 1.0 };
	GLfloat light3_position[] = { lamp3.translation[0], lamp3.translation[1], lamp3.translation[2], 1.0 };
	GLfloat light4_position[] = { lamp4.translation[0], lamp4.translation[1], lamp4.translation[2], 1.0 };
	GLfloat spot_direction[] = { 0.0, -1.0, 0.0 };

	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 15.0);

	glLightfv(GL_LIGHT0, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT1, GL_POSITION, light2_position);
	glLightfv(GL_LIGHT2, GL_POSITION, light3_position);
	glLightfv(GL_LIGHT3, GL_POSITION, light4_position);

	glPushMatrix();
	glTranslatef(-15.0, 10.0, -15.0);
	glutSolidSphere(1, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15.0, 10.0, -15.0);
	glutSolidSphere(1, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(lamp3.translation[0], lamp3.translation[1], lamp3.translation[2]);
	glutSolidSphere(1, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(lamp4.translation[0], lamp4.translation[1], lamp4.translation[2]);
	glutSolidSphere(1, 20, 20);
	glPopMatrix();

	//set posisi dan gambar lantai
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[9]);
	GLfloat v0[3], v1[3], v2[3], v3[3];
	glBegin(GL_POLYGON);
	glNormal3f(0, 1, 0);
	v0[1] = v1[1] = v2[1] = v3[1] = -10.5f;
	v0[0] = v0[2] = v1[0] = v3[2] = -32.75;
	v1[2] = v2[0] = v2[2] = v3[0] = 32.75;
	glTexCoord2f(0.0, 0.0);
	glVertex3fv(v0);
	glTexCoord2f(0.0, 1.0);
	glVertex3fv(v1);
	glTexCoord2f(1.0, 0.0);
	glVertex3fv(v2);
	glTexCoord2f(1.0, 1.0);
	glVertex3fv(v3);
	glEnd();

	glLoadIdentity();
	glutSwapBuffers();
}

void menu(int id)
{
	if (id == 0) exit(0);
}

void myReshape(int w, int h)
{
	GLdouble aspect_ratio; // width-height ratio

	if (0 >= w || 0 >= h)
	{
		return;
	}

	glViewport(0, 0, w, h);

	aspect_ratio = (GLdouble)w / (GLdouble)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(40.0f, aspect_ratio, 0.5f, 300.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void myinit()
{
	initObjectPos();

	//Settingan lampu
	GLfloat light_specular[] = { 1, 1, 1, 1.0 };
	GLfloat light_diffuse[] = { 1, 1, 1, 1.0 };

	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse);

	//setting lampu 4 sebagai lampu sorot


	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.5, 0.5, 0.5, 0);

	/* allocate quadrics with filled drawing style */
	//quadric untuk objek spider
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

	//quadric untuk objek anjing
	h = gluNewQuadric();
	gluQuadricDrawStyle(h, GLU_FILL);
	gluQuadricTexture(h, GL_TRUE);
	b = gluNewQuadric();
	gluQuadricDrawStyle(b, GLU_FILL);
	gluQuadricTexture(b, GL_TRUE);
	lua = gluNewQuadric();
	gluQuadricDrawStyle(lua, GLU_FILL);
	gluQuadricTexture(lua, GL_TRUE);
	lla = gluNewQuadric();
	gluQuadricDrawStyle(lla, GLU_FILL);
	gluQuadricTexture(lla, GL_TRUE);
	rua = gluNewQuadric();
	gluQuadricDrawStyle(rua, GLU_FILL);
	gluQuadricTexture(rua, GL_TRUE);
	rla = gluNewQuadric();
	gluQuadricDrawStyle(rla, GLU_FILL);
	gluQuadricTexture(rla, GL_TRUE);
	lul = gluNewQuadric();
	gluQuadricDrawStyle(lul, GLU_FILL);
	gluQuadricTexture(lul, GL_TRUE);
	lll = gluNewQuadric();
	gluQuadricDrawStyle(lll, GLU_FILL);
	gluQuadricTexture(lll, GL_TRUE);
	rul = gluNewQuadric();
	gluQuadricDrawStyle(rul, GLU_FILL);
	gluQuadricTexture(rul, GL_TRUE);
	rll = gluNewQuadric();
	gluQuadricDrawStyle(rll, GLU_FILL);
	gluQuadricTexture(rll, GL_TRUE);
	t1 = gluNewQuadric();
	gluQuadricDrawStyle(t1, GLU_FILL);
	gluQuadricTexture(t1, GL_TRUE);

	//quadric untuk kereta
	e = gluNewQuadric();
	gluQuadricDrawStyle(e, GLU_FILL);
	gluQuadricTexture(e, GL_TRUE);
	c = gluNewQuadric();
	gluQuadricDrawStyle(c, GLU_FILL);
	gluQuadricTexture(c, GL_TRUE);
	lfw = gluNewQuadric();
	gluQuadricDrawStyle(lfw, GLU_FILL);
	gluQuadricTexture(lfw, GL_TRUE);
	lrw = gluNewQuadric();
	gluQuadricDrawStyle(lrw, GLU_FILL);
	gluQuadricTexture(lrw, GL_TRUE);
	rfw = gluNewQuadric();
	gluQuadricDrawStyle(rfw, GLU_FILL);
	gluQuadricTexture(rfw, GL_TRUE);
	rrw = gluNewQuadric();
	gluQuadricDrawStyle(rrw, GLU_FILL);
	gluQuadricTexture(rrw, GL_TRUE);
	ss = gluNewQuadric();
	gluQuadricDrawStyle(ss, GLU_FILL);
	gluQuadricTexture(ss, GL_TRUE);

	//quadric untuk officer
	h2 = gluNewQuadric();
	gluQuadricDrawStyle(h2, GLU_FILL);
	gluQuadricTexture(h2, GL_TRUE);
	t2 = gluNewQuadric();
	gluQuadricDrawStyle(t2, GLU_FILL);
	gluQuadricTexture(t2, GL_TRUE);
	lua2 = gluNewQuadric();
	gluQuadricDrawStyle(lua2, GLU_FILL);
	gluQuadricTexture(lua2, GL_TRUE);
	lla2 = gluNewQuadric();
	gluQuadricDrawStyle(lla2, GLU_FILL);
	gluQuadricTexture(lla2, GL_TRUE);
	rua2 = gluNewQuadric();
	gluQuadricDrawStyle(rua2, GLU_FILL);
	gluQuadricTexture(rua2, GL_TRUE);
	rla2 = gluNewQuadric();
	gluQuadricDrawStyle(rla2, GLU_FILL);
	gluQuadricTexture(rla2, GL_TRUE);
	lul2 = gluNewQuadric();
	gluQuadricDrawStyle(lul2, GLU_FILL);
	gluQuadricTexture(lul2, GL_TRUE);
	lll2 = gluNewQuadric();
	gluQuadricDrawStyle(lll2, GLU_FILL);
	gluQuadricTexture(lll2, GL_TRUE);
	rul2 = gluNewQuadric();
	gluQuadricDrawStyle(rul2, GLU_FILL);
	gluQuadricTexture(rul2, GL_TRUE);
	rll2 = gluNewQuadric();
	gluQuadricDrawStyle(rll2, GLU_FILL);
	gluQuadricTexture(rll2, GL_TRUE);

	//quadric untuk meja
	ut1 = gluNewQuadric();
	gluQuadricDrawStyle(ut1, GLU_FILL);
	gluQuadricTexture(ut1, GL_TRUE);
	ut2 = gluNewQuadric();
	gluQuadricDrawStyle(ut2, GLU_FILL);
	gluQuadricTexture(ut2, GL_TRUE);
	ut3 = gluNewQuadric();
	gluQuadricDrawStyle(ut3, GLU_FILL);
	gluQuadricTexture(ut3, GL_TRUE);
	lt = gluNewQuadric();
	gluQuadricDrawStyle(lt, GLU_FILL);
	gluQuadricTexture(lt, GL_TRUE);

	//set tree untuk spider
	t_node.f = torso;
	t_node.sibling = NULL;
	t_node.child = &ua1_node;

	ua1_node.f = upper_arm_1;
	ua1_node.sibling = &ua2_node;
	ua1_node.child = &la1_node;

	ua2_node.f = upper_arm_2;
	ua2_node.sibling = &ua3_node;
	ua2_node.child = &la2_node;

	ua3_node.f = upper_arm_3;
	ua3_node.sibling = &ua4_node;
	ua3_node.child = &la3_node;

	ua4_node.f = upper_arm_4;
	ua4_node.sibling = NULL;
	ua4_node.child = &la4_node;

	la1_node.f = lower_arm_1;
	la1_node.sibling = NULL;
	la1_node.child = NULL;

	la2_node.f = lower_arm_2;
	la2_node.sibling = NULL;
	la2_node.child = NULL;

	la3_node.f = lower_arm_3;
	la3_node.sibling = NULL;
	la3_node.child = NULL;

	la4_node.f = lower_arm_4;
	la4_node.sibling = NULL;
	la4_node.child = NULL;

	//set tree untuk anjing
	body_node.f = body;
	body_node.sibling = NULL;
	body_node.child = &head_node;

	head_node.f = head;
	head_node.sibling = &lua_node;
	head_node.child = NULL;

	lua_node.f = left_upper_arm;
	lua_node.sibling = &rua_node;
	lua_node.child = &lla_node;

	rua_node.f = right_upper_arm;
	rua_node.sibling = &lul_node;
	rua_node.child = &rla_node;

	lul_node.f = left_upper_leg;
	lul_node.sibling = &rul_node;
	lul_node.child = &lll_node;

	rul_node.f = right_upper_leg;
	rul_node.sibling = &tail_node;
	rul_node.child = &rll_node;

	tail_node.f = tail;
	tail_node.sibling = NULL;
	tail_node.child = NULL;

	lla_node.f = left_lower_arm;
	lla_node.sibling = NULL;
	lla_node.child = NULL;

	rla_node.f = right_lower_arm;
	rla_node.sibling = NULL;
	rla_node.child = NULL;

	lll_node.f = left_lower_leg;
	lll_node.sibling = NULL;
	lll_node.child = NULL;

	rll_node.f = right_lower_leg;
	rll_node.sibling = NULL;
	rll_node.child = NULL;

	//set tree untuk lokomotif
	glLoadIdentity();
	engine_node.f = engine;
	engine_node.sibling = NULL;
	engine_node.child = &cab_node;

	cab_node.f = cab;
	cab_node.sibling = &lfw_node;
	cab_node.child = NULL;

	lfw_node.f = left_front_wheel;
	lfw_node.sibling = &lrw_node;
	lfw_node.child = NULL;

	lrw_node.f = left_rear_wheel;
	lrw_node.sibling = &rfw_node;
	lrw_node.child = NULL;

	rfw_node.f = right_front_wheel;
	rfw_node.sibling = &rrw_node;
	rfw_node.child = NULL;

	rrw_node.f = right_rear_wheel;
	rrw_node.sibling = &ss_node;
	rrw_node.child = NULL;

	ss_node.f = smoke_stack;
	ss_node.sibling = NULL;
	ss_node.child = NULL;

	//set tree untuk officer
	torso_node.f = torso2;
	torso_node.sibling = NULL;
	torso_node.child = &head2_node;

	head2_node.f = head2;
	head2_node.sibling = &lua2_node;
	head2_node.child = NULL;

	lua2_node.f = left_upper_arm2;
	lua2_node.sibling = &rua2_node;
	lua2_node.child = &lla2_node;

	rua2_node.f = right_upper_arm2;
	rua2_node.sibling = &lul2_node;
	rua2_node.child = &rla2_node;

	lul2_node.f = left_upper_leg2;
	lul2_node.sibling = &rul2_node;
	lul2_node.child = &lll2_node;

	rul2_node.f = right_upper_leg2;
	rul2_node.sibling = NULL;
	rul2_node.child = &rll2_node;

	lla2_node.f = left_lower_leg2;
	lla2_node.sibling = NULL;
	lla2_node.child = NULL;

	rla2_node.f = right_lower_arm2;
	rla2_node.sibling = NULL;
	rla2_node.child = NULL;

	lll2_node.f = left_lower_leg2;
	lll2_node.sibling = NULL;
	lll2_node.child = NULL;

	rll2_node.f = right_lower_leg2;
	rll2_node.sibling = NULL;
	rll2_node.child = NULL;

	//cari plane equation dan shadow matrix
	GLfloat v0[3] = { 0.0, 0.0, 0.0 };
	GLfloat v1[3] = { 1.0, 0.0, 0.0 };
	GLfloat v2[3] = { 0.0, 0.0, 1.0 };

	findplane(plane, v0, v1, v2);

	glLoadIdentity();
}

void key(unsigned char key, int x, int y) {
	//untuk mengontrol pergerakan
	if (control.compare("camera") == 0)
	{
		switch (key) {
		case 'f':
			camera.translation[2] += 0.25;
			break;
		case 'b':
			camera.translation[2] -= 0.25;
			break;
		}
	}
	else if (control.compare("lamp 3") == 0)
	{
		switch (key) {
		case 'f':
			lamp3.translation[2] -= 0.25;
			break;
		case 'b':
			lamp3.translation[2] += 0.25;
			break;
		}
	}
	else
	{
		switch (key) {
		case 'f':
			lamp4.translation[2] -= 0.25;
			break;
		case 'b':
			lamp4.translation[2] += 0.25;
			break;
		}
	}
}

void specialKey(int key, int x, int y)
{
	//untuk mengontrol pergerakan
	if (control.compare("camera") == 0)
	{
		switch (key) {
		case GLUT_KEY_LEFT:
			camera.translation[0] += 0.25;
			break;
		case GLUT_KEY_RIGHT:
			camera.translation[0] -= 0.25;
			break;
		case GLUT_KEY_UP:
			camera.translation[1] -= 0.25;
			break;
		case GLUT_KEY_DOWN:
			camera.translation[1] += 0.25;
			break;
		}
	}
	else if (control.compare("dog") == 0)
	{
		switch (key) {
		case GLUT_KEY_LEFT:
			dog.direction = "left";
			dog.translation[0] -= 0.25;
			dog.rotation = 0;
			break;
		case GLUT_KEY_RIGHT:
			dog.direction = "right";
			dog.translation[0] += 0.25;
			dog.rotation = 180;
			break;
		case GLUT_KEY_UP:
			dog.direction = "up";
			dog.translation[2] -= 0.25;
			dog.rotation = -90;
			break;
		case GLUT_KEY_DOWN:
			dog.direction = "down";
			dog.translation[2] += 0.25;
			dog.rotation = 90;
			break;
		}
	}
	else if (control.compare("train") == 0)
	{
		switch (key) {
		case GLUT_KEY_LEFT:
			train.direction = "left";
			train.translation[0] -= 0.25;
			train.rotation = 0;
			break;
		case GLUT_KEY_RIGHT:
			train.direction = "right";
			train.translation[0] += 0.25;
			train.rotation = 180;
			break;
		case GLUT_KEY_UP:
			train.direction = "up";
			train.translation[2] -= 0.25;
			train.rotation = -90;
			break;
		case GLUT_KEY_DOWN:
			train.direction = "down";
			train.translation[2] += 0.25;
			train.rotation = 90;
			break;
		}
	}
	else if (control.compare("officer") == 0)
	{
		switch (key) {
		case GLUT_KEY_LEFT:
			officer.direction = "left";
			officer.translation[0] -= 0.25;
			officer.rotation = 0;
			break;
		case GLUT_KEY_RIGHT:
			officer.direction = "right";
			officer.translation[0] += 0.25;
			officer.rotation = 180;
			break;
		case GLUT_KEY_UP:
			officer.direction = "up";
			officer.translation[2] -= 0.25;
			officer.rotation = -90;
			break;
		case GLUT_KEY_DOWN:
			officer.direction = "down";
			officer.translation[2] += 0.25;
			officer.rotation = 90;
			break;
		}
	}
	else if (control.compare("lamp 3") == 0)
	{
		switch (key) {
		case GLUT_KEY_LEFT:
			lamp3.translation[0] -= 0.25;
			break;
		case GLUT_KEY_RIGHT:
			lamp3.translation[0] += 0.25;
			break;
		case GLUT_KEY_UP:
			lamp3.translation[1] += 0.25;
			break;
		case GLUT_KEY_DOWN:
			lamp3.translation[1] -= 0.25;
			break;
		}
	}
	else
	{
		switch (key) {
		case GLUT_KEY_LEFT:
			lamp4.translation[0] -= 0.25;
			break;
		case GLUT_KEY_RIGHT:
			lamp4.translation[0] += 0.25;
			break;
		case GLUT_KEY_UP:
			lamp4.translation[1] += 0.25;
			break;
		case GLUT_KEY_DOWN:
			lamp4.translation[1] -= 0.25;
			break;
		}
	}

}

void initObjectPos()
{
	//posisi semula kamera
	camera.translation[2] = -75;

	//posisi semula spider
	spider.direction = "left";
	spider.translation[0] = -25;
	spider.translation[1] = -10.0;
	spider.translation[2] = 20;
	spider.rotation = 0;
	theta[0] = 90.0;
	theta[9] = -90.0;

	//posisi semula anjing
	thetaDog[3] = 180.0;
	thetaDog[4] = 0.0;
	thetaDog[5] = 180.0;
	thetaDog[6] = 0.0;
	thetaDog[7] = 180.0;
	thetaDog[8] = 0.0;
	thetaDog[9] = 180.0;
	thetaDog[10] = 0.0;
	thetaDog[11] = 90.0;
	dog.direction = "left";
	dog.translation[0] = -10;
	dog.translation[1] = -5.5;
	dog.translation[2] = 20;
	dog.rotation = 0;

	//posisi semula train
	thetaTrain[0] = 0;
	smokeStack = 0;
	train.direction = "left";
	train.translation[0] = 15;
	train.translation[1] = -6.5;
	train.translation[2] = 20;
	train.rotation = 0;

	//posisi semula officer
	thetaOfficer[3] = 90.0;
	thetaOfficer[5] = 90.0;
	officer.direction = "left";
	officer.translation[0] = 0;
	officer.translation[1] = -1.0;
	officer.translation[2] = 20;
	officer.rotation = 0;

	//posisi semula lampu
	lamp3.translation[0] = 10;
	lamp3.translation[1] = 0;
	lamp3.translation[2] = 25;
	lamp4.translation[0] = -20;
	lamp4.translation[1] = 20;
	lamp4.translation[2] = 20;
}

void animation()
{
	if (mode.compare("animation") == 0) {
		//animasi anjing
		for (int i = 0; i < 4; i++) {
			if (thetaDog[2 * i + 3] > 225) forwardDog[i] = false;
			if (thetaDog[2 * i + 3] < 135) forwardDog[i] = true;
			if (forwardDog[i]) thetaDog[2 * i + 3] += 2.5;
			else thetaDog[2 * i + 3] -= 2.5;
		}
		if (thetaDog[11] > 135) forwardDog[4] = false;
		if (thetaDog[11] < 45) forwardDog[4] = true;
		if (forwardDog[4]) thetaDog[11] += 2.5;
		else thetaDog[11] -= 2.5;

		if (dog.direction.compare("left") == 0 && dog.translation[0] <= -20.0)
		{
			dog.direction = "up";
			dog.rotation = -90;
		}
		if (dog.direction.compare("up") == 0 && dog.translation[2] <= -20.0)
		{
			dog.direction = "right";
			dog.rotation = 180;
		}
		if (dog.direction.compare("right") == 0 && dog.translation[0] >= 20.0)
		{
			dog.direction = "down";
			dog.rotation = 90;
		}
		if (dog.direction.compare("down") == 0 && dog.translation[2] >= 20.0)
		{
			dog.direction = "left";
			dog.rotation = 0;
		}

		if (dog.direction.compare("left") == 0) dog.translation[0] -= 0.1;
		else if (dog.direction.compare("up") == 0) dog.translation[2] -= 0.1;
		else if (dog.direction.compare("right") == 0) dog.translation[0] += 0.1;
		else dog.translation[2] += 0.1;

		//animasi spider
		if(forwardSpider)
		{
			theta[1] += 1.0;
			theta[7] -= 1.0;

			if(theta[5] >= 0.0)
			{
				theta[3] += 1.0;
				theta[5] -= 1.0;
			}

			if(theta[1] >= 45.0)
			{
				forwardSpider = false;
			}
		}
		else
		{
			theta[1] -= 1.0;
			theta[7] += 1.0;
			theta[3] -= 1.0;
			theta[5] += 1.0;

			if(theta[1] <= 0.0)
			{
				forwardSpider = true;
			}
		}

		if (spider.direction.compare("left") == 0 && spider.translation[0] <= -20.0)
		{
			spider.direction = "up";
			spider.rotation = -90;
		}
		if (spider.direction.compare("up") == 0 && spider.translation[2] <= -20.0)
		{
			spider.direction = "right";
			spider.rotation = 180;
		}
		if (spider.direction.compare("right") == 0 && spider.translation[0] >= 20.0)
		{
			spider.direction = "down";
			spider.rotation = 90;
		}
		if (spider.direction.compare("down") == 0 && spider.translation[2] >= 20.0)
		{
			spider.direction = "left";
			spider.rotation = 0;
		}

		if (spider.direction.compare("left") == 0) spider.translation[0] -= 0.1;
		else if (spider.direction.compare("up") == 0) spider.translation[2] -= 0.1;
		else if (spider.direction.compare("right") == 0) spider.translation[0] += 0.1;
		else spider.translation[2] += 0.1;

		//animasi train
		thetaTrain[0] += 2.5;
		if (smokeStack >= 0) topStack = true;
		if (smokeStack <= -1) topStack = false;

		if (topStack) smokeStack -= 0.05;
		else smokeStack += 0.05;

		for (int i = 0; i < 2; i++) {
			if (thetaOfficer[2 * i + 3] > 135) forwardOfficer[i] = false;
			if (thetaOfficer[2 * i + 3] < 45) forwardOfficer[i] = true;
			if (forwardOfficer[i]) thetaOfficer[2 * i + 3] += 2.5;
			else thetaOfficer[2 * i + 3] -= 2.5;
		}

		if (train.direction.compare("left") == 0 && train.translation[0] <= -20.0)
		{
			train.direction = "up";
			train.rotation = -90;
		}
		if (train.direction.compare("up") == 0 && train.translation[2] <= -20.0)
		{
			train.direction = "right";
			train.rotation = 180;
		}
		if (train.direction.compare("right") == 0 && train.translation[0] >= 20.0)
		{
			train.direction = "down";
			train.rotation = 90;
		}
		if (train.direction.compare("down") == 0 && train.translation[2] >= 20.0)
		{
			train.direction = "left";
			train.rotation = 0;
		}

		if (train.direction.compare("left") == 0) train.translation[0] -= 0.1;
		else if (train.direction.compare("up") == 0) train.translation[2] -= 0.1;
		else if (train.direction.compare("right") == 0) train.translation[0] += 0.1;
		else train.translation[2] += 0.1;

		//animasi officer
		for (int i = 0; i < 2; i++) {
			if (thetaOfficer[2 * i + 3] > 135) forwardOfficer[i] = false;
			if (thetaOfficer[2 * i + 3] < 45) forwardOfficer[i] = true;
			if (forwardOfficer[i]) thetaOfficer[2 * i + 3] += 2.5;
			else thetaOfficer[2 * i + 3] -= 2.5;
		}

		if (officer.direction.compare("left") == 0 && officer.translation[0] <= -20.0)
		{
			officer.direction = "up";
			officer.rotation = -90;
		}
		if (officer.direction.compare("up") == 0 && officer.translation[2] <= -20.0)
		{
			officer.direction = "right";
			officer.rotation = 180;
		}
		if (officer.direction.compare("right") == 0 && officer.translation[0] >= 20.0)
		{
			officer.direction = "down";
			officer.rotation = 90;
		}
		if (officer.direction.compare("down") == 0 && officer.translation[2] >= 20.0)
		{
			officer.direction = "left";
			officer.rotation = 0;
		}

		if (officer.direction.compare("left") == 0) officer.translation[0] -= 0.1;
		else if (officer.direction.compare("up") == 0) officer.translation[2] -= 0.1;
		else if (officer.direction.compare("right") == 0) officer.translation[0] += 0.1;
		else officer.translation[2] += 0.1;
	}
	glutPostRedisplay();
}

bool LoadTextureRAW(int wrap, GLuint * texture)
{
	int width, height;
	BYTE * data;
	FILE * file;

	// open texture data
	file = fopen("fur1.raw", "rb");
	if (file == NULL) return 0;

	// allocate buffer
	width = 256;
	height = 256;
	data = (unsigned char *)malloc(width * height * 3); //3 for each R,G,B

														// read texture data
	fread(data, width * height * 3, 1, file);
	//fclose(file);

	// allocate a texture name
	glGenTextures(12, texture);

	// select our current texture
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// select modulate to mix texture with color for shading
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// when texture area is small, bilinear filter the closest MIP map
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	// when texture area is large, bilinear filter the first MIP map
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// if wrap is true, the texture wraps over at the edges (repeat)
	//       ... false, the texture ends at the edges (clamp)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP);

	// build our texture MIP maps
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	file = fopen("fur2.raw", "rb");
	if (file == NULL) return 0;
	fread(data, width * height * 3, 1, file);
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	file = fopen("fur3.raw", "rb");
	if (file == NULL) return 0;
	fread(data, width * height * 3, 1, file);
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	file = fopen("texture1.raw", "rb");
	if (file == NULL) return 0;
	fread(data, width * height * 3, 1, file);
	glBindTexture(GL_TEXTURE_2D, texture[3]);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	file = fopen("texture2.raw", "rb");
	if (file == NULL) return 0;
	fread(data, width * height * 3, 1, file);
	glBindTexture(GL_TEXTURE_2D, texture[4]);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	file = fopen("texture3.raw", "rb");
	if (file == NULL) return 0;
	fread(data, width * height * 3, 1, file);
	glBindTexture(GL_TEXTURE_2D, texture[5]);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	file = fopen("scalp.raw", "rb");
	if (file == NULL) return 0;
	fread(data, width * height * 3, 1, file);
	glBindTexture(GL_TEXTURE_2D, texture[6]);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	file = fopen("hand-skin.raw", "rb");
	if (file == NULL) return 0;
	fread(data, width * height * 3, 1, file);
	glBindTexture(GL_TEXTURE_2D, texture[7]);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	file = fopen("shirt.raw", "rb");
	if (file == NULL) return 0;
	fread(data, width * height * 3, 1, file);
	glBindTexture(GL_TEXTURE_2D, texture[8]);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	file = fopen("texture-asphalt.raw", "rb");
	if (file == NULL) return 0;
	fread(data, width * height * 3, 1, file);
	glBindTexture(GL_TEXTURE_2D, texture[9]);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	file = fopen("texture-meja.raw", "rb");
	if (file == NULL) return 0;
	fread(data, width * height * 3, 1, file);
	glBindTexture(GL_TEXTURE_2D, texture[10]);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	file = fopen("texture-teapot.raw", "rb");
	if (file == NULL) return 0;
	fread(data, width * height * 3, 1, file);
	glBindTexture(GL_TEXTURE_2D, texture[11]);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	free(data);

	return 1;
}

void viewOptions(int id)
{
	switch (id)
	{
	case 0:
		view = "camera";
		break;
	case 1:
		view = "dog";
		break;
	case 2:
		view = "train";
		break;
	case 3:
		view = "officer";
		break;
	}
}

void controlOptions(int id)
{
	switch (id)
	{
	case 0:
		control = "camera";
		break;
	case 1:
		control = "dog";
		break;
	case 2:
		control = "train";
		break;
	case 3:
		control = "officer";
		break;
	case 4:
		control = "lamp 3";
		break;
	case 5:
		control = "lamp 4";
		break;
	}
}

void lampOptions(int id)
{

}

void lamp1Options(int id)
{
	id == 0 ? glDisable(GL_LIGHT0) : glEnable(GL_LIGHT0);
}

void lamp2Options(int id)
{
	id == 0 ? glDisable(GL_LIGHT1) : glEnable(GL_LIGHT1);
}

void lamp3Options(int id)
{
	id == 0 ? glDisable(GL_LIGHT2) : glEnable(GL_LIGHT2);
}

void lamp4Options(int id)
{
	id == 0 ? glDisable(GL_LIGHT3) : glEnable(GL_LIGHT3);
}

void objmodeOptions(int id)
{
	objmode = id == 0 ? "shading" : "wireframe";
}

void modeOptions(int id)
{
	mode = id == 0 ? "animation" : "interactive";
	initObjectPos();
	if (id == 0) {
		thetaDog[0] = 0.0;
		thetaDog[1] = 0.0;
		thetaDog[2] = 0.0;
		thetaDog[3] = 180.0;
		thetaDog[4] = 45.0;
		thetaDog[5] = 180.0;
		thetaDog[6] = 45.0;
		thetaDog[7] = 180.0;
		thetaDog[8] = 45.0;
		thetaDog[9] = 180.0;
		thetaDog[10] = 45.0;

		forwardDog[0] = true;
		forwardDog[1] = false;
		forwardDog[2] = true;
		forwardDog[3] = false;

		forwardOfficer[0] = true;
		forwardOfficer[1] = false;
	}
}

void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(900, 600);
	glutCreateWindow("robot binatang");
	myinit();
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);

	int view_menu, control_menu, lamp_menu, lamp1_menu, lamp2_menu, lamp3_menu, lamp4_menu, objmode_menu, mode_menu;

	view_menu = glutCreateMenu(viewOptions);
	glutAddMenuEntry("Camera", 0);
	glutAddMenuEntry("Dog", 1);
	glutAddMenuEntry("Train", 2);
	glutAddMenuEntry("Officer", 3);

	control_menu = glutCreateMenu(controlOptions);
	glutAddMenuEntry("Camera", 0);
	glutAddMenuEntry("Dog", 1);
	glutAddMenuEntry("Train", 2);
	glutAddMenuEntry("Officer", 3);
	glutAddMenuEntry("Lamp 3", 4);
	glutAddMenuEntry("Lamp 4", 5);

	lamp1_menu = glutCreateMenu(lamp1Options);
	glutAddMenuEntry("Off", 0);
	glutAddMenuEntry("On", 1);

	lamp2_menu = glutCreateMenu(lamp2Options);
	glutAddMenuEntry("Off", 0);
	glutAddMenuEntry("On", 1);

	lamp3_menu = glutCreateMenu(lamp3Options);
	glutAddMenuEntry("Off", 0);
	glutAddMenuEntry("On", 1);

	lamp4_menu = glutCreateMenu(lamp4Options);
	glutAddMenuEntry("Off", 0);
	glutAddMenuEntry("On", 1);

	lamp_menu = glutCreateMenu(lampOptions);
	glutAddSubMenu("Lamp 1", lamp1_menu);
	glutAddSubMenu("Lamp 2", lamp2_menu);
	glutAddSubMenu("Lamp 3", lamp3_menu);
	glutAddSubMenu("Lamp 4", lamp4_menu);

	objmode_menu = glutCreateMenu(objmodeOptions);
	glutAddMenuEntry("Shading", 0);
	glutAddMenuEntry("Wireframe", 1);

	mode_menu = glutCreateMenu(modeOptions);
	glutAddMenuEntry("Animation", 0);
	glutAddMenuEntry("Interactive", 1);

	glutCreateMenu(menu);
	glutAddSubMenu("View", view_menu);
	glutAddSubMenu("Control", control_menu);
	glutAddSubMenu("Lamp", lamp_menu);
	glutAddSubMenu("Object Mode", objmode_menu);
	glutAddSubMenu("Mode", mode_menu);

	glutAddMenuEntry("Quit", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutSpecialFunc(specialKey);
	glutKeyboardFunc(key);
	glutIdleFunc(animation);

	// load and check our texture from file
	bool loaded;
	loaded = LoadTextureRAW(TRUE, texture);
	if (!loaded) printf("Texture File not found !");

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glutMainLoop();
}