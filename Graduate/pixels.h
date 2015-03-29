//
//  pixels.h
//  Graduate
//
//  Created by King on 15/3/25.
//  Copyright (c) 2015年 King. All rights reserved.
//

#ifndef __Graduate__pixels__
#define __Graduate__pixels__

#include "macro.h"

class pixels
{
public:
    pixels();
    ~pixels();
    
    void init(GLint x, GLint y, GLint radius, int tag); //All the pixels need to be inited at first.
    
    void movingProbability();
    void movingProbability(float up, float down, float left, float right, float topLeft, float topRight, float bottomLeft, float bottomRight);
    
    void moveOneStep();
    
    bool judgeOutOfRange(GLint windowWidth, GLint windowHeight);

    
    GLint getPositionX();
    GLint getPositionY();
    
    void setOnTree(bool onTree);
    bool isOnTree();
    
    void setDisappear(bool disappear);
    bool isDisappear();
    
    void gravity(float gv, int type);
    
    GLfloat getColorRed();
    GLfloat getColorGreen();
    GLfloat getColorBlue();
    
    int getTag();
    
    void update();

private:
    void m_setRadius(GLint radius);
    
    void m_setColor(GLfloat red, GLfloat green, GLfloat blue);
    
    float m_radius;
    
    bool m_onTree;
    bool m_disappear;
    
    struct m_color
    {
        GLfloat m_red;
        GLfloat m_green;
        GLfloat m_blue;
    }m_color;
    
    int m_tag;
    
    struct m_direction
    {
        float m_up;
        float m_down;
        float m_left;
        float m_right;
        float m_topLeft;
        float m_topRight;
        float m_bottomLeft;
        float m_bottomRight;
    }m_direction;
    
    struct m_directionProbability
    {
        int m_upProbability;
        int m_downProbability;
        int m_leftProbability;
        int m_rightProbability;
        int m_topLeftProbability;
        int m_topRightProbability;
        int m_bottomLeftProbability;
        int m_bottomRightProbability;
    }m_directionProbability;
    
    struct position
    {
        GLint oX;
        GLint oY;
        GLint x;
        GLint y;
    }m_position;
};

#endif /* defined(__Graduate__pixels__) */
