//
//  main.cpp
//  Graduate
//
//  Created by King on 15/3/25.
//  Copyright (c) 2015年 King. All rights reserved.
//

#include "macro.h"
//#include <assert.h>

int maxPixels = 30000;    // 最大粒子数
int onePace = 1;
bool drawRect = false;
bool pause = false;
GLint sandboxWidth = 0, sandboxHeight = 0;

#define clean         0
#define init_one       1
#define init_three     2
#define init_circle    3
#define init_light     4
#define init_centerLine 5
#define init_blackHole  6
#define init_one_2  8

#define p_0   101
#define p_1   102
#define p_2   104
#define p_3   108
#define p_4   116
#define p_5   132
#define p_6   164
#define p_add 200
#define p_minus 201

#define k_10  10000
#define k_20  20000
#define k_30  30000
#define k_40  40000
#define k_50  50000
#define k_60  60000
#define k_70  70000
#define k_80  80000
#define k_90  90000
#define k_add 11111
#define k_minus 22222

#define redOn 10
#define redOff 11

#define blockEffect 999
#define bg_white  998
#define bg_black  997

int init_Particles();
int initial_One();
int initial_Three();
int initial_Circle();
int initialLight();
int initial_CenterLine();
int initial_BlackHole();

bool isBlockEffect = false;

void setType() {
  init_Particles();
  
  //  initial_One();      // 中间一个点
  //    initial_Three();  // 三个点
  //  initial_Circle();   // 一个圈，向内生长
  //  initial_CenterLine(); // 中间一条线
  //  initial_BlackHole();  // 黑洞
  //    initialLight(); // 趋光性模拟
}

struct particlesMap {  // 窗口所有像素点组成的二维数组
  bool onTree;        // 是否在树上
  int tag;            // 标记（用于多种颜色）
}particlesMap[windowWidth][windowHeight];

pixels (*particles) = new pixels[maxPixels];        //一定数量的粒子

bool isBlack = particles[maxPixels - 1].blackBG;

typedef struct {
  GLint x, y;
}GLintPoint;
GLintPoint mousePos[2];

bool touched = false;
bool isBlockLR = false;

inline void setMap(GLint x, GLint y, int tag) {     // 当粒子粘上的时候执行，在窗口像素点的二维数组结构体中做标记
  particlesMap[x][y].onTree = true;
  particlesMap[x][y].tag = tag;
}

struct position {        // 用于3D交互，纪录鼠标位移、缩放
  float tx, ty, tz;
  float rx, ry, rz;
  float s;
  float angle;
}pos;

class sunClass {
public:
  GLint x;
  GLint y;
  GLint size;
  GLint n;
}sun;

bool light = false;
bool circle = false;
bool isClean = false;

int cleanFunc() {
  delete []particles;
  particles = new pixels[maxPixels];
  
  glClear(GL_COLOR_BUFFER_BIT);
  isClean = true;
  light = false;
  circle = false;
  isBlockLR = false;
  return 0;
}

int frame = 0;
int curTime = 0, lastTime = 0;
float fps = 0.0;
char fpsText[8];

void displayFPS() {
  frame++;
  curTime = glutGet(GLUT_ELAPSED_TIME);
  
  if (curTime - lastTime > 100) {
    fps = frame * 1000.0 / (curTime - lastTime);
    //    printf("%.1f\n", fps);
    lastTime = curTime;
    frame = 0;
  }
  sprintf(fpsText, "FPS : %.1f", fps);
}

void displayFPSText(GLint x, GLint y, GLfloat r, GLfloat g, GLfloat b, const char *string ) {
  displayFPS();
  
  glColor3f(r, g, b);
  glRasterPos2f(x, y);
  for( int i = 0; i < strlen(string); i++ ) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
  }
  glutPostRedisplay();
}

char maxPixelsText[8];

void displayMaxPixels() {
  sprintf(maxPixelsText, "Max Pixels : %d", maxPixels);
}

void displayMaxPixelsText(GLint x, GLint y, GLfloat r, GLfloat g, GLfloat b, const char *string ) {
  displayMaxPixels();
  
  glColor3f(r, g, b);
  glRasterPos2f(x, y);
  for( int i = 0; i < strlen(string); i++ ) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
  }
  glutPostRedisplay();
}

char paceText[4];

void displayPace() {
  sprintf(paceText, "Pace : %d", particles[maxPixels - 1].getPace());
}

