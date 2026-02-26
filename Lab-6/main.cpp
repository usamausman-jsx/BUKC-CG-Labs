/*
================================================================================
    LAB 06: LINE AND CIRCLE DRAWING ALGORITHMS
    Computer Graphics Course - Bahria University Karachi
================================================================================

OBJECTIVES:
    1. Implement DDA (Digital Differential Analyzer) Line Drawing Algorithm
    2. Implement Bresenham's Line Drawing Algorithm
    3. Implement Simple Circle Drawing Algorithm (using equation)
    4. Implement Midpoint Circle Drawing Algorithm

NEW CONCEPTS IN THIS LAB:
    - Scan conversion (rasterization)
    - Integer-only arithmetic for efficiency
    - Decision parameters
    - Incremental algorithms
    - 8-way symmetry for circles
    - Pixel-level drawing precision

================================================================================
WHY DO WE NEED THESE ALGORITHMS?
================================================================================

PROBLEM WITH BASIC glVertex():
    glVertex2f(x, y) uses floating-point coordinates
    OpenGL does rasterization internally (we don't see how)
    
    These algorithms show us HOW rasterization actually works!

REAL-WORLD APPLICATIONS:
    - Hardware graphics implementation
    - Understanding GPU internals
    - Optimizing rendering
    - Custom graphics systems
    - Embedded systems with no GPU

HISTORICAL CONTEXT:
    - 1960s-1970s: No hardware acceleration
    - Every pixel computed by CPU
    - Speed was CRITICAL
    - Integer math much faster than floating-point
    - These algorithms were revolutionary optimizations!

================================================================================
SCAN CONVERSION (RASTERIZATION)
================================================================================

RASTERIZATION = Converting geometric shapes to pixels

Example: Line from (2, 1) to (7, 4)

Continuous line:  /
                 /
                /
               /

Pixel grid:
    5 |           
    4 |         ¦
    3 |       ¦
    2 |     ¦
    1 |   ¦
      +----------
        2 3 4 5 6 7

CHALLENGE: Which pixels should we light up?

NAIVE APPROACH: y = mx + b
    Problem: Floating-point math, gaps, inefficient

SMART APPROACH: Incremental algorithms (DDA, Bresenham)
    Advantage: Integer math, fast, no gaps

================================================================================
*/

// HEADER FILES
// ============================================================================
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>

using namespace std;

#define PI 3.14159265358979323846

// Window dimensions
int windowWidth = 800;
int windowHeight = 600;


// HELPER: Draw a single pixel
// ============================================================================
void DrawPixel(int x, int y)
{
    /*
    In real hardware, this would write directly to framebuffer
    In OpenGL, we use glVertex2i() inside GL_POINTS
    */
    glBegin(GL_POINTS);
        glVertex2i(x, y);
    glEnd();
}


// ALGORITHM 1: DDA (Digital Differential Analyzer) Line
// ============================================================================
void DDA_Line(int x1, int y1, int x2, int y2)
{
    /*
    DDA ALGORITHM:
    --------------
    1. Calculate dx = x2 - x1, dy = y2 - y1
    2. Determine steps = max(|dx|, |dy|)
    3. Calculate x_increment = dx / steps
    4. Calculate y_increment = dy / steps
    5. Start at (x1, y1)
    6. Loop 'steps' times:
       - Plot current (x, y)
       - x += x_increment
       - y += y_increment
    
    PROS:
        ? Simple to understand
        ? Works for all slopes
    
    CONS:
        ? Uses floating-point arithmetic (slower)
        ? Requires rounding (round() function)
        ? Accumulation of rounding errors
    */
    
    cout << "DDA Line: (" << x1 << "," << y1 << ") to (" << x2 << "," << y2 << ")" << endl;
    
    // Calculate deltas
    int dx = x2 - x1;
    int dy = y2 - y1;
    
    // Determine number of steps
    int steps;
    if (abs(dx) > abs(dy))
        steps = abs(dx);  // More horizontal
    else
        steps = abs(dy);  // More vertical
    
    // Calculate increments (floating point!)
    float x_increment = (float)dx / (float)steps;
    float y_increment = (float)dy / (float)steps;
    
    // Start at first point
    float x = x1;
    float y = y1;
    
    // Draw line
    glBegin(GL_POINTS);
        for (int i = 0; i <= steps; i++)
        {
            // Round to nearest integer pixel
            glVertex2i((int)(x + 0.5), (int)(y + 0.5));
            
            // Increment
            x += x_increment;
            y += y_increment;
        }
    glEnd();
    
    cout << "  Steps: " << steps << ", dx: " << dx << ", dy: " << dy << endl;
}


