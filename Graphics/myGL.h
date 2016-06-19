#pragma once
#include "common.h"
#include "shapes.h"

void GLOnCreate();
void GL_show();
void myDisplay(void);
void addLastPoint(LPARAM param);
void resize_Viewport(GLsizei w, GLsizei h);
void addLines(LPARAM param1, LPARAM param2);
void modifyLastLine(LPARAM param);
void modifyLastPoint(LPARAM param);
Color getColor(Point p);
void createPixels();
void getPixels();
void setPoints_pixels(Point p1, Point p2, Color color);
void setPoints_pixels(std::vector<Point> *points, Color color);
std::vector<Point>* getPoints(Point p1, Point p2);
void cutPoints_pixels(std::vector<Point> *points);

extern int** pixels_cnt;
extern Color** pixels;
extern std::vector<PicElem*> vec;
extern HDC	g_hdc;
extern HGLRC g_hRC;
extern HWND hWnd;
extern RECT g_clientRect;
extern int	g_cliHeight;
extern int	g_cliWidth;
extern Color g_defColor;
extern Color g_whiteColor;
extern Color g_blackColor;
extern float g_defSize;
extern GLfloat	g_aspect;

extern MODE mode;
extern bool is_lbtn_down ;