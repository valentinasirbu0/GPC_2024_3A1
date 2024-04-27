#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <glut.h>
#include <iostream>
#include <cmath>

#define dim 300
unsigned char prevKey;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void DisplayAxe() {
	int X, Y, Z;
	X = Y = 200;
	Z = 200;

	glLineWidth(2);

	// Ox axis - green
	glColor3f(0, 1, 0);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, 0);
	glVertex3f(X, 0, 0);
	glEnd();

	// Oy axis - blue
	glColor3f(0, 0, 1);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, 0);
	glVertex3f(0, Y, 0);
	glEnd();

	// Oz axis - red
	glColor3f(1, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, Z);
	glEnd();

	glLineWidth(1);
}

void DrawTriunghi(float v1[3], float v2[3], float v3[3]) {
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3fv(v1);
	glVertex3fv(v2);
	glVertex3fv(v3);
	glEnd();
}

void DrawCube(float v1[3], float v2[3], float v3[3], float v4[3], float v5[3], float v6[3], float v7[3], float v8[3]) {
	// Draw the edges of the cube
	glBegin(GL_LINES);
	// Front face
	glColor3f(1.0f, 0.0f, 0.0f); // Red
	glVertex3fv(v1);
	glVertex3fv(v2);

	glVertex3fv(v2);
	glVertex3fv(v3);

	glVertex3fv(v3);
	glVertex3fv(v4);

	glVertex3fv(v4);
	glVertex3fv(v1);

	// Back face
	glColor3f(0.0f, 1.0f, 0.0f); // Green
	glVertex3fv(v5);
	glVertex3fv(v6);

	glVertex3fv(v6);
	glVertex3fv(v7);

	glVertex3fv(v7);
	glVertex3fv(v8);

	glVertex3fv(v8);
	glVertex3fv(v5);

	// Connecting edges
	glColor3f(0.0f, 0.0f, 1.0f); // Blue
	glVertex3fv(v1);
	glVertex3fv(v5);

	glVertex3fv(v2);
	glVertex3fv(v6);

	glVertex3fv(v3);
	glVertex3fv(v7);

	glVertex3fv(v4);
	glVertex3fv(v8);

	// New edge
	glColor3f(1.0f, 1.0f, 0.0f); // Yellow
	glVertex3fv(v1);
	glVertex3fv(v7);
	glEnd();
}



void Init(void) {
	glClearColor(1, 1, 1, 1);
	glLineWidth(2);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10, 10, -10, 10, 30, -30);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotated(20, 1, 0, 0);
	glRotated(-20, 0, 1, 0);
}

float v1[3] = { 3.0f, 0.0f, 2.0f };   //x1,y1,z1
float v2[3] = { 6.0f, 0.0f, 5.0f };   //x2,y2,z2
float v3[3] = { 4.0f, 4.0f, 2.0f };   //x3,y3,z3

float c1[3] = { 0.0f, 0.0f, 0.0f };
float c2[3] = { 1.0f, 0.0f, 0.0f };
float c3[3] = { 1.0f, 1.0f, 0.0f };
float c4[3] = { 0.0f, 1.0f, 0.0f };
float c5[3] = { 0.0f, 0.0f, 1.0f };
float c6[3] = { 1.0f, 0.0f, 1.0f };
float c7[3] = { 1.0f, 1.0f, 1.0f };
float c8[3] = { 0.0f, 1.0f, 1.0f };