// ALGORITHM 2: Bresenham's Line Drawing Algorithm
// ============================================================================
void Bresenham_Line(int x1, int y1, int x2, int y2)
{
    /*
    BRESENHAM'S ALGORITHM:
    ----------------------
    Key Insight: Use only INTEGER arithmetic!
    
    Decision parameter: p
        If p < 0: Choose pixel at same Y (move right)
        If p = 0: Choose pixel at Y+1 (move diagonal)
    
    ALGORITHM (for slope 0 < m < 1):
    1. dx = x2 - x1, dy = y2 - y1
    2. p = 2*dy - dx  (initial decision parameter)
    3. x = x1, y = y1
    4. While x = x2:
       a. Plot (x, y)
       b. If p < 0:
          - p = p + 2*dy
          - x = x + 1 (only move right)
       c. Else:
          - p = p + 2*dy - 2*dx
          - x = x + 1, y = y + 1 (move diagonal)
    
    PROS:
        ? Only integer arithmetic (FAST!)
        ? No rounding needed
        ? No floating-point errors
        ? Optimal pixel selection
    
    CONS:
        ? More complex to understand
        ? Different cases for different slopes
    
    NOTE: This simplified version assumes 0 < slope < 1
          Full implementation handles all 8 octants
    */
    
    cout << "Bresenham Line: (" << x1 << "," << y1 << ") to (" << x2 << "," << y2 << ")" << endl;
    
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    
    int sx = (x1 < x2) ? 1 : -1;  // Step direction for x
    int sy = (y1 < y2) ? 1 : -1;  // Step direction for y
    
    int err = dx - dy;  // Error value (decision parameter)
    
    int x = x1;
    int y = y1;
    
    glBegin(GL_POINTS);
        while (true)
        {
            // Plot current pixel
            glVertex2i(x, y);
            
            // Check if we've reached end point
            if (x == x2 && y == y2)
                break;
            
            // Calculate error
            int e2 = 2 * err;
            
            // Adjust x coordinate
            if (e2 > -dy)
            {
                err -= dy;
                x += sx;
            }
            
            // Adjust y coordinate
            if (e2 < dx)
            {
                err += dx;
                y += sy;
            }
        }
    glEnd();
}


// ALGORITHM 3: Simple Circle (using equation)
// ============================================================================
void SimpleCircle(int xc, int yc, int radius)
{
    /*
    SIMPLE CIRCLE ALGORITHM:
    ------------------------
    Using: x² + y² = r²
    Solve for y: y = v(r² - x²)
    
    ALGORITHM:
    1. For x from -r to +r:
       a. Calculate y = v(r² - x²)
       b. Plot 4 points (using symmetry):
          - (xc+x, yc+y)   top-right quadrant
          - (xc+x, yc-y)   bottom-right quadrant
          - (xc-x, yc+y)   top-left quadrant
          - (xc-x, yc-y)   bottom-left quadrant
    
    PROS:
        ? Simple and intuitive
    
    CONS:
        ? Uses sqrt() - VERY slow!
        ? Floating-point arithmetic
        ? Not efficient for hardware
        ? Only calculates 4 points (not 8-way symmetry)
    */
    
    cout << "Simple Circle: center (" << xc << "," << yc << "), radius " << radius << endl;
    
    glBegin(GL_POINTS);
        for (int x = -radius; x <= radius; x++)
        {
            // Calculate y from circle equation: y = sqrt(r² - x²)
            int y_squared = radius * radius - x * x;
            
            if (y_squared >= 0)  // Only if inside circle
            {
                int y = (int)(sqrt(y_squared) + 0.5);  // Round
                
                // Plot 4 symmetric points
                glVertex2i(xc + x, yc + y);   // Top
                glVertex2i(xc + x, yc - y);   // Bottom
                glVertex2i(xc - x, yc + y);   // Top (other side)
                glVertex2i(xc - x, yc - y);   // Bottom (other side)
            }
        }
    glEnd();
}


