#pragma once
#include <iostream>
#include <vector>

#include "GL/freeglut.h"

#include "vector3.hpp"
#include "octree.hpp"

namespace flp
{
    class BaseWindow
    {
    protected:
        int FPS = 60;
        float screenWidth = 400;
        float screenHeight = 400;
        std::string name = "New Window";
        Vector3 mousePos;
    public:
        BaseWindow(
            int argc, char** argv,
            float screenWidth, float screenHeight,
            std::string name);

        virtual void display() = 0;
        virtual void keyboardLetters(unsigned char key, int x, int y) = 0;
        virtual void mouse(int button, int state, int x, int y) = 0;
        virtual void mousePassive(int x, int y) = 0;
        virtual void exitingFunction() = 0;

        void run(int FPS);

        friend void glutOnTimer(int millisec);
        friend void glutExitingFunction();
        friend void glutDisplay();
        friend void glutReshape(GLint w, GLint h);
        friend void glutKeyboardLetters(unsigned char key, int x, int y);
        friend void glutMouse(int button, int state, int x, int y);
        friend void glutMousePassive(int x, int y);
    private:
        void baseDisplay();
        void baseOnTimer(int millisec);
        void baseExitingFunction();
        void baseReshape(GLint w, GLint h);

        void baseKeyboardLetters(unsigned char key, int x, int y);
        void baseMouse(int button, int state, int x, int y);
        void baseMousePassive(int x, int y);
    };
}