#pragma once
#include "common.h"
extern int	g_cliHeight;
extern int	g_cliWidth;
class Cube
{
public:
	Cube() {}
	void init() {
		for (int i = 0; i < 8; i++) {
			vertex_list[i][0] = init_vertex[i][0] * (float)g_cliHeight+(g_cliWidth-g_cliHeight)/2;
			vertex_list[i][1] = init_vertex[i][1] * (float)g_cliHeight;
			vertex_list[i][2] = init_vertex[i][2] * (float)g_cliHeight;
		}
	}
	void render() {

		//glTranslatef(-0.2, 0, 0); // 平移

		//glScalef(2, 1, 1);    // 缩放

		//glRotatef(rotate, 0, 1, 0);
		//glRotatef(rotate, 1, 0, 0);
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(1, 0, 0);
		glTranslatef((float)g_cliWidth/2, (float)g_cliHeight/2, (float)g_cliHeight / 2);
		glRotatef(x_rotate, 0, 1, 0);
		glRotatef(y_rotate, 1, 0, 0);
		glTranslatef(-(float)g_cliWidth / 2, -(float)g_cliHeight / 2, -(float)g_cliHeight/2);
		int i, j;
		glBegin(GL_QUADS);

		for (i = 0; i < 6; ++i)         // 有六个面，循环六次
			for (j = 0; j < 4; ++j)     // 每个面有四个顶点，循环四次
				glVertex3fv(vertex_list[index_list[i][j]]);
		glEnd();
	}
	void rotate(Point oldPoint, Point newPoint)
	{
		float x0 = oldPoint.pnt[0];
		float y0 = oldPoint.pnt[1];
		float x1 = newPoint.pnt[0];
		float y1 = newPoint.pnt[1];
		x_rotate += (x1 - x0) / g_cliWidth * 360;
		y_rotate -= (y1 - y0) / g_cliHeight * 360;
	}
	void finish() {
		glDisable(GL_CULL_FACE);
	}
private:
	float vertex_list[8][3];
	int index_list[6][4] = {
		0, 2, 3, 1,
		0, 4, 6, 2,
		0, 1, 5, 4,
		4, 5, 7, 6,
		1, 3, 7, 5,
		2, 6, 7, 3,
	};
	float init_vertex[8][3] = {
		0.25f, 0.25f, 0.25f,
		0.75f, 0.25f, 0.25f,
		0.25f,  0.75f, 0.25f,
		0.75f,  0.75f, 0.25f,
		0.25f, 0.25f,  0.75f,
		0.75f, 0.25f,  0.75f,
		0.25f,  0.75f,  0.75f,
		0.75f,  0.75f,  0.75f,
	};
	int times = 0;
	float x_rotate = 45, y_rotate = 45;
};
extern Cube cube;