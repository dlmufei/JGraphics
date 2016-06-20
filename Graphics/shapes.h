#pragma once
#include "common.h"

#define PI	3.141592654f
#define PI2	PI*2

enum shape_type{shape_bezier,shape_circle,shape_cut_in,shape_cut_out,shape_fill,shape_lines,shape_pencil,shape_polygon};
struct Color
{
	Color() {}

	Color(float r, float g, float b)
	{
		color[0] = r>1.0f ? 1.0f : r;
		color[1] = g>1.0f ? 1.0f : g;
		color[2] = b>1.0f ? 1.0f : b;
	}
	float color[3];
	bool operator == (Color b)
	{
		if (color[0] == b.color[0] && color[1] == b.color[1] && color[2] == b.color[2])
			return true;
		else return false;
	}
};
class Point
{
public:
	Point(int vx, int vy)
	{
		pnt[0] = vx;
		pnt[1] = vy;
	}
	void draw(Color color) 
	{
		glColor3fv(color.color);
		glBegin(GL_POINTS);
		glVertex2iv(pnt);
		glEnd();
	}
	bool valid()
	{
		if (pnt[0] < 0 || pnt[0] >= g_cliWidth) return false;
		if (pnt[1] < 0 || pnt[1] >= g_cliWidth) return false;
		return true;
	}
	int pnt[2];
};


class PicElem
{
public:
	// PicElem() {};
	virtual void render() = 0;
	virtual void add(float, float, float, float, float, float) = 0;
	virtual void add(Point, Color, float) = 0;
	virtual void add(float, float, float, float, float, float, float, float) = 0;
	virtual void add(Point, Point, Color, float) = 0;
	virtual void replaceLast(float, float) = 0;
	virtual void clear() = 0;
	virtual void eraseLast() {}
	virtual void update() {}
	virtual int* output() { return NULL; }
	virtual void input(int *,int) {}
	//	virtual void add(float, float, Color, float);
	//	virtual void 
	virtual ~PicElem() {};
	shape_type type;
};

class Points : public PicElem
{
public:
	Points() {}
	void render()
	{
		for (size_t i = 0; i != m_points.size(); ++i)
		{
			glPointSize(m_size[i]);
			glColor3fv(m_colors[i].color);
			glBegin(GL_POINTS);
			glVertex2iv(m_points[i].pnt);
			glEnd();
		}
	}
	virtual void add(float, float, float, float, float, float, float, float) {};
	virtual void add(Point, Point, Color, float) {};
	void add(Point p, Color color, float size)
	{
		m_points.push_back(p);
		m_colors.push_back(color);
		m_size.push_back(size);
	}
	void add(float x, float y, float r, float g, float b, float size)
	{
		add(Point(x, y), Color(r, g, b), size);
	}

	void replaceLast(float x, float y)
	{
		m_points[m_points.size() - 1] = Point(x, y);
	}
	void clear()
	{
		m_points.clear();
		m_colors.clear();
		m_size.clear();
	}
	void eraseLast()
	{
		if (m_points.empty()) return;
		m_points.erase(m_points.end() - 1);
		m_colors.erase(m_colors.end() - 1);
		m_size.erase(m_size.end() - 1);
	}
private:
	std::vector<Point> m_points;
	std::vector<Color> m_colors;
	std::vector<float> m_size;
};

extern Color** pixels;
extern std::vector<PicElem*> vec;
extern Color g_defColor;
extern Color g_whiteColor;
extern Color g_blackColor;
extern Color g_redColor;