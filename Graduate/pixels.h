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
    void init3D(GLint x, GLint y, GLint z, GLint radius, int tag);
    
    void movingProbability();       // 移动概率（一个默认，如果输入参数则需要概率总和为1，参数分别是 上、下、左、右   左上、右上、左下、右下）
    void movingProbability(float up, float down, float left, float right, float topLeft, float topRight, float bottomLeft, float bottomRight);
    
    void moveOneStep(); //移动一步
    void moveOneStep3D();
    
    bool judgeOutOfRange(GLint windowWidth, GLint windowHeight);        // 判断是否逃逸
    bool judgeOutOfRange(GLint maxX, GLint maxY, GLint maxZ);
    
    // 获取当前坐标，分别是 x 、 y 和 z
    GLint getPositionX();
    GLint getPositionY();
    GLint getPositionZ();
    
    void setOnTree(bool onTree);   // 设置在树上
    bool isOnTree();        //  判断是否在树上，下面有个私有成员变量 m_onTree 用来保存是否在树上
    
    void setDisappear(bool disappear);  // 设置消失
    bool isDisappear();     // 判断是否消失，下面有个私有成员变量 m_disppear
    
    void gravity(float gv, int type);   // 本来想做重力系统的，暂且放弃
    
    // 获取粒子颜色值
    GLfloat getColorRed();
    GLfloat getColorGreen();
    GLfloat getColorBlue();
    
    // 获取标签
    int getTag();
    
    // 每一帧都调用一次这个函数，更改粒子颜色
    void update();
    
private:
    void m_setRadius(GLint radius);     // 设置半径
    
    void m_setColor(GLfloat red, GLfloat green, GLfloat blue);  // 设置颜色
    
    float m_radius;  // 半径
    
    bool m_onTree;  // 是否在树上
    bool m_disappear; // 是否消失
    
    struct m_color //颜色结构体
    {
        GLfloat m_red;
        GLfloat m_green;
        GLfloat m_blue;
    }m_color;
    
    int m_tag; // 标签
    
    struct m_direction  // 方向
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
    
    struct m_directionProbability  // 方向概率
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
    
    //    struct m_direction3D      //26 个方向，暂且放弃
    //    {
    //        float m_;
    //    }m_direction3D;
    //    struct m_directionProbability3D
    //    {
    //
    //    }m_directionProbability3D;
    
    struct position // 纪录当前位置
    {
        GLint oX;
        GLint oY;
        GLint oZ;
        GLint x;
        GLint y;
        GLint z;
    }m_position;
};

#endif /* defined(__Graduate__pixels__) */
