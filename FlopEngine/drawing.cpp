#include "drawing.hpp"
#include "math.hpp"

namespace draw
{
    Color::Color(UCHAR r, UCHAR g, UCHAR b, UCHAR a) :
        r(r), g(g), b(b), a(a)
    {
    }
    
    void draw_triangle(vector2 a, vector2 b, vector2 c, bool filled)
    {
        if(filled)
        {
            glBegin(GL_TRIANGLES);
        }
        else
        {
            glBegin(GL_LINE_LOOP);
        }

        {
            glVertex3f(a.x, a.y, 0);
            glVertex3f(b.x, b.y, 0);
            glVertex3f(c.x, c.y, 0);
        }
        glEnd();
    }

    Color generate_random_color()
    {
        return Color(
            math::random_in_range(0, 255),
            math::random_in_range(0, 255),
            math::random_in_range(0, 255));
    }

    constexpr float LETTER_SCALE = 0.01;

    void render_string(vector2 position, float size, const std::string& string)
    {
        glMatrixMode(GL_PROJECTION);
        {
            glPushMatrix();
            glTranslatef(position.x, position.y, 0);
            glScalef(LETTER_SCALE * size, -LETTER_SCALE * size, -LETTER_SCALE * size);

            for(char c : string)
            {
                glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
            }

            glPopMatrix();
        }
        glMatrixMode(GL_MODELVIEW);
    }
    
    void render_letter(vector2 position, float size, char letter)
    {
        glMatrixMode(GL_PROJECTION);
        {
            glPushMatrix();
            glTranslatef(position.x, position.y, 0);
            glScalef(LETTER_SCALE * size, -LETTER_SCALE * size, -LETTER_SCALE * size);
            glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, letter);
            glPopMatrix();
        }
        glMatrixMode(GL_MODELVIEW);
    }
}
