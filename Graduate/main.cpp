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
    particles[0].init(windowWidth / 2, windowHeight / 3 * 2, movingRadius, 0);
    particles[0].setOnTree(true);
    setMap(particles[0].getPositionX(), particles[0].getPositionY(), 0);
    
    
    particles[1].init(windowWidth / 3, windowHeight / 3, movingRadius, 1);
    particles[1].setOnTree(true);
    setMap(particles[1].getPositionX(), particles[1].getPositionY(), 1);
    
    particles[2].init(windowWidth / 3 * 2, windowHeight / 3, movingRadius, 2);
    particles[2].setOnTree(true);
    setMap(particles[2].getPositionX(), particles[2].getPositionY(), 2);
    
//    cout << "red :"  << particles[0].getColorRed() << "     " << endl;
    
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

void keyBoard(unsigned char key, int x, int y)
{
    DLA();
    glutPostRedisplay();
}

int main(int argc, char ** argv)
{
    glutInit(&argc, argv);
    
    init();
    
    glutDisplayFunc(Display);
    glutKeyboardFunc(keyBoard);
    
    glutMainLoop();
}
