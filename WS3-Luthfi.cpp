//Worksheet 2 - Muhammad Luthfi - 1306386825
//Bersama Mgs. M. Rizqi Fadhlurrahman
//Manual penggunaan dapat dilihat pada readme-WS2-Luthfi.txt
//Source Code dibuat dengan inspirasi dari figuretr.c


#include <glut.h>

#define TORSO_LENGTH 5.0
#define TORSO_RADIUS 1.5
#define UPPER_ARM_LENGTH 3.0
#define LOWER_ARM_LENGTH 2.0
#define ARM_RADIUS 0.5
#define NULL 0

void applyNewAngle(void);
void animation(void);

double size = 1.0;
GLUquadricObj *t, *ua1, *la1, *ua2, *la2, *ua3, *la3, *ua4, *la4, *dka, *db, *dki;

//array penyimpan angle
static GLfloat theta[9] = { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 };
static GLfloat thetax[9] = { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 };
static GLint angle = 0;

//nilai angle sementara untuk animasi
GLfloat newTheta = 0.0;

//variabel boolean untuk keperluan animasi dan pergeseran lampu
bool lampuGeser = false;
bool animasi = false;

bool naik = true;
bool baru = true;

//variabel penyimpan posisi lampu
GLfloat lampuX = 10.0;
GLfloat lampuY = 10.0;
GLfloat lampuZ = 10.0;

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

void bidangKiri(void)
{
	return;
}

void bidangBelakang(void)
{
	return;
}

void bidangKanan(void)
{
	return;
}

void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glColor3f(1.0, 0.0, 0.0);

	traverse(&t_node);

	bidangKiri();
	bidangBelakang();
	bidangKanan();

	//Draw posisi lampu
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(lampuX, lampuY, lampuZ);
	glutSolidCube(1.0);
	glPopMatrix();

	glutSwapBuffers();
}

