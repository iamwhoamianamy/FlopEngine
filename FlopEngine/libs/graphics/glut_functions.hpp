#pragma once

#include "GL/freeglut.h"

namespace flp
{
    void registerFunctions();
    void glutOnTimer(int millisec);
    void glutExitingFunction();
    void glutDisplay();
    void glutReshape(int w, int h);
    void glutKeyboardLetters(unsigned char key, int x, int y);
    void glutMouse(int button, int state, int x, int y);
    void glutMousePassive(int x, int y);
    void glutIdle();
}