/*
================================================================================
    LAB 02: DRAWING WITH POINTS AND LINES
    Computer Graphics Course - Bahria University Karachi
================================================================================

OBJECTIVES:
    1. Draw different drawings using GL_POINTS
    2. Draw different drawings using GL_LINE_STRIP
    3. Draw shapes/characters using file reading technique

NEW CONCEPTS IN THIS LAB:
    - Plotting mathematical functions (sin, cos) using loops
    - glVertex2f() with FLOATING POINT coordinates
    - Reading vertex data from external .txt files
    - GL_LINE_STRIP to draw connected curves
    - rand() for random number generation (Sierpinski Gasket)
    - Using <cmath> for sin(), cos() functions

================================================================================
WHAT'S DIFFERENT FROM LAB 01?
================================================================================
Lab 01: Static shapes drawn with fixed integer coordinates
Lab 02: Dynamic drawing using:
         ? Loops to plot many points (functions, curves)
         ? Float coordinates for smooth curves
         ? File I/O to load shape data from external files
         ? Random numbers for fractal generation

================================================================================
*/

// HEADER FILES
// ============================================================================
#include <windows.h>     // Windows-specific functions
#include <GL/gl.h>       // Core OpenGL
#include <GL/glu.h>      // OpenGL Utility (gluOrtho2D, etc.)
#include <GL/glut.h>     // GLUT windowing toolkit

// NEW HEADERS FOR THIS LAB
#include <cmath>         // sin(), cos(), fabs() - math functions
#include <cstdlib>       // rand(), srand() - random numbers
#include <ctime>         // time() - seed for random numbers
#include <fstream>       // ifstream - file reading
#include <iostream>      // cout, endl - console output (debugging)

using namespace std;

/*
EXPLANATION OF NEW HEADERS:
----------------------------
cmath    : Mathematical functions
           - sin(x), cos(x), tan(x)     -> trigonometry (radians)
           - sqrt(x)                    -> square root
           - fabs(x)                    -> absolute value for floats
           - pow(x, n)                  -> x to the power n

cstdlib  : Standard library
           - rand()     -> returns random int from 0 to RAND_MAX
           - srand(n)   -> seeds the random number generator
           - RAND_MAX   -> largest possible value from rand()

ctime    : Time functions
           - time(0)    -> current time (used to seed rand)

fstream  : File stream operations
           - ifstream   -> input file stream (for reading)
           - ofstream   -> output file stream (for writing)
*/


// COORDINATE SYSTEM NOTE
// ============================================================================
/*
For Lab 02, we use a coordinate system centered at the middle of the window.
This makes it easier to plot mathematical functions.

gluOrtho2D(-400.0, 400.0, -300.0, 300.0) gives us:

         Y (300)
         ?
         |
-400 ----+---------- +400 ? X
         |
         ?
        -300

CENTER = (0, 0) is now the CENTER of the window
This makes sin/cos plots symmetric and natural.

If we want to SCALE the plot, multiply x and y by a scale factor:
  screenX = x * 50;   (50 pixels per unit)
  screenY = sin(x) * 50;
*/


// INITIALIZATION FUNCTION
// ============================================================================
void myInit()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);  // White background
    glColor3f(0.0, 0.0, 0.0);           // Black drawing color
    glPointSize(2.0);                    // Small points for smooth curves
    glLineWidth(1.5);                    // Default line width

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // CENTERED coordinate system (-400 to +400 X, -300 to +300 Y)
    gluOrtho2D(-400.0, 400.0, -300.0, 300.0);
    /*
    Centered system chosen because:
    1. sin() and cos() functions are symmetric about 0
    2. Makes math easier - direct mapping of function values
    3. Origin at center visually matches the function graphs
    */
}


