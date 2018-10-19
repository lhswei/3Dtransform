#include <stdio.h>
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
typedef GLfloat Vertex4 [g_row];

Matrix4x4 matComposite = { 0 };

//正方形
Matrix4x4 g_squareVertex =
{
	{ 35.0,  30.0,   50.0,  37.0 },   //x
	{ 25.0,  5.0,    20.0,  90.0 },   //y
	{ 60.0,  -10.0,  10.0,  0.0 },    //z
	{ 1.0,   1.0,    1.0,   1.0 },    //w
};

const GLuint g_vertexCount = 4;		// 顶点个数

//三棱锥初始数据
GLfloat g_initVertexs[g_vertexCount][g_row] =
{
	{ 35.0,  25.0,   60.0,   1.0 },   //x
	{ 30.0,  5.0,    -10.0,  1.0 },   //y
	{ 50.0,  20.0,   10.0,   1.0 },    //z
	{ 37.0,  90.0,   0.0,    1.0 },    //w
};

GLfloat g_shapeVertexs[g_vertexCount][g_row];
GLuint g_drawOrder[3][3] =
{
	{ 0, 1, 2 },
	{ 1, 3, 2 },
	{ 3, 0, 2 },
};

GLfloat g_xAxis[2] = { -100.0, 500.0 };
GLfloat g_yAxis[2] = { -100.0, 500.0 };
GLfloat g_zAxis[2] = { -300.0, 200.0 };

wcPt3D g_p1(-2, -10, 1);
wcPt3D g_p2(20, 100, -10);

void transforData();

void printMatrix(Matrix4x4 ma)
{
	printf("------------------------------------\n");
	
	printf("|"); printf(" %.6f ", ma[0][0]);	printf(" %.6f ", ma[0][1]);  printf(" %.6f ", ma[0][2]);  printf(" %.6f ", ma[0][3]); printf("|\n");
	printf("|"); printf(" %.6f ", ma[1][0]);	printf(" %.6f ", ma[1][1]);  printf(" %.6f ", ma[1][2]);  printf(" %.6f ", ma[1][3]); printf("|\n");
	printf("|"); printf(" %.6f ", ma[2][0]);	printf(" %.6f ", ma[2][1]);  printf(" %.6f ", ma[2][2]);  printf(" %.6f ", ma[2][3]); printf("|\n");
	printf("|"); printf(" %.6f ", ma[3][0]);	printf(" %.6f ", ma[3][1]);  printf(" %.6f ", ma[3][2]);  printf(" %.6f ", ma[3][3]); printf("|\n");

	printf("-------------------------------------\n");
}

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

/* Premultiply matri m1 times vertex4 v2, store result in v2*/
void matrix4x4PreVertex4(Matrix4x4 m1, Vertex4 v2)
{
	GLint row, col;
	Vertex4 vertex4Temp;

	for (row = 0; row < 4; row++)
	{
		vertex4Temp[row] = m1[row][0] * v2[0] + m1[row][1] * v2[1] + m1[row][2] * v2[2] + m1[row][3] * v2[3];
	}

	for (row = 0; row < 4; row++)
	{
		v2[row] = vertex4Temp[row];
	}
}

/* Procedure for generating 3D translation matrix. */
void translate3D(GLfloat tx, GLfloat ty, GLfloat tz)
{
	Matrix4x4 matTransl3D;

	// 创建单位矩阵
	matrix4x4SetIdentity(matTransl3D);

	// 设置平移分量
	matTransl3D[0][3] = tx;
	matTransl3D[1][3] = ty;
	matTransl3D[2][3] = tz;

	printf("开始平移 tx = %.6f ty = %.6f tz = %.6f \n", tx, ty, tz);
	printMatrix(matTransl3D);

	for (int i = 0; i < g_vertexCount; i++)
	{
		matrix4x4PreVertex4(matTransl3D, g_shapeVertexs[i]);
	}
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

	// 平移到p1点
	translate3D(-p1.x, -p1.y, -p1.z);

	// 创建单位矩阵
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

	printf("开始旋转 radianAngle = %.6f \n", radianAngle);
	printMatrix(matQuatRot);

	// 进行旋转
	for (int i = 0; i < g_vertexCount; i++)
	{
		matrix4x4PreVertex4(matQuatRot, g_shapeVertexs[i]);
	}

	// 平移回原来的位置
	translate3D(p1.x, p1.y, p1.z);
}

