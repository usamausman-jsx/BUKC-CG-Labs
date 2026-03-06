/*
================================================================================
    LAB 07: TRANSFORMATIONS
    Computer Graphics Course - Bahria University Karachi
================================================================================

OBJECTIVES:
    1. Understand transformation matrices
    2. Apply basic transformations in OpenGL (translate, rotate, scale)
    3. Combine multiple transformations
    4. Use glPushMatrix() and glPopMatrix()

NEW CONCEPTS IN THIS LAB:
    - Geometric transformations
    - Transformation matrices (4x4 homogeneous)
    - Translation: glTranslatef(dx, dy, dz)
    - Rotation: glRotatef(angle, x, y, z)
    - Scaling: glScalef(sx, sy, sz)
    - Matrix stack (push/pop)
    - Transformation order matters!
    - Model coordinate system

================================================================================
WHAT ARE TRANSFORMATIONS?
================================================================================

TRANSFORMATION = Mathematical operation that changes position, size, or 
                 orientation of an object

THREE BASIC TRANSFORMATIONS:
    1. TRANSLATION - Move (change position)
    2. ROTATION - Turn (change orientation)
    3. SCALING - Resize (change size)

REAL-WORLD ANALOGY:
    Translation = Sliding a book across the table
    Rotation    = Spinning a book around
    Scaling     = Zooming in/out (making bigger/smaller)


WHY DO WE NEED THEM?
    - Animate objects (move, spin, grow)
    - Position objects in scene
    - Create complex shapes from simple ones
    - Camera movement (translation + rotation)
    - Hierarchical modeling (robot arms, solar system)

================================================================================
TRANSFORMATION MATRICES
================================================================================

OpenGL uses 4×4 MATRICES for transformations (homogeneous coordinates)

TRANSLATION MATRIX:
    +              +
    ¦ 1  0  0  dx ¦     dx, dy, dz = translation distances
    ¦ 0  1  0  dy ¦
    ¦ 0  0  1  dz ¦
    ¦ 0  0  0  1  ¦
    +              +

SCALING MATRIX:
    +              +
    ¦ sx 0  0  0  ¦     sx, sy, sz = scale factors
    ¦ 0  sy 0  0  ¦     (1.0 = no change)
    ¦ 0  0  sz 0  ¦
    ¦ 0  0  0  1  ¦
    +              +

ROTATION MATRIX (2D, around Z-axis):
    +                    +
    ¦ cos(?)  -sin(?)  0  0 ¦     ? = rotation angle
    ¦ sin(?)   cos(?)  0  0 ¦
    ¦   0        0     1  0 ¦
    ¦   0        0     0  1 ¦
    +                    +

Note: OpenGL does the matrix math internally!
You just call glTranslatef(), glRotatef(), glScalef()

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


// HELPER: Draw a simple square (unit square at origin)
// ============================================================================
void DrawSquare()
{
    /*
    Unit square: 1×1, centered at origin
    Corners: (-0.5, -0.5) to (0.5, 0.5)
    
    WHY START AT ORIGIN?
    - Easy to transform
    - Symmetric transformations work naturally
    - Standard practice in graphics
    */
    glBegin(GL_QUADS);
        glVertex2f(-0.5, -0.5);  // Bottom-left
        glVertex2f( 0.5, -0.5);  // Bottom-right
        glVertex2f( 0.5,  0.5);  // Top-right
        glVertex2f(-0.5,  0.5);  // Top-left
    glEnd();
}


// INITIALIZATION
// ============================================================================
void myInit()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);  // White background
    glColor3f(0.0, 0.0, 0.0);           // Black objects
    glPointSize(5.0);
    glLineWidth(2.0);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-10.0, 10.0, -10.0, 10.0);  // World coordinates: -10 to 10
    /*
    NOTE: Using -10 to 10 (instead of 0 to 640)
    This gives us a coordinate system with origin at CENTER
    Makes transformations more intuitive!
    */
}


