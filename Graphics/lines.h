#pragma once
#include "shapes.h"
#include "myGL.h"
class Lines : public PicElem
{
public:
	Lines() {}

	void render()
	{

		for (size_t i = 0; i != m_point1.size(); ++i)
		{
			glLineWidth(m_width[i]);
			glColor3fv(m_colors[i].color);
			glBegin(GL_LINES);
			glVertex2iv(m_point1[i].pnt);
			glVertex2iv(m_point2[i].pnt);
			glEnd();
		}
	}

	virtual void add(float, float, float, float, float, float) {}
	virtual void add(Point, Color, float) {};
	void add(Point p1, Point p2, Color color, float width)
	{
		m_point1.push_back(p1);
		m_point2.push_back(p2);
		m_colors.push_back(color);
		m_width.push_back(width);
	}
	void add(float x1, float y1, float x2, float y2, float r, float g, float b, float width)
	{
		add(Point(x1, y1), Point(x2, y2), Color(r, g, b), width);
	}

	void replaceLast(float x, float y)
	{
		m_point2[m_point2.size() - 1] = Point(x, y);
	}
	void update()
	{
		for (int i = 0; i < m_point1.size(); i++) {
			setPoints_pixels(m_point1[i], m_point2[i], g_defColor);
		}
	}
	void clear()
	{
		m_point1.clear();
		m_point2.clear();
		m_colors.clear();
		m_width.clear();
	}
	void eraseLast()
	{
		if (m_point1.empty()) return;
		m_point1.erase(m_point1.end() - 1);
		m_point2.erase(m_point2.end() - 1);
		m_colors.erase(m_colors.end() - 1);
		m_width.erase(m_width.end() - 1);
	}
	int* output() {
		int size = m_point1.size();
		int* output = new int[4 * size + 2];
		output[0] = 4 * size + 2;
		output[1] = type;
		for (int i = 0; i < size; i++) {
			output[4 * i + 2] = m_point1[i].pnt[0];
			output[4 * i + 3] = m_point1[i].pnt[1];
			output[4 * i + 4] = m_point2[i].pnt[0];
			output[4 * i + 5] = m_point2[i].pnt[1];
		}
		return output;
	}
	void input(int *input, int size) {
		int cnt = (size - 2) / 4;
		m_point1.clear();
		m_point2.clear();
		for (int i = 0; i < cnt; i++) {
			m_point1.push_back(Point(input[4 * i], input[4 * i + 1]));
			m_point2.push_back(Point(input[4 * i+2], input[4 * i + 3]));
		}
		update();
	}
	int get_size() {
		return m_point1.size();
	}
private:
	std::vector<Point> m_point1;
	std::vector<Point> m_point2;
	std::vector<Color> m_colors;
	std::vector<float> m_width;
	shape_type type = shape_lines;
};