/*
================================================================================
    LAB 04: KEYBOARD AND MOUSE INTERACTION
    Computer Graphics Course - Bahria University Karachi
================================================================================

OBJECTIVES:
    1. Interact with keyboard using OpenGL/GLUT
    2. Interact with mouse using OpenGL/GLUT
    3. Respond to user input in real-time

NEW CONCEPTS IN THIS LAB:
    - Event-driven programming
    - Callback functions
    - glutKeyboardFunc() - keyboard input
    - glutMouseFunc() - mouse button clicks
    - glutMotionFunc() - mouse drag (optional)
    - glutPostRedisplay() - request screen redraw
    - Global variables for state management
    - Mouse coordinate system conversion

================================================================================
WHAT IS EVENT-DRIVEN PROGRAMMING?
================================================================================

TRADITIONAL PROGRAMMING:
    Your program runs line by line from top to bottom.
    You control the flow with loops and conditionals.

EVENT-DRIVEN PROGRAMMING:
    Your program sits idle, WAITING for events (user input, timer, etc.)
    When an event occurs, your CALLBACK function is called.
    After callback finishes, program goes back to waiting.

GLUT EVENT LOOP:
    glutMainLoop() sits in an infinite loop:
        1. Check for events (keyboard, mouse, window resize, etc.)
        2. If event occurs, call the appropriate callback function
        3. If screen needs redrawing, call display function
        4. Go back to step 1

YOUR ROLE:
    Register callback functions that tell GLUT what to do when events happen.

================================================================================
THE CALLBACK FUNCTIONS
================================================================================

DISPLAY CALLBACK:
    void myDisplay(void)
    Registered with: glutDisplayFunc(myDisplay)
    Called when: Window needs redrawing (initially, after resize, after glutPostRedisplay)

KEYBOARD CALLBACK:
    void myKeyboard(unsigned char key, int x, int y)
    Registered with: glutKeyboardFunc(myKeyboard)
    Called when: User presses a key
    Parameters:
        key - ASCII value of the pressed key ('a', 'b', '1', ' ', etc.)
        x, y - mouse position when key was pressed

MOUSE CALLBACK:
    void myMouse(int button, int state, int x, int y)
    Registered with: glutMouseFunc(myMouse)
    Called when: User clicks/releases a mouse button
    Parameters:
        button - which button (GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON, GLUT_MIDDLE_BUTTON)
        state  - button state (GLUT_DOWN for press, GLUT_UP for release)
        x, y   - mouse position in window coordinates

MOTION CALLBACK (optional):
    void myMotion(int x, int y)
    Registered with: glutMotionFunc(myMotion)
    Called when: Mouse is moved WHILE a button is held down (dragging)

PASSIVE MOTION CALLBACK (optional):
    void myPassiveMotion(int x, int y)
    Registered with: glutPassiveMotionFunc(myPassiveMotion)
    Called when: Mouse is moved WITHOUT any button pressed

================================================================================
*/

// HEADER FILES
// ============================================================================
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <cstdlib>  // exit()

using namespace std;


// GLOBAL VARIABLES (for state management)
// ============================================================================
/*
WHY GLOBAL VARIABLES?
    Callbacks cannot pass custom data between each other.
    We need to REMEMBER state between events.
    Example: User presses 'r' ? remember current color is red
             Display function needs to know what color to use

COMMON STATE VARIABLES:
    - Current drawing color
    - Points clicked by user
    - Whether animation is running
    - Current mode/tool selected
*/

// Drawing color (RGB)
float currentColorR = 0.0f;
float currentColorG = 0.0f;
float currentColorB = 0.0f;

// Window dimensions (needed for coordinate conversion)
int windowWidth = 640;
int windowHeight = 480;

// Status message
const char* statusMessage = "Ready. Press keys: R=red, G=green, B=blue, W=white, Q=quit";


// INITIALIZATION
// ============================================================================
void myInit()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);  // White background
    glPointSize(4.0);
    glLineWidth(2.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, windowWidth, 0.0, windowHeight);
}


// DISPLAY FUNCTION
// ============================================================================
void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Set current color
    glColor3f(currentColorR, currentColorG, currentColorB);
    
    // Draw a simple shape to show the current color
    glBegin(GL_QUADS);
        glVertex2i(250, 200);
        glVertex2i(390, 200);
        glVertex2i(390, 280);
        glVertex2i(250, 280);
    glEnd();
    
    // Draw instructions (as colored rectangles for visual feedback)
    // In a real app, you'd use text rendering
    
    glFlush();
}


