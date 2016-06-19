#pragma once
#include "common.h"
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
					//p2.draw(g_defColor);
					pixels[p2.pnt[0]][p2.pnt[1]] = g_defColor;
					//std::cout << p2.pnt[0] << " " << p2.pnt[1] << "\n";
				}
			}
		}
	}
	void render() {
		for (int i = 0; i < m_points.size(); i++) {
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
						pixels[p2.pnt[0]][p2.pnt[1]] = g_defColor;
						//std::cout << p2.pnt[0] << " " << p2.pnt[1] << "\n";
					}
				}
			}
		}
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