void Display(void) {
	switch (prevKey)
	{
	case '1':
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotated(20, 1, 0, 0);
		glRotated(-20, 0, 1, 0);
		DisplayAxe();
		DrawTriunghi(v1, v2, v3);
		break;
	case '2':
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotated(20, 1, 0, 0);
		glRotated(-20, 0, 1, 0);
		DisplayAxe();

		float angle = atan2(v2[0] - v1[0], v2[2] - v1[2]);
		glRotatef(-angle * 180.0 / M_PI, 0.0f, 1.0f, 0.0f);
		
		glTranslatef(-v1[0], -v1[1], -v1[2]); 

		DrawTriunghi(v1, v2, v3);
		break;
	}
	case '3':
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotated(20, 1, 0, 0);
		glRotated(-20, 0, 1, 0);
		DisplayAxe();
		float angle = atan2(v2[0] - v1[0], v2[2] - v1[2]);
		glRotatef(-angle * 180.0 / M_PI, 0.0f, 1.0f, 0.0f);
		glRotatef(0.09 * 180.0 / M_PI, 1.0f, 0.0f, 0.0f);
		glRotatef(-0.07 * 180.0 / M_PI, 0.0f, 1.0f, 0.0f);
		glTranslatef(-v1[0], -v1[1], -v1[2]);
		DrawTriunghi(v1, v2, v3);
		break;
	}
	case '4':
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotated(20, 1, 0, 0);
		glRotated(-20, 0, 1, 0);
		DisplayAxe();

		DrawCube(c1, c2, c3, c4, c5, c6, c7, c8);
		break;
	}
	case '5':
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotated(20, 1, 0, 0);
		glRotated(-20, 0, 1, 0);
		DisplayAxe();

		float angle_y = atan2(c7[0] - c1[0], c7[2] - c1[2]) * 180.0 / M_PI;
		glRotatef(-angle_y, 0.0f, 1.0f, 0.0f);

		DrawCube(c1, c2, c3, c4, c5, c6, c7, c8);
		break;
	}
	case '6':
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotated(20, 1, 0, 0);
		glRotated(-20, 0, 1, 0);
		DisplayAxe();
		
		float angle_y = atan2(c7[0], c7[2]) * 180.0 / M_PI;
		glRotatef(-angle_y, 0.0f, 1.0f, 0.0f);

		float angle_x = atan2(c7[1], c7[2]) * 180.0 / M_PI;
		glRotatef(angle_x + 10, 1.0f, 0.0f, 0.0f);


		DrawCube(c1, c2, c3, c4, c5, c6, c7, c8);
		break;
	}
	case '7':
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotated(20, 1, 0, 0);
		glRotated(-20, 0, 1, 0);
		DisplayAxe();

		float angle_y = atan2(c7[0], c7[2]) * 180.0 / M_PI;
		glRotatef(-angle_y, 0.0f, 1.0f, 0.0f);

		float angle_x = atan2(c7[1], c7[2]) * 180.0 / M_PI;
		glRotatef(angle_x - 5 , 1.0f, 0.0f, 0.0f);

		glRotatef(-13.0f, 0.0f, 0.0f, 1.0f);

		DrawCube(c1, c2, c3, c4, c5, c6, c7, c8);
		break;
	}

	case '8':
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotated(20, 1, 0, 0);
		glRotated(-20, 0, 1, 0);
		DisplayAxe();

		float angle_y = atan2(c7[0], c7[2]) * 180.0 / M_PI;
		glRotatef(-angle_y, 0.0f, 1.0f, 0.0f);

		float angle_x = atan2(c7[1], c7[2]) * 180.0 / M_PI;
		glRotatef(angle_x - 5, 1.0f, 0.0f, 0.0f);

		glRotatef(-13.0f, 0.0f, 0.0f, 1.0f);
		glRotatef(-30.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(30.0f, 0.0f, 0.0f, 1.0f);

		DrawCube(c1, c2, c3, c4, c5, c6, c7, c8);
		break;
	}
	
	case '9':
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotated(20, 1, 0, 0);
		glRotated(-20, 0, 1, 0);
		DisplayAxe();

		float angle_y = atan2(c7[0], c7[2]) * 180.0 / M_PI;
		glRotatef(-angle_y, 0.0f, 1.0f, 0.0f);

		float angle_x = atan2(c7[1], c7[2]) * 180.0 / M_PI;
		glRotatef(angle_x - 5, 1.0f, 0.0f, 0.0f);

		glRotatef(-13.0f, 0.0f, 0.0f, 1.0f);
		glRotatef(-30.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(30.0f, 0.0f, 0.0f, 1.0f);

		float angle_b = atan2(c7[0], c7[2]) * 180.0 / M_PI;
		glRotatef(angle_b, 0.0f, 1.0f, 0.0f);

		DrawCube(c1, c2, c3, c4, c5, c6, c7, c8);
		break;
	}


	default:
		break;
	}
	glutSwapBuffers();
}


void Reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
	prevKey = key;
	if (key == 27)
		exit(0);
	glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitWindowSize(dim, dim);

	glutInitWindowPosition(100, 100);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutCreateWindow(argv[0]);

	Init();

	glutReshapeFunc(Reshape);

	glutKeyboardFunc(KeyboardFunc);

	glutMouseFunc(MouseFunc);

	glutDisplayFunc(Display);

	glutMainLoop();

	return 0;
}
