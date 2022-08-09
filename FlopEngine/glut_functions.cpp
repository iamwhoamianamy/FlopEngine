#pragma once
#include "glut_functions.hpp"
#include "flop_engine.hpp"

BaseWindow* window;

void registerFunctions()
{
   glutDisplayFunc(glutDisplay);
   glutReshapeFunc(glutReshape);
   glutKeyboardFunc(glutKeyboardLetters);
   glutMouseFunc(glutMouse);
   glutPassiveMotionFunc(glutMousePassive);
   atexit(glutExitingFunction);
   glutTimerFunc(0, glutOnTimer, 0);
}

void glutOnTimer(int millisec)
{
   if(window)
      window->baseOnTimer(millisec);
}

void glutExitingFunction()
{
   if(window)
      window->baseExitingFunction();
}

void glutDisplay()
{
   if(window)
      window->baseDisplay();
}

void glutReshape(GLint w, GLint h)
{
   if(window)
      window->baseReshape(w, h);
}

void glutKeyboardLetters(unsigned char key, int x, int y)
{
   if(window)
      window->baseKeyboardLetters(key, x, y);
}

void glutMouse(int button, int state, int x, int y)
{
   if(window)
      window->baseMouse(button, state, x, y);
}

void glutMousePassive(int x, int y)
{
   if(window)
      window->baseMousePassive(x, y);
}
