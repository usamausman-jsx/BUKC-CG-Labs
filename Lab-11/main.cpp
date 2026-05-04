/*
 * ============================================================
 *  CSL 444 - Computer Graphics Lab
 *  Lab 11: Camera Settings in OpenGL
 *  Semester 06 (Spring 2026) | Bahria University
 *  Lecturer: Ghazala Shafi | Lab Engineer: Mr. M. Osama
 * ============================================================
 *
 *  HOW TO COMPILE IN DEV C++:
 *  -----------------------------------------------------------
 *  1. Open Dev-C++
 *  2. Go to:  Tools > Compiler Options > Linker
 *  3. Add these libraries (in "Add these commands to linker"):
 *       -lopengl32 -lglu32 -lglut32   (or -lfreeglut)
 *  4. Press Compile & Run (F11)
 *
 *  MAKE SURE: freeglut.dll is in the same folder as your .exe
 *  OR install freeglut via Dev-C++ package manager.
 * ============================================================
 *
 *  WHAT THIS PROGRAM TEACHES:
 *  - Setting up a 3D scene with OpenGL
 *  - Using gluLookAt() to position and aim a camera
 *  - Using gluPerspective() for realistic 3D projection
 *  - Using glOrtho() for flat/technical projection
 *  - Drawing simple 3D shapes (cube)
 *  - Rotating objects with keyboard input
 *
 *  KEYBOARD CONTROLS (once the window opens):
 *  - LEFT / RIGHT arrow : rotate cube horizontally
 *  - UP / DOWN arrow    : rotate cube vertically
 *  - P : switch to Perspective projection
 *  - O : switch to Orthographic projection
 *  - ESC : exit
 * ============================================================
 */

// -------------------------------------------------------
// HEADERS
// Include the necessary OpenGL and GLUT libraries
// -------------------------------------------------------
#include <GL/glut.h>    // GLUT for window creation and drawing utilities
#include <GL/glu.h>     // GLU for gluLookAt and gluPerspective
#include <GL/gl.h>      // Core OpenGL functions

// -------------------------------------------------------
// GLOBAL VARIABLES
// These store state that multiple functions need to share
// -------------------------------------------------------
float rotX = 20.0f;    // Rotation angle around X-axis (up/down tilt of cube)
float rotY = 30.0f;    // Rotation angle around Y-axis (left/right spin of cube)

// Camera position (eye point) — where you are standing
float eyeX = 3.0f, eyeY = 3.0f, eyeZ = 5.0f;

// Camera target (look-at point) — what you are looking at
float centerX = 0.0f, centerY = 0.0f, centerZ = 0.0f;

// Flag to toggle between perspective and orthographic
// true  = perspective  (realistic, objects get smaller far away)
// false = orthographic (flat/technical, no depth distortion)
bool usePerspective = true;

// Window dimensions (used for aspect ratio)
int windowWidth  = 800;
int windowHeight = 600;


