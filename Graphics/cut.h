#pragma once
#include "common.h"
#include "myGL.h"
#include "polygons.h"

class Cut:public Polygons
{
public:
	void cut_inside()
	{
		std::vector<Point> points = inner_points();
		cutPoints_pixels(&points);
	}
	void cut_outside()
	{
		std::vector<Point> points = inner_points();
		for (int i = 0; i < points.size(); i++) {
			int x = points[i].pnt[0];
			int y = points[i].pnt[1];
			pixels_cnt[x][y] += 1000;
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

private:

};
