#pragma once
#include "myGL.h"
static int DX[] = { -1, 1, 0, 0 };
static int DY[] = { 0, 0, -1, 1 };
class Fill :public PicElem
{
public:
	void add(Point p,Color bcolor,float f)
	{
		Color color = getColor(p);
		m_points.push_back(p);
		if (g_defColor == color) return;
		std::queue<Point> q;
		q.push(p);
		int cnt = 0;
		while (!q.empty()) {
			cnt++;
			Point p = q.front();
			q.pop();
			for (int k = 0; k < 4; k++) {
				Point p2(p.pnt[0] + DX[k], p.pnt[1] + DY[k]);
				if (p2.valid() && getColor(p2) == color) {
					q.push(p2);
					setPoints_pixels(p2, g_defColor);
				}
			}
		}
	}
	void render() {
		for (unsigned int i = 0; i < m_points.size(); i++) {
			Point p = m_points[i];
			Color color = getColor(p);
			if (g_defColor == color) return;
			std::queue<Point> q;
			q.push(p);
			int cnt = 0;
			while (!q.empty()) {
				cnt++;
				Point p = q.front();
				q.pop();
				for (int k = 0; k < 4; k++) {
					Point p2(p.pnt[0] + DX[k], p.pnt[1] + DY[k]);
					if (p2.valid()&&getColor(p2) == color) {
						q.push(p2);
						p2.draw(g_defColor);
						pixels[(int)round(p2.pnt[0])][(int)round(p2.pnt[1])] = g_defColor;
					}
				}
			}
		}
	}
	float* output() {
		shape_type type = shape_fill;
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
			add(Point(input[2 * i], input[2 * i + 1]),g_defColor,1);
		}
		update();
	}
	virtual void add(float, float, float, float, float, float) {};
	virtual void add(float, float, float, float, float, float, float, float) {};
	virtual void add(Point, Point, Color, float) {};
	virtual void replaceLast(float, float) {};
	virtual void clear() {};
	virtual void eraseLast() {}
private:
	std::vector<Point> m_points;
};