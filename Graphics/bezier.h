#pragma once
#include "polygons.h"
//extern Color g_whiteColor;
class Bezier : public Polygons
{
public:
	Bezier() {

	}
	void render()
	{
		Point tmp = m_points[0];
		glLineWidth(m_width);
		glColor3fv(g_whiteColor.color);
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < m_points.size(); i++) {
			glVertex2iv(m_points[i].pnt);
		}
		glEnd();
		glColor3fv(m_color.color);
		glBegin(GL_LINE_STRIP);
		for (double t = 0; t < 1; t += STEP) {
			Point p = getBezierPoint(t);
			glVertex2iv(p.pnt);
			tmp = p;
		}
		glEnd();
	}
	void add(Point p1, Point p2, Color color, float width)
	{
		m_points.push_back(p1);
		m_points.push_back(p2);
		m_color = color;
		m_width = width;
		update();
	}

	void add(Point p, Color color, float width)
	{
		m_points.push_back(p);
		m_color = color;
		m_width = width;
	};

	void replaceLast(float x, float y)
	{
		m_points[m_points.size() - 1] = Point(x, y);
	}
	void update()
	{
		setPoints_pixels(&line_points, g_blackColor);
		setPoints_pixels(&bezier_points, g_blackColor);
		line_points.clear();
		bezier_points.clear();
		if (m_points.size() == 0) {
			return;
		}
		Point tmp = m_points[0];
		std::vector<Point> *tmp_points;
		for (int i = 1; i < m_points.size(); i++) {
			tmp_points = getPoints(m_points[i - 1], m_points[i]);
			line_points.insert(line_points.begin(), tmp_points->begin(), tmp_points->end());
		}

		for (double t = 0; t < 1; t += STEP) {
			Point p = getBezierPoint(t);
			tmp_points = getPoints(tmp, p);
			bezier_points.insert(bezier_points.begin(), tmp_points->begin(), tmp_points->end());
			tmp = p;
		}
		setPoints_pixels(&line_points, g_whiteColor);
		setPoints_pixels(&bezier_points, g_defColor);
	}
	int* output() {
		shape_type type = shape_bezier;
		int size = m_points.size();
		int* output = new int[2*size + 2];
		output[0] = 2 * size+2;
		output[1] = type;
		for (int i = 0; i < size; i++) {
			output[2 * i + 2] = m_points[i].pnt[0];
			output[2 * i + 3] = m_points[i].pnt[1];
		}
		return output;
	}
	void input(int *input,int size) {
		int cnt = (size - 2) / 2;
		m_points.clear();
		for (int i = 0; i < cnt; i++) {
			m_points.push_back(Point(input[2 * i ], input[2 * i+1]));
		}
		update();
	}
private:
	double STEP = 0.001;
	Point getBezierPoint(double t)
	{
		std::vector<double> fact(2, 1);
		double x = 0, y = 0;
		int n = m_points.size() - 1;
		for (int i = 2; i <= n; i++) fact.push_back(*fact.rbegin() * i);
		for (int i = 0; i <= n; i++) {
			double factor = pow(t, i) * pow(1 - t, n - i);
			factor *= fact[n] / fact[n - i] / fact[i];
			x += factor * m_points[i].pnt[0];
			y += factor * m_points[i].pnt[1];
		}
		return Point((int)round(x), (int)round(y));
	}
	std::vector<Point> line_points;
	std::vector<Point> bezier_points;
};