void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-10.0, 10.0, -10.0 * (GLfloat)h / (GLfloat)w,
			10.0 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
	else
		glOrtho(-10.0 * (GLfloat)w / (GLfloat)h,
			10.0 * (GLfloat)w / (GLfloat)h, 0.0, 10.0, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//Method untuk menginisialisasi cahaya
void setLampu()
{
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess = { 100.0 };
	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { lampuX, lampuY, lampuZ, 1.0 };

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

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glColor3f(1.0, 0.0, 0.0);
}

void myinit()
{
	setLampu();

	/* allocate quadrics with filled drawing style */

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
	glTranslatef(0.75 * UPPER_ARM_LENGTH, 0.0, 0.75 * UPPER_ARM_LENGTH);
	glRotatef(theta[8], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, la4_node.m);
	la4_node.f = lower_arm_4;
	la4_node.sibling = NULL;
	la4_node.child = NULL;

	glLoadIdentity();

}

//method special func untuk melakukan rotasi pada hirarki atau pergeseran posisi lampu
void geser(int key, int xmouse, int ymouse)
{
	if (animasi)
		return;

	if (lampuGeser)
	{
		//pergesaran lampu mencakup atas,bawah,kiri, dan kanan
		switch (key)
		{
		case GLUT_KEY_RIGHT:
			lampuX += 1;
			if (lampuX > 10.0) lampuX = 10.0;
			break;
		case GLUT_KEY_LEFT:
			lampuX -= 1;
			if (lampuX < -10.0) lampuX = -10.0;
			break;
		case GLUT_KEY_UP:
			lampuY += 1;
			if (lampuY > 10.0) lampuY = 10.0;
			break;
		case GLUT_KEY_DOWN:
			lampuY -= 1;
			if (lampuY < -10.0) lampuY = -10.0;
			break;
		}
		setLampu();
	}
	else
	{
		//rotasi bagian hirarki
		switch (key)
		{
		case GLUT_KEY_RIGHT:
			theta[angle] += 5.0;
			if (theta[angle] > 360.0) theta[angle] -= 360.0;
			break;
		case GLUT_KEY_LEFT:
			theta[angle] -= 5.0;
			if (theta[angle] < 0.0) theta[angle] += 360.0;
			break;
		}
	}

	applyNewAngle();
	glutPostRedisplay();
}

void menu(int id)
{
	if (id < 9 && id >= 0)
	{
		angle = id;
		lampuGeser = false;
	}
	else if (id == 9) lampuGeser = !lampuGeser;
	else if (id == 10)
	{
		//toggle animasi
		animasi = !animasi;

		if (animasi)
		{
			glutIdleFunc(animation);
		}
		else
		{
			myinit();
			glutIdleFunc(NULL);
		}

	}
	else if (id == 11) exit(0);
}

//method untuk menerapkan angle baru pada poros y
void applyNewAngle(void)
{
	glPushMatrix();
	switch (angle)
	{

	case 0:
		glLoadIdentity();
		glRotatef(theta[0], 0.0, 1.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, t_node.m);
		break;

	case 1:
		glLoadIdentity();
		glTranslatef(-0.5 * TORSO_RADIUS, 0.5 * TORSO_LENGTH, 0.0);
		glRotatef(theta[1], 0.0, 1.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, ua1_node.m);
		break;

	case 2:
		glLoadIdentity();
		glTranslatef(-0.75 * UPPER_ARM_LENGTH, 0.0, 0.75 * UPPER_ARM_LENGTH);
		glRotatef(theta[2], 0.0, 1.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, la1_node.m);
		break;

	case 3:
		glLoadIdentity();
		glTranslatef(0.5 * TORSO_RADIUS, 0.5 * TORSO_LENGTH, 0.0);
		glRotatef(theta[3], 0.0, 1.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, ua2_node.m);
		break;

	case 4:
		glLoadIdentity();
		glTranslatef(0.75 * UPPER_ARM_LENGTH, 0.0, 0.75 * UPPER_ARM_LENGTH);
		glRotatef(theta[4], 0.0, 1.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, la2_node.m);
		break;

	case 5:
		glLoadIdentity();
		glTranslatef(-0.5 * TORSO_RADIUS, -0.5 * TORSO_LENGTH, 0.0);
		glRotatef(theta[5], 0.0, 1.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, ua3_node.m);
		break;

	case 6:
		glLoadIdentity();
		glTranslatef(-0.75 * UPPER_ARM_LENGTH, 0.0, 0.75 * UPPER_ARM_LENGTH);
		glRotatef(theta[6], 0.0, 1.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, la3_node.m);
		break;

	case 7:
		glLoadIdentity();
		glTranslatef(0.5 * TORSO_RADIUS, -0.5 * TORSO_LENGTH, 0.0);
		glRotatef(theta[7], 0.0, 1.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, ua4_node.m);
		break;

	case 8:
		glLoadIdentity();
		glTranslatef(0.75 * UPPER_ARM_LENGTH, 0.0, 0.75 * UPPER_ARM_LENGTH);
		glRotatef(theta[8], 0.0, 1.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, la4_node.m);
		break;
	}
	glPopMatrix();
}

//method untuk menerapkan angle baru pada poros z (buat animasi)
void applyNewAnglex(void)
{
	glPushMatrix();
	switch (angle)
	{
	case 1:
		glLoadIdentity();
		glTranslatef(-0.5 * TORSO_RADIUS, 0.5 * TORSO_LENGTH, 0.0);
		glRotatef(thetax[1], 0.0, 0.0, 1.0);
		glRotatef(-thetax[1], 0.0, 1.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, ua1_node.m);
		break;

	case 3:
		glLoadIdentity();
		glTranslatef(0.5 * TORSO_RADIUS, 0.5 * TORSO_LENGTH, 0.0);
		glRotatef(thetax[3], 0.0, 0.0, 1.0);
		glRotatef(thetax[3], 0.0, 1.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, ua2_node.m);
		break;

	case 5:
		glLoadIdentity();
		glTranslatef(-0.5 * TORSO_RADIUS, -0.5 * TORSO_LENGTH, 0.0);
		glRotatef(thetax[5], 0.0, 0.0, 1.0);
		glRotatef(-thetax[5], 0.0, 1.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, ua3_node.m);
		break;

	case 7:
		glLoadIdentity();
		glTranslatef(0.5 * TORSO_RADIUS, -0.5 * TORSO_LENGTH, 0.0);
		glRotatef(thetax[7], 0.0, 0.0, 1.0);
		glRotatef(thetax[7], 0.0, 1.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, ua4_node.m);
		break;
	}
	glPopMatrix();
}


//method animasi, menggerakan kaki depan dan kaki belakang laba-laba
void animation(void)
{
	int tempAngle = angle;

	if (naik)
	{
		thetax[1] -= newTheta;
		thetax[7] -= newTheta;
		thetax[3] += newTheta;
		thetax[5] += newTheta;

		newTheta += 0.1;
		thetax[1] += newTheta;
		thetax[7] += newTheta;
		thetax[3] -= newTheta;
		thetax[5] -= newTheta;

		angle = 1;
		applyNewAnglex();
		angle = 3;
		applyNewAnglex();
		angle = 5;
		applyNewAnglex();
		angle = 7;
		applyNewAnglex();

		if (baru)
		{
			if (newTheta > 45.0)
			{
				newTheta = 0.0;
				naik = false;
				baru = false;
			}
		}

		if (newTheta > 90.0)
		{
			newTheta = 0.0;
			naik = false;
		}
	}
	else
	{
		thetax[1] += newTheta;
		thetax[7] += newTheta;
		thetax[3] -= newTheta;
		thetax[5] -= newTheta;

		newTheta += 0.1;
		thetax[1] -= newTheta;
		thetax[7] -= newTheta;
		thetax[3] += newTheta;
		thetax[5] += newTheta;

		angle = 1;
		applyNewAnglex();
		angle = 3;
		applyNewAnglex();
		angle = 5;
		applyNewAnglex();
		angle = 7;
		applyNewAnglex();

		if (newTheta > 90.0)
		{
			newTheta = 0.0;
			naik = true;
		}
	}

	angle = tempAngle;
	glutPostRedisplay();
}

void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("WS 2 - Muhammad Luthfi - 1306386825");
	myinit();
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutSpecialFunc(geser);
	glutIdleFunc(NULL);

	glutCreateMenu(menu);
	glutAddMenuEntry("torso", 0);
	glutAddMenuEntry("left_upper_arm_1", 1);
	glutAddMenuEntry("left_lower_arm_1", 2);
	glutAddMenuEntry("right_upper_arm_1", 3);
	glutAddMenuEntry("right_lower_arm_1", 4);
	glutAddMenuEntry("left_upper_arm_2", 5);
	glutAddMenuEntry("left_lower_arm_2", 6);
	glutAddMenuEntry("right_upper_arm_2", 7);
	glutAddMenuEntry("right_lower_arm_2", 8);
	glutAddMenuEntry("", -1);
	glutAddMenuEntry("toggle kontrol lampu", 9);
	glutAddMenuEntry("toggle animasi", 10);
	glutAddMenuEntry("", -1);
	glutAddMenuEntry("quit", 11);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
}