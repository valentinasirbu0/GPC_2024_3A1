#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <utility>
#include "glut.h"
#include <cmath>
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CartesianGrid {
private:
    int rows;
    int columns;
    float gridSize;
    vector<pair<int, int>> pixels;

public:
    CartesianGrid(int r, int c) : rows(r), columns(c), gridSize(1.0f) {}

    void addPixel(int x, int y) {
        pixels.push_back(make_pair(x, y));
    }

    void drawGrid() {
        glPushMatrix();
        glColor3f(0.0f, 0.0f, 0.0f); 
        glLineWidth(1.0f);
        glBegin(GL_LINES);
        for (int i = 0; i <= columns; ++i) {
            glVertex2f(i * gridSize / columns - gridSize / 2.0f, -gridSize / 2.0f);
            glVertex2f(i * gridSize / columns - gridSize / 2.0f, gridSize / 2.0f);
        }
        for (int j = 0; j <= rows; ++j) {
            glVertex2f(-gridSize / 2.0f, j * gridSize / rows - gridSize / 2.0f);
            glVertex2f(gridSize / 2.0f, j * gridSize / rows - gridSize / 2.0f);
        }
        glEnd();


        float squareSize = 0.035f;
        glColor3f(0.0f, 0.0f, 0.0f); 
        glRectf(-gridSize / 2.0f - 0.3f * squareSize, -gridSize / 2.0f - 0.3f * squareSize, -gridSize / 2.0f + 0.5f * squareSize, -gridSize / 2.0f + 0.5f * squareSize);
        glPopMatrix();

    }

    void InitPixelSet1(int x, int y, int z, int w) {
        pixels.push_back(make_pair(x, y - 1));
    }

    void deletePixel(int x, int y) {
        for (auto it = pixels.begin(); it != pixels.end(); ) {
            if (it->first == x && it->second == y) {
                it = pixels.erase(it); 
            }
            else {
                ++it; 
            }
        }
    }

    void InitPixelSet2(int x, int y, int z , int w) {
        pixels.push_back(make_pair(13, 6));
        deletePixel(10, 6);
    }

    void drawPixels() {
        glPushMatrix();
        glColor3f(0.2f, 0.2f, 0.2f); 
        for (const auto& pixel : pixels) {
            if (pixel.first >= 0 && pixel.first <= columns && pixel.second >= 0 && pixel.second <= rows) {
                glPushMatrix();
                glTranslatef(pixel.first * gridSize / columns - gridSize / 2.0f,
                    pixel.second * gridSize / rows - gridSize / 2.0f,
                    0.0f);
                glutSolidSphere(0.02f, 20, 20); 
                glPopMatrix();
            }
        }
        glPopMatrix();
    }

    void drawLine(int x1, int y1, int x2, int y2) {
        glPushMatrix();
        glColor3f(1.0f, 0.0f, 0.0f); 
        glLineWidth(3.0f); 
        glBegin(GL_LINES);
        glVertex2f(x1 * gridSize / columns - gridSize / 2.0f, y1 * gridSize / rows - gridSize / 2.0f);
        glVertex2f(x2 * gridSize / columns - gridSize / 2.0f, y2 * gridSize / rows - gridSize / 2.0f);
        glEnd();
        glPopMatrix();
    }

    void drawCircle(float ox, float oy, float radius) {
        const int numSegments = 100;
        glColor3f(1.0f, 0.0f, 0.0f);
        glLineWidth(3.0f);
        glBegin(GL_LINE_STRIP);

        float startAngle = 0.0f; 
        float endAngle = 0.5f * 3.14f; 

        for (int i = 0; i < numSegments; i++) {
            float theta = startAngle + (endAngle - startAngle) * float(i) / float(numSegments); 
            float x = radius * cosf(theta); 
            float y = radius * sinf(theta); 

            if (x + ox >= -gridSize / 2.0f && x + ox <= gridSize / 2.0f &&
                y + oy >= -gridSize / 2.0f && y + oy <= gridSize / 2.0f) {
                glVertex2f(x + ox, y + oy); 
            }
        }
        glEnd();
    }

    void ScanConvertCircle4(int ox, int oy, int r, vector<pair<int, int>>& M) {
        drawCircle(ox * gridSize / columns - gridSize / 2.0f, oy * gridSize / rows - gridSize / 2.0f, r * gridSize / columns); 

        int x = 0;
        int y = r;

        int d = 3 - 2 * r;
        int dE = 3;
        int dSE = -2 * r + 5;

        while (y > x) {
            if (d < 0) {
                d += dE;
                dE += 2;
                dSE += 2;
            }
            else {
                d += dSE;
                dE += 2;
                dSE += 4;
                y--;
            }

            x++;
            M.push_back(make_pair(ox + y, oy + x - 1));
            M.push_back(make_pair(ox + y + 1, oy + x - 1));
            M.push_back(make_pair(ox + y - 1, oy + x - 1));
        }
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ScanConvertSegments3(int x0, int y0, int xn, int yn, vector<pair<int, int>>& M, float toleranceX = 0, float toleranceY = 2) {
    if (x0 == xn && y0 == yn) {
        M.push_back(make_pair(x0, y0));
        return;
    }

    int dx = xn - x0;
    int dy = yn - y0;
    int maxDelta = max(abs(dx), abs(dy));
    float toleranceX_scaled = toleranceX * abs(dx) / maxDelta;
    float toleranceY_scaled = toleranceY * abs(dy) / maxDelta;

    int xStep = dx > 0 ? 1 : -1; 
    int yStep = dy > 0 ? 1 : -1; 

    dx = abs(dx);
    dy = abs(dy); 

    int d = 2 * dy - dx;
    int dE = 2 * dy;
    int dNE = 2 * (dy - dx);

    int x = x0, y = y0;
    M.push_back(make_pair(x, y));

    while (x != xn || y != yn) {
        if (d <= 0) {
            d += dE;
            x += xStep;
        }
        else {
            d += dNE;
            x += xStep;
            y += yStep;
        }

        for (float i = -toleranceX_scaled; i <= toleranceX_scaled; i += 0.1f) {
            for (float j = -toleranceY_scaled; j <= toleranceY_scaled; j += 0.1f) {
                M.push_back(make_pair(round(x + i), round(y + j)));
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CartesianGrid grid1(15, 15); 
CartesianGrid grid2(15, 15); 

bool displayFirst = true;

void Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (displayFirst) {
        grid1.drawGrid(); 
        vector<pair<int, int>> chosenPixels1;
        ScanConvertSegments3(0, 0, 15, 7, chosenPixels1, 0, 0);
        for (const auto& pixel : chosenPixels1) {
            grid1.addPixel(pixel.first, pixel.second);
        }
        grid1.drawLine(0, 0, 15, 7);
        vector<pair<int, int>> chosenPixels2;
        ScanConvertSegments3(0, 15, 15, 10, chosenPixels2);
        for (const auto& pixel : chosenPixels2) {
            grid1.addPixel(pixel.first, pixel.second);
        }
        grid1.InitPixelSet1(0, 15, 0, 15);
        grid1.drawPixels();
        grid1.drawLine(0, 15, 15, 10);
    }
    else {

        grid2.drawGrid();
        vector<pair<int, int>> chosenPixels2;
        grid2.ScanConvertCircle4(0, 0, 13, chosenPixels2);
        for (const auto& pixel : chosenPixels2) {
            grid2.addPixel(pixel.first, pixel.second);
        }
        grid2.InitPixelSet2(0, 15, 0, 15);
        grid2.drawPixels();

    }

    glutSwapBuffers();
}

void Keyboard(unsigned char key, int x, int y) {
    if (key == ' ') {
        displayFirst = !displayFirst;
        glutPostRedisplay(); 
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Cartesian Grid");
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(Display);
    glutKeyboardFunc(Keyboard);

    glutMainLoop();
    return 0;
}
