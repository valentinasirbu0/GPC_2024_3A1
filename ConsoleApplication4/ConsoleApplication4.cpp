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
        glColor3f(0.5f, 0.5f, 0.5f); // Gray color
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
        glPopMatrix();
    }

    void drawPixels() {
        glPushMatrix();
        glColor3f(1.0f, 1.0f, 1.0f); // White color
        for (const auto& pixel : pixels) {
            // Check if pixel coordinates are within grid bounds
            if (pixel.first >= 0 && pixel.first <= columns && pixel.second >= 0 && pixel.second <= rows) {
                glPushMatrix();
                glTranslatef(pixel.first * gridSize / columns - gridSize / 2.0f,
                    pixel.second * gridSize / rows - gridSize / 2.0f,
                    0.0f);
                glutSolidSphere(0.02f, 20, 20); // Draw a solid sphere as a pixel
                glPopMatrix();
            }
        }
        glPopMatrix();
    }

    void drawLine(int x1, int y1, int x2, int y2) {
        glPushMatrix();
        glColor3f(1.0f, 0.0f, 0.0f); // Red color
        glLineWidth(3.0f); // Set line width to 3.0 units
        glBegin(GL_LINES);
        glVertex2f(x1 * gridSize / columns - gridSize / 2.0f, y1 * gridSize / rows - gridSize / 2.0f);
        glVertex2f(x2 * gridSize / columns - gridSize / 2.0f, y2 * gridSize / rows - gridSize / 2.0f);
        glEnd();
        glPopMatrix();
    }

    void drawCircle(float ox, float oy, float radius) {
        const int numSegments = 100;
        glColor3f(1.0f, 0.0f, 0.0f); // Set color to red
        glLineWidth(3.0f);
        glBegin(GL_LINE_STRIP);

        float startAngle = 0.0f; // Start angle at 0
        float endAngle = 0.5f * 3.14f; // End angle at π/2

        for (int i = 0; i < numSegments; i++) {
            float theta = startAngle + (endAngle - startAngle) * float(i) / float(numSegments); // Angle for each segment
            float x = radius * cosf(theta); // Calculate x-coordinate
            float y = radius * sinf(theta); // Calculate y-coordinate

            // Check if the segment lies within the grid boundaries
            if (x + ox >= -gridSize / 2.0f && x + ox <= gridSize / 2.0f &&
                y + oy >= -gridSize / 2.0f && y + oy <= gridSize / 2.0f) {
                glVertex2f(x + ox, y + oy); // Offset by center coordinates and draw vertex
            }
        }
        glEnd();
    }

    void ScanConvertCircle4(int ox, int oy, int r, vector<pair<int, int>>& M) {
        drawCircle(ox * gridSize / columns - gridSize / 2.0f, oy * gridSize / rows - gridSize / 2.0f, r * gridSize / columns); // Translate and scale coordinates

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
void ScanConvertSegments3(int x0, int y0, int xn, int yn, vector<pair<int, int>>& M, float toleranceX = 2.7, float toleranceY = 1) {
    // Check for invalid inputs (start and end points are the same)
    if (x0 == xn && y0 == yn) {
        M.push_back(make_pair(x0, y0));
        return;
    }

    int dx = xn - x0;
    int dy = yn - y0;

    // Determine the maximum absolute change in x and y
    int maxDelta = max(abs(dx), abs(dy));

    // Calculate scaled tolerances based on the maximum delta
    float toleranceX_scaled = toleranceX * abs(dx) / maxDelta;
    float toleranceY_scaled = toleranceY * abs(dy) / maxDelta;

    int xStep = dx > 0 ? 1 : -1; // Determine the direction of movement for x
    int yStep = dy > 0 ? 1 : -1; // Determine the direction of movement for y

    dx = abs(dx); // Make sure dx is positive
    dy = abs(dy); // Make sure dy is positive

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

        // Add pixels within scaled tolerance area around the line
        for (float i = -toleranceX_scaled; i <= toleranceX_scaled; i += 0.1f) {
            for (float j = -toleranceY_scaled; j <= toleranceY_scaled; j += 0.1f) {
                M.push_back(make_pair(round(x + i), round(y + j)));
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CartesianGrid grid1(15, 15); // Create a Cartesian grid with 15 rows and 15 columns for the first display
CartesianGrid grid2(15, 15); // Create a Cartesian grid with 15 rows and 15 columns for the second display

bool displayFirst = true; // Flag to determine which display to show

void Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (displayFirst) {
        grid1.drawGrid(); // Draw the grid for the first display

        // Draw chosen pixels for the first segment in the first display
        vector<pair<int, int>> chosenPixels1;
        ScanConvertSegments3(0, 0, 15, 7, chosenPixels1, 0, 0);
        for (const auto& pixel : chosenPixels1) {
            grid1.addPixel(pixel.first, pixel.second);
        }

        // Draw line for the segment in the first display
        grid1.drawLine(0, 0, 15, 7);

        // Draw chosen pixels for the second segment in the second display
        vector<pair<int, int>> chosenPixels2;
        ScanConvertSegments3(0, 15, 15, 10, chosenPixels2);
        for (const auto& pixel : chosenPixels2) {
            grid1.addPixel(pixel.first, pixel.second);
        }
        grid1.drawPixels();

        // Draw line for the segment in the second display
        grid1.drawLine(0, 15, 15, 10);
    }
    else {

        grid2.drawGrid(); // Draw the grid

        vector<pair<int, int>> chosenPixels2;
        grid2.ScanConvertCircle4(0, 0, 13, chosenPixels2);

        for (const auto& pixel : chosenPixels2) {
            grid2.addPixel(pixel.first, pixel.second);
        }
        grid2.drawPixels();

    }

    glutSwapBuffers();
}

void Keyboard(unsigned char key, int x, int y) {
    if (key == ' ') {
        displayFirst = !displayFirst; // Toggle between the displays
        glutPostRedisplay(); // Trigger display update
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Cartesian Grid");
    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(Display);
    glutKeyboardFunc(Keyboard);

    glutMainLoop();
    return 0;
}
