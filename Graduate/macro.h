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
#include <vector>
#include <assert.h>
#include <time.h>

using namespace std;

const double PI = 3.14159265;

const int maxPace = 00;

const GLint lightDisappearRadius = 80; // 距离太阳多近就消失

const int windowWidth = 700;
const int windowHeight = 700;

const int movingRadius = 300;    // 最大移动半径

const int initRadiusRatio = 2; // 比例

const int initRadius = 100;  // 初始粒子生成半径

const GLfloat epsilon = 0.005; // 误差限

const int CircleRadius = windowWidth / 9;

const int lightRadius = 100;

// 下面3个分别表示初始粒子最大的 x、y、z坐标
const int max3DX = 100;
const int max3DY = 100;
const int max3DZ = 100;

const bool is3D = false; // 是否需要 3D 实现

#endif /* defined(__Graduate__macro__) */
