/*
================================================================================
    LAB 01: INTRODUCTION TO OPENGL - DRAWING WITH PRIMITIVES
    Computer Graphics Course - Bahria University Karachi
================================================================================

OBJECTIVE:
    1. Use OpenGL libraries with C++
    2. Draw basic drawings using basic primitives

SETUP INSTRUCTIONS FOR DEV C++:
================================================================================
1. Download and install Dev-C++ (if not already installed)
2. Download freeglut for MinGW:
   - Go to: https://www.transmissionzero.co.uk/software/freeglut-devel/
   - Download "freeglut 3.0.0 MinGW Package"
   
3. Extract the downloaded zip file

4. Copy files to Dev-C++ directory:
   - Copy all files from "include/GL/" to "Dev-Cpp/MinGW64/include/GL/"
   - Copy all files from "lib/" to "Dev-Cpp/MinGW64/lib/"
   - Copy "freeglut.dll" from "bin/" to "C:/Windows/System32/" (for 64-bit)
     or "C:/Windows/SysWOW64/" (for 32-bit)

5. In Dev-C++, go to: Tools > Compiler Options > Settings > Linker
   Add these libraries:
   -lopengl32
   -lglu32
   -lfreeglut


================================================================================
*/

// HEADER FILES
// ============================================================================
#include <windows.h>      // Windows specific functions (for Windows OS)
#include <GL/gl.h>        // OpenGL core library - contains basic GL functions
#include <GL/glu.h>       // OpenGL Utility library - helper functions
#include <GL/glut.h>      // OpenGL Utility Toolkit - window management

/*
EXPLANATION OF HEADER FILES:
----------------------------
1. windows.h  : Required for Windows-specific operations
2. gl.h       : Contains OpenGL functions like glBegin(), glEnd(), glVertex()
3. glu.h      : Contains utility functions like gluOrtho2D()
4. glut.h     : Contains window functions like glutInit(), glutCreateWindow()
*/


// INITIALIZATION FUNCTION
// ============================================================================
void myInit()
{
    /*
    PURPOSE: Initialize OpenGL settings before drawing
    CALLED: Once at program start from main()
    */
    
    // SET BACKGROUND COLOR
    // ------------------------------------------------------------------------
//    glClearColor(1.0, 1.0, 1.0, 1.0);
		glClearColor(0,0.5, 0.5, 0.5);

    
    /*
    glClearColor(Red, Green, Blue, Alpha)
    - Values range from 0.0 to 1.0
    - (1.0, 1.0, 1.0, 1.0) = White background
    - (0.0, 0.0, 0.0, 1.0) = Black background
    - (1.0, 0.0, 0.0, 1.0) = Red background
    
    COMMON COLORS:
    White:  (1.0, 1.0, 1.0)
    Black:  (0.0, 0.0, 0.0)
    Red:    (1.0, 0.0, 0.0)
    Green:  (0.0, 1.0, 0.0)
    Blue:   (0.0, 0.0, 1.0)
    Yellow: (1.0, 1.0, 0.0)
    Cyan:   (0.0, 1.0, 1.0)
    Magenta:(1.0, 0.0, 1.0)
    Gray:   (0.5, 0.5, 0.5)
    */
    
    // SET DRAWING COLOR
    // ------------------------------------------------------------------------
    glColor3f(0.0f, 0.0f, 0.0f);
    /*
    glColor3f(Red, Green, Blue)
    - Sets the color for drawing objects
    - (0.0, 0.0, 0.0) = Black (default drawing color)
    - This color will be used for all shapes until changed
    */
    
    // SET POINT SIZE
    // ------------------------------------------------------------------------
    glPointSize(4.0);
    /*
    glPointSize(size)
    - Sets the diameter of points in pixels
    - Default is 1.0
    - Try values: 1.0, 2.0, 4.0, 8.0, 10.0
    - Larger values = bigger points
    */
    
    // SET LINE WIDTH (optional - not in original code)
    // ------------------------------------------------------------------------
    glLineWidth(2.0);
    /*
    glLineWidth(width)
    - Sets the width of lines in pixels
    - Default is 1.0
    - Try values: 1.0, 2.0, 3.0, 5.0
    */
    
    // SET PROJECTION MODE
    // ------------------------------------------------------------------------
    glMatrixMode(GL_PROJECTION);
    /*
    glMatrixMode(mode)
    - Specifies which matrix stack is the target
    - GL_PROJECTION: For setting up the coordinate system
    - GL_MODELVIEW: For transformations (we'll use this later)
    
    Think of it as: "I want to work with the projection settings now"
    */
    
    // LOAD IDENTITY MATRIX
    // ------------------------------------------------------------------------
    glLoadIdentity();
    /*
    glLoadIdentity()
    - Resets the current matrix to identity matrix
    - Clears any previous transformations
    - Always call this before setting up new coordinates
    - Think of it as: "Start fresh with default settings"
    */
    
    // SET 2D ORTHOGRAPHIC PROJECTION
    // ------------------------------------------------------------------------
    gluOrtho2D(0.0, 640.0, 0.0, 480.0);
    /*
    gluOrtho2D(left, right, bottom, top)
    - Sets up a 2D orthographic viewing region
    - Defines the coordinate system for your drawings
    
    PARAMETERS:
    - left:   0.0   = Left edge of window (X minimum)
    - right:  640.0 = Right edge of window (X maximum)
    - bottom: 0.0   = Bottom edge of window (Y minimum)
    - top:    480.0 = Top edge of window (Y maximum)
    
    COORDINATE SYSTEM:
         Y (480)
         ^
         |
         |
    (0,0)|-----------> X (640)
    
    EXAMPLES:
    - Point at (0, 0) is at bottom-left corner
    - Point at (640, 480) is at top-right corner
    - Point at (320, 240) is at center
    - Point at (100, 100) is near bottom-left
    */
}


