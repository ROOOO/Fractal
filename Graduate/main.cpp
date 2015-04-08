//
//  main.cpp
//  Graduate
//
//  Created by King on 15/3/25.
//  Copyright (c) 2015年 King. All rights reserved.
//

//2015-01-07 19:22:03 ÷ÿ–¥2
//2015-03-25 重写3

#include "macro.h"
//#include <assert.h>

struct position {        // 用于3D交互，纪录鼠标位移、缩放
  float tx, ty, tz;
  float rx, ry, rz;
  float s;
  float angle;
}pos;

struct particlesMap {  // 窗口所有像素点组成的二维数组
  bool onTree;        // 是否在树上
  int tag;            // 标记（用于多种颜色）
}particlesMap[windowWidth][windowHeight];

pixels (*particles) = new pixels[maxPixels];        //一定数量的粒子

//void mySetOnTree(int x,int y)
//{
//    assert(x>=0&&x<windowWidth);
//    assert(y>0&&y<windowHeight);
//    particlesMap[x][y]=true;
//}

inline void setMap(GLint x, GLint y, int tag) {     // 当粒子粘上的时候执行，在窗口像素点的二维数组结构体中做标记
  particlesMap[x][y].onTree = true;
  particlesMap[x][y].tag = tag;
}

bool isNearTree(int x,int y, int tag) {  // 判断是否在旁边
  int xmin = x - 1 > -1 ? x - 1 : 0, xmax = x + 1 < windowWidth ? x + 1 : windowWidth - 1;
  int ymin = y - 1 > -1 ? y - 1 : 0, ymax = y + 1 < windowHeight ? y + 1 : windowHeight - 1;
  for (int i = xmin; i <= xmax; i++) { // 判断周围8个点是否是树上的点
    for (int j = ymin; j <= ymax; j++) {
      if (!(i == x && j == y) &&      // 除掉本身的点
          particlesMap[i][j].tag == tag &&    //标签相同
          particlesMap[i][j].onTree) { // 且周围有点在树上
        return true;
      }
    }
  }
  return false;
}

void init() {
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(windowWidth, windowHeight);
  glutInitWindowPosition(600, 100);
  glutCreateWindow("DLA Model");
  
  glClearColor(0.0, 0.0, 0.0, 0.0);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, windowWidth, 0.0, windowHeight);
  
  srand((unsigned)time(0));
  
  // 窗口像素点数组结构体初始化
  for (GLint x = 0; x < windowWidth; x++) {
    for (GLint y = 0; y < windowHeight; y++) {
      particlesMap[x][y].onTree = false;
    }
  }
  
  // 常规初始化
  for (GLint x = 0; x < maxPixels; x++) {
    // init 函数包含 x 坐标、 y 坐标、移动半径、颜色值（标签）
    // 下面使得粒子初始化随机在窗口中间，占据整个窗口 1 / 4 面积
    particles[x].init(rand() % (windowWidth / initRadiusRatio) + (windowWidth / 2 - windowWidth / initRadiusRatio / 2),
                      rand() % (windowHeight / initRadiusRatio) + (windowHeight / 2 - windowHeight / initRadiusRatio / 2), movingRadius,
                      //rand() % 3);  // 3 种颜色选用这个
    0);   // 一种颜色选用这个
  }
  
#if(0)
  // 中间一条线
  for (GLint i = 0 ; i < windowWidth / 2; i++) {
    particles[i].init(i + windowWidth / 4, windowHeight / 2, movingRadius, 0);
    particles[i].setOnTree(true);
    setMap(i + windowWidth / 4, windowHeight / 2, 0);
  }
#endif
  
#if(0)
  // 黑洞
  for (GLint x = 0; x < maxPixels / 5 ; x++) { // 中心圆
    particles[x].init(2 * (rand() % initRadius) * cos(x) + windowWidth / 2,
                      2 * (rand() % initRadius) * sin(x) + windowHeight / 2,
                      movingRadius * 2, 0);
  }
  
  for (GLint x = maxPixels / 5; x < maxPixels * 4 / 5; x++) { // 中心圆
    particles[x].init((initRadius) * cos(x) + windowWidth / 2,
                      (initRadius) * sin(x) + windowHeight / 2,
                      movingRadius * 2, 0);
  }
  
  for (GLint x = 0; x < maxPixels; x++) {
    particles[x].movingProbability(0.5, 0.5, 0, 0, 0, 0, 0, 0);
  }
