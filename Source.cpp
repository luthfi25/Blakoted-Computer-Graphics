/* Source code for Grafkom Project Assignment   */
/* modified from figuretr.c   */
/* E. Angel, Interactive Computer Graphics */
/* A Top-Down Approach with OpenGL, Third Edition */
/* Addison-Wesley Longman, 2003 */

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
//Object train;
Object wallE;
Object officer;
Object lamp3;
Object lamp4;
Object spider;

#define NULL 0
#define TORSO_LENGTH 6.25
#define TORSO_RADIUS 1.5
#define UPPER_ARM_LENGTH 3.75
#define LOWER_ARM_LENGTH 2.5
#define ARM_RADIUS 0.5

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

#define EYE_RADIUS 0.7
#define EYE_HEIGHT 3.0
#define UPPER_NECK_RADIUS 0.45
#define UPPER_NECK_HEIGHT 1.5
#define LOWER_NECK_RADIUS 0.5
#define LOWER_NECK_HEIGHT 2.25
#define BODY_RADIUS 3.6
#define BODY_HEIGHT 4.5
#define UPPER_ARM_RADIUS 0.9
#define UPPER_ARM_HEIGHT 4.5
#define LOWER_ARM_RADIUS 0.66
#define LOWER_ARM_HEIGHT 1.2
#define WHEEL_RADIUS 1.8
#define WHEEL_HEIGHT 1.5

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
static GLfloat thetaSpider[10] = { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 };
bool forwardSpider = true;
//static GLfloat thetaTrain[2] = { 0.0, 0.0 };
//GLfloat smokeStack = 0;
//bool topStack = true;
static GLfloat thetaWallE[11] = { 0.0, 0.0, 140.0, 40.0, -90.0,0.0,0.0,0.0,0.0,0.0,0.0 };
bool stateWallE = true;
float walleBodyTranslate = 0.0;
static GLfloat thetaOfficer[12] = { 90.0, 0.0, 0.0, 90.0, 0.0, 90.0, 0.0, 180.0, 0.0, 180.0, 0.0, 90.0 };
bool forwardOfficer[2];


//quadric spider
GLUquadricObj *t, *ua1, *la1, *ua2, *la2, *ua3, *la3, *ua4, *la4, *te;
//quadric kereta
//GLUquadricObj *e, *c, *lfw, *lrw, *rfw, *rrw, *ss;
GLUquadricObj *le, *re, *un, *ln, *b, *rua, *lua, *rla, *lla, *rw, *lw;
//quadric officer
GLUquadricObj *t2, *h2, *lua2, *lla2, *rua2, *rla2, *lll2, *rll2, *rul2, *lul2;
//quadric meja
GLUquadricObj *ut1, *ut2, *ut3, *lt;

treenode t_node, ua1_node, la1_node, ua2_node, la2_node, ua3_node, la3_node, ua4_node, la4_node;
//treenode engine_node, cab_node, lfw_node, lrw_node, rfw_node, rrw_node, ss_node;
treenode re_node, le_node, un_node, ln_node, b_node, rua_node, lua_node, rla_node, lla_node, rw_node, lw_node;
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

float i_global = 0.0;
const float DEG2RAD = 3.14159/180;

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

/*
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
*/

