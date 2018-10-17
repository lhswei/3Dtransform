#include <GL/glut.h>
#include <math.h>

GLuint windWidth = 600, winHeight = 600;
GLdouble x_0 = 50.0, y_0 = 50.0, z_0 = 50.0;
GLdouble xref = 40.0, yref = 40.0, zref = 0.0;
GLdouble Vx = 0.0, Vy = 1.0, Vz = 0.0;

GLdouble xwMin = -40.0, ywMin = -60.0, xwMax = 40.0, ywMax = 60.0;

GLdouble dnear = -500.0, dfar = 500.0;

class wcPt3D 
{
public:
	wcPt3D(GLfloat px, GLfloat py, GLfloat pz) : x(px), y(py), z(pz){}
	GLfloat x, y, z;
};

const GLuint g_row = 4;
const GLuint g_col = 4;
const GLdouble PI = 3.14159;
const GLdouble degper = PI / 180;

typedef GLfloat Matrix4x4 [g_row][g_col];

Matrix4x4 matComposite = { 0 };

//������
Matrix4x4 g_squareVertex =
{
	{ 35.0,  30.0,   50.0,  37.0 },   //x
	{ 25.0,  5.0,    20.0,  90.0 },   //y
	{ 60.0,  -10.0,  10.0,  0.0 },    //z
	{ 1.0,   1.0,    1.0,   1.0 },    //w
};

GLfloat g_xAxis[2] = { -100.0, 500.0 };
GLfloat g_yAxis[2] = { -100.0, 500.0 };
GLfloat g_zAxis[2] = { -300.0, 200.0 };

wcPt3D g_p1(-2, -10, 1);
wcPt3D g_p2(20, 100, -10);

/*Construct the 4 by 4 identity matrix.*/
void matrix4x4SetIdentity (Matrix4x4 matIdent4x4)
{
	GLint row, col;
	for (row = 0; row < 4; row++)
	{
		for (col = 0; col < 4; col++)
		{
			matIdent4x4[row][col] = (row == col);
		}
	}
}

/* Premultiply matri m1 times matrix m2, store result in m2*/
void matrix4x4PreMultiply(Matrix4x4 m1, Matrix4x4 m2)
{
	GLint row, col;
	Matrix4x4 matTemp;

	for (row = 0; row < 4; row++)
	{
		for (col = 0; col < 4; col++)
		{
			matTemp[row][col] = m1[row][0] * m2[0][col] + m1[row][1] * m2[1][col] + m1[row][2] * m2[2][col] + m1[row][3] * m2[3][col];
		}
	}

	for (row = 0; row < 4; row++)
	{
		for (col = 0; col < 4; col++)
		{
			m2[row][col] = matTemp[row][col];
		}
	}
}

/* Procedure for generating 3D translation matrix. */
void translate3D(GLfloat tx, GLfloat ty, GLfloat tz)
{
	Matrix4x4 matTransl3D;

	// ������λ����
	matrix4x4SetIdentity(matTransl3D);

	// ����ƽ�Ʒ���
	matTransl3D[0][3] = tx;
	matTransl3D[1][3] = ty;
	matTransl3D[2][3] = tz;

	matrix4x4PreMultiply(matTransl3D, matComposite);
}

/* Procedure for generating 3D a quaternion rotation matrix. */
void rotate3D(wcPt3D p1, wcPt3D p2, GLfloat radianAngle)
{
	Matrix4x4 matQuatRot;

	float axisVectLength = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z));
	float cosA = cosf(radianAngle);
	float oneC = 1 - cosA;
	float sinA = sinf(radianAngle);
	float ux = (p2.x - p1.x) / axisVectLength;
	float uy = (p2.y - p1.y) / axisVectLength;
	float uz = (p2.z - p1.z) / axisVectLength;

	// ƽ�Ƶ�p1��
	translate3D(-p1.x, -p1.y, -p1.z);

	// ������λ����
	matrix4x4SetIdentity(matQuatRot);

	matQuatRot[0][0] = ux * ux * oneC + cosA;
	matQuatRot[0][1] = ux * uy * oneC - uz * sinA;
	matQuatRot[0][2] = ux * uz * oneC + uy * sinA;
	matQuatRot[1][0] = uy * ux * oneC + uz * sinA;
	matQuatRot[1][1] = uy * uy * oneC + cosA;
	matQuatRot[1][2] = uy * uz * oneC - ux * sinA;
	matQuatRot[2][0] = uz * ux * oneC - uy * sinA;
	matQuatRot[2][1] = uz * uy * oneC + ux * sinA;
	matQuatRot[2][2] = uz * uz * oneC + cosA;

	// ������ת
	matrix4x4PreMultiply(matQuatRot, matComposite);

	// ƽ�ƻ�ԭ����λ��
	translate3D(p1.x, p1.y, p1.z);
}

