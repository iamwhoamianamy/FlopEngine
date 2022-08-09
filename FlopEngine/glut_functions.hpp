#pragma once
#include "GL/freeglut.h"

void registerFunctions();
void glutOnTimer(int millisec);
void glutExitingFunction();
void glutDisplay();
void glutReshape(GLint w, GLint h);
void glutKeyboardLetters(unsigned char key, int x, int y);
void glutMouse(int button, int state, int x, int y);
void glutMousePassive(int x, int y);