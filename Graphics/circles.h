#pragma once
#include "shapes.h"

class Circles : public PicElem
{
public:
	Circles() {}

	void render()
	{
		glLineWidth(m_width);
		glColor3fv(m_color.color);
		glBegin(GL_LINE_STRIP);
		for (int i = 1; i < m_points.size(); ++i)
		{
			glVertex2iv(m_points[i].pnt);
		}
		glEnd();
	}

	virtual void add(float, float, float, float, float, float) {}

	virtual void add(float x1, float y1, float x2, float y2, float r, float g, float b, float width) {}

	void add(Point p1, Point p2, Color color, float width)
	{
		m_points.clear();
		float centerX = (p1.pnt[0] + p2.pnt[0]) / 2;
		float centerY = (p1.pnt[1] + p2.pnt[1]) / 2;
		float radiusX = fabs(p1.pnt[0] - p2.pnt[0]) / 2;
		float radiusY = fabs(p1.pnt[1] - p2.pnt[1]) / 2;
		m_points.push_back(p1);
		for (float f = .0f; f < PI2; f += 0.01f)
		{
			m_points.push_back(Point((int)round(radiusX*cos(f) + centerX), (int)round(radiusY*sin(f) + centerY)));
		}
		m_points.push_back(Point((int)round(radiusX + centerX), (int)round(centerY)));
		m_color = color;
		m_width = width;
	}
	void update()
	{
		setPoints_pixels(&circle_points, g_blackColor);
		circle_points.clear();
		std::vector<Point> *tmp;
		for (int i = 2; i < m_points.size(); ++i)
		{
			tmp = getPoints(m_points[i], m_points[i - 1]);
			circle_points.insert(circle_points.begin(), tmp->begin(), tmp->end());
		}
		setPoints_pixels(&circle_points, g_defColor);
	}
	void add(Point p, Color color, float width)
	{
		//		m_points.clear();
		m_points.push_back(p);
		m_color = color;
		m_width = width;
	};

	void replaceLast(float x, float y)
	{
		if (m_points.size() == 0) return;
		m_points.erase(m_points.begin() + 1, m_points.end());
		float centerX = (m_points[0].pnt[0] + x) / 2;
		float centerY = (m_points[0].pnt[1] + y) / 2;
		float radiusX = fabs(m_points[0].pnt[0] - x) / 2;
		float radiusY = fabs(m_points[0].pnt[1] - y) / 2;
		for (float f = .0f; f < PI2; f += 0.01f)
		{
			m_points.push_back(Point(radiusX*cos(f) + centerX, radiusY*sin(f) + centerY));
		}
		m_points.push_back(Point(radiusX + centerX, centerY));
	}
	virtual void clear() {}
private:
	std::vector<Point> m_points;
	std::vector<Point> circle_points;
	Color m_color;
	float m_width;
};