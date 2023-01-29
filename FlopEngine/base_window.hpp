#pragma once
#include <iostream>
#include <vector>
#include <chrono>

#include "vector2.hpp"

namespace flp
{
    class BaseWindow
    {
    protected:
        std::chrono::milliseconds drawing_interval{1000 / 60};
        std::chrono::milliseconds physics_interval{1000 / 60};

        float screenWidth;
        float screenHeight;
        std::string name;
        Vector2 mousePos;
    public:
        BaseWindow(
            int argc, char** argv,
            float screenWidth = 400, float screenHeight = 400,
            std::string name = "New Window");

        virtual void display() = 0;
        virtual void keyboardLetters(unsigned char key, int x, int y) = 0;
        virtual void mouse(int button, int state, int x, int y) = 0;
        virtual void mousePassive(int x, int y) = 0;
        virtual void exitingFunction() = 0;

        void run();

        friend void glutOnTimer(int millisec);
        friend void glutExitingFunction();
        friend void glutDisplay();
        friend void glutReshape(int w, int h);
        friend void glutKeyboardLetters(unsigned char key, int x, int y);
        friend void glutMouse(int button, int state, int x, int y);
        friend void glutMousePassive(int x, int y);
    private:
        void baseDisplay();
        void baseOnTimer(int millisec);
        void baseExitingFunction();
        void baseReshape(int w, int h);

        void baseKeyboardLetters(unsigned char key, int x, int y);
        void baseMouse(int button, int state, int x, int y);
        void baseMousePassive(int x, int y);
    };
}