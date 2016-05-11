/* Worksheet 2 Grafkom						*/
/* Modified figure.c by E. Angel from Scele	*/

/* Mgs. M. Rizqi Fadhlurrahman				*/
/* 1306464543								*/

#include <glut.h>
#include <stdio.h>

#define EYE_RADIUS 0.23
#define EYE_HEIGHT 1.0
#define UPPER_NECK_RADIUS 0.15
#define UPPER_NECK_HEIGHT 0.5
#define LOWER_NECK_RADIUS 0.16
#define LOWER_NECK_HEIGHT 0.75
#define BODY_RADIUS 1.2
#define BODY_HEIGHT 1.5
#define UPPER_ARM_RADIUS 0.18
#define UPPER_ARM_HEIGHT 1.5
#define LOWER_ARM_RADIUS 0.22
#define LOWER_ARM_HEIGHT 0.4
#define WHEEL_RADIUS 0.6
#define WHEEL_HEIGHT 0.5

typedef float point[3];
bool animate = false;
int animationState = 0;

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

GLUquadricObj *le, *re, *un, *ln, *b, *rua, *lua, *rla, *lla, *rw, *lw;

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

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glColor3f(0.0, 0.0, 0.0);
	traverse(&b_node);
	glFlush();
	glutSwapBuffers();
}

void idle() {
	glLoadIdentity();
	// If animation toggle started
	if (animate) {
		// Change theta so it can animate the parts
		if (animationState == 0) {
			if (theta[1] < 25.0) {
				theta[1] += 0.04;
				theta[2] -= 0.03;
				theta[3] += 0.03;
				theta[5] += 0.01;
				theta[6] -= 0.01;
				bodyTranslate += 0.0001;
			}
			else {
				animationState = 1;
			}
		}
		else if (animationState == 1) {
			if (theta[1] > 0.0) {
				theta[1] -= 0.04;
				theta[2] += 0.03;
				theta[3] -= 0.03;
				theta[5] -= 0.01;
				theta[6] += 0.01;
				bodyTranslate -= 0.0001;
			}
			else {
				animationState = 0;
			}
		}
		theta[9] += 0.1;
		theta[10] += 0.1;
		// Animate the parts
		// Body
		glLoadIdentity();
		glTranslatef(0.0, bodyTranslate, 0.0);
		glRotatef(theta[4], 0.0, 1.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, b_node.m);
		// Neck
		glLoadIdentity();
		glTranslatef(0.0, 0.5*LOWER_NECK_HEIGHT, 0.0);
		glRotatef(theta[2], 1.0, 0.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, un_node.m);
		glLoadIdentity();
		glRotatef(theta[3], 1.0, 0.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, ln_node.m);
		// Eyes
		glLoadIdentity();
		glTranslatef(-0.75*UPPER_NECK_RADIUS, -UPPER_NECK_HEIGHT, -0.65*EYE_HEIGHT);
		glRotatef(-theta[1], 0.0, 0.0, 1.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, re_node.m);
		glLoadIdentity();
		glTranslatef(0.75*UPPER_NECK_RADIUS, -UPPER_NECK_HEIGHT, -0.65*EYE_HEIGHT);
		glRotatef(theta[1], 0.0, 0.0, 1.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, le_node.m);
		// Hands
		glLoadIdentity();
		glRotatef(theta[5], 1.0, 0.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, rua_node.m);
		glLoadIdentity();
		glRotatef(theta[6], 1.0, 0.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, lua_node.m);
		// Wheels
		glLoadIdentity();
		glTranslatef(-0.7*BODY_RADIUS - WHEEL_HEIGHT, -0.9*BODY_HEIGHT, 0.0);
		glTranslatef(0.0, -bodyTranslate, 0.0);
		glRotatef(theta[9], 1.0, 0.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, rw_node.m);
		glLoadIdentity();
		glTranslatef(0.7*BODY_RADIUS, -0.9*BODY_HEIGHT, 0.0);
		glTranslatef(0.0, -bodyTranslate, 0.0);
		glRotatef(theta[10], 1.0, 0.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, lw_node.m);
	}
	glutPostRedisplay();
}