// ALGORITHM 4: Midpoint Circle Algorithm
// ============================================================================
void PlotCirclePoints(int xc, int yc, int x, int y)
{
    /*
    8-WAY SYMMETRY:
    ---------------
    Given a point (x, y) on the circle in the first octant,
    we can derive 7 other points by reflection:
    
        (x,y)     (-x,y)      ? Top quadrants
        (y,x)     (-y,x)
        
        (y,-x)    (-y,-x)     ? Bottom quadrants
        (x,-y)    (-x,-y)
    
    This is the KEY optimization of midpoint circle algorithm!
    */
    glBegin(GL_POINTS);
        glVertex2i(xc + x, yc + y);   // Octant 1
        glVertex2i(xc - x, yc + y);   // Octant 2
        glVertex2i(xc + x, yc - y);   // Octant 8
        glVertex2i(xc - x, yc - y);   // Octant 7
        glVertex2i(xc + y, yc + x);   // Octant 3
        glVertex2i(xc - y, yc + x);   // Octant 4
        glVertex2i(xc + y, yc - x);   // Octant 6
        glVertex2i(xc - y, yc - x);   // Octant 5
    glEnd();
}

void MidpointCircle(int xc, int yc, int radius)
{
    /*
    MIDPOINT CIRCLE ALGORITHM:
    --------------------------
    Key Insight: Only calculate 1/8 of circle, use symmetry for rest!
    
    Decision parameter: p
        If p < 0: Choose pixel to the right (E)
        If p = 0: Choose pixel to the right-down (SE)
    
    ALGORITHM:
    1. x = 0, y = radius
    2. p = 1 - radius  (initial decision parameter)
    3. Plot 8 symmetric points for (x, y)
    4. While x < y:
       a. x = x + 1
       b. If p < 0:
          - p = p + 2*x + 1
       c. Else:
          - y = y - 1
          - p = p + 2*x - 2*y + 1
       d. Plot 8 symmetric points for (x, y)
    
    PROS:
        ? Only INTEGER arithmetic!
        ? 8-way symmetry (only calculate 1/8 of circle)
        ? Very fast (no sqrt, no floating-point)
        ? Perfect for hardware
    
    CONS:
        ? Complex to derive (requires calculus)
    */
    
    cout << "Midpoint Circle: center (" << xc << "," << yc << "), radius " << radius << endl;
    
    int x = 0;
    int y = radius;
    int p = 1 - radius;  // Initial decision parameter
    
    // Plot initial points (at x = 0)
    PlotCirclePoints(xc, yc, x, y);
    
    while (x < y)
    {
        x++;
        
        if (p < 0)
        {
            // Choose E (East) - pixel to the right
            p = p + 2*x + 1;
        }
        else
        {
            // Choose SE (South-East) - pixel to the right-down
            y--;
            p = p + 2*x - 2*y + 1;
        }
        
        // Plot 8 symmetric points
        PlotCirclePoints(xc, yc, x, y);
    }
}


// INITIALIZATION
// ============================================================================
void myInit()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);  // White background
    glColor3f(0.0, 0.0, 0.0);           // Black pixels
    glPointSize(2.0);  // Larger pixels to see individual points
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, windowWidth, 0.0, windowHeight);
}


// DISPLAY FUNCTION - DEMO: Compare All Algorithms
// ============================================================================
void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    // ========================================================================
    // LINE ALGORITHMS COMPARISON
    // ========================================================================
    
    // DDA Line (RED)
    glColor3f(1.0, 0.0, 0.0);
    DDA_Line(50, 50, 250, 150);
    
    // Bresenham Line (BLUE)
    glColor3f(0.0, 0.0, 1.0);
    Bresenham_Line(50, 200, 250, 300);
    
    // For comparison: OpenGL's built-in line (GREEN)
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
        glVertex2i(50, 350);
        glVertex2i(250, 450);
    glEnd();
    
    
    // ========================================================================
    // CIRCLE ALGORITHMS COMPARISON
    // ========================================================================
    
    // Simple Circle (RED)
    glColor3f(1.0, 0.0, 0.0);
    SimpleCircle(450, 150, 80);
    
    // Midpoint Circle (BLUE)
    glColor3f(0.0, 0.0, 1.0);
    MidpointCircle(650, 150, 80);
    
    // For comparison: OpenGL's circle from Lab 5 (GREEN)
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 100; i++)
        {
            float theta = 2.0f * PI * i / 100.0f;
            glVertex2f(550 + 80 * cosf(theta), 350 + 80 * sinf(theta));
        }
    glEnd();
    
    
    glFlush();
}


