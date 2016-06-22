#pragma once
#include "shapes.h"
#include "myGL.h"

class Polygons : public PicElem
{
public:
	Polygons() {}

	void render()
	{
		glLineWidth(m_width);
		glBegin(GL_POLYGON);
		glColor3fv(m_color.color);
		for (int i = 0; i != m_points.size(); ++i)
		{
			glVertex2fv(m_points[i].pnt);
		}
		glEnd();
	}
	virtual void add(float, float, float, float, float, float) {}

	virtual void add(float x1, float y1, float x2, float y2, float r, float g, float b, float width) {}

	void add(Point p1, Point p2, Color color, float width)
	{
		m_points.push_back(p1);
		m_points.push_back(p2);
		m_color = color;
		m_width = width;
		std::vector<Point> *tmp_points = getPoints(p1, p2);
		m_lines.push_back(*tmp_points);
	}

	void add(Point p, Color color, float width)
	{
		m_points.push_back(p);
		update();
		//m_color = color;
		//m_width = width;
	};
	
	void clear() {
		for (unsigned int i = 0; i < m_lines.size(); i++) {
			setPoints_pixels(&m_lines[i], g_blackColor);
			m_lines[i].clear();
		}
		m_lines.clear();
	}
	void update() {
		clear();
		for (unsigned int i = 2; i < m_points.size(); i++) {
			std::vector<Point> *tmp_points=getPoints(m_points[i - 1], m_points[i]);
			m_lines.push_back(*tmp_points);
			setPoints_pixels(tmp_points, g_defColor);
		}
		std::vector<Point> *tmp_points = getPoints(m_points[1], m_points[m_points.size()-1]);
		m_lines.push_back(*tmp_points);
		setPoints_pixels(tmp_points, g_defColor);
	}
	void move(int x, int y)
	{
		for (unsigned int i = 0; i < m_points.size(); i++) {
			m_points[i].pnt[0] += x;
			m_points[i].pnt[1] += y;
		}
	}
	void rotation(Point last, Point newP) {
		float o_x = 0, o_y = 0;
		for (unsigned int i = 0; i < m_points.size(); i++) {
			o_x += m_points[i].pnt[0];
			o_y += m_points[i].pnt[1];
		}
		o_x /= m_points.size();
		o_y /= m_points.size();
		double x0 = last.pnt[0]-o_x;
		double y0 = last.pnt[1]-o_y;
		double x1 = newP.pnt[0]-o_x;
		double y1 = newP.pnt[1]-o_y;
		double s = (x1*y0 - x0*y1) / (x0*x0 + y0*y0);
		double c = (x0*x1 + y0*y1) / (x0*x0 + y0*y0);
		for (unsigned int i = 0; i < m_points.size(); i++) {
			double x0 = m_points[i].pnt[0], y0 = m_points[i].pnt[1];
			x0 -= o_x;
			y0 -= o_y;
			m_points[i].pnt[0] = (float)(x0*c + y0*s + o_x);
			m_points[i].pnt[1] = (float)(-x0*s + y0*c + o_y);
		}
	}
	void rotation(double angle) {
		float o_x = 0, o_y = 0;
		for (unsigned int i = 0; i < m_points.size(); i++) {
			o_x += m_points[i].pnt[0];
			o_y += m_points[i].pnt[1];
		}
		o_x /= m_points.size();
		o_y /= m_points.size();
		for (unsigned int i = 0; i < m_points.size(); i++) {
			float x0 = m_points[i].pnt[0], y0 = m_points[i].pnt[1];
			x0 -= o_x;
			y0 -= o_y;
			m_points[i].pnt[0] = x0*cos(angle) + y0*sin(angle)+o_x;
			m_points[i].pnt[1] = -x0*sin(angle) + y0*cos(angle)+o_y;
		}
	}
	void replaceLast(float x, float y)
	{
		clear();
		//if(m_points.size()==m_lines.size()&&m_points.size()>=5)
		//	setPoints_pixels(&m_lines[m_lines.size() - 1], g_blackColor);
		m_points[m_points.size() - 1] = Point(x, y);
	}
	std::vector<Point> inner_points() {
		std::vector<Point> inner_points;
		std::vector<int> *yIndex = new std::vector<int>[g_cliWidth];
		//inner_points.insert(inner_points.end(), m_points.begin()+1, m_points.end());
		for (unsigned int i = 0; i < m_lines.size(); i++) {
			std::vector<Point> tmp = m_lines[i];
			inner_points.insert(inner_points.end(), tmp.begin(), tmp.end());
			int lastx = (int)round(tmp[0].pnt[0]), lasty = (int)round(tmp[0].pnt[1]);
			yIndex[lastx].push_back(lasty);
			for (unsigned int j = 1; j < tmp.size(); j++) {
				int x = (int)round(tmp[j].pnt[0]), y = (int)round(tmp[j].pnt[1]);
				if (x == lastx) continue;
				else yIndex[x].push_back(y);
				lastx = x;
				lasty = y;
			}
			if (lastx < tmp[0].pnt[0]) yIndex[lastx].push_back(lasty);
			else yIndex[(int)round(tmp[0].pnt[0])].push_back((int)round(tmp[0].pnt[1]));
		}
		for (int x = 0; x < g_cliWidth; x++) {
			sort(yIndex[x].begin(), yIndex[x].end());
			for (unsigned int i = 0; i+1 < yIndex[x].size() ; i += 2) {
				for (int y = yIndex[x][i]; y <= yIndex[x][i + 1]; y++) {
					inner_points.push_back(Point(x, y));
				}
			}
		}
		return inner_points;
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
protected:
	std::vector<Point> m_points;
	std::vector<std::vector<Point>> m_lines;
	Color m_color;
	float m_width;
	shape_type type = shape_polygon;
};