#endif
}

void DLA() {
  // 下面初始化3个粒子，如果只需要1个，将另外2个注释掉
  particles[0].init(windowWidth / 2, windowHeight / 5 * 3, movingRadius, 0);
  particles[0].setOnTree(true);
  setMap(particles[0].getPositionX(), particles[0].getPositionY(), 0);
  
  
  particles[1].init(windowWidth / 5 * 2, windowHeight / 5 * 2, movingRadius, 1);
  particles[1].setOnTree(true);
  setMap(particles[1].getPositionX(), particles[1].getPositionY(), 1);
  
  particles[2].init(windowWidth / 5 * 3, windowHeight / 5 * 2, movingRadius, 2);
  particles[2].setOnTree(true);
  setMap(particles[2].getPositionX(), particles[2].getPositionY(), 2);
  
  // 每个粒子移动一步
  for (GLint x = 0; x < maxPixels; x++) {
    // 首先判断是否在树上，以及是否越界
    // 如果没有，所有粒子移动一步
    if (!particles[x].isOnTree() && !particles[x].judgeOutOfRange(windowWidth, windowHeight)) {
      particles[x].moveOneStep();
    }
  }
  
  for (int i = 0; i < maxPixels; i++) {
    if(!particles[i].isOnTree() && !particles[i].judgeOutOfRange(windowWidth, windowHeight)) {
      // 通过窗口的二维数组，判断当前粒子所在位置 周围 有没有在树上的像素点
      if(isNearTree(particles[i].getPositionX(), particles[i].getPositionY(), particles[i].getTag())) {
        particles[i].setOnTree(true);
        setMap(particles[i].getPositionX(), particles[i].getPositionY(), particles[i].getTag());
      }
    }
  }
}

void drawDLA() {     // 绘制函数
  for (GLint x = 0; x < maxPixels; x++) {
    if(!particles[x].isDisappear()) {
      glBegin(GL_POINTS);
      {
        glColor3f(particles[x].getColorRed(), particles[x].getColorGreen(), particles[x].getColorBlue());
        glVertex2i(particles[x].getPositionX(), particles[x].getPositionY());
      }
      glEnd();
    }
  }
}

inline void Display() {
  glClear(GL_COLOR_BUFFER_BIT);
  
  DLA();
  
  for (GLint i = 0; i < maxPixels; i++) {     // 每一帧update一次
    particles[i].update();
  }
  
  drawDLA();
  
  glutSwapBuffers();
  glutPostRedisplay();
}

struct particlesMap3D {
  bool onTree;
  int tag;
}particlesMap3D[max3DX][max3DY][max3DZ];

pixels *particles3D = new pixels[maxPixels];

void setMap3D(GLint x, GLint y, GLint z, int tag) {
  particlesMap3D[x][y][z].onTree = true;
  particlesMap3D[x][y][z].tag = tag;
}