// DISPLAY FUNCTION
// ============================================================================
void myDisplay(void)
{
    /*
    PURPOSE: This function is called whenever the window needs to be redrawn
    CALLED: Automatically by OpenGL when window is shown, resized, or refreshed
    */
    
    // CLEAR THE WINDOW
    // ------------------------------------------------------------------------
    glClear(GL_COLOR_BUFFER_BIT);
    /*
    glClear(buffer)
    - Clears the specified buffer
    - GL_COLOR_BUFFER_BIT: Clears the color buffer (screen)
    - Must be called at the start of every frame
    - Without this, old drawings remain on screen
    */
    
    
    // ========================================================================
    // DRAWING WITH GL_POINTS PRIMITIVE
    // ========================================================================
    /*
    GL_POINTS: Each vertex is drawn as a single point
    - Size controlled by glPointSize()
    - Each glVertex draws one point
    */
    
    glBegin(GL_POINTS);
    /*
    glBegin(primitive_type)
    - Marks the beginning of vertex data
    - primitive_type specifies how to interpret vertices
    - Must be paired with glEnd()
    */
    
        // Draw 4 points to form corners of a rectangle
        glVertex2i(200, 100);  // Bottom-left corner
        glVertex2i(400, 100);  // Bottom-right corner
        glVertex2i(400, 250);  // Top-right corner
        glVertex2i(200, 250);  // Top-left corner
        /*
        glVertex2i(x, y)
        - Specifies a 2D vertex (point) with integer coordinates
        - x: horizontal position (left to right)
        - y: vertical position (bottom to top)
        
        VARIANTS:
        - glVertex2i(x, y): 2D integer coordinates
        - glVertex2f(x, y): 2D floating-point coordinates
        - glVertex3f(x, y, z): 3D floating-point coordinates
        
        TIP: Use 'i' for integers, 'f' for floats
        */
        
    glEnd();
    /*
    glEnd()
    - Marks the end of vertex data
    - Completes the primitive started by glBegin()
    - Always pair with glBegin()
    */
    
    
    // ========================================================================
    // UNDERSTANDING PRIMITIVES
    // ========================================================================
    /*
    OpenGL has several primitive types. Here are the main ones:
    
    1. GL_POINTS
       - Each vertex is a point
       - Example: 4 vertices = 4 points
    
    2. GL_LINES
       - Every 2 vertices form a line segment
       - Example: 4 vertices = 2 separate lines
    
    3. GL_LINE_STRIP
       - Connected line segments
       - Example: 4 vertices = 3 connected lines
    
    4. GL_LINE_LOOP
       - Connected line segments forming a closed loop
       - Example: 4 vertices = 4 connected lines (last connects to first)
    
    5. GL_TRIANGLES
       - Every 3 vertices form a triangle
       - Example: 6 vertices = 2 triangles
    
    6. GL_TRIANGLE_STRIP
       - Connected triangles sharing vertices
    
    7. GL_TRIANGLE_FAN
       - Triangles sharing a common vertex
    
    8. GL_QUADS
       - Every 4 vertices form a quadrilateral (4-sided polygon)
       - Example: 4 vertices = 1 quad (filled rectangle)
    
    9. GL_POLYGON
       - A single polygon with any number of vertices
       - Vertices should be in order (clockwise or counter-clockwise)
    */
    
    
    // OPTIONAL: Drawing different primitives (commented out)
    // ------------------------------------------------------------------------
    
    // EXAMPLE 1: Draw a line strip (uncomment to test)
    /*
    glColor3f(1.0, 0.0, 0.0);  // Red color
    glBegin(GL_LINE_STRIP);
        glVertex2i(200, 100);
        glVertex2i(400, 100);
        glVertex2i(400, 250);
        glVertex2i(200, 250);
    glEnd();
    */
    
    // EXAMPLE 2: Draw a closed loop (uncomment to test)
    /*
    glColor3f(0.0, 0.0, 1.0);  // Blue color
    glBegin(GL_LINE_LOOP);
        glVertex2i(200, 100);
        glVertex2i(400, 100);
        glVertex2i(400, 250);
        glVertex2i(200, 250);
    glEnd();
    */
    
    // EXAMPLE 3: Draw a filled rectangle using GL_QUADS (uncomment to test)
    /*
    glColor3f(0.0, 1.0, 0.0);  // Green color
    glBegin(GL_QUADS);
        glVertex2i(200, 100);
        glVertex2i(400, 100);
        glVertex2i(400, 250);
        glVertex2i(200, 250);
    glEnd();
    */
    
    // EXAMPLE 4: Draw a triangle (uncomment to test)
    /*
    glColor3f(1.0, 1.0, 0.0);  // Yellow color
    glBegin(GL_TRIANGLES);
        glVertex2i(300, 300);  // Top vertex
        glVertex2i(200, 200);  // Bottom-left vertex
        glVertex2i(400, 200);  // Bottom-right vertex
    glEnd();
    */
    
    
    // FLUSH OUTPUT BUFFER
    // ------------------------------------------------------------------------
    glFlush();
    /*
    glFlush()
    - Forces execution of OpenGL commands
    - Ensures all drawing commands are processed
    - Must be called at the end of display function
    - Without this, drawings may not appear
    */
}


