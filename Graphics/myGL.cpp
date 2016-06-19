#include "stdafx.h"
#include "myGL.h"
Color** pixels;
int** pixels_cnt;
void createPixels()
{
	pixels = new Color*[g_cliWidth];
	pixels_cnt = new int*[g_cliWidth];
	for (int i = 0; i < g_cliWidth; i++) {
		pixels[i] = new Color[g_cliHeight];
		pixels_cnt[i] = new int[g_cliHeight];
	}
	for (int i = 0; i < g_cliWidth; i++) {
		for (int j = 0; j < g_cliHeight; j++) {
			pixels[i][j] = g_blackColor;
			pixels_cnt[i][j] = 1;
		}
	}
}
Color getColor(Point p)
{
	return pixels[p.pnt[0]][p.pnt[1]];
}
void GLOnCreate()
{
	PIXELFORMATDESCRIPTOR pfd;
	int iFormat;

	g_hdc = GetDC(hWnd);

	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 16;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	iFormat = ChoosePixelFormat(g_hdc, &pfd);

	SetPixelFormat(g_hdc, iFormat, &pfd);

	g_hRC = wglCreateContext(g_hdc);
	wglMakeCurrent(g_hdc, g_hRC);
}
void resize_Viewport(GLsizei w, GLsizei h)
{
	if (h == 0)
	{
		h = 1;
	}
	g_aspect = (GLfloat)w / (GLfloat)h;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, g_clientRect.right, 0, g_clientRect.bottom, 10.0f, -10.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 8.0f, 0, 0, 0, 0, 1, 0);
}
void GL_show()
{

	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();

	if (is_lbtn_down) {
		switch (mode)
		{
		case mode_POLYGON:
			break;
		case mode_LINES:
			vec[1]->render();
			break;
		case mode_POLYGON2:
		case mode_CIRCLE:
		case mode_Bezier:
		case mode_Bezier2:
		case mode_CUT_IN2:
		case mode_CUT_OUT2:
		case mode_MOVE:
			vec[vec.size() - 1]->render();
			break;
		case mode_Fill:
			break;
		default:
			break;
		}
	}
	glColor3fv(g_defColor.color);
	glBegin(GL_POINTS);
	for (int i = 0; i < g_cliWidth; i++) {
		for (int j = 0; j < g_cliHeight; j++) {
			if (pixels[i][j] == g_blackColor)
				continue;
			else if(pixels[i][j]==g_defColor)
				glVertex2i(i, j);
			else {
				glColor3fv(pixels[i][j].color);
				glVertex2i(i, j);
				glColor3fv(g_defColor.color);
			}
		}
	}
	glEnd();
	glFlush();
	glPopMatrix();
	
	HDC hdc = ::GetDC(hWnd);
	::SwapBuffers(hdc);
}

void addLastPoint(LPARAM param)
{
	vec[vec.size() - 1]->add(Point(LOWORD(param), g_cliHeight - HIWORD(param)), g_defColor, g_defSize);
}
void addLines(LPARAM param1, LPARAM param2)
{
	vec[1]->add(Point(LOWORD(param1), g_cliHeight - HIWORD(param1)),
		Point(LOWORD(param2), g_cliHeight - HIWORD(param2)), g_defColor, g_defSize);
}
void modifyLastLine(LPARAM param)
{
	vec[1]->replaceLast(LOWORD(param), g_cliHeight - HIWORD(param));
}
void modifyLastPoint(LPARAM param)
{
	vec[vec.size() - 1]->replaceLast(LOWORD(param), g_cliHeight - HIWORD(param));
}
std::vector<Point>* getPoints(Point p1, Point p2) {
	std::vector<Point> *points = new std::vector<Point>;
	int x1 = (int)p1.pnt[0];
	int y1 = (int)p1.pnt[1];
	int x2 = (int)p2.pnt[0];
	int y2 = (int)p2.pnt[1];
	double k, dx, dy, x, y, xend, yend;
	dx = x2 - x1;
	dy = y2 - y1;
	if (fabs(dx) >= fabs(dy))
	{
		k = dy / dx;
		if (dx > 0)
		{
			x = x1;
			y = y1;
			xend = x2;
		}
		else
		{
			x = x2;
			y = y2;
			xend = x1;
		}
		while (x <= xend)
		{
			points->push_back(Point((int)x, (int)round(y)));
			y = y + k;
			x = x + 1;
		}
	}
	else
	{
		k = dx / dy;
		if (dy > 0)
		{
			x = x1;
			y = y1;
			yend = y2;
		}
		else
		{
			x = x2;
			y = y2;
			yend = y1;
		}
		while (y <= yend)
		{
			points->push_back(Point((int)x, (int)round(y)));
			x = x + k;
			y = y + 1;
		}
	}
	return points;
}
void setPoints_pixels(Point p1,Point p2,Color color)
{
	std::vector<Point> *points=getPoints(p1,p2);
	setPoints_pixels(points, color);
}
void cutPoints_pixels(std::vector<Point> *points)
{
	for (int i = 0; i < points->size(); i++)
	{
		int x = (*points)[i].pnt[0];
		int y = (*points)[i].pnt[1];
		pixels[x][y] = g_blackColor;
	}
}
void setPoints_pixels(std::vector<Point> *points, Color color)
{
	for (int i = 0; i < points->size(); i++)
	{
		int x = (*points)[i].pnt[0];
		int y = (*points)[i].pnt[1];
		if (pixels[x][y] == color) pixels_cnt[x][y]++;
		else if (color == g_blackColor) {
			if(pixels_cnt[x][y]==1) pixels[x][y] = color;
			else pixels_cnt[x][y]--;
		}
		else pixels[x][y] = color;
	}
}