void displayPaceText(GLint x, GLint y, GLfloat r, GLfloat g, GLfloat b, const char *string ) {
  displayPace();
  
  glColor3f(r, g, b);
  glRasterPos2f(x, y);
  for( int i = 0; i < strlen(string); i++ ) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
  }
  glutPostRedisplay();
}

char typeText[16] = { "Type : Initial" };

void displayType(char *str) {
  sprintf(typeText, "Type : %s", str);
}

void displayTypeText(GLint x, GLint y, GLfloat r, GLfloat g, GLfloat b, const char *string ) {
  glColor3f(r, g, b);
  glRasterPos2f(x, y);
  for( int i = 0; i < strlen(string); i++ ) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
  }
  glutPostRedisplay();
}

char sunText[32];

void displaySun() {
  sprintf(sunText, "Sun : x : %d   y : %d", sun.x, sun.y);
}

void displaySunText(GLint x, GLint y, GLfloat r, GLfloat g, GLfloat b, const char *string ) {
  displaySun();
  
  glColor3f(r, g, b);
  glRasterPos2f(x, y);
  for( int i = 0; i < strlen(string); i++ ) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
  }
  glutPostRedisplay();
}

char totalOnTreePixels[8];
int countPixels();

void displayTotalOnTreePixels() {
  sprintf(totalOnTreePixels, "Total seeds : %d", countPixels());
}
void displayTOtalOnTreePixelsText(GLint x, GLint y, GLfloat r, GLfloat g, GLfloat b, const char *string) {
  displayTotalOnTreePixels();
  
  glColor3f(r, g, b);
  glRasterPos2f(x, y);
  for( int i = 0; i < strlen(string); i++ ) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
  }
  glutPostRedisplay();
}

char dimensionText[16];
double coefficient = 0;
void LinearRegression();

void displayDimension() {
  LinearRegression();
  coefficient = (coefficient > 0 || coefficient < 4) ? coefficient : 0;
  sprintf(dimensionText, "Fractal Dimension : %.3lf", coefficient);
}
void displayDimensionText(GLint x, GLint y, GLfloat r, GLfloat g, GLfloat b, const char *string) {
  displayDimension();
  
  glColor3f(r, g, b);
  glRasterPos2f(x, y);
  for( int i = 0; i < strlen(string); i++ ) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
  }
  glutPostRedisplay();
}

int init_Particles() {
  delete []particles;
  particles = new pixels[maxPixels];
  
  light = false;
  circle = false;
  isBlockLR = false;
  isBlockEffect = false;
  drawRect = false;
  
  // 像素点数组结构体初始化
  for (GLint x = 0; x < windowWidth; x++) {
    for (GLint y = 0; y < windowHeight; y++) {
      particlesMap[x][y].onTree = false;
    }
  }
  
  // 常规初始化
  for (int x = 0; x < maxPixels; x++) {
    // init 函数包含 x 坐标、 y 坐标、移动半径、颜色值（标签）
    // 下面使得粒子初始化随机在窗口中间，占据整个窗口 1 / 4 面积
    particles[x].init(rand() % (windowWidth / initRadiusRatio) + (windowWidth / 2 - windowWidth / initRadiusRatio / 2),
                      rand() % (windowHeight / initRadiusRatio) + (windowHeight / 2 - windowHeight / initRadiusRatio / 2), movingRadius,
                      //rand() % 3);  // 3 种颜色选用这个
                      0);   // 一种颜色选用这个
    particles[x].threeColor = false;
    particles[x].blackBG = isBlack;
    //    particles[x].movingProbability(0.25, 0.25, 0.25, 0.25, 0, 0, 0, 0);
  }
  
  return 0;
}

bool isNearTree(int x,int y, int tag) {  // 判断是否在旁边
  int xmin = x - 1 > -1 ? x - 1 : 0, xmax = x + 1 < windowWidth ? x + 1 : windowWidth - 1;
  int ymin = y - 1 > -1 ? y - 1 : 0, ymax = y + 1 < windowHeight ? y + 1 : windowHeight - 1;
  
  srand((unsigned)time(0));
  int random = rand() % 100;
  
  for (int i = xmin, count = 0; i <= xmax; i++, count++) { // 判断周围8个点是否是树上的点
    for (int j = ymin; j <= ymax; j++, count++) {
      if (!(i == x && j == y) &&      // 除掉本身的点
          particlesMap[i][j].tag == tag &&    //标签相同
          particlesMap[i][j].onTree) { // 且周围有点在树上
        
        if (!light) {
          return true;    // 非趋光性取消注释这个
        } else {
          
          // 趋光性采用下面这个
          if (((x - i) * (sun.x - i) + (y - j) * (sun.y - j)) /
              sqrt(double((x - i) * (x - i) + (y - j) * (y - j))) /
              sqrt(double((sun.x - i) * (sun.x - i) + (sun.y - j) * (sun.y - j))) >= (sqrt(2) / 2)) {
            if (random < 80) {
              return true;
            } else {
              return false;
            }
          } else if (((x - i) * (sun.x - i) + (y - j) * (sun.y - j)) /
                     sqrt(double((x - i) * (x - i) + (y - j) * (y - j))) /
                     sqrt(double((sun.x - i) * (sun.x - i) + (sun.y - j) * (sun.y - j))) > 0) {
            if (random < 50) {
              return true;
            } else {
              return false;
            }
          } else {
            if (random < 20) {
              return true;
            } else {
              return false;
            }
          }
        }
      }
    }
  }
  return false;
}