// MAIN FUNCTION
// ============================================================================
int main(int argc, char** argv)
{
    /*
    PURPOSE: Entry point of the program
    PARAMETERS:
    - argc: number of command-line arguments
    - argv: array of command-line argument strings
    */
    
    // INITIALIZE GLUT
    // ------------------------------------------------------------------------
    glutInit(&argc, argv);
    /*
    glutInit(&argc, argv)
    - Initializes the GLUT library
    - Must be called before any other GLUT functions
    - Processes command-line arguments
    */
    
    // SET DISPLAY MODE
    // ------------------------------------------------------------------------
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    /*
    glutInitDisplayMode(mode)
    - Specifies the display mode for the window
    
    PARAMETERS:
    - GLUT_SINGLE: Single buffering (simpler, may flicker)
    - GLUT_DOUBLE: Double buffering (smoother, no flicker)
    - GLUT_RGB: RGB color mode (Red-Green-Blue)
    - GLUT_RGBA: RGB + Alpha (transparency)
    
    Use | (OR operator) to combine modes
    
    COMMON COMBINATIONS:
    - GLUT_SINGLE | GLUT_RGB: Simple programs
    - GLUT_DOUBLE | GLUT_RGB: Animations
    - GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH: 3D programs
    */
    
    // SET WINDOW SIZE
    // ------------------------------------------------------------------------
    glutInitWindowSize(640, 480);
    /*
    glutInitWindowSize(width, height)
    - Sets the initial size of the window in pixels
    - width: 640 pixels wide
    - height: 480 pixels high
    
    COMMON SIZES:
    - 640 x 480 (VGA - standard)
    - 800 x 600
    - 1024 x 768
    - 1280 x 720 (HD)
    - 1920 x 1080 (Full HD)
    */
    
    // SET WINDOW POSITION
    // ------------------------------------------------------------------------
    glutInitWindowPosition(100, 150);
    /*
    glutInitWindowPosition(x, y)
    - Sets the initial position of the window on screen
    - x: 100 pixels from left edge of screen
    - y: 150 pixels from top edge of screen
    
    NOTE: (0, 0) is the top-left corner of the screen
    
    EXAMPLES:
    - (0, 0): Top-left corner
    - (100, 100): Slightly offset from top-left
    - (500, 300): Centered on most screens
    */
    
    // CREATE WINDOW
    // ------------------------------------------------------------------------
    glutCreateWindow("LAB 01 - OpenGL Primitives Demo");
    /*
    glutCreateWindow(title)
    - Creates a window with the specified title
    - Title appears in the window's title bar
    - Returns a window identifier (not used in simple programs)
    
    TIP: Use descriptive titles to identify your program
    */
    
    // REGISTER DISPLAY CALLBACK
    // ------------------------------------------------------------------------
    glutDisplayFunc(myDisplay);
    /*
    glutDisplayFunc(function_name)
    - Registers a function to be called when window needs redrawing
    - The function must have signature: void function(void)
    - In this case, myDisplay() will be called
    - Called automatically when window is shown or refreshed
    */
    
    // INITIALIZE OPENGL SETTINGS
    // ------------------------------------------------------------------------
    myInit();
    /*
    myInit()
    - Calls our custom initialization function
    - Sets up colors, coordinate system, etc.
    - Must be called before entering main loop
    */
    
    // ENTER MAIN EVENT LOOP
    // ------------------------------------------------------------------------
    glutMainLoop();
    /*
    glutMainLoop()
    - Enters the GLUT event processing loop
    - Never returns (infinite loop)
    - Keeps the window open and responsive
    - Processes events (mouse, keyboard, redraw, etc.)
    - This must be the last GLUT call in main()
    
    IMPORTANT: No code after glutMainLoop() will execute!
    */
}