bool isNearTree3D(GLint x, GLint y, GLint z, GLint tag) {
  int xmin = x - 1 > -1 ? x - 1 : 0, xmax = x + 1 < max3DX ? x + 1 : max3DX - 1;
  int ymin = y - 1 > -1 ? y - 1 : 0, ymax = y + 1 < max3DY ? y + 1 : max3DY - 1;
  int zmin = z - 1 > -1 ? z - 1 : 0, zmax = z + 1 < max3DZ ? z + 1 : max3DZ - 1;
  
  //    cout << x << "   " << y << "    " << z << endl;
  
  for (int i = xmin; i <= xmax; i++) {
    for (int j = ymin; j <= ymax; j++) {
      for (int k = zmin; k <= zmax; k++) {
        if (!(i == x && j == y && k == z) &&
            particlesMap3D[i][j][k].tag == tag &&
            particlesMap3D[i][j][k].onTree) {
          return true;
        }
      }
    }
  }
  
  //    x = x > -1 ? x : 0; x = x < max3DX ? x : max3DX - 1;
  //    y = y > -1 ? y : 0; y = y < max3DY ? y : max3DY - 1;
  //    z = z > -1 ? z : 0; z = z < max3DZ ? z : max3DZ - 1;
  //
  //    if (particlesMap3D[xmin][y][z].tag == tag && particlesMap3D[xmin][y][z].onTree)
  //        return true;
  //    else if (particlesMap3D[xmax][y][z].tag == tag && particlesMap3D[xmax][y][z].onTree)
  //        return true;
  //    else if (particlesMap3D[x][ymin][z].tag == tag && particlesMap3D[x][ymin][z].onTree)
  //        return true;
  //    else if (particlesMap3D[x][ymax][z].tag == tag && particlesMap3D[x][ymax][z].onTree)
  //        return true;
  //    else if (particlesMap3D[x][y][zmin].tag == tag && particlesMap3D[x][y][zmin].onTree)
  //        return true;
  //    else if (particlesMap3D[x][y][zmax].tag == tag && particlesMap3D[x][y][zmax].onTree)
  //        return true;
  
  
  return false;
}