// MAIN FUNCTION
// ============================================================================
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("LAB 06 - Line & Circle Drawing Algorithms");
    glutDisplayFunc(myDisplay);
    myInit();
    
    cout << "\n=== LAB 06 DEMO - Algorithm Comparison ===" << endl;
    cout << "Lines (left side):" << endl;
    cout << "  RED   = DDA Algorithm" << endl;
    cout << "  BLUE  = Bresenham Algorithm" << endl;
    cout << "  GREEN = OpenGL Built-in" << endl;
    cout << "\nCircles (right side):" << endl;
    cout << "  RED   = Simple (equation-based)" << endl;
    cout << "  BLUE  = Midpoint Algorithm" << endl;
    cout << "  GREEN = Parametric (Lab 5)" << endl;
    cout << "=========================================\n" << endl;
    
    glutMainLoop();
}


/*
================================================================================
UNDERSTANDING THE ALGORITHMS - DETAILED WALKTHROUGH
================================================================================

DDA EXAMPLE:
------------
Line from (2, 1) to (7, 4)

1. dx = 7 - 2 = 5
   dy = 4 - 1 = 3

2. steps = max(|5|, |3|) = 5

3. x_increment = 5/5 = 1.0
   y_increment = 3/5 = 0.6

4. Iteration:
   i=0: (2.0, 1.0) ? pixel (2, 1)
   i=1: (3.0, 1.6) ? pixel (3, 2)
   i=2: (4.0, 2.2) ? pixel (4, 2)
   i=3: (5.0, 2.8) ? pixel (5, 3)
   i=4: (6.0, 3.4) ? pixel (6, 3)
   i=5: (7.0, 4.0) ? pixel (7, 4)


BRESENHAM EXAMPLE:
------------------
Same line: (2, 1) to (7, 4)

1. dx = 5, dy = 3
2. p = 2*3 - 5 = 1 (initial)
3. x = 2, y = 1

Iteration:
   Plot (2, 1), p = 1
   p = 0 ? x=3, y=2, p = 1 + 2*3 - 2*5 = -3
   
   Plot (3, 2), p = -3
   p < 0 ? x=4, y=2, p = -3 + 2*3 = 3
   
   Plot (4, 2), p = 3
   p = 0 ? x=5, y=3, p = 3 + 2*3 - 2*5 = -1
   
   ... and so on


MIDPOINT CIRCLE EXAMPLE:
------------------------
Circle with radius 5

1. x = 0, y = 5
2. p = 1 - 5 = -4

Iteration:
   Plot 8 points for (0, 5)
   x = 1, p = -4
   p < 0 ? p = -4 + 2*1 + 1 = -1
   
   Plot 8 points for (1, 5)
   x = 2, p = -1
   p < 0 ? p = -1 + 2*2 + 1 = 4
   
   Plot 8 points for (2, 5)
   x = 3, p = 4
   p = 0 ? y = 4, p = 4 + 2*3 - 2*4 + 1 = 3
   
   ... continue until x = y


8-WAY SYMMETRY VISUALIZATION:
------------------------------
For point (3, 5) in octant 1:

      (-3,5) (3,5)
    (-5,3)     (5,3)
         \   /
          \ /
           O
          / \
         /   \
    (-5,-3)   (5,-3)
      (-3,-5) (3,-5)

Only calculate (3, 5), get other 7 for FREE!

================================================================================
WHY INTEGER-ONLY MATH MATTERS
================================================================================

1960s-1980s HARDWARE:
    Floating-point operations: 100+ CPU cycles
    Integer operations: 1 CPU cycle
    
    Speed difference: 100x faster!

MODERN HARDWARE:
    Still faster (GPUs use integer pipelines)
    More predictable (no rounding errors)
    Better for embedded systems

COMPARISON:
    DDA:       100 lines/sec (with sqrt)
    Bresenham: 10,000 lines/sec (integer only)
    
    This mattered when drawing thousands of lines per frame!

================================================================================
*/
