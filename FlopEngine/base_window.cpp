#include "base_window.hpp"
#include "glut_functions.hpp"

using namespace flp;

BaseWindow::BaseWindow(
    int argc, char** argv,
    float screenWidth, float screenHeight,
    std::string name) :
    screenWidth(screenWidth), screenHeight(screenHeight), name(name)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow(name.c_str());

    registerFunctions();
}

void BaseWindow::run(int FPS)
{
    this->FPS = FPS;
    glutMainLoop();
}

void BaseWindow::baseOnTimer(int millisec)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, glutOnTimer, 0);
}

void BaseWindow::baseExitingFunction()
{
    exitingFunction();
}

void BaseWindow::baseReshape(GLint w, GLint h)
{
    screenWidth = w;
    screenHeight = h;

    glViewport(0, 0, screenWidth, screenHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, screenHeight, 0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void BaseWindow::baseKeyboardLetters(unsigned char key, int x, int y)
{
    keyboardLetters(key, x, y);
}

void BaseWindow::baseMouse(int button, int state, int x, int y)
{
    mouse(button, state, x, y);
}

void BaseWindow::baseMousePassive(int x, int y)
{
    mousePassive(x, y);
}

void BaseWindow::baseDisplay()
{
    display();
}

void baseDisplay()
{
}