// ============================================================
//  FUNCTION: drawColoredCube()
//  Draws a cube where each face has a different color.
//  This helps you visually see all 6 faces clearly.
// ============================================================
void drawColoredCube() {

    glBegin(GL_QUADS);  // Draw filled quadrilaterals (4-sided polygons)

    // ---- FRONT FACE (facing you, z = +0.5) ----
    glColor3f(1.0f, 0.0f, 0.0f);   // Red
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f,  0.5f,  0.5f);
    glVertex3f(-0.5f,  0.5f,  0.5f);

    // ---- BACK FACE (facing away, z = -0.5) ----
    glColor3f(0.0f, 1.0f, 0.0f);   // Green
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f,  0.5f, -0.5f);
    glVertex3f( 0.5f,  0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f, -0.5f);

    // ---- TOP FACE (y = +0.5) ----
    glColor3f(0.0f, 0.0f, 1.0f);   // Blue
    glVertex3f(-0.5f,  0.5f, -0.5f);
    glVertex3f(-0.5f,  0.5f,  0.5f);
    glVertex3f( 0.5f,  0.5f,  0.5f);
    glVertex3f( 0.5f,  0.5f, -0.5f);

    // ---- BOTTOM FACE (y = -0.5) ----
    glColor3f(1.0f, 1.0f, 0.0f);   // Yellow
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f,  0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);

    // ---- RIGHT FACE (x = +0.5) ----
    glColor3f(1.0f, 0.0f, 1.0f);   // Magenta
    glVertex3f( 0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f,  0.5f, -0.5f);
    glVertex3f( 0.5f,  0.5f,  0.5f);
    glVertex3f( 0.5f, -0.5f,  0.5f);

    // ---- LEFT FACE (x = -0.5) ----
    glColor3f(0.0f, 1.0f, 1.0f);   // Cyan
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glVertex3f(-0.5f,  0.5f,  0.5f);
    glVertex3f(-0.5f,  0.5f, -0.5f);

    glEnd();  // Done drawing quads

    // ---- DRAW EDGES (wireframe outline) ----
    // This makes the cube edges visible as black lines
    glColor3f(0.0f, 0.0f, 0.0f);   // Black edges
    glutWireCube(1.01);             // Slightly larger than the solid cube so edges show
}


// ============================================================
//  FUNCTION: setupCamera()
//  Sets up WHICH kind of projection we use (Perspective or Ortho)
//  and positions the camera using gluLookAt().
//
//  This is called every frame (inside display()) and also
//  when the window is resized (inside reshape()).
// ============================================================
void setupCamera() {

    // -------------------------------------------------------
    //  STEP 1: Switch to the PROJECTION matrix
    //  The projection matrix controls HOW the 3D scene
    //  is flattened into a 2D image on screen.
    // -------------------------------------------------------
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();   // Reset to no projection (blank slate)

    if (usePerspective) {
        // ===================================================
        //  PERSPECTIVE PROJECTION — gluPerspective()
        //  Objects farther away appear SMALLER (realistic)
        //
        //  gluPerspective(fovy, aspect, near, far)
        //
        //  fovy   = Field of View (angle in degrees, Y direction)
        //           Typical values: 45 to 90 degrees
        //           SMALLER fovy = zoomed in (telephoto lens)
        //           LARGER  fovy = wide angle (fish-eye effect)
        //           Try: 30.0, 45.0, 60.0, 90.0, 120.0
        //
        //  aspect = width / height of the viewport
        //           This prevents stretching when window is resized
        //
        //  near   = distance to NEAR clipping plane
        //           Objects CLOSER than this are NOT drawn
        //           Must be > 0.0 (never use 0!)
        //           Typical: 0.1
        //
        //  far    = distance to FAR clipping plane
        //           Objects FARTHER than this are NOT drawn
        //           Typical: 100.0 or 1000.0
        //
        //  IMPORTANT: Only objects between near and far are visible!
        // ===================================================
        float fovy   = 45.0f;   // <-- CHANGE THIS to see the effect (try 30, 60, 90, 120)
        float aspect = (float)windowWidth / (float)windowHeight;
        float near_z = 0.1f;    // near clipping distance
        float far_z  = 100.0f;  // far clipping distance

        gluPerspective(fovy, aspect, near_z, far_z);

    } else {
        // ===================================================
        //  ORTHOGRAPHIC PROJECTION — glOrtho()
        //  Objects at any distance appear the SAME SIZE (flat)
        //  Used for: blueprints, 2D games, technical drawings
        //
        //  glOrtho(left, right, bottom, top, near, far)
        //
        //  left/right = X bounds of the visible area
        //  bottom/top = Y bounds of the visible area
        //  near/far   = Z depth range (can be negative)
        //
        //  Example: glOrtho(-3, 3, -3, 3, -10, 10)
        //  shows a 6x6 unit box centered at origin
        // ===================================================
        float aspect = (float)windowWidth / (float)windowHeight;
        glOrtho(-3.0 * aspect, 3.0 * aspect,   // left, right
                -3.0,          3.0,              // bottom, top
                -10.0,         10.0);            // near, far
    }

    // -------------------------------------------------------
    //  STEP 2: Switch to the MODELVIEW matrix
    //  The modelview matrix controls WHERE the camera is
    //  and WHERE/HOW objects are placed in the world.
    // -------------------------------------------------------
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();   // Reset to identity (no camera movement yet)

    // ===================================================
    //  SET CAMERA POSITION — gluLookAt()
    //
    //  gluLookAt(eyeX, eyeY, eyeZ,          // Camera position
    //            centerX, centerY, centerZ,  // What to look at
    //            upX, upY, upZ)              // "Up" direction
    //
    //  eyeX/Y/Z    : WHERE the camera is located in 3D space
    //                Think of it as where YOU are standing
    //                Try: (5,5,5), (0,0,5), (3,0,0)
    //
    //  centerX/Y/Z : The POINT the camera is aimed at
    //                Usually the origin (0,0,0) or the object
    //
    //  upX/Y/Z     : Which direction is "up" for the camera
    //                (0,1,0) means Y-axis is up — the standard
    //                (0,0,1) would tilt the camera sideways
    //
    //  How gluLookAt works internally:
    //    n = eye - center  (forward direction, reversed)
    //    u = up  x  n      (right direction)
    //    v = n   x  u      (true up direction)
    //  These three vectors form the camera's coordinate system.
    // ===================================================
    gluLookAt(
        eyeX, eyeY, eyeZ,           // Camera position
        centerX, centerY, centerZ,  // Look-at target
        0.0f, 1.0f, 0.0f            // Up vector (Y is up)
    );
}