int initial_One() { //  一个在中间的粒子
  if (!isBlockEffect) {
    init_Particles();
  }
  
  particles[0].init(windowWidth / 2, windowHeight / 2, movingRadius, 0);
  particles[0].setOnTree(true);
  setMap(particles[0].getPositionX(), particles[0].getPositionY(), 0);
  
  return 0;
}
int initial_One_2() { //  一个在中间的粒子
  init_Particles();
  isBlockLR = true;
  particles[0].init(windowWidth / 2, windowHeight / 2, movingRadius, 0);
  particles[0].setOnTree(true);
  setMap(particles[0].getPositionX(), particles[0].getPositionY(), 0);
  
  return 0;
}

int initial_Three() { // 3 个粒子的情况（不同颜色）
  init_Particles();
  
  particles[0].init(windowWidth / 2, windowHeight / 5 * 3, movingRadius, 0);
  particles[0].setOnTree(true);
  setMap(particles[0].getPositionX(), particles[0].getPositionY(), 0);
  
  
  particles[1].init(windowWidth / 5 * 2, windowHeight / 5 * 2, movingRadius, 1);
  particles[1].setOnTree(true);
  setMap(particles[1].getPositionX(), particles[1].getPositionY(), 1);
  
  particles[2].init(windowWidth / 5 * 3, windowHeight / 5 * 2, movingRadius, 2);
  particles[2].setOnTree(true);
  setMap(particles[2].getPositionX(), particles[2].getPositionY(), 2);
  
  for (GLint x = 0; x < maxPixels; x++) {
    // init 函数包含 x 坐标、 y 坐标、移动半径、颜色值（标签）
    // 下面使得粒子初始化随机在窗口中间，占据整个窗口 1 / 4 面积
    particles[x].init(rand() % (windowWidth / initRadiusRatio) + (windowWidth / 2 - windowWidth / initRadiusRatio / 2),
                      rand() % (windowHeight / initRadiusRatio) + (windowHeight / 2 - windowHeight / initRadiusRatio / 2), movingRadius,
                      rand() % 3);  // 3 种颜色选用这个
    particles[x].threeColor = true;
    //    particles[x].movingProbability(0.5, 0.4,0.1,0,0,0,0,0);
  }
  
  return 0;
}

int initial_Circle() { // 圈
  init_Particles();
  circle = true;
  for (int i = 0; i < CircleRadius * 2 * 3; i++) {
    particles[i].init(CircleRadius * cos((double)i) + windowWidth / 2, CircleRadius * sin((double)i) + windowHeight / 2, movingRadius, 0);
    particles[i].setOnTree(true);
    setMap(particles[i].getPositionX(), particles[i].getPositionY(), 0);
  }
  
  return 0;
}
bool CircleOffRange(int i) {
  if ((particles[i].getPositionX() - windowWidth / 2) * (particles[i].getPositionX() - windowWidth / 2)
      + (particles[i].getPositionY() - windowHeight / 2) * (particles[i].getPositionY() - windowHeight / 2)
      > CircleRadius * CircleRadius) {
    return true;
  }
  
  return false;
}

