#pragma once
#include "polygons.h"


class Pencil : public Polygons
{
public:
	void render()
	{
		glLineWidth(m_width);
		glBegin(GL_LINE_STRIP);
		glColor3fv(m_color.color);
		for (int i = 0; i != m_points.size(); ++i)
		{
			glVertex2fv(m_points[i].pnt);
		}
		glEnd();
	}

	void add(Point p, Color color, float width)
	{
		if(m_points.size()>0)
			setPoints_pixels(m_points[m_points.size() - 1], p, color);
		m_points.push_back(p);
		m_color = color;
		m_width = width;
	};
	void update() {
		for (unsigned int i = 1; i < m_points.size(); i++) {
			std::vector<Point> *tmp_points = getPoints(m_points[i - 1], m_points[i]);
			setPoints_pixels(tmp_points, g_defColor);
		}
	}
	float* output() {
		int size = m_points.size();
		float* output = new float[2 * size + 2];
		output[0] = 2 * size + 2;
		output[1] = type;
		for (int i = 0; i < size; i++) {
			output[2 * i + 2] = m_points[i].pnt[0];
			output[2 * i + 3] = m_points[i].pnt[1];
		}
		return output;
	}
	void input(float *input, int size) {
		int cnt = (size - 2) / 2;
		m_points.clear();
		for (int i = 0; i < cnt; i++) {
			m_points.push_back(Point(input[2 * i], input[2 * i + 1]));
		}
		update();
	}
	shape_type type = shape_pencil;
};