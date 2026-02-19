/*
================================================================================
    LAB 03: SETTING VIEWPORT
    Computer Graphics Course - Bahria University Karachi
================================================================================

OBJECTIVES:
    1. Explain how a scene within a domain window is mapped onto viewport
    2. Understand how window-viewport mapping is done in OpenGL
    3. Map a 2D scene onto multiple viewports
    4. Program reshape callback to maintain aspect ratios

NEW CONCEPTS IN THIS LAB:
    - glViewport() - dividing the window into regions
    - Multiple viewports - showing same/different content in different areas
    - SetViewport() helper function
    - Aspect ratio preservation
    - Window coordinate system vs World coordinate system

================================================================================
WHAT IS A VIEWPORT?
================================================================================

VIEWPORT = A rectangular region of the display window where OpenGL renders.

Think of it like this:
    WINDOW = The entire display surface (e.g., 800x600 pixels)
    VIEWPORT = A rectangular "drawing canvas" within that window

You can have:
    - ONE viewport filling the entire window (default)
    - MULTIPLE viewports dividing the window into sections
    - Each viewport can show different content or different views of same scene

REAL-WORLD ANALOGY:
    Window = Your TV screen
    Viewport = The picture-in-picture boxes (split screen)

================================================================================
THE TWO COORDINATE SYSTEMS
================================================================================

1. WINDOW COORDINATES (Screen Space)
   -----------------------------------
   - Measured in PIXELS
   - Origin at BOTTOM-LEFT of window
   - Used by glViewport()
   
   Example: 800x600 window
        Y (600)
        ?
        |
   (0,0)|------------? X (800)


2. WORLD COORDINATES (Mathematical Space)
   ---------------------------------------
   - Your drawing coordinates
   - Can be any range you choose
   - Set by gluOrtho2D()
   
   Example: gluOrtho2D(-4, 4, -3, 3)
        Y (+3)
        ?
        |
    ----+---- X
   -4   |  +4
        |
        ?
       -3


THE MAPPING:
    World coords ? Viewport coords
    Your drawing coordinates are automatically scaled and translated
    to fit into the viewport region you specified.

================================================================================
*/

// HEADER FILES
// ============================================================================
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>

#define PI 3.14159265358979323846

// Global window dimensions (so we can use them in various functions)
int windowWidth = 800;
int windowHeight = 600;


// HELPER FUNCTION: Set World Window (Coordinate System)
// ============================================================================
void SetWindow(float left, float right, float bottom, float top)
{
    /*
    PURPOSE: Define the world coordinate system (what coordinates your 
             drawings will use)
    
    PARAMETERS:
        left, right : X-axis range
        bottom, top : Y-axis range
    
    EXAMPLE:
        SetWindow(-4.0, 4.0, -3.0, 3.0);
        ? Now you can draw at positions like (0, 0), (2.5, 1.3), etc.
    */
    glMatrixMode(GL_PROJECTION);  // Switch to projection matrix
    glLoadIdentity();              // Reset it
    gluOrtho2D(left, right, bottom, top);  // Set world coordinates
}


// HELPER FUNCTION: Set Viewport (Screen Region)
// ============================================================================
void SetViewport(int left, int right, int bottom, int top)
{
    /*
    PURPOSE: Define which part of the window to draw in (measured in pixels)
    
    PARAMETERS:
        left, right, bottom, top : boundaries in SCREEN PIXELS
    
    IMPORTANT: 
        glViewport takes (x, y, width, height)
        But this helper takes (left, right, bottom, top) for convenience
        
    CALCULATION:
        x      = left
        y      = bottom
        width  = right - left
        height = top - bottom
    
    EXAMPLE:
        SetViewport(0, 400, 0, 600);
        ? Left half of an 800x600 window
        
        SetViewport(400, 800, 0, 600);
        ? Right half of an 800x600 window
    */
    glViewport(left, bottom, right - left, top - bottom);
    /*
    glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
    
    x, y       : lower-left corner in WINDOW COORDINATES (pixels)
    width      : viewport width in pixels
    height     : viewport height in pixels
    
    DEFAULT: glViewport(0, 0, windowWidth, windowHeight) - entire window
    */
}


// FUNCTION: Draw a simple sine wave (reusable in multiple viewports)
// ============================================================================
void DrawSineWave()
{
    /*
    This function draws a sine wave in the CURRENT viewport with the
    CURRENT world coordinate system.
    
    It will look different depending on:
    - The viewport size (large vs small)
    - The gluOrtho2D range (zoomed in vs zoomed out)
    */
    glColor3f(0.0, 0.0, 1.0);  // Blue
    glLineWidth(2.0);
    
    glBegin(GL_LINE_STRIP);
        for (float x = -4.0; x <= 4.0; x += 0.02)
        {
            float y = sin(PI * x) / PI;
            glVertex2f(x, y);
        }
    glEnd();
    
    // Draw axes for reference
    glColor3f(0.5, 0.5, 0.5);  // Gray
    glLineWidth(1.0);
    glBegin(GL_LINES);
        glVertex2f(-4.0, 0.0); glVertex2f(4.0, 0.0);  // X-axis
        glVertex2f(0.0, -0.5); glVertex2f(0.0, 0.5);  // Y-axis
    glEnd();
}