int initialLight() { // 光照
  init_Particles();
  light = true;
  const int numberOfSeed = 20; // 种子数量
  for (int i = 0; i < numberOfSeed; i++) {
    particles[i].init(windowWidth / numberOfSeed * i + windowWidth / (numberOfSeed - 1) / 2, windowHeight / 3, 0, 0);
    particles[i].setOnTree(true);
    setMap(particles[i].getPositionX(), particles[0].getPositionY(), 0);
  }
  
  for (int i = numberOfSeed; i < maxPixels; i++) {
    particles[i].init(rand() % windowWidth,
                      rand() % (windowHeight / initRadiusRatio) + (windowHeight / 2 - windowHeight / initRadiusRatio / 2),
                      movingRadius, 0);
    particles[i].setOnTree(false);
    //    particles[i].movingProbability(0, 0, 0.1, 0.3, 0, 0.3, 0, 0.3);
  }
  
  return 0;
}
bool judgeLightStop(int lightRadius, int i, GLint lightX, GLint lightY, GLint sunX, GLint sunY) {
  if ((particles[i].getPositionX() - lightX) * (particles[i].getPositionX() - lightX)
      + (particles[i].getPositionY() - lightY) * (particles[i].getPositionY() - lightY)
      < lightRadius * lightRadius) {
    return true;
  }
  return false;
}

int initial_CenterLine() {
  init_Particles();
  // 中间一条线
  for (GLint i = 0 ; i < windowWidth / 2; i++) {
    particles[i].init(i + windowWidth / 4, windowHeight / 2, movingRadius, 0);
    particles[i].setOnTree(true);
    setMap(i + windowWidth / 4, windowHeight / 2, 0);
  }
  
  return 0;
}

int initial_BlackHole() {
  init_Particles();
  // 黑洞
  initial_One();
  
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
  
  //  for (GLint x = 0; x < maxPixels; x++) {
  //    particles[x].movingProbability(0.5, 0.5, 0, 0, 0, 0, 0, 0);
  //  }
  
  return 0;
}

void init() {
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(windowWidth * 1.5, windowHeight);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("DLA Model");
  
  glClearColor(1.0, 1.0, 1.0, 0.0);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, windowWidth * 1.5, 0.0, windowHeight);
  
  srand((unsigned)time(0));
  
  pos.rx = pos.ry = pos.tx = pos.ry = pos.rz = pos.tz = 0;
  pos.s = 1;
  pos.angle = 0;
  
  setType();
}

void DLA() {
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
        int random = rand() % 100;
        if (random < 40) {
          //continue;
          particles[i].setDisappear(true);
          continue;
        }
        if (circle) {
          // 下面的 if 用于屏蔽在圆外生成点，init() 中使用 initial_Circle(); 的时候取消注释
          if (CircleOffRange(i)) {
            continue;
          }
        } else if (light) {
          if ((particles[i].getPositionX() - sun.x) * (particles[i].getPositionX() - sun.x) +
              (particles[i].getPositionY() - sun.y) * (particles[i].getPositionY() - sun.y) <=
              lightDisappearRadius * lightDisappearRadius) {
            particles[i].setDisappear(true);
            continue;
          }
        } else if (isBlockLR) {
          random = rand() % 100;
          
          if (particles[i].getPositionY() > windowHeight / 2
              && (particlesMap[particles[i].getPositionX() - 1][particles[i].getPositionY() - 1].onTree
                  || particlesMap[particles[i].getPositionX()][particles[i].getPositionY() - 1].onTree
                  || particlesMap[particles[i].getPositionX() + 1][particles[i].getPositionY() - 1].onTree)) {
                if (random < 80) {
                  particles[i].setDisappear(true);
                  continue;
                }
              } else if (particles[i].getPositionY() < windowHeight / 2
                         && (particlesMap[particles[i].getPositionX() - 1][particles[i].getPositionY() + 1].onTree
                             || particlesMap[particles[i].getPositionX()][particles[i].getPositionY() + 1].onTree
                             || particlesMap[particles[i].getPositionX() + 1][particles[i].getPositionY() + 1].onTree
                             )) {
                           if (random < 80) {
                             particles[i].setDisappear(true);
                             continue;
                           }
                         }
        }
        
        particles[i].setOnTree(true);
        
        setMap(particles[i].getPositionX(), particles[i].getPositionY(), particles[i].getTag());
      }
    }
  }
}

void drawDLA() {     // 绘制函数
  glPushMatrix();
  
  glTranslatef(windowWidth / 2, windowHeight / 2, 0);
  
  glRotatef(pos.ry, 0, 0, 1);
  glRotatef(pos.rx, 0, 0, -1);
  
  glTranslatef(-windowWidth / 2, -windowHeight / 2, 0);
  
  glPushMatrix();
  glTranslatef(windowWidth / 2, windowHeight / 2, 0);
  glScalef(pos.s, pos.s, pos.s);
  glTranslatef(-windowWidth / 2, -windowHeight / 2, 0);
  
  for (GLint x = 0; x < maxPixels; x++) {
    if(!particles[x].isDisappear()) {
      if (isBlockEffect) {
        if (abs(particles[x].getPositionX() - particles[0].getPositionX()) * abs(particles[x].getPositionX() - particles[0].getPositionX())
            + abs(particles[x].getPositionY() - particles[0].getPositionY()) * abs(particles[x].getPositionY() - particles[0].getPositionY()) < 100 * 100
            ) {
          glPointSize(2.5);
        } else {
          glPointSize(1.0f);
        }
      }
      glBegin(GL_POINTS);
      {
        glColor3f(particles[x].getColorRed(), particles[x].getColorGreen(), particles[x].getColorBlue());
        glVertex2i(particles[x].getPositionX(), particles[x].getPositionY());
      }
      glEnd();
    }
  }
  
  glPopMatrix();
  glPopMatrix();
  
  glutPostRedisplay();
}