// DISPLAY FUNCTION - DEMO: All Three Basic Transformations
// ============================================================================
void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw axes for reference
    glColor3f(0.7, 0.7, 0.7);  // Gray
    glBegin(GL_LINES);
        glVertex2f(-10, 0); glVertex2f(10, 0);  // X-axis
        glVertex2f(0, -10); glVertex2f(0, 10);  // Y-axis
    glEnd();
    
    
    // ========================================================================
    // EXAMPLE 1: TRANSLATION
    // ========================================================================
    /*
    glTranslatef(dx, dy, dz)
    - Moves object by (dx, dy, dz)
    - In 2D, we usually use dz = 0
    
    Effect: All vertices shifted by the translation vector
    */
    
    glColor3f(1.0, 0.0, 0.0);  // Red
    glPushMatrix();  // Save current matrix
        glTranslatef(3.0, 2.0, 0.0);  // Move right 3, up 2
        DrawSquare();
    glPopMatrix();   // Restore matrix
    /*
    glPushMatrix() and glPopMatrix():
    - Push saves the current transformation matrix
    - Pop restores it
    - This isolates transformations to specific objects
    
    Without push/pop, transformations would ACCUMULATE!
    */
    
    
    // ========================================================================
    // EXAMPLE 2: ROTATION
    // ========================================================================
    /*
    glRotatef(angle, x, y, z)
    - angle: rotation angle in DEGREES
    - (x, y, z): axis of rotation
    
    For 2D rotation (around Z-axis):
        glRotatef(angle, 0, 0, 1)
    
    Rotates COUNTER-CLOCKWISE around the axis
    */
    
    glColor3f(0.0, 0.0, 1.0);  // Blue
    glPushMatrix();
        glTranslatef(-3.0, 2.0, 0.0);  // Position it
        glRotatef(45.0, 0.0, 0.0, 1.0);  // Rotate 45° around Z
        DrawSquare();
    glPopMatrix();
    
    
    // ========================================================================
    // EXAMPLE 3: SCALING
    // ========================================================================
    /*
    glScalef(sx, sy, sz)
    - sx, sy, sz: scale factors for each axis
    
    Examples:
        glScalef(2.0, 2.0, 1.0)  ? Double size
        glScalef(0.5, 0.5, 1.0)  ? Half size
        glScalef(2.0, 1.0, 1.0)  ? Stretch horizontally
        glScalef(1.0, 0.5, 1.0)  ? Compress vertically
    */
    
    glColor3f(0.0, 1.0, 0.0);  // Green
    glPushMatrix();
        glTranslatef(0.0, -3.0, 0.0);  // Position it
        glScalef(2.0, 0.5, 1.0);  // Wide and short
        DrawSquare();
    glPopMatrix();
    
    
    // ========================================================================
    // EXAMPLE 4: COMBINED TRANSFORMATIONS
    // ========================================================================
    /*
    CRITICAL: ORDER MATTERS!
    
    Transformations are applied in REVERSE order from how they appear in code:
        glTranslatef(...);
        glRotatef(...);
        DrawSquare();
    
    Actual order: Rotate first, THEN translate
    
    Think of it as: "First rotate the square, then move the rotated square"
    */
    
    glColor3f(1.0, 0.0, 1.0);  // Magenta
    glPushMatrix();
        glTranslatef(3.0, -3.0, 0.0);  // Applied SECOND (final position)
        glRotatef(30.0, 0.0, 0.0, 1.0);  // Applied FIRST (rotation)
        glScalef(1.5, 1.5, 1.0);  // Applied BEFORE rotation
        DrawSquare();
    glPopMatrix();
    
    
    // ========================================================================
    // EXAMPLE 5: Original square (for comparison)
    // ========================================================================
    glColor3f(0.5, 0.5, 0.5);  // Gray
    glBegin(GL_LINE_LOOP);
        glVertex2f(-0.5, -0.5);
        glVertex2f( 0.5, -0.5);
        glVertex2f( 0.5,  0.5);
        glVertex2f(-0.5,  0.5);
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
    glutCreateWindow("LAB 07 - Transformations Demo");
    glutDisplayFunc(myDisplay);
    myInit();
    
    cout << "\n=== LAB 07 DEMO - Transformations ===" << endl;
    cout << "Gray outline = Original square at origin" << endl;
    cout << "Red    = Translated (3, 2)" << endl;
    cout << "Blue   = Translated + Rotated 45°" << endl;
    cout << "Green  = Translated + Scaled (2x, 0.5y)" << endl;
    cout << "Magenta= Translated + Rotated + Scaled (combined)" << endl;
    cout << "======================================\n" << endl;
    
    glutMainLoop();
}


