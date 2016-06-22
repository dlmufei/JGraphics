#pragma once
#include "shapes.h"
#include "cube.h"

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
void setPoints_pixels(Point p, Color color);
void setPoints_pixels(int x, int y, Color color);
std::vector<Point>* getPoints(Point p1, Point p2);
void cutPoints_pixels(std::vector<Point> *points);
void initPixels();
void clear();
void vec_init();