void init3D() {
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(windowWidth, windowHeight);
  glutInitWindowPosition(600, 150);
  glutCreateWindow("3D DLA Model");
  
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
  glClearDepth(10.0f);                   // Set background depth to farthest
  glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
  
  glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
  glShadeModel(GL_SMOOTH);   // Enable smooth shading
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //    glOrtho(-8.0 * 64 / 48.0, 8.0 * 64 / 48.0, -8.0, 8.0, 0.1, 100);
  gluPerspective(60.0f, 1.0f, .01f, 2000.0f);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(150.0, 150.0, 150.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  
  srand((unsigned int)time(0));
  
  static int a = 0;
  
  for (int i = 0; i < max3DX; i++) {
    for (int j = 0; j < max3DY; j++) {
      for (int k = 0; k < max3DZ; k++) {
        particlesMap3D[i][j][k].onTree = false;
        //                particlesMap3D[i][j][k].tag = 0;
        //                cout << a++ << endl;
      }
    }
  }
  
  for (int i = 0; i <= maxPixels; i++) {
    particles3D[i].init3D(rand() % max3DX, rand() % max3DY, rand() % max3DZ, movingRadius, 0);
  }
  
  pos.rx = pos.ry = pos.tx = pos.ry = pos.rz = pos.tz = 0;
  pos.s = 1;
  pos.angle = 0;
  
  //    glPointSize(1.0f);
}

void axis(double length) {
  glLineWidth(5.0);
  glBegin(GL_LINES);
  {
    glVertex3d(0, 0, 0);
    glVertex3d(0, 0, length);
  }
  glEnd();
  
  glPushMatrix();
  glTranslated(0, 0, length - 0.2);
  glutWireCone(4, 0.2, 12, 9);
  
  glLineWidth(2.0);
  glutWireCone(4, 10, 10, 10);
  
  glPopMatrix();
}

void drawAxis(double length)
{
  glPushMatrix();
  glRotatef(pos.ry, 0, 1, 0);
  glRotatef(pos.rx, 0, 0, 1);
  glScalef(pos.s, pos.s, pos.s);
  
  glColor3d(0, 0, 1);
  axis(length);
  
  glPushMatrix();
  glRotated(90, 0, 1.0, 0);
  glColor3d(1, 0, 0);
  axis(length);
  glPopMatrix();
  
  glPushMatrix();
  glRotated(-90.0, 1, 0, 0);
  glColor3d(0, 1, 0);
  axis(length);
  glPopMatrix();
  
  glPopMatrix();
}

void Grid(int gridLength) {
  glBegin(GL_LINES);
  {
    glColor3f(0.5, 0.5, 0.5);
    glVertex3d(-gridLength, 0, 0);
    glVertex3d(gridLength, 0, 0);
  }
  glEnd();
}
void drawGrid(int gridLength) {
  for (int i = -gridLength; i <= gridLength; i += 5) {
    glLineWidth(1.0);
    glPushMatrix();
    glRotatef(pos.ry, 0, 1, 0);
    glRotatef(pos.rx, 0, 0, 1);
    glScalef(pos.s, pos.s, pos.s);
    
    glPushMatrix();
    glTranslated(0, 0, i);
    Grid(gridLength);
    glPopMatrix();
    
    glPushMatrix();
    glRotated(90, 0, 1, 0);
    glTranslated(0, 0, i);
    Grid(gridLength);
    glPopMatrix();
    
    glPopMatrix();
  }
}

void DLA3D() {
  particles3D[0].init3D(max3DX / 2, max3DY / 2, max3DZ / 2, movingRadius, 0);
  particles3D[0].setOnTree(true);
  setMap3D(particles3D[0].getPositionX(), particles3D[0].getPositionY(), particles3D[0].getPositionZ(), 0);
  
  for (GLint x = 0; x < maxPixels; x++) {
    if (!particles3D[x].judgeOutOfRange(max3DX, max3DY, max3DZ) && !particles3D[x].isOnTree()) {
      particles3D[x].moveOneStep();
    }
  }
  
  for (int i = 0; i < maxPixels; i++) {
    if (!particles3D[i].judgeOutOfRange(max3DX, max3DY, max3DZ) &&
        !particles3D[i].isOnTree()) {
      if (isNearTree3D(
                       particles3D[i].getPositionX(),
                       particles3D[i].getPositionY(),
                       particles3D[i].getPositionZ(),
                       particles3D[i].getTag())) {
        particles3D[i].setOnTree(true);
        setMap3D(particles3D[i].getPositionX(),
                 particles3D[i].getPositionY(),
                 particles3D[i].getPositionZ(),
                 particles3D[i].getTag());
      }
    }
  }
}

void drawDLA3D() {
  glPushMatrix();
  glRotatef(pos.ry, 0, 1, 0);
  glRotatef(pos.rx, 0, 0, 1);
  glScalef(pos.s, pos.s, pos.s);
  
  for (GLint x = 0; x < maxPixels; x++) {
    if(!particles3D[x].isDisappear()) {
      glBegin(GL_POINTS);
      {
        glColor3f(particles3D[x].getColorRed(), particles3D[x].getColorGreen(), particles3D[x].getColorBlue());
        glVertex3i(particles3D[x].getPositionX(), particles3D[x].getPositionY(), particles3D[x].getPositionZ());
      }
      glEnd();
    }
  }
  
  glPopMatrix();
}

void display3D() {
  //glClearColor(1, 1, 1, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //    glEnable(GL_POINT_SMOOTH);
  
  drawGrid(100);
  drawAxis(50);
  
  DLA3D();
  drawDLA3D();
  
  for (int i = 0; i < maxPixels; i++) {
    particles3D[i].update();
  }
  
  glutSwapBuffers();
  glutPostRedisplay();
}

void keyBoard(unsigned char key, int x, int y) {
  //DLA();
  if (key == '+') {
    pos.s += 0.1;
  } else if (key == '-') {
    pos.s -= 0.1;
  }
  
  glutPostRedisplay();
}

typedef struct {
  GLint x, y;
}GLintPoint;
GLintPoint mousePos[2];

void mouseMotion(int x, int y) {
  mousePos[1].x = x;
  mousePos[1].y = windowHeight - y;
  pos.ry += (mousePos[1].x - mousePos[0].x);
  pos.rx += (mousePos[1].y - mousePos[0].y);
  mousePos[0] = mousePos[1];
}

void mouse(int button, int state, int x, int y) {
  if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
    mousePos[0].x = x;
    mousePos[0].y = windowHeight - y;
  }
}

int main(int argc, char ** argv) {
  glutInit(&argc, argv);
  
  if (!is3D) {
    init();
    glutDisplayFunc(Display);
  } else {
    init3D();
    glutDisplayFunc(display3D);
  }
  glutKeyboardFunc(keyBoard);
  
  glutMouseFunc(mouse);
  glutMotionFunc(mouseMotion);
  
  glutMainLoop();
}