// INITIALIZATION
// ============================================================================
void myInit()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);  // White background
    glColor3f(0.0, 0.0, 0.0);           // Black drawing color
    glPointSize(2.0);
    glLineWidth(1.5);
}


// DISPLAY FUNCTION - DEMO: 4 Viewports (Quadrants)
// ============================================================================
void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    /*
    CONCEPT DEMONSTRATION:
    We'll divide the 800x600 window into 4 equal quadrants.
    Each quadrant will show the SAME sine wave but with DIFFERENT
    world coordinate ranges (zoom levels).
    
    Window layout:
    +-----------------+
    ¦   3    ¦   4    ¦  ? top half (300-600 in Y)
    +--------+--------¦
    ¦   1    ¦   2    ¦  ? bottom half (0-300 in Y)
    +-----------------+
      0-400    400-800  (X positions)
    */
    
    // ========================================================================
    // VIEWPORT 1: Bottom-Left (normal view)
    // ========================================================================
    SetViewport(0, 400, 0, 300);
    /*
    This viewport occupies:
        X: 0 to 400 pixels (left half)
        Y: 0 to 300 pixels (bottom half)
    Total: 400x300 pixel region at bottom-left
    */
    
    SetWindow(-4.0, 4.0, -0.5, 0.5);
    /*
    World coordinates for this viewport:
        X: -4 to 4
        Y: -0.5 to 0.5
    This gives a "normal" view of the entire sine wave
    */
    
    DrawSineWave();  // Draw in viewport 1
    
    
    // ========================================================================
    // VIEWPORT 2: Bottom-Right (zoomed in on peak)
    // ========================================================================
    SetViewport(400, 800, 0, 300);
    SetWindow(-1.0, 1.0, -0.5, 0.5);
    /*
    Same Y range but X is zoomed: -1 to 1 (instead of -4 to 4)
    This shows only the CENTER portion of the wave (zoomed in 4x)
    */
    
    DrawSineWave();  // Same function, different zoom!
    
    
    // ========================================================================
    // VIEWPORT 3: Top-Left (tall aspect - stretched vertically)
    // ========================================================================
    SetViewport(0, 400, 300, 600);
    SetWindow(-4.0, 4.0, -1.0, 1.0);
    /*
    X: -4 to 4 (same as viewport 1)
    Y: -1 to 1 (double the range)
    Effect: Wave appears vertically compressed (more Y range in same pixels)
    */
    
    DrawSineWave();
    
    
    // ========================================================================
    // VIEWPORT 4: Top-Right (zoomed out)
    // ========================================================================
    SetViewport(400, 800, 300, 600);
    SetWindow(-8.0, 8.0, -1.0, 1.0);
    /*
    X: -8 to 8 (double the range)
    Y: -1 to 1
    Effect: Shows TWO full wave cycles instead of one
    */
    
    DrawSineWave();
    
    
    // ========================================================================
    // DRAW VIEWPORT BORDERS (for visual clarity)
    // ========================================================================
    /*
    To help see where each viewport is, we can draw borders.
    But CAREFUL: we need to set world coords to match screen pixels!
    */
    SetViewport(0, windowWidth, 0, windowHeight);  // Full window
    SetWindow(0, windowWidth, 0, windowHeight);     // 1:1 mapping (pixels)
    
    glColor3f(1.0, 0.0, 0.0);  // Red borders
    glLineWidth(3.0);
    
    // Vertical line splitting left/right
    glBegin(GL_LINES);
        glVertex2f(400, 0);
        glVertex2f(400, 600);
    glEnd();
    
    // Horizontal line splitting top/bottom
    glBegin(GL_LINES);
        glVertex2f(0, 300);
        glVertex2f(800, 300);
    glEnd();
    
    glFlush();
}