/* Procedure for generating a 3D scaling matrix. */
void scal3D(GLfloat sx, GLfloat sy, GLfloat sz, wcPt3D fixedPt)
{
	Matrix4x4 matScale3D;
	// 创建单位矩阵
	matrix4x4SetIdentity(matScale3D);

	matScale3D[0][0] = sx;
	matScale3D[0][3] = (1 - sx) * fixedPt.x;
	matScale3D[1][1] = sy;
	matScale3D[1][3] = (1 - sy) * fixedPt.y;
	matScale3D[2][2] = sz;
	matScale3D[2][3] = (1 - sz) * fixedPt.z;

	printf("开始缩放 sx = %.6f sy = %.6f sz = %.6f \n", sx, sy, sz);
	printMatrix(matScale3D);
	// 进行缩放
	// 进行旋转
	for (int i = 0; i < g_vertexCount; i++)
	{
		matrix4x4PreVertex4(matScale3D, g_shapeVertexs[i]);
	}
}

// 画坐标系
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

// 画旋转轴
void drawRotate()
{
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3d(g_p1.x, g_p1.y, g_p1.z);
	glVertex3d(g_p2.x, g_p2.y, g_p2.z);
	glEnd();

}

// 画三棱锥
void drawTetra()
{
	glColor3f(0.0, 1.0, 0.0);

	for (int i = 0; i < 3; i++)
	{
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < 3; j++)
		{
			int index = g_drawOrder[i][j];
			glVertex3d(g_shapeVertexs[index][0], g_shapeVertexs[index][1], g_shapeVertexs[index][2]);
		}
		glEnd();
	}
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

void resetShapData()
{
	for (int i = 0; i < g_vertexCount; i++)
	{
		for (int j = 0; j < g_row; j++)
		{
			g_shapeVertexs[i][j] = g_initVertexs[i][j];
		}
	}
}

void transforData()
{
	for (int i = 0; i < g_vertexCount; i++)
	{
		matrix4x4PreVertex4(matComposite, g_shapeVertexs[i]);
	}
}

void init(void)
{
	resetShapData();
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
		printf("===============向前移动=========================\n");
		translate3D(0, 0, -2);
		printf("===============================================\n");
		break;
	}
	case 's':
	{
		printf("===============向后移动=========================\n");
		translate3D(0, 0, 2);
		printf("===============================================\n");
		break;
	}
	case 'a':
	{
		printf("===============向左移动=========================\n");
		translate3D(-2, 0, 0);
		printf("===============================================\n");
		break;
	}
	case 'd':
	{
		printf("===============向右移动=========================\n");
		translate3D(2, 0, 0);
		printf("===============================================\n");
		break;
	}
	case 'q':
	{
		printf("===============向上移动=========================\n");
		translate3D(0, 2, 0);
		printf("===============================================\n");
		break;
	}
	case 'e':
	{
		printf("===============向下移动=========================\n");
		translate3D(0, -2, 0);
		printf("===============================================\n");
		break;
	}
	case 'c':
	{
		printf("===============沿着指定轴移动=========================\n");
		translate3D(-2, -10, 1);
		printf("===============================================\n");
		break;
	}
	case 'v':
	{
		printf("===============沿着指定轴移动=========================\n");
		printf("===============================================\n");
		translate3D(2, 10, -1);
		break;
	}
	case '=':
	{
		printf("===============等比例放大=========================\n");
		scal3D(1.1, 1.1, 1.1, g_p1);
		printf("===============================================\n");
		break;
	}
	case '-':
	{
		printf("===============等比例缩小=========================\n");
		scal3D(0.9, 0.9, 0.9, g_p1);
		printf("===============================================\n");
		break;
	}
	case 'z':
	{
		printf("===============旋转操作=========================\n");
		rotate3D(g_p1, g_p2, degper * 5);
		printf("===============================================\n");
		break;
	}
	case 'x':
	{
		printf("===============旋转操作=========================\n");
		rotate3D(g_p1, g_p2, -degper * 5);
		printf("===============================================\n");
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