void mouse(int btn, int state, int x, int y) {
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		theta[angle] += 5.0;
		if (theta[angle] > 360.0) theta[angle] -= 360.0;
	}
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		theta[angle] -= 5.0;
		if (theta[angle] < 360.0) theta[angle] += 360.0;
	}
	printf("%f %f\n", angle, theta[angle]);
	glPushMatrix();
	switch (angle) {
	case 1:
		glLoadIdentity();
		glTranslatef(-0.75*UPPER_NECK_RADIUS, -UPPER_NECK_HEIGHT, -0.65*EYE_HEIGHT);
		glRotatef(-theta[1], 0.0, 0.0, 1.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, re_node.m);
		glLoadIdentity();
		glTranslatef(0.75*UPPER_NECK_RADIUS, -UPPER_NECK_HEIGHT, -0.65*EYE_HEIGHT);
		glRotatef(theta[1], 0.0, 0.0, 1.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, le_node.m);
		break;
	case 2:
		glLoadIdentity();
		glTranslatef(0.0, 0.5*LOWER_NECK_HEIGHT, 0.0);
		glRotatef(theta[2], 1.0, 0.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, un_node.m);
		break;
	case 3:
		glLoadIdentity();
		glRotatef(theta[3], 1.0, 0.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, ln_node.m);
		break;
	case 4:
		glLoadIdentity();
		glRotatef(theta[4], 0.0, 1.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, b_node.m);
		break;
	case 5:
		glLoadIdentity();
		glRotatef(theta[5], 1.0, 0.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, rua_node.m);
		break;
	case 6:
		glLoadIdentity();
		glRotatef(theta[6], 1.0, 0.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, lua_node.m);
		break;
	case 7:
		glLoadIdentity();
		glTranslatef(-0.81*BODY_RADIUS, -0.15*BODY_HEIGHT, 0.9*UPPER_ARM_HEIGHT);
		glRotatef(theta[7], 0.0, 1.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, rla_node.m);
		break;
	case 8:
		glLoadIdentity();
		glTranslatef(0.81*BODY_RADIUS, -0.15*BODY_HEIGHT, 0.9*UPPER_ARM_HEIGHT);
		glRotatef(theta[8], 0.0, 1.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, lla_node.m);
		break;
	case 9:
		glLoadIdentity();
		glTranslatef(-0.7*BODY_RADIUS - WHEEL_HEIGHT, -0.9*BODY_HEIGHT, 0.0);
		glRotatef(theta[9], 1.0, 0.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, rw_node.m);
		break;
	case 10:
		glLoadIdentity();
		glTranslatef(0.7*BODY_RADIUS, -0.9*BODY_HEIGHT, 0.0);
		glRotatef(theta[10], 1.0, 0.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, lw_node.m);
		break;
	}
	glPopMatrix();
	glutPostRedisplay();
}

void menu(int id) {
	if (id == 0) {
		// Reset all except body
		theta[1] = 0.0;
		theta[2] = 140.0;
		theta[3] = 40.0;
		theta[5] = 0.0;
		theta[6] = 0.0;
		theta[7] = 0.0;
		theta[8] = 0.0;
		theta[9] = 0.0;
		theta[10] = 0.0;
		animate = true;
	}
	else if (id < 11) {
		angle = id;
		animate = false;
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
	glOrtho(-3.0 * aspect_ratio, 3.0 * aspect_ratio, -3.0, 3.0, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void myinit() {
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess = { 100.0 };
	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 10.0, 10.0, 10.0, 0.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
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

void main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("WS2 - Wall-E");
	myinit();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutIdleFunc(idle);

	glutCreateMenu(menu);
	glutAddMenuEntry("Toggle Animasi", 0);
	glutAddMenuEntry("Eyes", 1);
	glutAddMenuEntry("Upper Neck", 2);
	glutAddMenuEntry("Lower Neck", 3);
	glutAddMenuEntry("Body", 4);
	glutAddMenuEntry("Right Upper Arm", 5);
	glutAddMenuEntry("Left Upper Arm", 6);
	glutAddMenuEntry("Right Lower Arm", 7);
	glutAddMenuEntry("Left Lower Arm", 8);
	glutAddMenuEntry("Right Wheel", 9);
	glutAddMenuEntry("Left Wheel", 10);
	glutAddMenuEntry("Quit", 11);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);

	glutMainLoop();
}