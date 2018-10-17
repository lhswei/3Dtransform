#include <GL/glut.h>
#include <math.h>

GLuint windWidth = 600, winHeight = 600;
GLdouble x_0 = 50.0, y_0 = 50.0, z_0 = 50.0;
GLdouble xref = 50.0, yref = 50.0, zref = 0;
GLdouble Vx = 0.0, Vy = 1.0, Vz = 0.0;

GLdouble xwMin = -40.0, ywMin = -60.0, xwMax = 40.0, ywMax = 60.0;

GLdouble dnear = 25.0, dfar = 125.0;

GLdouble PI = 3.14159;
GLdouble algper = 180 / PI;
GLdouble degper = PI / 180;

const GLuint g_row = 4;
const GLuint g_col = 3;
//          ^y
//			|
//			|
//			|
//	(0, 100)|
//			|--------|(100, 100)
//			|		 |
//			|		 |	
//----------|--------|--------------->x
//	   (0,0)|		(100, 0)
//			|
//			|
//			|
//			|
//正方形
GLfloat g_squareVertex[g_row][g_col] =
{
	{ 0.0, 0.0, 0.0 },
	{ 100.0, 0.0, 0.0 },
	{ 100.0, 100.0, 0.0 },
	{ 0.0, 100.0, 0.0 },
};

// 进裁剪平面和裁剪窗口重合
void MyPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	GLdouble top = zNear * tan((fovy / 2) * degper);
	GLdouble right = top * aspect;
	glFrustum(-right, right, -top, top, zNear, zFar);
}

// 透视投影棱柱台描述函数
void MyFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
	GLdouble Width = right - left;
	GLdouble Height = top - bottom;
	GLdouble ratio = 1.0 * Width / Height;
	GLdouble angle = atan2(Height / 2, zNear) * algper * 2;
	gluPerspective(angle, ratio, zNear, zFar);
}

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x_0, y_0, z_0, xref, yref, zref, Vx, Vy, Vz);
}

void setPerspProjection(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(xwMin, xwMax, ywMin, ywMax, dnear, dfar);
	//MyPerspective(134.7604, 0.6666667, 25, 125);
	//MyFrustum(xwMin, xwMax, ywMin, ywMax, dnear, dfar);
}

void setOrhtonProjection(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(xwMin * 2, xwMax * 2, ywMin * 2, ywMax * 2, dnear, dfar);
}

void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0, 1.0, 0.0);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	glBegin(GL_QUADS);
	{
		for (int i = 0; i < g_row; ++i)
		{
			glVertex3f(g_squareVertex[i][0], g_squareVertex[i][1], g_squareVertex[i][2]);
		}
	}
	glEnd();

	glFlush();

}

void reshepeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	windWidth = newWidth;
	winHeight = newHeight;
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
	if (key == 'w')
	{
		glRotatef(5, 1, 0, 0);
	}

	if (key == 's')
	{
		glRotatef(-5, 1, 0, 0);
	}

	if (key == 'a')
	{
		glRotatef(5, 0, 1, 0);
	}

	if (key == 'd')
	{
		glRotatef(-5, 0, 1, 0);
	}

	if (key == 'o')
	{
		init();
		setOrhtonProjection();
	}

	if (key == 'p')
	{
		init();
		setPerspProjection();
	}

	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(windWidth, winHeight);
	glutCreateWindow("Respective View of A Square");

	init();
	setOrhtonProjection();
	glutDisplayFunc(displayFcn);
	glutReshapeFunc(reshepeFcn);
	glutKeyboardFunc(KeyboardKeys);
	glutSpecialFunc(SpecialKeys);
	glutMainLoop();
	return 0;
}

