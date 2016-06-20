#pragma once
#include "gl/GL.h"
#include "gl/GLU.h"
#include "math.h"
#include "windows.h"
#include <vector>
#include <iostream>
#include <string.h>
#include <queue>
#include <malloc.h>


enum MODE { mode_PENCIL, mode_POINTS, mode_LINES, mode_POLYGON, mode_POLYGON2, mode_CIRCLE, mode_Bezier, mode_Bezier2, mode_Fill ,\
	mode_CUT_IN, mode_CUT_IN2, mode_CUT_OUT, mode_CUT_OUT2,mode_MOVE,mode_ERROR,mode_CUBE};