/*
================================================================================
UNDERSTANDING glPushMatrix() and glPopMatrix()
================================================================================

THE MATRIX STACK:
    OpenGL maintains a STACK of transformation matrices
    
    Like a stack of plates:
        - Push adds a plate on top (saves current state)
        - Pop removes top plate (restores previous state)


WHY USE IT?
    Without push/pop, transformations ACCUMULATE:
    
    glTranslatef(2, 0, 0);
    DrawSquare();           // Square at x=2
    glTranslatef(2, 0, 0);
    DrawSquare();           // Square at x=4 (2+2, accumulated!)
    
    With push/pop, transformations are ISOLATED:
    
    glPushMatrix();
        glTranslatef(2, 0, 0);
        DrawSquare();       // Square at x=2
    glPopMatrix();
    glPushMatrix();
        glTranslatef(2, 0, 0);
        DrawSquare();       // Square at x=2 (NOT accumulated)
    glPopMatrix();


COMMON PATTERN:
    glPushMatrix();
        // Apply transformations
        // Draw object
    glPopMatrix();
    
    This creates a "local" transformation that doesn't affect other objects.


HIERARCHICAL TRANSFORMATIONS (Lab 8 preview):
    glPushMatrix();
        glTranslatef(5, 0, 0);  // Position parent
        DrawBody();
        
        glPushMatrix();
            glTranslatef(1, 0, 0);  // Position relative to parent
            glRotatef(angle, 0, 0, 1);
            DrawArm();
        glPopMatrix();
    glPopMatrix();
    
    The arm inherits the parent's translation!

================================================================================
TRANSFORMATION ORDER - THE CRITICAL CONCEPT
================================================================================

CODE:
    glTranslatef(5, 0, 0);
    glRotatef(45, 0, 0, 1);
    DrawSquare();

ACTUAL APPLICATION ORDER (read bottom to top):
    1. DrawSquare() - square at origin
    2. glRotatef() - rotate the square 45°
    3. glTranslatef() - move the rotated square to (5, 0)

Think of it as reading from the object outward:
    "Take a square, rotate it, then move it"


DIFFERENT ORDER = DIFFERENT RESULT:
    
    Version A:
        glTranslatef(5, 0, 0);  // Second
        glRotatef(45, 0, 0, 1);  // First
        DrawSquare();
    Result: Square rotated at origin, then moved right
    
    Version B:
        glRotatef(45, 0, 0, 1);  // Second
        glTranslatef(5, 0, 0);  // First
        DrawSquare();
    Result: Square moved right, then rotated around origin (orbits!)


MNEMONIC:
    "First come, last served"
    - First transformation in code is applied LAST to object
    - Last transformation in code is applied FIRST to object

================================================================================
TRANSLATION DETAILS
================================================================================

glTranslatef(dx, dy, dz)

PARAMETERS:
    dx = distance to move along X-axis
    dy = distance to move along Y-axis
    dz = distance to move along Z-axis (0 for 2D)

EXAMPLES:
    glTranslatef(2, 3, 0)   ? Move right 2, up 3
    glTranslatef(-1, 0, 0)  ? Move left 1
    glTranslatef(0, -2, 0)  ? Move down 2

EFFECT:
    All vertices of the object are shifted by (dx, dy, dz)
    
    If square has vertex at (0.5, 0.5)
    After glTranslatef(3, 2, 0)
    Vertex is at (3.5, 2.5)

================================================================================
ROTATION DETAILS
================================================================================

glRotatef(angle, x, y, z)

PARAMETERS:
    angle = rotation angle in DEGREES (not radians!)
    (x, y, z) = axis of rotation (unit vector)

COMMON ROTATIONS:
    glRotatef(angle, 0, 0, 1)  ? Rotate around Z-axis (2D rotation)
    glRotatef(angle, 1, 0, 0)  ? Rotate around X-axis (pitch)
    glRotatef(angle, 0, 1, 0)  ? Rotate around Y-axis (yaw)

DIRECTION:
    Positive angle = counter-clockwise (when looking down the axis)
    Negative angle = clockwise

ROTATION CENTER:
    Rotation happens around the ORIGIN!
    To rotate around a different point:
        1. Translate to origin
        2. Rotate
        3. Translate back
    
    Example (rotate around point (5, 3)):
        glTranslatef(5, 3, 0);      // Move center to final position
        glRotatef(45, 0, 0, 1);     // Rotate around origin
        glTranslatef(-5, -3, 0);    // Move center to origin first
        DrawObject();

================================================================================
SCALING DETAILS
================================================================================

glScalef(sx, sy, sz)

PARAMETERS:
    sx = scale factor for X-axis
    sy = scale factor for Y-axis
    sz = scale factor for Z-axis

EXAMPLES:
    glScalef(2, 2, 1)    ? Double size (uniform)
    glScalef(0.5, 0.5, 1) ? Half size
    glScalef(2, 1, 1)    ? Stretch horizontally
    glScalef(1, 0.5, 1)  ? Compress vertically
    glScalef(-1, 1, 1)   ? Mirror horizontally (flip)

EFFECT:
    Each vertex coordinate is multiplied by the scale factor
    
    If vertex at (2, 3)
    After glScalef(2, 0.5, 1)
    Vertex at (4, 1.5)

SCALING CENTER:
    Like rotation, scaling happens from the ORIGIN
    To scale around a different point:
        glTranslatef(cx, cy, 0);     // Move center to final position
        glScalef(sx, sy, 1);         // Scale
        glTranslatef(-cx, -cy, 0);   // Move center to origin
        DrawObject();

================================================================================
COMMON TRANSFORMATION PATTERNS
================================================================================

PATTERN 1: Position an object
    glPushMatrix();
        glTranslatef(x, y, 0);
        DrawObject();
    glPopMatrix();

PATTERN 2: Rotate an object at a position
    glPushMatrix();
        glTranslatef(x, y, 0);       // Position first
        glRotatef(angle, 0, 0, 1);   // Then rotate
        DrawObject();
    glPopMatrix();

PATTERN 3: Scale and position
    glPushMatrix();
        glTranslatef(x, y, 0);       // Position
        glScalef(sx, sy, 1);         // Scale
        DrawObject();
    glPopMatrix();

PATTERN 4: Full transformation
    glPushMatrix();
        glTranslatef(x, y, 0);       // Position (last)
        glRotatef(angle, 0, 0, 1);   // Rotate (middle)
        glScalef(sx, sy, 1);         // Scale (first)
        DrawObject();
    glPopMatrix();
    
    Remember: TRS = Translate Rotate Scale (in code)
              Applied as: SRT (Scale, Rotate, Translate)

================================================================================
*/