/* Procedure for generating a 3D scaling matrix. */
void scal3D(GLfloat sx, GLfloat sy, GLfloat sz, wcPt3D fixedPt)
{
	Matrix4x4 matScale3D;
	// ������λ����
	matrix4x4SetIdentity(matScale3D);

	matScale3D[0][0] = sx;
	matScale3D[0][3] = (1 - sx) * fixedPt.x;
	matScale3D[1][1] = sy;
	matScale3D[1][3] = (1 - sy) * fixedPt.y;
	matScale3D[2][2] = sz;
	matScale3D[2][3] = (1 - sz) * fixedPt.z;

	// ��������
	matrix4x4PreMultiply(matScale3D, matComposite);

}

// ������ϵ
void drawCoordiante()
{
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3d(g_xAxis[0], 0, 0);
	glVertex3d(g_xAxis[1], 0, 0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3d(0, g_yAxis[0], 0);
	glVertex3d(0, g_yAxis[1], 0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3d(0, 0, g_zAxis[0]);
	glVertex3d(0, 0, g_zAxis[1]);
	glEnd();
}

// ����ת��
void drawRotate()
{
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3d(g_p1.x, g_p1.y, g_p1.z);
	glVertex3d(g_p2.x, g_p2.y, g_p2.z);
	glEnd();

}

// ������׶
void drawTetra()
{
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINE_STRIP);
	glVertex3d(matComposite[0][0], matComposite[1][0], matComposite[2][0]);
	glVertex3d(matComposite[0][1], matComposite[1][1], matComposite[2][1]);
	glVertex3d(matComposite[0][3], matComposite[1][3], matComposite[2][3]);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3d(matComposite[0][3], matComposite[1][3], matComposite[2][3]);
	glVertex3d(matComposite[0][2], matComposite[1][2], matComposite[2][2]);
	glVertex3d(matComposite[0][1], matComposite[1][1], matComposite[2][1]);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3d(matComposite[0][3], matComposite[1][3], matComposite[2][3]);
	glVertex3d(matComposite[0][0], matComposite[1][0], matComposite[2][0]);
	glVertex3d(matComposite[0][2], matComposite[1][2], matComposite[2][2]);
	glEnd();
}

void displayFcn(void)
{

	
	glClear(GL_COLOR_BUFFER_BIT);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	drawCoordiante();

	drawRotate();

	drawTetra();

	drawTetra();

	glFlush();
}

void reshepeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	windWidth = newWidth;
	winHeight = newHeight;
}

void init(void)
{
	matrix4x4SetIdentity(matComposite);
	matrix4x4PreMultiply(g_squareVertex, matComposite);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x_0, y_0, z_0, xref, yref, zref, Vx, Vy, Vz);
}

void setOrhtonProjection(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(xwMin * 2, xwMax * 2, ywMin * 2, ywMax * 2, dnear, dfar);
}

void SpecialKeys(int key, int x, int y)
{

	if (key == GLUT_KEY_UP)
	{
		glTranslatef(0, 0, 1);
	}

	if (key == GLUT_KEY_DOWN)
	{
		glTranslatef(0, 0, -1);
	}

	if (key == GLUT_KEY_LEFT)
	{
		glTranslatef(-1, 0, 0);
	}

	if (key == GLUT_KEY_RIGHT)
	{
		glTranslatef(1, 0, 0);
	}
	glutPostRedisplay();
}

void KeyboardKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
	{
		translate3D(0, 0, -2);
		break;
	}
	case 's':
	{
		translate3D(0, 0, 2);
		break;
	}
	case 'a':
	{
		translate3D(-2, 0, 0);
		break;
	}
	case 'd':
	{
		translate3D(2, 0, 0);
		break;
	}
	case 'q':
	{
		translate3D(0, 2, 0);
		break;
	}
	case 'e':
	{
		translate3D(0, -2, 0);
		break;
	}
	case 'c':
	{
		translate3D(-2, -10, 1);
		break;
	}
	case 'v':
	{
		translate3D(2, 10, -1);
		break;
	}
	case '=':
	{
		scal3D(1.1, 1.1, 1.1, g_p1);
		break;
	}
	case '-':
	{
		scal3D(0.9, 0.9, 0.9, g_p1);
		break;
	}
	case 'z':
	{
		rotate3D(g_p1, g_p2, degper * 5);
		break;
	}
	case 'x':
	{
		rotate3D(g_p1, g_p2, -degper * 5);
		break;
	}
	case 'o':
	{
		init();
		setOrhtonProjection();
		break;
	}
	default:
		break;
	}

	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(windWidth, winHeight);
	glutCreateWindow("3D transform");

	init();
	setOrhtonProjection();
	glutDisplayFunc(displayFcn);
	glutReshapeFunc(reshepeFcn);
	glutKeyboardFunc(KeyboardKeys);
	glutSpecialFunc(SpecialKeys);
	glutMainLoop();
	return 0;
}
