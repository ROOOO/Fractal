//
//  pixels.cpp
//  Graduate
//
//  Created by King on 15/3/25.
//  Copyright (c) 2015年 King. All rights reserved.
//

#include "pixels.h"

pixels::pixels() {
  std::srand((unsigned)std::time(0));
  
  m_direction.m_left = 1.0/8;
  m_direction.m_right = 1.0/8;
  m_direction.m_up = 1.0/8;
  m_direction.m_down = 1.0/8;
  m_direction.m_topLeft = 1.0/8;
  m_direction.m_topRight = 1.0/8;
  m_direction.m_bottomLeft = 1.0/8;
  m_direction.m_bottomRight = 1.0/8;
}

pixels::~pixels() { }

void pixels::init(GLint x, GLint y, GLint radius, int tag) {
  m_onTree = false;
  m_disappear = false;
  m_position.oX = x;
  m_position.oY = y;
  m_position.x = x;
  m_position.y = y;
  
  m_setRadius(radius);
  movingProbability();
  
  m_tag = tag;
}
void pixels::init3D(GLint x, GLint y, GLint z, GLint radius, int tag) {
  m_onTree = false;
  m_disappear = false;
  m_position.oX = x;
  m_position.oY = y;
  m_position.oZ = z;
  m_position.x = x;
  m_position.y = y;
  m_position.z = z;
  
  m_setRadius(radius);
  movingProbability();
  
  m_tag = tag;
}

void pixels::m_setColor(float red, float green, float blue) {
  m_color.m_red = red;
  m_color.m_green = green;
  m_color.m_blue = blue;
}

void pixels::movingProbability() {
  m_directionProbability.m_upProbability = m_direction.m_up * 1000;
  m_directionProbability.m_downProbability = m_direction.m_down * 1000 + m_directionProbability.m_upProbability;
  m_directionProbability.m_leftProbability = m_direction.m_left * 1000 + m_directionProbability.m_downProbability;
  m_directionProbability.m_rightProbability = m_direction.m_right * 1000 + m_directionProbability.m_leftProbability;
  m_directionProbability.m_topLeftProbability = m_direction.m_topLeft * 1000 + m_directionProbability.m_rightProbability;
  m_directionProbability.m_topRightProbability = m_direction.m_topRight * 1000 + m_directionProbability.m_topLeftProbability;
  m_directionProbability.m_bottomLeftProbability = m_direction.m_bottomLeft * 1000 + m_directionProbability.m_topRightProbability;
  m_directionProbability.m_bottomRightProbability = m_direction.m_bottomRight * 1000 + m_directionProbability.m_bottomRightProbability;
}
void pixels::movingProbability(float up, float down, float left, float right,
                               float upperLeft, float upperRight, float bottomLeft, float bottomRight) {
  if (up + down + left + right + upperLeft + upperRight + bottomLeft + bottomRight == 1.0) {
    m_direction.m_left = left;
    m_direction.m_right = right;
    m_direction.m_up = up;
    m_direction.m_down = down;
    m_direction.m_topLeft = upperLeft;
    m_direction.m_topRight = upperRight;
    m_direction.m_bottomLeft = bottomLeft;
    m_direction.m_bottomRight = bottomRight;
  }
  
  m_directionProbability.m_upProbability = m_direction.m_up * 1000;
  m_directionProbability.m_downProbability = m_direction.m_down * 1000 + m_directionProbability.m_upProbability;
  m_directionProbability.m_leftProbability = m_direction.m_left * 1000 + m_directionProbability.m_downProbability;
  m_directionProbability.m_rightProbability = m_direction.m_right * 1000 + m_directionProbability.m_leftProbability;
  m_directionProbability.m_topLeftProbability = m_direction.m_topLeft * 1000 + m_directionProbability.m_rightProbability;
  m_directionProbability.m_topRightProbability = m_direction.m_topRight * 1000 + m_directionProbability.m_topLeftProbability;
  m_directionProbability.m_bottomLeftProbability = m_direction.m_bottomLeft * 1000 + m_directionProbability.m_topRightProbability;
  m_directionProbability.m_bottomRightProbability = m_direction.m_bottomRight * 1000 + m_directionProbability.m_bottomRightProbability;
  
  //    cout << "up: " << m_directionProbability.m_upProbability << "\n"
  //    << "down: " << m_directionProbability.m_downProbability << "\n"
  //    << "left: " << m_directionProbability.m_leftProbability << "\n"
  //    << "right: " << m_directionProbability.m_rightProbability << "\n"
  //    << "tl: " << m_directionProbability.m_topLeftProbability << "\n"
  //    << "tr: " << m_directionProbability.m_topRightProbability << "\n"
  //    << "bl: " << m_directionProbability.m_bottomLeftProbability << "\n"
  //    << "br: " << m_directionProbability.m_bottomRightProbability << endl;
}