void drawSun() {
  if (light) {
    glPushMatrix();
    
    glTranslatef(windowWidth / 2, windowHeight / 2, 0);
    
    glRotatef(pos.ry, 0, 0, 1);
    glRotatef(pos.rx, 0, 0, -1);
    
    glTranslatef(-windowWidth / 2, -windowHeight / 2, 0);
    
    glPushMatrix();
    glTranslatef(windowWidth / 2, windowHeight / 2, 0);
    glScalef(pos.s, pos.s, pos.s);
    glTranslatef(-windowWidth / 2, -windowHeight / 2, 0);
    
    sun.size = 20;
    sun.n = 1000;
    
    if (!touched) {
      sun.x = windowWidth / 2;
      sun.y = windowHeight / 2;
      
      //    } else {
      //      sun.x = mousePos->x;
      //      sun.y = mousePos->y;
    }
    
    
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(255, 255, 0);
    for (int t = 0; t < 1000; t++) {
      glVertex2d(sun.size * cos(2 * PI / sun.n * t) + sun.x, sun.size * sin(2 * PI / sun.n * t) + sun.y);
    }
    glEnd();
    
    glPopMatrix();
    glPopMatrix();
  }
}

void update() {
  for (GLint i = 0; i < maxPixels; i++) {     // 每一帧update一次
    particles[i].update();
  }
}

int countPixels() {
  int count = 0;
  for (int i = 0; i < maxPixels; i++) {
    if (particles[i].isOnTree()) {
      count++;
    }
  }
  return count;
}

void displayTexts() {
  if (!particles[0].blackBG) {
    displayFPSText(0, 0, 0.0, 0.0, 0.0, fpsText);
    displayMaxPixelsText(windowWidth + 50, windowHeight - 50, 0.0, 0.0, 0.0, maxPixelsText);
    displayPaceText(windowWidth + 50, windowHeight - 100, 0.0, 0., 0., paceText);
    displayTypeText(windowWidth + 50, windowHeight - 150, 0., 0.0, 0., typeText);
    displayTOtalOnTreePixelsText(windowWidth + 50, windowHeight - 200, 0., 0., 0., totalOnTreePixels);
    if (pause) {
    displayDimensionText(windowWidth + 50, windowHeight - 250, 0., 0., 0., dimensionText);
    }
    if (light) {
      displaySunText(windowWidth + 50, windowHeight - 300, 0., 0., 0., sunText);
    }
    
  } else {
    displayFPSText(0, 0, 1.0, 1.0, 1.0, fpsText);
    displayMaxPixelsText(windowWidth + 50, windowHeight - 50, 1.0, 1.0, 1.0, maxPixelsText);
    displayPaceText(windowWidth + 50, windowHeight - 100, 1.0, 1., 1., paceText);
    displayTypeText(windowWidth + 50, windowHeight - 150, 1., 1.0, 1., typeText);
    displayTOtalOnTreePixelsText(windowWidth + 50, windowHeight - 200, 1., 1., 1., totalOnTreePixels);
    if (pause) {
    displayDimensionText(windowWidth + 50, windowHeight - 250, 1., 1., 1., dimensionText);
    }
    if (light) {
      displaySunText(windowWidth + 50, windowHeight - 300, 1., 1., 1., sunText);
    }
  }
}

int countPixelsInBox[10];

void calculateDimension(GLint maxX, GLint maxY, GLint particleX, GLint particleY) {
  sandboxWidth = abs(maxX - particleX), sandboxHeight = abs(maxY - particleY);
  
  sandboxWidth = sandboxWidth / 100 * 100 + (sandboxWidth - sandboxWidth / 100 * 100) / 10 * 10;
  sandboxHeight = sandboxHeight / 100 * 100 + (sandboxHeight - sandboxHeight / 100 * 100) / 10 * 10;
  
  for (int i = 0; i < 10; i++) {
    int count = 0;
    for (int x = 0; x < maxPixels; x++) {
      if (particles[x].isOnTree()
          && abs(particles[x].getPositionX() - particleX) < sandboxWidth * (i + 1) * 0.1
          && abs(particles[x].getPositionY() - particleY) < sandboxHeight * (i + 1) * 0.1) {
        count++;
      }
    }
    countPixelsInBox[i] = count;
  }
}