//Method objek Wall E
void wallErightEye() {
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

void wallEleftEye() {
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

void wallEupperNeck() {
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

void wallElowerNeck() {
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

void wallEbody() {
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

void wallErightUpperArm() {
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

void wallEleftUpperArm() {
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

void wallErightLowerArm() {
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

void wallEleftLowerArm() {
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

void wallErightWheel() {
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

void wallEleftWheel() {
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
	gluSphere(h2, 1.0, 10, 10);
	glPopMatrix();
}

void left_upper_arm2()
{
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(lua2, UPPER_LEG_RADIUS, UPPER_LEG_RADIUS, UPPER_LEG_HEIGHT, 10, 10);
	glPopMatrix();
}

void left_lower_arm2()
{
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(lla2, LOWER_LEG_RADIUS, LOWER_LEG_RADIUS, LOWER_LEG_HEIGHT, 10, 10);
	glPopMatrix();
}

void right_upper_arm2()
{
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(rua2, UPPER_LEG_RADIUS, UPPER_LEG_RADIUS, UPPER_LEG_HEIGHT, 10, 10);
	glPopMatrix();
}

void right_lower_arm2()
{
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(rla2, LOWER_LEG_RADIUS, LOWER_LEG_RADIUS, LOWER_LEG_HEIGHT, 10, 10);
	glPopMatrix();
}

void left_upper_leg2()
{
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(lul2, UPPER_LEG_RADIUS, UPPER_LEG_RADIUS, UPPER_LEG_HEIGHT, 10, 10);
	glPopMatrix();
}

void left_lower_leg2()
{
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(lll2, LOWER_LEG_RADIUS, LOWER_LEG_RADIUS, LOWER_LEG_HEIGHT, 10, 10);
	glPopMatrix();
}

void right_upper_leg2()
{
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(rul2, UPPER_LEG_RADIUS, UPPER_LEG_RADIUS, UPPER_LEG_HEIGHT, 10, 10);
	glPopMatrix();
}

void right_lower_leg2()
{
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(rll2, LOWER_LEG_RADIUS, LOWER_LEG_RADIUS, LOWER_LEG_HEIGHT, 10, 10);
	glPopMatrix();
}

//method objek spider
void torso()
{
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glPushMatrix();
	glScalef(TORSO_RADIUS, TORSO_LENGTH, TORSO_RADIUS);
	gluSphere(t, 1.0, 10, 10);
	glPopMatrix();
}

void upper_arm_1()
{
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glPushMatrix();
	glRotatef(-45.0, 0.0, 1.0, 0.0);
	gluCylinder(ua1, ARM_RADIUS, ARM_RADIUS, UPPER_ARM_LENGTH, 10, 10);
	glPopMatrix();
}

void lower_arm_1()
{
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glPushMatrix();
	glRotatef(-135.0, 0.0, 1.0, 0.0);
	gluCylinder(la1, ARM_RADIUS, ARM_RADIUS, LOWER_ARM_LENGTH, 10, 10);
	glPopMatrix();
}

void upper_arm_2()
{
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glPushMatrix();
	glRotatef(45.0, 0.0, 1.0, 0.0);
	gluCylinder(ua2, ARM_RADIUS, ARM_RADIUS, UPPER_ARM_LENGTH, 10, 10);
	glPopMatrix();
}

void lower_arm_2()
{
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glPushMatrix();
	glRotatef(135.0, 0.0, 1.0, 0.0);
	gluCylinder(la2, ARM_RADIUS, ARM_RADIUS, LOWER_ARM_LENGTH, 10, 10);
	glPopMatrix();
}

void upper_arm_3()
{
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glPushMatrix();
	glRotatef(-45.0, 0.0, 1.0, 0.0);
	gluCylinder(ua3, ARM_RADIUS, ARM_RADIUS, UPPER_ARM_LENGTH, 10, 10);
	glPopMatrix();

}

void lower_arm_3()
{
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glPushMatrix();
	glRotatef(-135.0, 0.0, 1.0, 0.0);
	gluCylinder(la3, ARM_RADIUS, ARM_RADIUS, LOWER_ARM_LENGTH, 10, 10);
	glPopMatrix();
}

void upper_arm_4()
{
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glPushMatrix();
	glRotatef(45.0, 0.0, 1.0, 0.0);
	gluCylinder(ua4, ARM_RADIUS, ARM_RADIUS, UPPER_ARM_LENGTH, 10, 10);
	glPopMatrix();
}

void lower_arm_4()
{
	glBindTexture(GL_TEXTURE_2D, texture[0]);
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
	else if (view.compare("spider") == 0)
	{
		if (spider.direction.compare("left") == 0)
		{
			glTranslated(spider.translation[2], spider.translation[1] + 15, -spider.translation[0] - 7.5);
			glRotated(-90, 0, 1, 0);
		}
		else if (spider.direction.compare("right") == 0)
		{
			glTranslated(-spider.translation[2], spider.translation[1] + 15, spider.translation[0] - 7.5);
			glRotated(90, 0, 1, 0);
		}
		else if (spider.direction.compare("up") == 0)
		{
			glTranslated(-spider.translation[0], spider.translation[1] + 15, -spider.translation[2] - 7.5);
			glRotated(0, 0, 1, 0);
		}
		else
		{
			glTranslated(spider.translation[0], spider.translation[1] + 15, spider.translation[2] - 7.5);
			glRotated(180, 0, 1, 0);
		}

	}/*
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
	}*/
	else if (view.compare("wallE") == 0)
	{
		if (wallE.direction.compare("left") == 0)
		{
			glTranslated(wallE.translation[2], wallE.translation[1] + 12, -wallE.translation[0] - 22.5);
			glRotated(-90, 0, 1, 0);
		}
		else if (wallE.direction.compare("right") == 0)
		{
			glTranslated(-wallE.translation[2], wallE.translation[1] + 12, wallE.translation[0] - 22.5);
			glRotated(90, 0, 1, 0);
		}
		else if (wallE.direction.compare("up") == 0)
		{
			glTranslated(-wallE.translation[0], wallE.translation[1] + 12, -wallE.translation[2] - 22.5);
			glRotated(0, 0, 1, 0);
		}
		else
		{
			glTranslated(wallE.translation[0], wallE.translation[1] + 12, wallE.translation[2] - 22.5);
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set posisi dan gambar spider
	glPushMatrix();
	glTranslated(spider.translation[0], spider.translation[1], spider.translation[2]);
	glRotatef(spider.rotation, 0, 1, 0);

	glRotatef(thetaSpider[0], 0.0, 1.0, 0.0);
	glRotatef(thetaSpider[9], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, t_node.m);

	glLoadIdentity();
	glTranslatef(-0.5 * TORSO_RADIUS, 0.5 * TORSO_LENGTH, 0.0);
	glRotatef(thetaSpider[1], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, ua1_node.m);

	glLoadIdentity();
	glTranslatef(0.5 * TORSO_RADIUS, 0.5 * TORSO_LENGTH, 0.0);
	glRotatef(thetaSpider[3], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, ua2_node.m);

	glLoadIdentity();
	glTranslatef(-0.5 * TORSO_RADIUS, -0.5 * TORSO_LENGTH, 0.0);
	glRotatef(thetaSpider[5], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, ua3_node.m);

	glLoadIdentity();
	glTranslatef(0.5 * TORSO_RADIUS, -0.5 * TORSO_LENGTH, 0.0);
	glRotatef(thetaSpider[7], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, ua4_node.m);

	glLoadIdentity();
	glTranslatef(-0.75 * UPPER_ARM_LENGTH, 0.0, 0.75 * UPPER_ARM_LENGTH);
	glRotatef(thetaSpider[2], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, la1_node.m);

	glLoadIdentity();
	glTranslatef(0.75 * UPPER_ARM_LENGTH, 0.0, 0.75 * UPPER_ARM_LENGTH);
	glRotatef(thetaSpider[4], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, la2_node.m);

	glLoadIdentity();
	glTranslatef(-0.75 * UPPER_ARM_LENGTH, 0.0, 0.75 * UPPER_ARM_LENGTH);
	glRotatef(thetaSpider[6], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, la3_node.m);

	glLoadIdentity();
	glTranslatef(0.75 * UPPER_ARM_LENGTH, 0.0, 0.75 * UPPER_ARM_LENGTH);
	glRotatef(thetaSpider[8], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, la4_node.m);

	glLoadIdentity();
	traverse(&t_node);

	glPopMatrix();
	/*
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
	*/

	//set posisi dan gambar walle
	glPushMatrix();
	glTranslated(wallE.translation[0], wallE.translation[1], wallE.translation[2]);
	glRotatef(wallE.rotation, 0, 1, 0);

	// Body
	//glLoadIdentity();
	glTranslatef(0.0, walleBodyTranslate, 0.0);
	glRotatef(thetaWallE[4], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, b_node.m);
	// Neck
	glLoadIdentity();
	glTranslatef(0.0, 0.5*LOWER_NECK_HEIGHT, 0.0);
	glRotatef(thetaWallE[2], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, un_node.m);
	glLoadIdentity();
	glRotatef(thetaWallE[3], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, ln_node.m);
	// Eyes
	glLoadIdentity();
	glTranslatef(-0.75*UPPER_NECK_RADIUS, -UPPER_NECK_HEIGHT, -0.65*EYE_HEIGHT);
	glRotatef(-thetaWallE[1], 0.0, 0.0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, re_node.m);
	glLoadIdentity();
	glTranslatef(0.75*UPPER_NECK_RADIUS, -UPPER_NECK_HEIGHT, -0.65*EYE_HEIGHT);
	glRotatef(thetaWallE[1], 0.0, 0.0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, le_node.m);
	// Hands
	glLoadIdentity();
	glRotatef(thetaWallE[5], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, rua_node.m);
	glLoadIdentity();
	glRotatef(thetaWallE[6], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lua_node.m);
	// Wheels
	glLoadIdentity();
	glTranslatef(-0.7*BODY_RADIUS - WHEEL_HEIGHT, -0.9*BODY_HEIGHT, 0.0);
	glTranslatef(0.0, -walleBodyTranslate, 0.0);
	glRotatef(thetaWallE[9], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, rw_node.m);
	glLoadIdentity();
	glTranslatef(0.7*BODY_RADIUS, -0.9*BODY_HEIGHT, 0.0);
	glTranslatef(0.0, -walleBodyTranslate, 0.0);
	glRotatef(thetaWallE[10], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lw_node.m);

	glLoadIdentity();
	traverse(&b_node);
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
	glutSolidCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15.0, 10.0, -15.0);
	glutSolidCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(lamp3.translation[0], lamp3.translation[1], lamp3.translation[2]);
	glutSolidCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(lamp4.translation[0], lamp4.translation[1], lamp4.translation[2]);
	glutSolidCube(2);
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);

	//rotate texture
	glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glTranslatef(0.5,0.5,0.0);
    glRotatef(90.0,0.0,1.0,0.0);
    glTranslatef(-0.5,-0.5,0.0);
    glMatrixMode(GL_MODELVIEW);

    //set posisi dan gambar lanta
     glBindTexture(GL_TEXTURE_2D, texture[9]);
     glBegin(GL_POLYGON);

   for (int i=0; i <= 360; i++)
   {
      float degInRad = i*DEG2RAD;
      float xcos = cos(degInRad);
      float ysin = sin(degInRad);
      float tx = xcos * 0.5 + 0.5;
	  float ty = ysin * 0.5 + 0.5;

      glTexCoord2f(tx, ty);
      glVertex3f(xcos*33, -10.5, ysin*33);
   }

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
	gluQuadricTexture(t, GL_TRUE);
	ua1 = gluNewQuadric();
	gluQuadricDrawStyle(ua1, GLU_FILL);
	gluQuadricTexture(ua1, GL_TRUE);
	la1 = gluNewQuadric();
	gluQuadricDrawStyle(la1, GLU_FILL);
	gluQuadricTexture(la1, GL_TRUE);
	ua2 = gluNewQuadric();
	gluQuadricDrawStyle(ua2, GLU_FILL);
	gluQuadricTexture(ua2, GL_TRUE);
	la2 = gluNewQuadric();
	gluQuadricDrawStyle(la2, GLU_FILL);
	gluQuadricTexture(la2, GL_TRUE);
	ua3 = gluNewQuadric();
	gluQuadricDrawStyle(ua3, GLU_FILL);
	gluQuadricTexture(ua3, GL_TRUE);
	la3 = gluNewQuadric();
	gluQuadricDrawStyle(la3, GLU_FILL);
	gluQuadricTexture(la3, GL_TRUE);
	ua4 = gluNewQuadric();
	gluQuadricDrawStyle(ua4, GLU_FILL);
	gluQuadricTexture(ua4, GL_TRUE);
	la4 = gluNewQuadric();
	gluQuadricDrawStyle(la4, GLU_FILL);
	gluQuadricTexture(la4, GL_TRUE);
	/*
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
	*/

	//quadric untuk walle
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
	/*
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
	*/

	//set tree untuk walle
	glLoadIdentity();
	b_node.f = wallEbody;
	b_node.sibling = 0;
	b_node.child = &ln_node;

	ln_node.f = wallElowerNeck;
	ln_node.sibling = &rua_node;
	ln_node.child = &un_node;

	un_node.f = wallEupperNeck;
	un_node.sibling = 0;
	un_node.child = &re_node;

	re_node.f = wallErightEye;
	re_node.sibling = &le_node;
	re_node.child = 0;

	le_node.f = wallEleftEye;
	le_node.sibling = 0;
	le_node.child = 0;

	rua_node.f = wallErightUpperArm;
	rua_node.sibling = &lua_node;
	rua_node.child = &rla_node;

	rla_node.f = wallErightLowerArm;
	rla_node.sibling = 0;
	rla_node.child = 0;

	lua_node.f = wallEleftUpperArm;
	lua_node.sibling = &lw_node;
	lua_node.child = &lla_node;

	lla_node.f = wallEleftLowerArm;
	lla_node.sibling = 0;
	lla_node.child = 0;

	lw_node.f = wallEleftWheel;
	lw_node.sibling = &rw_node;
	lw_node.child = 0;

	
	rw_node.f = wallErightWheel;
	rw_node.sibling = 0;
	rw_node.child = 0;

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
		case 'w':
			camera.translation[2] += 0.25;
			break;
		case 's':
			camera.translation[2] -= 0.25;
			break;
		}
	}
	else if (control.compare("lamp 3") == 0)
	{
		switch (key) {
		case 'w':
			lamp3.translation[2] -= 0.25;
			break;
		case 's':
			lamp3.translation[2] += 0.25;
			break;
		}
	}
	else
	{
		switch (key) {
		case 'w':
			lamp4.translation[2] -= 0.25;
			break;
		case 's':
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
	else if (control.compare("spider") == 0)
	{
		switch (key) {
		case GLUT_KEY_LEFT:
			spider.direction = "left";
			spider.translation[0] -= 0.25;
			spider.rotation = 0;
			break;
		case GLUT_KEY_RIGHT:
			spider.direction = "right";
			spider.translation[0] += 0.25;
			spider.rotation = 180;
			break;
		case GLUT_KEY_UP:
			spider.direction = "up";
			spider.translation[2] -= 0.25;
			spider.rotation = -90;
			break;
		case GLUT_KEY_DOWN:
			spider.direction = "down";
			spider.translation[2] += 0.25;
			spider.rotation = 90;
			break;
		}
	}/*
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
	}*/
	else if (control.compare("wallE") == 0)
	{
		switch (key) {
		case GLUT_KEY_LEFT:
			wallE.direction = "left";
			wallE.translation[0] -= 0.25;
			wallE.rotation = 0;
			break;
		case GLUT_KEY_RIGHT:
			wallE.direction = "right";
			wallE.translation[0] += 0.25;
			wallE.rotation = 180;
			break;
		case GLUT_KEY_UP:
			wallE.direction = "up";
			wallE.translation[2] -= 0.25;
			wallE.rotation = -90;
			break;
		case GLUT_KEY_DOWN:
			wallE.direction = "down";
			wallE.translation[2] += 0.25;
			wallE.rotation = 90;
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
	spider.translation[0] = -10;
	spider.translation[1] = -10.0;
	spider.translation[2] = 20;
	spider.rotation = 0;
	thetaSpider[0] = 90.0;
	thetaSpider[1] = 0.0;
	thetaSpider[2] = 0.0;
	thetaSpider[3] = 0.0;
	thetaSpider[4] = 0.0;
	thetaSpider[5] = 0.0;
	thetaSpider[6] = 0.0;
	thetaSpider[7] = 0.0;
	thetaSpider[8] = 0.0;
	thetaSpider[9] = -90.0;

	/*
	//posisi semula train
	thetaTrain[0] = 0;
	smokeStack = 0;
	train.direction = "left";
	train.translation[0] = 15;
	train.translation[1] = -6.5;
	train.translation[2] = 20;
	train.rotation = 0;
	*/

	//posisi semula walle
	wallE.direction = "left";
	wallE.translation[0] = 15;
	wallE.translation[1] = -4.7;
	wallE.translation[2] = 20;
	wallE.rotation = 0;
	thetaWallE[1] = 0.0;
	thetaWallE[2] = 140.0;
	thetaWallE[3] = 40.0;
	thetaWallE[5] = 0.0;
	thetaWallE[6] = 0.0;
	thetaWallE[7] = 0.0;
	thetaWallE[8] = 0.0;
	thetaWallE[9] = 0.0;
	thetaWallE[10] = 0.0;


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
		float degInRad, xcos, ysin;

		//animasi spider
		if(forwardSpider)
		{
			thetaSpider[1] += 2.0;
			thetaSpider[7] -= 2.0;

			if(thetaSpider[5] >= 0.0)
			{
				thetaSpider[3] += 2.0;
				thetaSpider[5] -= 2.0;
			}

			if(thetaSpider[1] >= 45.0)
			{
				forwardSpider = false;
			}
		}
		else
		{
			thetaSpider[1] -= 2.0;
			thetaSpider[7] += 2.0;
			thetaSpider[3] -= 2.0;
			thetaSpider[5] += 2.0;

			if(thetaSpider[1] <= 0.0)
			{
				forwardSpider = true;
			}
		}

	    degInRad = i_global*DEG2RAD;
	    xcos = cos(degInRad);
	    ysin = sin(degInRad);

	    spider.translation[0] = xcos*22;
	    spider.translation[2] = ysin*22;
	    spider.rotation = -i_global + 90.0;
		/*
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

		degInRad = (i_global-90.0)*DEG2RAD;
	    xcos = cos(degInRad);
	    ysin = sin(degInRad);

	    train.translation[0] = xcos*22;
	    train.translation[2] = ysin*22;
	    train.rotation = -(i_global-90.0) + 90.0;
		*/
		//Animasi walle
		if (stateWallE == 0) {
			if (thetaWallE[1] < 25.0) {
				thetaWallE[1] += 0.04;
				thetaWallE[2] -= 0.03;
				thetaWallE[3] += 0.03;
				thetaWallE[5] += 0.01;
				thetaWallE[6] -= 0.01;
				walleBodyTranslate += 0.0001;
			}
			else {
				stateWallE = 1;
			}
		}
		else if (stateWallE == 1) {
			if (thetaWallE[1] > 0.0) {
				thetaWallE[1] -= 0.04;
				thetaWallE[2] += 0.03;
				thetaWallE[3] -= 0.03;
				thetaWallE[5] -= 0.01;
				thetaWallE[6] += 0.01;
				walleBodyTranslate -= 0.0001;
			}
			else {
				stateWallE = 0;
			}
		}
		thetaWallE[9] += 0.1;
		thetaWallE[10] += 0.1;

		degInRad = (i_global - 90.0)*DEG2RAD;
		xcos = cos(degInRad);
		ysin = sin(degInRad);

		wallE.translation[0] = xcos * 22;
		wallE.translation[2] = ysin * 22;
		wallE.rotation = -(i_global - 90.0) + 90.0;

		//animasi officer
		for (int i = 0; i < 2; i++) {
			if (thetaOfficer[2 * i + 3] > 135) forwardOfficer[i] = false;
			if (thetaOfficer[2 * i + 3] < 45) forwardOfficer[i] = true;
			if (forwardOfficer[i]) thetaOfficer[2 * i + 3] += 2.5;
			else thetaOfficer[2 * i + 3] -= 2.5;
		}

		degInRad = (i_global-45.0)*DEG2RAD;
	    xcos = cos(degInRad);
	    ysin = sin(degInRad);

	    officer.translation[0] = xcos*22;
	    officer.translation[2] = ysin*22;
	    officer.rotation = -(i_global-45.0) + 90.0;

		//update i_global
		i_global += 0.25;
	    if(i_global >= 360.0) i_global = 0.0;
	}
	glutPostRedisplay();
}

bool LoadTextureRAW(int wrap, GLuint * texture)
{
	int width, height;
	BYTE * data;
	FILE * file;
	// allocate buffer
	width = 256;
	height = 256;
	data = (unsigned char *)malloc(width * height * 3); //3 for each R,G,B

														// read texture data

	// open texture data
	file = fopen("spider.raw", "rb");
	if (file == NULL) return 0;

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

	file = fopen("tiles.raw", "rb");
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
		view = "spider";
		break;
	case 2:
		//view = "train";
		view = "wallE";
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
		control = "spider";
		break;
	case 2:
		//control = "train";
		control = "wallE";
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
		forwardOfficer[0] = true;
		forwardOfficer[1] = false;

		thetaSpider[0] = 90.0;
		thetaSpider[1] = 0.0;
		thetaSpider[2] = 0.0;
		thetaSpider[3] = 0.0;
		thetaSpider[4] = 0.0;
		thetaSpider[5] = 0.0;
		thetaSpider[6] = 0.0;
		thetaSpider[7] = 0.0;
		thetaSpider[8] = 0.0;
		thetaSpider[9] = -90.0;

		thetaWallE[1] = 0.0;
		thetaWallE[2] = 140.0;
		thetaWallE[3] = 40.0;
		thetaWallE[5] = 0.0;
		thetaWallE[6] = 0.0;
		thetaWallE[7] = 0.0;
		thetaWallE[8] = 0.0;
		thetaWallE[9] = 0.0;
		thetaWallE[10] = 0.0;

		i_global = 135.0;
	}
}

void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(900, 600);
	glutCreateWindow("Tugas Akhir - Blakoted");
	myinit();
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);

	int view_menu, control_menu, lamp_menu, lamp1_menu, lamp2_menu, lamp3_menu, lamp4_menu, objmode_menu, mode_menu;

	view_menu = glutCreateMenu(viewOptions);
	glutAddMenuEntry("Camera", 0);
	glutAddMenuEntry("spider", 1);
	glutAddMenuEntry("WallE", 2);
	glutAddMenuEntry("Officer", 3);

	control_menu = glutCreateMenu(controlOptions);
	glutAddMenuEntry("Camera", 0);
	glutAddMenuEntry("spider", 1);
	glutAddMenuEntry("WallE", 2);
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
	loaded = LoadTextureRAW(FALSE, texture);
	if (!loaded) printf("Texture File not found !");

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glutMainLoop();
}