// ============================================================
//  FUNCTION: display()
//  This is the MAIN DRAWING FUNCTION.
//  OpenGL calls this every time it needs to redraw the screen.
// ============================================================
void display() {

    // Clear the screen and depth buffer before drawing
    // GL_COLOR_BUFFER_BIT : clears the color (pixels)
    // GL_DEPTH_BUFFER_BIT : clears the depth info (for 3D overlap)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up camera and projection for this frame
    setupCamera();

    // -------------------------------------------------------
    //  APPLY ROTATION to the cube
    //  glRotatef(angle, x, y, z) — rotate by 'angle' degrees
    //  around the axis (x, y, z)
    //
    //  We rotate BEFORE drawing so the cube appears rotated
    // -------------------------------------------------------
    glRotatef(rotX, 1.0f, 0.0f, 0.0f);  // Tilt up/down (around X-axis)
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);  // Spin left/right (around Y-axis)

    // Draw the cube at the origin (0,0,0)
    drawColoredCube();

    // Display a label in the window title based on projection mode
    if (usePerspective) {
        glutSetWindowTitle("Lab 11: Camera in OpenGL | PERSPECTIVE | Arrow keys rotate | P/O to switch");
    } else {
        glutSetWindowTitle("Lab 11: Camera in OpenGL | ORTHOGRAPHIC | Arrow keys rotate | P/O to switch");
    }

    // Swap the back buffer to screen (double buffering)
    // This prevents flickering by drawing off-screen first
    glutSwapBuffers();
}


// ============================================================
//  FUNCTION: reshape()
//  Called automatically whenever the WINDOW IS RESIZED.
//  We update the viewport and aspect ratio here.
// ============================================================
void reshape(int w, int h) {
    windowWidth  = w;
    windowHeight = (h == 0) ? 1 : h;  // Avoid divide-by-zero

    // glViewport tells OpenGL which part of the window to use
    // (0, 0, width, height) = use the entire window
    glViewport(0, 0, windowWidth, windowHeight);
}