GLint maxParticlesX(int seedIndex) {
  GLint max = particles[seedIndex].getPositionX();
  GLint min = particles[seedIndex].getPositionX();
  
  for (int i = 0; i < maxPixels; i++) {
    if (particles[i].getTag() == particles[seedIndex].getTag() && particles[i].isOnTree()) {
      if (particles[i].getPositionX() > max) {
        max = particles[i].getPositionX();
      } else if (particles[i].getPositionX() < min) {
        min = particles[i].getPositionX();
      }
    }
  }
  
  return (max - particles[seedIndex].getPositionX()) > (particles[seedIndex].getPositionX() - min) ? max: min;
}

void calculateFractalDimension(int seedIndex) {
  calculateDimension(
                     maxParticlesX(seedIndex),
                     maxParticlesX(seedIndex),
                     particles[seedIndex].getPositionX(),
                     particles[seedIndex].getPositionY());
}

GLint sandboxWidthArr[10];

void displayRect(GLfloat red, GLfloat green, GLfloat blue) {
  for (int i = 0; i < 10; i++) {
    sandboxWidthArr[i] = sandboxWidth * (i + 1) * 0.1;
    glColor3f(red, green, blue);
    glBegin(GL_LINE_LOOP);
    {
      glVertex2i(particles[0].getPositionX() - sandboxWidth * (i + 1) * 0.1, particles[0].getPositionY() + sandboxHeight * (i + 1) * 0.1);
      glVertex2i(particles[0].getPositionX() + sandboxWidth * (i + 1) * 0.1, particles[0].getPositionY() + sandboxHeight * (i + 1) * 0.1);
      glVertex2i(particles[0].getPositionX() + sandboxWidth * (i + 1) * 0.1, particles[0].getPositionY() - sandboxHeight * (i + 1) * 0.1);
      glVertex2i(particles[0].getPositionX() - sandboxWidth * (i + 1) * 0.1, particles[0].getPositionY() - sandboxHeight * (i + 1) * 0.1);
    }
    glEnd();
  }
}

double a, b;

void LinearRegression() {
  
  double Lxx = 0.0, Lxy = 0.0, xa = 0.0, ya = 0.0;
  
  double yi[10], xi[10];
  for (int i = 0; i < 10; i++) {
    yi[i] = log10((double)countPixelsInBox[i]);
    xi[i] = log10((double)sandboxWidth * (i + 1) * 0.1);
    
    xa += xi[i];
    ya += yi[i];
  }

  xa /= 10;                                     // X平均值
  ya /= 10;                                     // Y平均值
  for (int i = 0; i < 10; i++) {
    Lxx += (xi[i] - xa) * (xi[i] - xa);             // Lxx = Sum((X - Xa)平方)
    Lxy += (xi[i] - xa) * (yi[i] - ya);       // Lxy = Sum((X - Xa)(Y - Ya))
  }
  
  b = Lxy / Lxx;                                 // b = Lxy / Lxx
  a = ya - b * xa;                              // a = Ya - b*Xa
  
  
  coefficient = b;
//  cout << "a:" << a << "   b:   " << b << endl;
}

void drawAxis(double);

