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
			glVertex2iv(m_points[i].pnt);
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

};