// KEYBOARD CALLBACK
// ============================================================================
void myKeyboard(unsigned char key, int x, int y)
{
    /*
    PARAMETERS:
        key - The ASCII value of the key pressed
              Examples: 'a' = 97, 'A' = 65, '1' = 49, ' ' = 32, Enter = 13
        x   - Mouse X position when key was pressed (pixels from LEFT)
        y   - Mouse Y position when key was pressed (pixels from TOP)
    
    IMPORTANT: y is measured from TOP, not bottom!
               This is different from OpenGL's coordinate system.
    */
    
    cout << "Key pressed: " << key << " (ASCII: " << (int)key << ")";
    cout << " at position (" << x << ", " << y << ")" << endl;
    
    // Convert key to lowercase for case-insensitive comparison
    // (so both 'r' and 'R' trigger red)
    if (key >= 'A' && key <= 'Z') {
        key = key + 32;  // Convert to lowercase (A=65 ? a=97)
    }
    
    switch (key)
    {
        case 'r':  // Red
            currentColorR = 1.0f;
            currentColorG = 0.0f;
            currentColorB = 0.0f;
            statusMessage = "Color: RED";
            cout << "Color changed to RED" << endl;
            break;
            
        case 'g':  // Green
            currentColorR = 0.0f;
            currentColorG = 1.0f;
            currentColorB = 0.0f;
            statusMessage = "Color: GREEN";
            cout << "Color changed to GREEN" << endl;
            break;
            
        case 'b':  // Blue
            currentColorR = 0.0f;
            currentColorG = 0.0f;
            currentColorB = 1.0f;
            statusMessage = "Color: BLUE";
            cout << "Color changed to BLUE" << endl;
            break;
            
        case 'y':  // Yellow
            currentColorR = 1.0f;
            currentColorG = 1.0f;
            currentColorB = 0.0f;
            statusMessage = "Color: YELLOW";
            cout << "Color changed to YELLOW" << endl;
            break;
            
        case 'w':  // White
            currentColorR = 1.0f;
            currentColorG = 1.0f;
            currentColorB = 1.0f;
            statusMessage = "Color: WHITE";
            cout << "Color changed to WHITE" << endl;
            break;
            
        case 'k':  // blacK (can't use 'b' twice)
            currentColorR = 0.0f;
            currentColorG = 0.0f;
            currentColorB = 0.0f;
            statusMessage = "Color: BLACK";
            cout << "Color changed to BLACK" << endl;
            break;
            
        case 'q':  // Quit
        case 'Q':  // Also quit on capital Q
        case 27:   // ESC key (ASCII 27)
            cout << "Exiting program..." << endl;
            exit(0);  // Terminate program
            break;
            
        case ' ':  // Spacebar - reset to default
            currentColorR = 0.5f;
            currentColorG = 0.5f;
            currentColorB = 0.5f;
            statusMessage = "Color: GRAY (default)";
            cout << "Color reset to GRAY" << endl;
            break;
            
        default:
            cout << "Unrecognized key. Press R, G, B, Y, W, K, or Q" << endl;
            break;
    }
    
    // REQUEST REDRAW
    glutPostRedisplay();
    /*
    glutPostRedisplay()
    - Tells GLUT: "screen needs updating"
    - GLUT will call myDisplay() soon (not immediately!)
    - CRITICAL: Without this, your changes won't show on screen
    
    Think of it as: "Hey GLUT, I changed something, please redraw when you can"
    */
}