void drawAxis2D() {
  glPushMatrix();
  
  glTranslatef(windowWidth , windowHeight - 600, 0);
  drawAxis(200);
  for (double x = 0; x < sandboxWidth; x += 10) {
    glColor3b(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    {
      glVertex2d(x, x * b + a);
    }
    glEnd();
  }
  glPopMatrix();
}

void Display() {
  glClear(GL_COLOR_BUFFER_BIT);
  
  drawSun();
  
  DLA();
  
  update();
  
  drawDLA();
  
  displayTexts();
  
  if (drawRect) {
    if (!particles[maxPixels - 1].blackBG) {
      displayRect(0., 0., 0.);
    } else {
      displayRect(1., 1., 1.);
    }
  }
  
//  if (pause) {
//  drawAxis2D();
//  }
  
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
}

void axis(double length) {
  glLineWidth(2.0);
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
  if (is3D) {
    glPushMatrix();
    glRotatef(pos.ry, 0, 1, 0);
    glRotatef(pos.rx, 0, 0, 1);
    glScalef(pos.s, pos.s, pos.s);
  }
  
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
  
  if (is3D) {
    glPopMatrix();
  }
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

int lastPace = 1;

void keyBoard(unsigned char key, int x, int y) {
  //DLA();
  if (key == 'z') {
    pos.s += 0.1;
  } else if (key == 'x') {
    pos.s -= 0.1;
  } else if (key == 'w') {
    sun.y += 10;
    //    cout << "Sun: x:" << sun.x << "  " << "y: " << sun.y << endl;
  } else if (key == 's') {
    sun.y -= 10;
    //    cout << "Sun: x:" << sun.x << "  " << "y: " << sun.y << endl;
  } else if (key == 'a') {
    sun.x -= 10;
    //    cout << "Sun: x:" << sun.x << "  " << "y: " << sun.y << endl;
  } else if (key == 'd') {
    sun.x += 10;
    
  } else if (key == 'q') {
    pos.rx += 5;
  } else if (key == 'e') {
    pos.rx -= 5;
  } else if (key == ' ') {
    if (pause) {
      pause = false;
      drawRect = false;
      for (int i = 0; i < maxPixels; i++) {
        particles[i].setOnePace(lastPace);
      }
    } else {
      pause = true;
      
      lastPace = particles[maxPixels - 1].getPace();
      for (int i = 0; i < maxPixels; i++) {
        particles[i].setOnePace(0);
      }
      //      cout << "Seed Count: "<< countPixels() << endl;
    }
  } else if (key == 'c') {
    if (drawRect) {
      drawRect = false;
    } else {
      drawRect = true;
      calculateFractalDimension(0);
      
      for (int i = 0; i < 10; i++) {
//        cout << countPixelsInBox[i] << "  r: " << sandboxWidth * (i + 1) * 0.1 << endl;
//        cout << log10((double)countPixelsInBox[i]) / log10((double)sandboxWidth * (i + 1) * 0.1) << endl;
      }
    }
//     LinearRegression();
  }
  
  touched = true;
  
  glutPostRedisplay();
}

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
    touched = true;
  }
}

void processMenuEvents(int option) {
  switch (option) {
    case clean:
      init_Particles();
      displayType("Initial.");
      break;
    case init_one:
      initial_One();
      if (isBlockEffect) {
        displayType("Shielding Effect.");
      } else {
        displayType("One Seed.");
      }
      break;
    case init_one_2:
      initial_One_2();
      displayType("One seed.");
      break;
    case init_three:
      initial_Three();
      displayType("Multiple seeds.");
      break;
    case init_light:
      initialLight();
      displayType("Phototaxis.");
      break;
    case init_circle:
      initial_Circle();
      displayType("Circle.");
      break;
    case init_centerLine:
      initial_CenterLine();
      displayType("Line.");
      break;
    case init_blackHole:
      initial_BlackHole();
      displayType("One seed.");
      break;
    case blockEffect:
      init_Particles();
      isBlockEffect = true;
      displayType("Shielding Effect");
      break;
    default:
      break;
  }
}

void processRedOnOff(int option) {
  switch (option) {
    case redOn:
      for (int i = 0; i < maxPixels; i++) {
        particles[i].displayRed(true);
      }
      break;
    case redOff:
      for (int i = 0; i < maxPixels; i++) {
        particles[i].displayRed(false);
      }
      break;
    default:
      break;
  }
}

void processParticlesNumberMenuEvents(int option) {
  switch (option) {
    case k_10:
      maxPixels = 10000;
      break;
    case k_20:
      maxPixels = 20000;
      break;
    case k_30:
      maxPixels = 30000;
      break;
    case k_40:
      maxPixels = 40000;
      break;
    case k_50:
      maxPixels = 50000;
      break;
    case k_60:
      maxPixels = 60000;
      break;
    case k_70:
      maxPixels = 70000;
      break;
    case k_80:
      maxPixels = 80000;
      break;
    case k_90:
      maxPixels = 90000;
      break;
    case k_add:
      maxPixels += 20000;
      break;
    case k_minus:
      maxPixels -= 20000;
      
    default:
      break;
  }
  //  cout << "Max Pixels: " <<maxPixels << endl;
  delete []particles;
  particles = new pixels[maxPixels];
  init_Particles();
  glutSwapBuffers();
  glutPostRedisplay();
}