// ============================================================
//  FUNCTION: specialKeys()
//  Handles ARROW KEY input for rotating the cube.
//  "Special" keys in GLUT are non-ASCII (arrows, F-keys, etc.)
// ============================================================
void specialKeys(int key, int x, int y) {
    float step = 5.0f;  // Degrees to rotate per key press

    if      (key == GLUT_KEY_LEFT)  rotY -= step;  // Rotate left
    else if (key == GLUT_KEY_RIGHT) rotY += step;  // Rotate right
    else if (key == GLUT_KEY_UP)    rotX -= step;  // Tilt up
    else if (key == GLUT_KEY_DOWN)  rotX += step;  // Tilt down

    glutPostRedisplay();  // Request a redraw of the window
}


// ============================================================
//  FUNCTION: keyboard()
//  Handles regular keyboard input.
// ============================================================
void keyboard(unsigned char key, int x, int y) {
    if (key == 'p' || key == 'P') {
        usePerspective = true;   // Switch to perspective
        glutPostRedisplay();
    }
    else if (key == 'o' || key == 'O') {
        usePerspective = false;  // Switch to orthographic
        glutPostRedisplay();
    }
    else if (key == 27) {        // 27 = ESC key
        exit(0);                 // Close the program
    }
}


// ============================================================
//  FUNCTION: init()
//  One-time setup called before the main loop starts.
// ============================================================
void init() {
    // Set background color to dark gray (R, G, B, Alpha)
    // Values range from 0.0 (dark) to 1.0 (bright)
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);

    // Enable DEPTH TESTING
    // Without this, faces drawn later overwrite earlier ones
    // regardless of which is in front — causing visual glitches
    glEnable(GL_DEPTH_TEST);

    // Enable smooth shading (default, makes color gradients smooth)
    glShadeModel(GL_SMOOTH);
}


// ============================================================
//  FUNCTION: main()
//  Entry point. Sets up the GLUT window and starts the loop.
// ============================================================
int main(int argc, char** argv) {

    // Initialize GLUT with command-line arguments
    glutInit(&argc, argv);

    // Set display mode:
    // GLUT_DOUBLE  = double buffering (no flicker)
    // GLUT_RGB     = color mode
    // GLUT_DEPTH   = depth buffer (for 3D depth sorting)
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Set initial window size (width x height in pixels)
    glutInitWindowSize(windowWidth, windowHeight);

    // Set window position on screen (pixels from top-left)
    glutInitWindowPosition(100, 100);

    // Create the window with a title
    glutCreateWindow("Lab 11: Camera Settings in OpenGL");

    // Register callback functions:
    // display()     = called every time screen needs redrawing
    // reshape()     = called when window is resized
    // keyboard()    = called for regular key presses
    // specialKeys() = called for arrow/function keys
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    // Run one-time initialization
    init();

    // Start the GLUT event loop — this runs forever
    // until the window is closed or exit() is called
    glutMainLoop();

    return 0;
}

/*
 * ============================================================
 *  QUICK REFERENCE: KEY VALUES TO EXPERIMENT WITH
 * ============================================================
 *
 *  gluLookAt — Camera Positions to Try:
 *    (3, 3, 5)   = standard angled view (default)
 *    (0, 0, 5)   = straight-on front view
 *    (5, 0, 0)   = side view (looking along X)
 *    (0, 5, 0)   = top-down bird's eye view
 *    (3, 3, -5)  = behind the object
 *
 *  gluPerspective — Field of View (fovy) to Try:
 *    30.0  = narrow/zoomed in (telephoto)
 *    45.0  = standard (default)
 *    60.0  = normal wide
 *    90.0  = very wide angle
 *    120.0 = fish-eye distortion
 *
 *  glOrtho — Box Size to Try:
 *    (-3, 3, -3, 3, ...)  = standard (default)
 *    (-1, 1, -1, 1, ...)  = zoomed in
 *    (-5, 5, -5, 5, ...)  = zoomed out
 * ============================================================
 */
