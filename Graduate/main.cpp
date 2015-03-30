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

#if (0) // 2D
struct particlesMap
{
    bool onTree;
    int tag;
}particlesMap[windowWidth][windowHeight];

//bool (*particlesMap)[windowHeight] = new bool[windowWidth][windowHeight];
pixels (*particles) = new pixels[maxPixels];

//void mySetOnTree(int x,int y)
//{
//    assert(x>=0&&x<windowWidth);
//    assert(y>0&&y<windowHeight);
//    particlesMap[x][y]=true;
//}

void setMap(GLint x, GLint y, int tag)
{
    particlesMap[x][y].onTree = true;
    particlesMap[x][y].tag = tag;
}

bool isNearTree(int x,int y, int tag)
{
    int xmin = x - 1 > -1 ? x - 1 : 0, xmax = x + 1 < windowWidth ? x + 1 : windowWidth - 1;
    int ymin = y - 1 > -1 ? y - 1 : 0, ymax = y + 1 < windowHeight ? y + 1 : windowHeight - 1;
    for (int i = xmin; i <= xmax; i++)
        for (int j = ymin; j <= ymax; j++)
        {
            if (!(i == x && j == y) &&
                particlesMap[i][j].tag == tag &&
                particlesMap[i][j].onTree)
            {
                return true;
            }
        }
    return false;
}

void init()
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(600, 100);
    glutCreateWindow("DLA Model");
    
    glClearColor(0.0, 0.0, 0.0, 0.0);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, windowWidth, 0.0, windowHeight);
    
    srand((unsigned)time(0));
    
    for (GLint x = 0; x < windowWidth; x++) {
        for (GLint y = 0; y < windowHeight; y++) {
            particlesMap[x][y].onTree = false;
        }
    }
    
    
    // common
    for (GLint x = 0; x < maxPixels; x++)
    {
        particles[x].init(rand() % (windowWidth / initRadiusRatio) + (windowWidth / 2 - windowWidth / initRadiusRatio / 2), rand() % (windowHeight / initRadiusRatio) + (windowHeight / 2 - windowHeight / initRadiusRatio / 2), movingRadius,
                          rand() % 3);
        //0);
    }
    
#if(0)
    // line
    for (GLint i = 0 ; i < windowWidth / 2; i++)
    {
        particles[i].init(i + windowWidth / 4, windowHeight / 2, movingRadius, 0);
        particles[i].setOnTree(true);
        setMap(i + windowWidth / 4, windowHeight / 2, 0);
    }
#endif
    
#if(0)
    // black hole
    for (GLint x = 0; x < maxPixels / 5 ; x++) // 中心圆
    {
        particles[x].init(2 * (rand() % initRadius) * cos(x) + windowWidth / 2, 2 * (rand() % initRadius) * sin(x) + windowHeight / 2, movingRadius * 2, 0);
    }
    
    for (GLint x = maxPixels / 5; x < maxPixels * 4 / 5; x++) // 中心圆
    {
        particles[x].init((initRadius) * cos(x) + windowWidth / 2, (initRadius) * sin(x) + windowHeight / 2, movingRadius * 2, 0);
    }
    
    for (GLint x = 0; x < maxPixels; x++) {
        particles[x].movingProbability(0.5, 0.5, 0, 0, 0, 0, 0, 0);
    }
#endif
}

void DLA()
{
    particles[0].init(windowWidth / 2, windowHeight / 5 * 3, movingRadius, 0);
    particles[0].setOnTree(true);
    setMap(particles[0].getPositionX(), particles[0].getPositionY(), 0);
    
    
    particles[1].init(windowWidth / 5 * 2, windowHeight / 5 * 2, movingRadius, 1);
    particles[1].setOnTree(true);
    setMap(particles[1].getPositionX(), particles[1].getPositionY(), 1);
    
    particles[2].init(windowWidth / 5 * 3, windowHeight / 5 * 2, movingRadius, 2);
    particles[2].setOnTree(true);
    setMap(particles[2].getPositionX(), particles[2].getPositionY(), 2);
    
    
    for (GLint x = 0; x < maxPixels; x++)
    {
        if (!particles[x].isOnTree() && !particles[x].judgeOutOfRange(windowWidth, windowHeight))
        {
            particles[x].moveOneStep();
        }
    }
    
    for (int i=0;i<maxPixels;i++)
    {
        if(!particles[i].isOnTree() && !particles[i].judgeOutOfRange(windowWidth, windowHeight))
        {
            if(isNearTree(particles[i].getPositionX(), particles[i].getPositionY(), particles[i].getTag()))
            {
                particles[i].setOnTree(true);
                setMap(particles[i].getPositionX(), particles[i].getPositionY(), particles[i].getTag());
            }
        }
    }
}

void drawDLA()
{
    for (GLint x = 0; x < maxPixels; x++)
    {
        glBegin(GL_POINTS);
        {
            glColor3f(particles[x].getColorRed(), particles[x].getColorGreen(), particles[x].getColorBlue());
            glVertex2i(particles[x].getPositionX(), particles[x].getPositionY());
        }
        glEnd();
    }
}

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    DLA();
    
    for (GLint i = 0; i < maxPixels; i++) {
        particles[i].update();
    }
    
    drawDLA();
    
    glutSwapBuffers();
    glutPostRedisplay();
}
#endif