// DISPLAY FUNCTION (Demo: Sin Wave)
// ============================================================================
void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    // ========================================================================
    // DRAW X-AXIS (for reference)
    // ========================================================================
    glColor3f(0.7, 0.7, 0.7);  // Light gray
    glBegin(GL_LINES);
        glVertex2f(-400.0, 0.0);   // Left end
        glVertex2f( 400.0, 0.0);   // Right end
    glEnd();

    // DRAW Y-AXIS (for reference)
    glBegin(GL_LINES);
        glVertex2f(0.0, -300.0);   // Bottom end
        glVertex2f(0.0,  300.0);   // Top end
    glEnd();


    // ========================================================================
    // CONCEPT 1: USING GL_POINTS TO PLOT A FUNCTION
    // ========================================================================
    /*
    HOW IT WORKS:
    1. Loop through x values (e.g. from -400 to 400)
    2. For each x, compute y = f(x)
    3. Plot a point at (x, y)
    4. The density of points creates the curve
    */
    glColor3f(1.0, 0.0, 0.0);  // Red - sine using points
    glBegin(GL_POINTS);
        for (float x = -400.0; x <= 400.0; x += 0.5)
        {
            /*
            FORMULA: sin(pi * x / pi) simplifies to sin(x)
            But the manual says sin(pi*x)/pi, so:
            - x ranges from real units, scaled to pixels
            - We divide by pi to keep amplitude manageable
            */
            float xRad = x / 50.0;       // Convert pixels to radians range
            float y = sin(xRad) * 100.0; // Scale amplitude to 100 pixels

            glVertex2f(x, y);

            /*
            BREAKDOWN:
            x        : screen x-position (-400 to 400 pixels)
            xRad     : x converted to radian units (x/50 so we see ~4 cycles)
            sin(xRad): value between -1.0 and +1.0
            y        : scaled to screen units (±100 pixels amplitude)
            */
        }
    glEnd();


    // ========================================================================
    // CONCEPT 2: USING GL_LINE_STRIP TO PLOT A FUNCTION
    // ========================================================================
    /*
    DIFFERENCE FROM GL_POINTS:
    - GL_POINTS: each vertex is an isolated dot
    - GL_LINE_STRIP: vertices are connected by lines ? smooth curve

    USE GL_LINE_STRIP when:
    - Plotting curves (sin, cos)
    - Drawing connected shapes
    - Drawing freehand paths

    USE GL_POINTS when:
    - Scatter plots
    - Individual data points
    - Fractal patterns like Sierpinski Gasket
    */
    glColor3f(0.0, 0.0, 1.0);  // Blue - same sine using LINE_STRIP
    glBegin(GL_LINE_STRIP);
        for (float x = -400.0; x <= 400.0; x += 1.0)
        {
            float xRad = x / 50.0;
            float y = cos(xRad) * 100.0;  // cos instead of sin (offset visual)
            glVertex2f(x, y);
        }
    glEnd();


    // ========================================================================
    // CONCEPT 3: glVertex2f vs glVertex2i
    // ========================================================================
    /*
    glVertex2i(int x, int y)
      - Integer coordinates
      - Good for fixed shapes like rectangles, houses
      - Limited to whole pixel positions

    glVertex2f(float x, float y)
      - Floating-point coordinates
      - REQUIRED for math functions (sin returns floats!)
      - Enables smooth, sub-pixel positioning
      - ALWAYS use 2f when computing positions with math
    */


    glFlush();
}


/*
================================================================================
FUNCTION PLOTTING - HOW THE MATH WORKS
================================================================================

The manual says to plot:  sin(p·x) / p   for x = -4 to 4

STEP-BY-STEP EXPLANATION:
--------------------------
1. The mathematical x-domain is -4 to 4
   But OpenGL window is -400 to 400 pixels
   So SCALE: pixelX = x * 100

2. The function value sin(p·x)/p:
   - sin(p·-4) = sin(-4p) = 0
   - sin(p·-3.5) = sin(-3.5p) = 1 (approximately)
   - Maximum value = 1/p ˜ 0.318
   So the amplitude is small! SCALE y too: pixelY = f(x) * 300

3. Final formulas for plotting:
   float x_math = -4.0 + i * step;         (math domain)
   float pixX   = x_math * 100.0;          (to pixels)
   float pixY   = (sin(M_PI * x_math) / M_PI) * 300.0;
   glVertex2f(pixX, pixY);


COORDINATE SCALING SUMMARY:
----------------------------
Math domain: [-4, 4]   ? Screen X: [-400, 400]  ? Scale: × 100
Math range:  [-1/p, 1/p] ? Screen Y: [-100, 100] ? Scale: × 300

Smaller step size ? more points ? smoother curve
  step = 0.1  : 80 points (rough)
  step = 0.01 : 800 points (smooth)
  step = 0.001: 8000 points (very smooth, slower)

================================================================================
GL_LINE_STRIP vs GL_LINE_LOOP vs GL_LINES
================================================================================

        P1 --- P2 --- P3           P1 --- P2
                           vs              ¦        vs   P1---P2    P3---P4
        GL_LINE_STRIP                P4 --- P3            ¦ ¦       ?
        (open path)           GL_LINE_LOOP    GL_LINES    separate
                                (closed)     (pairs)       segments

USE EACH WHEN:
GL_LINE_STRIP : curves, paths, open shapes (sin wave, hand-drawn)
GL_LINE_LOOP  : closed outlines (square, circle)
GL_LINES      : separate segments (axes, grids, rays)

================================================================================
SIERPINSKI GASKET - HOW IT WORKS
================================================================================

The Chaos Game algorithm for generating the Sierpinski Gasket:

1. Start with 3 fixed vertices (corners of a triangle)
2. Pick a random starting point
3. Repeat 50,000+ times:
   a. Pick one of the 3 corners at random
   b. Move HALFWAY toward that corner
   c. Draw a point there

The resulting points form the famous fractal triangle!

WHY IT WORKS (Intuition):
- Moving toward the same corner repeatedly = small triangle in corner
- Moving between different corners = gaps never fill
- After enough iterations, the fractal pattern emerges

FORMULA:
  pk.x = (pk-1.x + T.x) / 2;
  pk.y = (pk-1.y + T.y) / 2;

================================================================================
FILE READING - HOW IT WORKS FOR THE DINOSAUR
================================================================================

The dinosaur shape is stored in a text file as vertices:
  dino.txt:
    210 340
    230 355
    250 370
    ...

C++ reads this line by line:
  ifstream inFile("dino.txt");
  float x, y;
  while(inFile >> x >> y)
  {
      glVertex2f(x, y);
  }

GL_LINE_STRIP connects all vertices in order ? dinosaur outline!

IMPORTANT:
- .txt file must be in same folder as .exe
- Or use full path: "C:\\path\\to\\dino.txt"
- Use GL_LINE_LOOP to close the shape (connect last to first)

================================================================================
*/