void pixels::m_setRadius(GLint radius) {
  m_radius = radius;
}

bool pixels::judgeOutOfRange(GLint windowWidth, GLint windowHeight) {
  if (isDisappear()) {
    return true;
  }
  
  if (m_position.x < 0 || m_position.x > windowWidth ||
      m_position.y < 0 || m_position.y > windowHeight ||
      ((m_position.x - m_position.oX) * (m_position.x - m_position.oX) +
       (m_position.y - m_position.oY) * (m_position.y - m_position.oY)) > m_radius * m_radius) {
        setDisappear(true);
        return true;
      } else {
        return false;
      }
}
bool pixels::judgeOutOfRange(GLint maxX, GLint maxY, GLint maxZ) {
  if (isDisappear()) {
    return true;
  }
  
  if (m_position.x < 0 || m_position.x > max3DX ||
      m_position.y < 0 || m_position.y > max3DY ||
      m_position.z < 0 || m_position.z > max3DZ ||
      (m_position.x - m_position.oX) * (m_position.x - m_position.oX) * (m_position.x - m_position.oX) +
      (m_position.y - m_position.oY) * (m_position.y - m_position.oY) * (m_position.y - m_position.oY) +
      (m_position.z - m_position.oZ) * (m_position.z - m_position.oZ) * (m_position.z - m_position.oZ) >
      m_radius * m_radius * m_radius) {
    setDisappear(true);
    return true;
  }
  return false;
}

void pixels::setOnTree(bool onTree) {
  m_onTree = onTree;
}
bool pixels::isOnTree() {
  return m_onTree;
}

void pixels::setDisappear(bool disappear) {
  m_disappear = disappear;
}
bool pixels::isDisappear() {
  return m_disappear;
}

void pixels::moveOneStep() {
  int step = 1;
  int random = rand() % 1000;
  
  if (random <= m_directionProbability.m_upProbability) {
    m_position.y += step;
  } else if (random <= m_directionProbability.m_downProbability) {
    m_position.y -= step;
  } else if (random <= m_directionProbability.m_leftProbability) {
    m_position.x -= step;
  } else if (random <= m_directionProbability.m_rightProbability) {
    m_position.x += step;
  } else if (random <= m_directionProbability.m_topLeftProbability) {
    m_position.x -= step;
    m_position.y += step;
  } else if (random <= m_directionProbability.m_topRightProbability) {
    m_position.x += step;
    m_position.y += step;
  } else if (random <= m_directionProbability.m_bottomLeftProbability) {
    m_position.x -= step;
    m_position.y -= step;
  } else {
    m_position.x += step;
    m_position.y -= step;
  }
}

void pixels::moveOneStep3D() {
  m_position.x += rand() % 2 - 1;
  m_position.y += rand() % 2 - 1;
  m_position.z += rand() % 2 - 1;
}

GLint pixels::getPositionX() {
  return m_position.x;
}
GLint pixels::getPositionY() {
  return m_position.y;
}
GLint pixels::getPositionZ() {
  return m_position.z;
}

void pixels::gravity(float gv, int type) { }

GLfloat pixels::getColorRed() {
  return m_color.m_red;
}
GLfloat pixels::getColorGreen() {
  return m_color.m_green;
}
GLfloat pixels::getColorBlue() {
  return m_color.m_blue;
}

int pixels::getTag() {
  return m_tag;
}

void pixels::update() {
  if (isOnTree()) {
    if (m_tag == 0) {
      m_setColor(1.0, 1.0, 0.0);
    } else if (m_tag == 1) {
      m_setColor(0.0, 1.0, 1.0);
    } else {
      m_setColor(1.0, 0.0, 1.0);
    }
  } else if (m_disappear) {
    //    m_setColor(0.0, 0.0, 0.0);
  } else {
    if (!is3D) {
      if (m_tag == 0) {
        m_setColor(1.0, 1.0, 0.0);
      } else if (m_tag == 1) {
        m_setColor(0.0, 1.0, 1.0);
      } else {
        m_setColor(1.0, 0.0, 1.0);
      }
    }
    m_setColor(1.0, 0.0, 0.0);
  }
}
