#pragma once
#include "common.h"
#include "myGL.h"
#include "polygons.h"

class Cut:public Polygons
{
public:
	Cut() {
		if (mode == mode_CUT_IN) type = shape_cut_in;
		else type = shape_cut_out;
	}
	void cut_inside()
	{
		is_cutted = 1;
		std::vector<Point> points = inner_points(); 
		cutPoints_pixels(&points);
	}
	void cut_outside()
	{
		is_cutted = 1;
		std::vector<Point> points = inner_points();
		for (int i = 0; i < points.size(); i++) {
			int x = points[i].pnt[0];
			int y = points[i].pnt[1];
			pixels_cnt[x][y] += 1000;
		}
		for (int i = 0; i < m_lines.size(); i++) {
			cutPoints_pixels(&m_lines[i]);
		}
		for (int i = 0; i < g_cliWidth; i++) {
			for (int j = 0; j < g_cliHeight; j++) {
				if (pixels_cnt[i][j] < 900) {
					pixels[i][j] = g_blackColor;
				}
				else {
					pixels_cnt[i][j] %= 1000;
				}
			}
		}
	}
	void update() {
		clear();
		for (int i = 1; i < m_points.size(); i++) {
			std::vector<Point> *tmp_points = getPoints(m_points[i - 1], m_points[i]);
			m_lines.push_back(*tmp_points);
			setPoints_pixels(tmp_points, g_redColor);
		}
		std::vector<Point> *tmp_points = getPoints(m_points[0], m_points[m_points.size() - 1]);
		m_lines.push_back(*tmp_points);
		setPoints_pixels(tmp_points, g_redColor);
		if (is_cutted) {
			if (type == shape_cut_in) cut_inside();
			else cut_outside();
		}
	}
	int* output() {
		int size = m_points.size();
		int* output = new int[2 * size + 2];
		output[0] = 2 * size + 2;
		output[1] = type;
		for (int i = 0; i < size; i++) {
			output[2 * i + 2] = m_points[i].pnt[0];
			output[2 * i + 3] = m_points[i].pnt[1];
		}
		return output;
	}
	void input(int *input, int size) {
		is_cutted = 1;
		int cnt = (size - 2) / 2;
		m_points.clear();
		for (int i = 0; i < cnt; i++) {
			m_points.push_back(Point(input[2 * i], input[2 * i + 1]));
		}
		update();
	}
	shape_type type;
	int is_cutted = 0;
};
