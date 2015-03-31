//
//  macro.h
//  Graduate
//
//  Created by King on 15/3/27.
//  Copyright (c) 2015年 King. All rights reserved.
//

#ifndef __Graduate__macro__
#define __Graduate__macro__

#include <stdio.h>

//#include <windows.h>
//#include "gl/glut.h"

#include <GLUT/GLUT.h>

#include <iostream>
#include <math.h>
#include "pixels.h"

using namespace std;

const int windowWidth = 700;
const int windowHeight = 700;

const int movingRadius = 30;

const int maxPixels = 30000;

const int initRadiusRatio = 2;

const int initRadius = 100;

const GLfloat epsilon = 0.005;

const int max3DX = 100;
const int max3DY = 100;
const int max3DZ = 100;

const bool is3D = false; // 3D setting.

#endif /* defined(__Graduate__macro__) */