// MOUSE CALLBACK
// ============================================================================
void myMouse(int button, int state, int x, int y)
{
    /*
    PARAMETERS:
        button - Which button:
                 GLUT_LEFT_BUTTON   (left mouse button)
                 GLUT_RIGHT_BUTTON  (right mouse button)
                 GLUT_MIDDLE_BUTTON (middle button/scroll wheel click)
        
        state  - Button state:
                 GLUT_DOWN (button just pressed)
                 GLUT_UP   (button just released)
        
        x      - Mouse X position in WINDOW coordinates (pixels from LEFT)
        y      - Mouse Y position in WINDOW coordinates (pixels from TOP)
    
    CRITICAL COORDINATE CONVERSION:
        GLUT gives y from TOP    (y=0 at top edge)
        OpenGL uses y from BOTTOM (y=0 at bottom edge)
        
        Conversion: opengl_y = windowHeight - glut_y
    */
    
    // Convert y coordinate from window space to OpenGL space
    int openglY = windowHeight - y;
    
    cout << "Mouse button " << button << " " 
         << (state == GLUT_DOWN ? "pressed" : "released")
         << " at window coords (" << x << ", " << y << ")"
         << " ? OpenGL coords (" << x << ", " << openglY << ")" << endl;
    
    // Only respond to button presses (not releases)
    if (state == GLUT_DOWN)
    {
        switch (button)
        {
            case GLUT_LEFT_BUTTON:
                // Example: cycle through colors on left click
                static int colorIndex = 0;
                colorIndex = (colorIndex + 1) % 6;
                
                switch (colorIndex) {
                    case 0: currentColorR = 1.0f; currentColorG = 0.0f; currentColorB = 0.0f; break; // Red
                    case 1: currentColorR = 0.0f; currentColorG = 1.0f; currentColorB = 0.0f; break; // Green
                    case 2: currentColorR = 0.0f; currentColorG = 0.0f; currentColorB = 1.0f; break; // Blue
                    case 3: currentColorR = 1.0f; currentColorG = 1.0f; currentColorB = 0.0f; break; // Yellow
                    case 4: currentColorR = 1.0f; currentColorG = 0.0f; currentColorB = 1.0f; break; // Magenta
                    case 5: currentColorR = 0.0f; currentColorG = 1.0f; currentColorB = 1.0f; break; // Cyan
                }
                
                cout << "Left click: Color changed" << endl;
                glutPostRedisplay();
                break;
                
            case GLUT_RIGHT_BUTTON:
                // Example: exit on right click
                cout << "Right click: Exiting program" << endl;
                exit(0);
                break;
                
            case GLUT_MIDDLE_BUTTON:
                // Example: reset on middle click
                cout << "Middle click: Reset color to black" << endl;
                currentColorR = currentColorG = currentColorB = 0.0f;
                glutPostRedisplay();
                break;
        }
    }
}


// MOUSE MOTION CALLBACK (optional, for dragging)
// ============================================================================
void myMotion(int x, int y)
{
    /*
    Called when mouse moves WHILE a button is held down (dragging).
    Useful for: drawing, selecting, dragging objects
    
    NOTE: Can generate MANY events per second (every pixel moved)
          Be careful with expensive operations here!
    */
    
    int openglY = windowHeight - y;
    
    // Example: print coordinates (comment out in real use - too much output!)
    // cout << "Mouse dragged to (" << x << ", " << openglY << ")" << endl;
    
    // Example use: update something based on mouse position
    // ...
    // glutPostRedisplay();
}


// PASSIVE MOTION CALLBACK (optional, for hovering)
// ============================================================================
void myPassiveMotion(int x, int y)
{
    /*
    Called when mouse moves WITHOUT any button pressed (hovering).
    Useful for: cursor effects, hover highlights, tooltips
    
    NOTE: Also generates MANY events! Use carefully.
    */
    
    int openglY = windowHeight - y;
    
    // Example: change color based on mouse X position
    // currentColorR = (float)x / windowWidth;  // More red on right side
    // glutPostRedisplay();
}


// MAIN FUNCTION
// ============================================================================
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("LAB 04 - Keyboard & Mouse Interaction Demo");
    
    myInit();
    
    // Register callback functions
    glutDisplayFunc(myDisplay);
    /*
    REQUIRED: Every program needs a display function
    */
    
    glutKeyboardFunc(myKeyboard);
    /*
    KEYBOARD CALLBACK
    - Called when user presses a key
    - Handles printable ASCII characters: 'a'-'z', '0'-'9', ' ', etc.
    - Does NOT handle: arrow keys, F1-F12, Shift/Ctrl/Alt alone
      (Use glutSpecialFunc for those)
    */
    
    glutMouseFunc(myMouse);
    /*
    MOUSE BUTTON CALLBACK
    - Called when user clicks/releases mouse button
    - Handles left, right, and middle buttons
    - Gets button, state, and position
    */
    
    // Optional: Register motion callbacks
    // glutMotionFunc(myMotion);              // Dragging (button held)
    // glutPassiveMotionFunc(myPassiveMotion); // Hovering (no button)
    
    cout << "\n=== LAB 04 DEMO - Keyboard & Mouse Interaction ===" << endl;
    cout << "Keyboard commands:" << endl;
    cout << "  R - Red" << endl;
    cout << "  G - Green" << endl;
    cout << "  B - Blue" << endl;
    cout << "  Y - Yellow" << endl;
    cout << "  W - White" << endl;
    cout << "  K - Black" << endl;
    cout << "  Q or ESC - Quit" << endl;
    cout << "  Spacebar - Reset to gray" << endl;
    cout << "\nMouse commands:" << endl;
    cout << "  Left click - Cycle through colors" << endl;
    cout << "  Right click - Exit" << endl;
    cout << "  Middle click - Reset to black" << endl;
    cout << "\nWatch the console for event messages!" << endl;
    cout << "==================================================\n" << endl;
    
    glutMainLoop();
}