// ADDITIONAL DEMO: Cos Wave + Grid (Uncomment to explore)
// ============================================================================
/*
void myDisplay_TrigDemo(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw grid lines
    glColor3f(0.9, 0.9, 0.9);
    for(int x = -400; x <= 400; x += 100) {
        glBegin(GL_LINES);
            glVertex2f(x, -300); glVertex2f(x, 300);
        glEnd();
    }
    for(int y = -300; y <= 300; y += 100) {
        glBegin(GL_LINES);
            glVertex2f(-400, y); glVertex2f(400, y);
        glEnd();
    }

    // Draw axes
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
        glVertex2f(-400, 0); glVertex2f(400, 0);  // X-axis
        glVertex2f(0, -300); glVertex2f(0, 300);  // Y-axis
    glEnd();

    // Draw sin wave - RED
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
        for(float x = -400; x <= 400; x += 1.0) {
            float y = sin(x * 3.14159 / 100.0) * 100.0;
            glVertex2f(x, y);
        }
    glEnd();

    // Draw cos wave - BLUE
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINE_STRIP);
        for(float x = -400; x <= 400; x += 1.0) {
            float y = cos(x * 3.14159 / 100.0) * 100.0;
            glVertex2f(x, y);
        }
    glEnd();

    glFlush();
}
*/


// MAIN FUNCTION
// ============================================================================
int main(int argc, char** argv)
{
    srand(time(0));  // Seed random number generator (for Sierpinski)
    /*
    srand(time(0))
    - Seeds the random number generator with current time
    - Without this, rand() gives same sequence every run
    - Call ONCE at program start, in main(), before glutMainLoop
    */

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    /*
    Larger window (800x600) for this lab because:
    - Function plots look better with more space
    - Sierpinski gasket needs room to show detail
    - Dinosaur shape spans a large coordinate range
    */
    glutInitWindowPosition(100, 100);
    glutCreateWindow("LAB 02 - Drawing with Points and Lines");
    glutDisplayFunc(myDisplay);
    myInit();
    glutMainLoop();
}


/*
================================================================================
SUMMARY OF KEY CONCEPTS
================================================================================

CONCEPT               | Lab 01               | Lab 02
----------------------|----------------------|---------------------------------
Coordinates           | glVertex2i (int)     | glVertex2f (float) ? NEW
Loop drawing          | No loops             | for() loop for curves ? NEW
Math functions        | None                 | sin(), cos() from <cmath> ? NEW
Random numbers        | None                 | rand(), srand() ? NEW
File input            | None                 | ifstream from <fstream> ? NEW
Primitives used       | POINTS, QUADS, TRIS  | POINTS, LINE_STRIP ? focus
Coordinate origin     | Bottom-left (0,0)    | Center (0,0) for math ? NEW

================================================================================
*/