#if (1)
struct particlesMap3D
{
    bool onTree;
    int tag;
}particlesMap3D[max3DX][max3DY][max3DZ];

pixels *particles3D = new pixels[maxPixels];

void setMap3D(GLint x, GLint y, GLint z, int tag)
{
    particlesMap3D[x][y][z].onTree = true;
    particlesMap3D[x][y][z].tag = tag;
}

bool isNearTree3D(GLint x, GLint y, GLint z, GLint tag)
{
    int xmin = x - 1 > -1 ? x - 1 : 0, xmax = x + 1 < max3DX ? x + 1 : max3DX - 1;
    int ymin = y - 1 > -1 ? y - 1 : 0, ymax = y + 1 < max3DY ? y + 1 : max3DY - 1;
    int zmin = z - 1 > -1 ? z - 1 : 0, zmax = z + 1 < max3DZ ? z + 1 : max3DZ - 1;
    
    for (int i = xmin; i <= xmax; i++)
    {
        for (int j = ymin; j <= ymax; j++)
        {
            for (int k = zmin; k <= zmax; k++)
            {
                if (!(i == x && j == y && k == z) &&
                    particlesMap3D[i][j][k].tag == tag &&
                    particlesMap3D[i][j][k].onTree)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

struct position
{
    float tx, ty, tz;
    float rx, ry, rz;
    float s;
    float angle;
}pos;
void init3D()
{
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
    
    for (int i = 0; i < max3DX; i++) {
        for (int j = 0; j < max3DY; j++) {
            for (int k = 0; k < max3DZ; k++) {
                particlesMap3D[i][j][k].onTree = false;
//                particlesMap3D[i][j][k].tag = 0;
            }
        }
    }
    
    for (int i = 0; i <= maxPixels; i++) {
        particles3D[i].init(rand() % max3DX, rand() % max3DY, rand() % max3DZ, movingRadius, 0);
    }
    
    pos.rx = pos.ry = pos.tx = pos.ry = pos.rz = pos.tz = 0;
    pos.s = 1;
    pos.angle = 0;
}

void axis(double length)
{
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

void Grid(int gridLength)
{
    glBegin(GL_LINES);
    {
        glColor3f(0.5, 0.5, 0.5);
        glVertex3d(-gridLength, 0, 0);
        glVertex3d(gridLength, 0, 0);
    }
    glEnd();
}
void drawGrid(int gridLength)
{
    for (int i = -gridLength; i <= gridLength; i += 5)
    {
        glLineWidth(1.0);
        glPushMatrix();
        glRotatef(pos.ry, 0, 1, 0);
        glRotatef(pos.rx, 0, 0, 1);
        
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

void DLA3D()
{
    particles3D[0].init(max3DX / 2, max3DY / 2, max3DZ / 2, movingRadius, 0);
    particles3D[0].setOnTree(true);
    setMap3D(particles3D[0].getPositionX(), particles3D[0].getPositionY(), particles3D[0].getPositionZ(), 0);
    
    for (GLint x = 0; x < maxPixels; x++)
    {
        if (!particles3D[x].judgeOutOfRange(max3DX, max3DY, max3DZ) && !particles3D[x].isOnTree())
        {
            particles3D[x].moveOneStep();
        }
    }
    
    for (int i = 0; i < maxPixels; i++)
    {
        if (!particles3D[i].judgeOutOfRange(max3DX, max3DY, max3DZ) && !particles3D[i].isOnTree())
        {
            if(isNearTree3D(particles3D[i].getPositionX(), particles3D[i].getPositionY(), particles3D[i].getPositionZ(), particles3D[i].getTag()))
            {
                particles3D[i].setOnTree(true);
                setMap3D(particles3D[i].getPositionX(), particles3D[i].getPositionY(), particles3D[i].getPositionZ(), particles3D[i].getTag());
            }
        }
    }
}

void drawDLA3D()
{
    glPushMatrix();
    glRotatef(pos.ry, 0, 1, 0);
    glRotatef(pos.rx, 0, 0, 1);
    
    for (GLint x = 0; x < maxPixels; x++)
    {
        glBegin(GL_POINTS);
        {
            glColor3f(particles3D[x].getColorRed(), particles3D[x].getColorGreen(), particles3D[x].getColorBlue());
            glVertex3i(particles3D[x].getPositionX(), particles3D[x].getPositionY(), particles3D[x].getPositionZ());
        }
        glEnd();
    }
    glPopMatrix();
}

void display3D()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
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

#endif

void keyBoard(unsigned char key, int x, int y)
{
    //DLA();
    glutPostRedisplay();
}

typedef struct
{
    GLint x, y;
}GLintPoint;
GLintPoint mousePos[2];

void mouseMotion(int x, int y)
{
    mousePos[1].x = x;
    mousePos[1].y = windowHeight - y;
    pos.ry += (mousePos[1].x - mousePos[0].x);
    pos.rx += (mousePos[1].y - mousePos[0].y);
    mousePos[0] = mousePos[1];
}

void mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
    {
        mousePos[0].x = x;
        mousePos[0].y = windowHeight - y;
    }
}

int main(int argc, char ** argv)
{
    glutInit(&argc, argv);
    
    //    init();
    //    glutDisplayFunc(Display);
    
    init3D();
    glutDisplayFunc(display3D);
    
    glutKeyboardFunc(keyBoard);
    
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    
    glutMainLoop();
}