/*
================================================================================
UNDERSTANDING glutPostRedisplay()
================================================================================

WHY WE NEED IT:
    After changing global state (like color), the screen doesn't automatically
    update. We must TELL GLUT to redraw the screen.

WHAT IT DOES:
    Marks the window as needing redraw. GLUT will call myDisplay() soon.

WHEN TO CALL IT:
    ? After changing any visual state (colors, positions, etc.)
    ? In keyboard callback (after changing something)
    ? In mouse callback (after changing something)
    ? After any event that affects what's on screen

WHEN NOT TO CALL IT:
    ? In display function itself (causes infinite loop!)
    ? If you didn't change anything
    ? In tight loops (causes flicker)

ALTERNATIVE: glutSwapBuffers() for double buffering
    Use with GLUT_DOUBLE instead of GLUT_SINGLE


================================================================================
MOUSE COORDINATE CONVERSION
================================================================================

THE PROBLEM:
    GLUT gives mouse Y from TOP (y=0 at top)
    OpenGL uses Y from BOTTOM (y=0 at bottom)
    
    Window (GLUT):          OpenGL:
    (0,0)----? X           Y (480) ?
      ¦                            |
      ?                    (0,0)--------? X (640)
      Y (480)

SOLUTION:
    openGL_y = windowHeight - glut_y

EXAMPLE:
    Window: 640×480
    Mouse clicked at window coords (100, 50) - near top-left
    OpenGL coords: (100, 430) - near top-left in OpenGL space
    
    Mouse clicked at window coords (100, 450) - near bottom-left
    OpenGL coords: (100, 30) - near bottom-left in OpenGL space

CODE:
    void myMouse(int button, int state, int x, int y)
    {
        int openglY = windowHeight - y;
        // Now use (x, openglY) for drawing
    }


================================================================================
HANDLING SPECIAL KEYS
================================================================================

glutKeyboardFunc() handles regular keys ('a', '1', space, etc.)

For ARROW KEYS, FUNCTION KEYS, etc., use glutSpecialFunc():

void mySpecialKeys(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_UP:
            // Up arrow pressed
            break;
        case GLUT_KEY_DOWN:
            // Down arrow pressed
            break;
        case GLUT_KEY_LEFT:
            // Left arrow pressed
            break;
        case GLUT_KEY_RIGHT:
            // Right arrow pressed
            break;
        case GLUT_KEY_F1:
            // F1 key pressed
            break;
        // ... GLUT_KEY_F2 through GLUT_KEY_F12
        case GLUT_KEY_PAGE_UP:
        case GLUT_KEY_PAGE_DOWN:
        case GLUT_KEY_HOME:
        case GLUT_KEY_END:
        case GLUT_KEY_INSERT:
            // ...
            break;
    }
    glutPostRedisplay();
}

// Register it:
glutSpecialFunc(mySpecialKeys);


================================================================================
COMMON PATTERNS
================================================================================

PATTERN 1: TOGGLE STATE
------------------------
static bool showGrid = false;

void myKeyboard(unsigned char key, int x, int y)
{
    if (key == 'g' || key == 'G') {
        showGrid = !showGrid;  // Toggle
        glutPostRedisplay();
    }
}

void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    // ... draw main content ...
    if (showGrid) {
        DrawGrid();  // Only draw if enabled
    }
    glFlush();
}


PATTERN 2: MODE SELECTION
--------------------------
enum DrawMode { MODE_DRAW, MODE_ERASE, MODE_SELECT };
DrawMode currentMode = MODE_DRAW;

void myKeyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 'd': currentMode = MODE_DRAW; break;
        case 'e': currentMode = MODE_ERASE; break;
        case 's': currentMode = MODE_SELECT; break;
    }
    glutPostRedisplay();
}


PATTERN 3: ACCUMULATING CLICKS
-------------------------------
#include <vector>

struct Point {
    int x, y;
};

vector<Point> clickedPoints;

void myMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        Point p;
        p.x = x;
        p.y = windowHeight - y;  // Convert to OpenGL coords
        clickedPoints.push_back(p);
        glutPostRedisplay();
    }
}

void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
    for (int i = 0; i < clickedPoints.size(); i++) {
        glVertex2i(clickedPoints[i].x, clickedPoints[i].y);
    }
    glEnd();
    glFlush();
}


================================================================================
NEXT STEPS
================================================================================

Now you're ready for the exercises:

Exercise 1: Sierpinski Gasket with keyboard and mouse control
Exercise 2: Draw dots on mouse click
Exercise 3: Interactive Sierpinski (user picks 3 corners)

See Lab04_All_Exercise_Solutions.cpp for complete implementations!

================================================================================
*/