/*
================================================================================
UNDERSTANDING THE VIEWPORT-WINDOW RELATIONSHIP
================================================================================

SCENARIO 1: Same world coords, different viewports
---------------------------------------------------
    Viewport A: SetViewport(0, 200, 0, 200);     // Small box
                SetWindow(-4, 4, -3, 3);
                
    Viewport B: SetViewport(0, 400, 0, 400);     // Big box
                SetWindow(-4, 4, -3, 3);          // Same coords!
    
    RESULT: Viewport B shows the SAME SCENE but LARGER (more pixels)


SCENARIO 2: Same viewport, different world coords
--------------------------------------------------
    Viewport: SetViewport(0, 400, 0, 400);       // Same size
    
    Window A: SetWindow(-4, 4, -3, 3);           // Wide view
    Window B: SetWindow(-1, 1, -1, 1);           // Narrow view (zoomed)
    
    RESULT: Window B shows a ZOOMED IN portion of the scene


SCENARIO 3: Aspect ratio mismatch
-----------------------------------
    Viewport: SetViewport(0, 400, 0, 200);       // Wide & short (2:1)
    Window:   SetWindow(-4, 4, -4, 4);           // Square coords (1:1)
    
    RESULT: Circles look like OVALS (stretched horizontally)
    
    TO FIX: Match aspect ratios:
        Viewport 400x200 ? aspect 2:1
        Window should be 2:1 too ? SetWindow(-4, 4, -2, 2)


FORMULA FOR ASPECT RATIO MATCHING:
-----------------------------------
    ViewportAspect = ViewportWidth / ViewportHeight
    
    WorldWidth / WorldHeight should equal ViewportAspect
    
    Example:
        Viewport: 800 / 600 = 1.333...
        World: If X range is 8 units, Y should be 8/1.333 = 6 units
        ? SetWindow(-4, 4, -3, 3)  ?

================================================================================
THE MAGIC OF MULTIPLE VIEWPORTS
================================================================================

USE CASES:
----------

1. SPLIT SCREEN (side-by-side comparisons)
   - Left viewport: original image
   - Right viewport: filtered image

2. PICTURE-IN-PICTURE
   - Main viewport: full scene
   - Small corner viewport: zoomed detail

3. CAD/MODELING SOFTWARE
   - Top viewport: top view
   - Front viewport: front view
   - Side viewport: side view
   - Perspective viewport: 3D view

4. DASHBOARD LAYOUTS
   - Graph in one viewport
   - Status indicators in another
   - Controls in a third

5. TILING PATTERNS (Lab 03 Exercise 1)
   - 5x5 grid of viewports
   - Same sine wave in each (or variations)


HOW TO APPROACH VIEWPORT EXERCISES:
------------------------------------

STEP 1: Calculate viewport positions
    For a 5x5 grid in an 800x600 window:
        Each cell: 800/5 = 160 wide, 600/5 = 120 tall
        
        Cell (row=0, col=0): x=0,   y=0,   w=160, h=120
        Cell (row=0, col=1): x=160, y=0,   w=160, h=120
        Cell (row=1, col=0): x=0,   y=120, w=160, h=120
        ...

STEP 2: Loop through grid positions
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            int x = col * 160;
            int y = row * 120;
            SetViewport(x, x+160, y, y+120);
            SetWindow(-4, 4, -0.5, 0.5);  // Same coords for all
            DrawSineWave();
        }
    }

STEP 3: Draw borders (optional)
    Use full-window viewport + pixel coordinates to draw grid lines

================================================================================
*/


// ALTERNATE DISPLAY: 2x2 Grid Demo (simpler)
// ============================================================================
void myDisplay_2x2Grid(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    int halfW = windowWidth / 2;   // 400
    int halfH = windowHeight / 2;  // 300
    
    // Draw in all 4 quadrants
    for (int row = 0; row < 2; row++)
    {
        for (int col = 0; col < 2; col++)
        {
            int x0 = col * halfW;
            int y0 = row * halfH;
            
            SetViewport(x0, x0 + halfW, y0, y0 + halfH);
            SetWindow(-4.0, 4.0, -0.5, 0.5);
            
            // Vary color by position
            glColor3f(col * 0.5, row * 0.5, 1.0);
            DrawSineWave();
        }
    }
    
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
    glutCreateWindow("LAB 03 - Viewport Demo (4 Quadrants)");
    glutDisplayFunc(myDisplay);
    myInit();
    glutMainLoop();
}


/*
================================================================================
KEY TAKEAWAYS FOR LAB 03
================================================================================

1. glViewport(x, y, width, height)
   - Defines WHERE in the window to draw
   - Measured in PIXELS
   - (x, y) is BOTTOM-LEFT corner

2. gluOrtho2D(left, right, bottom, top)
   - Defines WHAT coordinate range to use
   - Your "mathematical space"
   - Can be any values you want

3. The two work together:
   - Viewport says "use this region of the screen"
   - gluOrtho2D says "map these coordinates into that region"

4. Calling sequence for multiple viewports:
   SetViewport ? SetWindow ? Draw ? SetViewport ? SetWindow ? Draw ? ...

5. Aspect ratios:
   - Viewport aspect: width / height (in pixels)
   - World aspect: (right-left) / (top-bottom)
   - Match them to avoid distortion!

6. Common pattern for grids:
   for (row) {
       for (col) {
           calculate viewport position
           SetViewport(...)
           SetWindow(...)
           DrawContent()
       }
   }

================================================================================
NEXT STEPS
================================================================================

Now you can tackle the exercises:

Exercise 1: 5x5 grid (25 tiles) of sine waves
Exercise 2: Split screen - left and right halves of sine wave
Exercise 3: Grid pattern of dinosaurs from Lab 02

See Lab03_All_Exercise_Solutions.cpp for complete solutions!

================================================================================
*/