/*
================================================================================
COMMON ERRORS AND SOLUTIONS
================================================================================

1. ERROR: "Cannot find -lopengl32"
   SOLUTION: Install freeglut and add linker flags in Dev-C++ settings

2. ERROR: "freeglut.dll is missing"
   SOLUTION: Copy freeglut.dll to System32 or place in same folder as .exe

3. ERROR: "GL/glut.h: No such file or directory"
   SOLUTION: Copy GL header files to Dev-Cpp/MinGW64/include/GL/

4. ERROR: Window opens but nothing is drawn
   SOLUTION: Check if glFlush() is called at end of display function

5. ERROR: Window closes immediately
   SOLUTION: Make sure glutMainLoop() is called in main()

6. ERROR: Shapes appear in wrong position
   SOLUTION: Check gluOrtho2D() coordinates match your window size

================================================================================
TIPS FOR BEGINNERS
================================================================================

1. COORDINATE SYSTEM:
   - Origin (0,0) is at BOTTOM-LEFT corner
   - X increases to the RIGHT
   - Y increases UPWARD
   - This is different from screen coordinates!

2. COLOR RANGES:
   - All color values are between 0.0 and 1.0
   - 0.0 = none of that color
   - 1.0 = full intensity of that color

3. DRAWING ORDER:
   - Always: glClear() ? glBegin() ? glVertex() ? glEnd() ? glFlush()
   - Objects drawn later appear on top

4. TESTING PRIMITIVES:
   - Uncomment the example code sections above
   - Try changing colors with glColor3f()
   - Experiment with different vertex positions

5. WINDOW COORDINATES:
   - gluOrtho2D(0, 640, 0, 480) means:
     • X goes from 0 to 640
     • Y goes from 0 to 480
   - Center of window is at (320, 240)

================================================================================
EXERCISES TO TRY
================================================================================

1. Change background color to light blue: glClearColor(0.7, 0.9, 1.0, 1.0)

2. Change point size to 10: glPointSize(10.0)

3. Draw points in a diagonal line from (100,100) to (500,400)

4. Change GL_POINTS to GL_LINE_LOOP to see the rectangle outline

5. Add a triangle above the rectangle using GL_TRIANGLES

6. Draw your initials using GL_LINE_STRIP

7. Create a house shape using GL_QUADS and GL_TRIANGLES

8. Draw the flag of Pakistan using colored shapes

*/