void processPaceMenu(int option) {
  switch (option) {
    case p_0:
      for (int i = 0; i < maxPixels; i++) {
        particles[i].setOnePace(1);
      }
      break;
    case p_1:
      for (int i = 0; i < maxPixels; i++) {
        particles[i].setOnePace(2);
      }
      break;
    case p_2:
      for (int i = 0; i < maxPixels; i++) {
        particles[i].setOnePace(4);
      }
      break;
    case p_3:
      for (int i = 0; i < maxPixels; i++) {
        particles[i].setOnePace(8);
      }
      break;
    case p_4:
      for (int i = 0; i < maxPixels; i++) {
        particles[i].setOnePace(16);
      }
      break;
    case p_5:
      for (int i = 0; i < maxPixels; i++) {
        particles[i].setOnePace(32);
      }
      break;
    case p_6:
      for (int i = 0; i < maxPixels; i++) {
        particles[i].setOnePace(64);
      }
      break;
    case p_add:
      for (int i = 0; i < maxPixels; i++) {
        particles[i].setOnePace(particles[i].getPace() + 2);
      }
      break;
    case p_minus:
      for (int i = 0; i < maxPixels; i++) {
        particles[i].setOnePace(particles[i].getPace() - 2);
      }
      break;
      
    default:
      break;
  }
}

void processBackgroundMenu(int option) {
  switch (option) {
    case bg_black:
      for (int i = 0; i < maxPixels; i++) {
        particles[i].blackBG = true;
      }
      isBlack = true;
      glClearColor(0., 0., 0., 0.);
      glutPostRedisplay();
      break;
    case bg_white:
      for (int i = 0; i < maxPixels; i++) {
        particles[i].blackBG = false;
      }
      isBlack = false;
      glClearColor(1.0, 1.0, 1.0, 0.0);
      glutPostRedisplay();
      break;
      
    default:
      break;
  }
}

void menu() {
  int mainMenu, typeMenu, particlesNumMenu, redParticlesMenu, paceMenu, backgroundMenu;
  
  backgroundMenu = glutCreateMenu(processBackgroundMenu);
  glutAddMenuEntry("Black Background", bg_black);
  glutAddMenuEntry("White Background", bg_white);
  
  typeMenu = glutCreateMenu(processMenuEvents);
  glutAddMenuEntry("One Point", init_one);
  glutAddMenuEntry("One Point(Block)", init_one_2);
  glutAddMenuEntry("Three Points", init_three);
  glutAddMenuEntry("One Line", init_centerLine);
  glutAddMenuEntry("Circle", init_circle);
  glutAddMenuEntry("Light", init_light);
  glutAddMenuEntry("Black Hole", init_blackHole);
  
  particlesNumMenu = glutCreateMenu(processParticlesNumberMenuEvents);
  glutAddMenuEntry("10000", k_10);
  glutAddMenuEntry("20000", k_20);
  glutAddMenuEntry("30000", k_30);
  glutAddMenuEntry("40000", k_40);
  glutAddMenuEntry("50000", k_50);
  glutAddMenuEntry("60000", k_60);
  glutAddMenuEntry("70000", k_70);
  glutAddMenuEntry("80000", k_80);
  glutAddMenuEntry("90000", k_90);
  glutAddMenuEntry("Add 20000", k_add);
  glutAddMenuEntry("Minus 20000", k_minus);
  
  redParticlesMenu = glutCreateMenu(processRedOnOff);
  glutAddMenuEntry("Red On", redOn);
  glutAddMenuEntry("Red Off", redOff);
  
  paceMenu = glutCreateMenu(processPaceMenu);
  glutAddMenuEntry("1", p_0);
  glutAddMenuEntry("2", p_1);
  glutAddMenuEntry("4", p_2);
  glutAddMenuEntry("8", p_3);
  glutAddMenuEntry("16", p_4);
  glutAddMenuEntry("32", p_5);
  glutAddMenuEntry("64", p_6);
  glutAddMenuEntry("Add 2", p_add);
  glutAddMenuEntry("Minus 2", p_minus);
  
  mainMenu = glutCreateMenu(processMenuEvents);
  glutAddMenuEntry("Reset", clean);
  glutAddSubMenu("Background", backgroundMenu);
  glutAddMenuEntry("Shielding Effect", blockEffect);
  glutAddSubMenu("Type", typeMenu);
  glutAddSubMenu("Max Particles' Number", particlesNumMenu);
  glutAddSubMenu("Red Switch", redParticlesMenu);
  glutAddSubMenu("Set One Pace's Length", paceMenu);
  
  glutAttachMenu(GLUT_RIGHT_BUTTON);
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
  
  menu();
  
  glutMainLoop();
}
