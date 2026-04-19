/*
================================================================================
    LAB 08: HIERARCHICAL MODELING
    Computer Graphics Course - Bahria University Karachi
================================================================================

OBJECTIVES:
    1. Apply transformations to animate objects
    2. Use glPushMatrix() and glPopMatrix() for hierarchy
    3. Understand relative vs absolute transformations
    4. Create parent-child relationships between objects

NEW CONCEPTS IN THIS LAB:
    - Hierarchical modeling (parent-child relationships)
    - Relative transformations (child inherits parent's transform)
    - Animation using glutTimerFunc()
    - Compound objects (windmill, cart)
    - Transformation inheritance

================================================================================
WHAT IS HIERARCHICAL MODELING?
================================================================================

HIERARCHY = Parent-child relationships between objects

REAL-WORLD EXAMPLE: Human Arm
    Shoulder (parent)
      └─> Upper Arm
            └─> Elbow (joint)
                  └─> Lower Arm
                        └─> Wrist
                              └─> Hand
                                    └─> Fingers

WHEN YOU MOVE THE SHOULDER:
    - Entire arm moves (all children inherit the movement)
    
WHEN YOU BEND THE ELBOW:
    - Only lower arm moves (children of elbow)
    - Upper arm stays put (parent and siblings unaffected)


WINDMILL EXAMPLE (This Lab):
    Stand (parent)
      └─> Fan (child, rotates relative to stand)
            └─> Blades (rotate with fan)

When stand moves → entire windmill moves
When fan rotates → only fan rotates (stand stays fixed)

================================================================================
ABSOLUTE vs RELATIVE TRANSFORMATIONS
================================================================================

ABSOLUTE TRANSFORMATION:
    Each object positioned independently relative to world origin
    
    glPushMatrix();
        glTranslatef(100, 100, 0);  // Arm at (100, 100)
        DrawArm();
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(150, 100, 0);  // Hand at (150, 100)
        DrawHand();
    glPopMatrix();
    
    Problem: Hand doesn't follow arm when arm moves!


RELATIVE TRANSFORMATION:
    Child positioned relative to parent
    
    glPushMatrix();
        glTranslatef(100, 100, 0);  // Arm at (100, 100)
        DrawArm();
        
        glPushMatrix();
            glTranslatef(50, 0, 0);  // Hand 50 units from arm
            DrawHand();              // Hand is at (150, 100) in world
        glPopMatrix();
    glPopMatrix();
    
    Advantage: Hand automatically follows arm!

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

// Animation variables
float fanAngle = 0.0f;      // Windmill fan rotation
float cartX = -300.0f;      // Cart position
bool animating = false;     // Animation on/off


// WINDMILL COMPONENTS
// ============================================================================

void DrawStand()
{
    /*
    Windmill stand - vertical pole
    Drawn centered at origin, extends upward
    */
    glColor3f(0.5, 0.35, 0.2);  // Brown
    glBegin(GL_QUADS);
        glVertex2f(-10, 0);      // Bottom-left
        glVertex2f( 10, 0);      // Bottom-right
        glVertex2f( 10, 100);    // Top-right
        glVertex2f(-10, 100);    // Top-left
    glEnd();
}

void DrawFan()
{
    /*
    Windmill fan - four blades arranged in cross pattern
    Drawn centered at origin
    Will be rotated to animate
    */
    
    // Center hub
    glColor3f(0.3, 0.3, 0.3);  // Dark gray
    glBegin(GL_POLYGON);
        for (int i = 0; i < 20; i++)
        {
            float angle = 2.0f * PI * i / 20.0f;
            glVertex2f(10 * cos(angle), 10 * sin(angle));
        }
    glEnd();
    
    // Four blades
    glColor3f(0.8, 0.8, 0.9);  // Light blue-gray
    
    // Blade 1 (top)
    glBegin(GL_TRIANGLES);
        glVertex2f(-5, 10);
        glVertex2f( 5, 10);
        glVertex2f( 0, 60);
    glEnd();
    
    // Blade 2 (right)
    glBegin(GL_TRIANGLES);
        glVertex2f(10, -5);
        glVertex2f(10,  5);
        glVertex2f(60,  0);
    glEnd();
    
    // Blade 3 (bottom)
    glBegin(GL_TRIANGLES);
        glVertex2f(-5, -10);
        glVertex2f( 5, -10);
        glVertex2f( 0, -60);
    glEnd();
    
    // Blade 4 (left)
    glBegin(GL_TRIANGLES);
        glVertex2f(-10, -5);
        glVertex2f(-10,  5);
        glVertex2f(-60,  0);
    glEnd();
}

void DrawWindmill(float x, float y)
{
    /*
    Complete windmill at position (x, y)
    
    HIERARCHY:
        Stand (base)
          └─> Fan (rotates on top of stand)
    
    The fan INHERITS the stand's position
    Fan rotation is RELATIVE to stand position
    */
    
    glPushMatrix();
        // Position entire windmill
        glTranslatef(x, y, 0);
        
        // Draw stand
        DrawStand();
        
        // Draw fan (at top of stand, rotating)
        glPushMatrix();
            glTranslatef(0, 100, 0);        // Position at top of stand
            glRotatef(fanAngle, 0, 0, 1);   // Rotate (animated)
            DrawFan();
        glPopMatrix();
        
    glPopMatrix();
}


// CART COMPONENTS
// ============================================================================

void DrawWheel(float radius)
{
    /*
    Cart wheel - circle with spokes
    Drawn at origin
    */
    
    // Outer circle
    glColor3f(0.2, 0.2, 0.2);  // Dark gray
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 20; i++)
        {
            float angle = 2.0f * PI * i / 20.0f;
            glVertex2f(radius * cos(angle), radius * sin(angle));
        }
    glEnd();
    
    // Spokes (8 lines from center to edge)
    glBegin(GL_LINES);
        for (int i = 0; i < 8; i++)
        {
            float angle = 2.0f * PI * i / 8.0f;
            glVertex2f(0, 0);  // Center
            glVertex2f(radius * cos(angle), radius * sin(angle));  // Edge
        }
    glEnd();
}

void DrawCartBody()
{
    /*
    Cart body - rectangular box
    Drawn centered at origin
    */
    glColor3f(0.6, 0.3, 0.1);  // Brown
    glBegin(GL_QUADS);
        glVertex2f(-40, 0);
        glVertex2f( 40, 0);
        glVertex2f( 40, 30);
        glVertex2f(-40, 30);
    glEnd();
    
    // Front and back panels
    glColor3f(0.5, 0.25, 0.1);  // Darker brown
    glBegin(GL_QUADS);
        glVertex2f(-40, 0);
        glVertex2f(-40, 30);
        glVertex2f(-35, 35);
        glVertex2f(-35, 5);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2f(40, 0);
        glVertex2f(40, 30);
        glVertex2f(35, 35);
        glVertex2f(35, 5);
    glEnd();
}

void DrawCart(float x, float y)
{
    /*
    Complete cart at position (x, y)
    
    HIERARCHY:
        Cart Body
          ├─> Left Wheel
          └─> Right Wheel
    
    Wheels INHERIT body's position and movement
    */
    
    glPushMatrix();
        // Position entire cart
        glTranslatef(x, y, 0);
        
        // Draw body
        DrawCartBody();
        
        // Draw left wheel
        glPushMatrix();
            glTranslatef(-25, 0, 0);  // Position relative to body
            DrawWheel(15);
        glPopMatrix();
        
        // Draw right wheel
        glPushMatrix();
            glTranslatef(25, 0, 0);   // Position relative to body
            DrawWheel(15);
        glPopMatrix();
        
    glPopMatrix();
}


// INITIALIZATION
// ============================================================================
void myInit()
{
    glClearColor(0.5, 0.7, 1.0, 1.0);  // Sky blue
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(2.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-400.0, 400.0, -300.0, 300.0);
}


// DISPLAY FUNCTION
// ============================================================================
void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw ground
    glColor3f(0.4, 0.7, 0.3);  // Green
    glBegin(GL_QUADS);
        glVertex2f(-400, -300);
        glVertex2f( 400, -300);
        glVertex2f( 400, -200);
        glVertex2f(-400, -200);
    glEnd();
    
    // Draw windmill
    DrawWindmill(-200, -200);
    
    // Draw cart
    DrawCart(cartX, -185);
    
    glFlush();
}


// ANIMATION TIMER
// ============================================================================
void myTimer(int value)
{
    /*
    Called periodically to update animation
    
    glutTimerFunc(milliseconds, function, value)
    - Calls function after specified milliseconds
    - Must re-register itself to continue animation
    */
    
    if (animating)
    {
        // Update windmill fan rotation
        fanAngle += 5.0f;
        if (fanAngle >= 360.0f)
            fanAngle -= 360.0f;
        
        // Update cart position
        cartX += 2.0f;
        
        // Boundary check - wrap around
        if (cartX > 450.0f)
            cartX = -450.0f;
        
        glutPostRedisplay();  // Request redraw
    }
    
    // Re-register timer (30ms = ~33 FPS)
    glutTimerFunc(30, myTimer, 0);
}


// KEYBOARD CALLBACK
// ============================================================================
void myKeyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 's':
        case 'S':
            animating = !animating;  // Toggle animation
            cout << "Animation: " << (animating ? "ON" : "OFF") << endl;
            break;
            
        case 'q':
        case 'Q':
        case 27:  // ESC
            exit(0);
            break;
    }
}


// MAIN FUNCTION
// ============================================================================
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("LAB 08 - Hierarchical Modeling Demo");
    glutDisplayFunc(myDisplay);
    glutKeyboardFunc(myKeyboard);
    glutTimerFunc(30, myTimer, 0);  // Start timer
    myInit();
    
    cout << "\n=== LAB 08 DEMO - Hierarchical Modeling ===" << endl;
    cout << "Press 'S' to start/stop animation" << endl;
    cout << "Press 'Q' or ESC to quit" << endl;
    cout << "\nWatch how:" << endl;
    cout << "  - Windmill fan rotates on top of stand" << endl;
    cout << "  - Cart wheels move with cart body" << endl;
    cout << "  - This is HIERARCHICAL MODELING!" << endl;
    cout << "==========================================\n" << endl;
    
    glutMainLoop();
}


/*
================================================================================
HIERARCHICAL MODELING EXPLAINED
================================================================================

WINDMILL HIERARCHY:
───────────────────

DrawWindmill(x, y)
    glPushMatrix()
        glTranslatef(x, y, 0)     ← Position entire windmill
        
        DrawStand()               ← Draw at windmill position
        
        glPushMatrix()
            glTranslatef(0, 100, 0)   ← Relative to stand base
            glRotatef(fanAngle, ...)  ← Rotate fan
            DrawFan()                 ← Fan inherits stand position + top offset + rotation
        glPopMatrix()
        
    glPopMatrix()


TRANSFORMATION INHERITANCE:
──────────────────────────

Level 1: glTranslatef(x, y, 0)
    → Stand drawn at (x, y)

Level 2: glTranslatef(0, 100, 0)  [inside Level 1]
    → Fan center at (x, y+100)  [inherits parent's translation!]

Level 3: glRotatef(fanAngle, ...)  [inside Level 2]
    → Fan rotates around (x, y+100)  [inherits position, adds rotation]


WHY PUSH/POP?
─────────────

glPushMatrix() - "Remember current transformation state"
glPopMatrix()  - "Restore remembered state"

Without them:
    glTranslatef(100, 0, 0)
    DrawObject1()              ← at x=100
    glTranslatef(50, 0, 0)
    DrawObject2()              ← at x=150 (100+50, accumulated!)

With them:
    glPushMatrix()
        glTranslatef(100, 0, 0)
        DrawObject1()          ← at x=100
    glPopMatrix()
    glPushMatrix()
        glTranslatef(50, 0, 0)
        DrawObject2()          ← at x=50 (independent!)
    glPopMatrix()


ANIMATION WITH TIMER:
─────────────────────

glutTimerFunc(milliseconds, callback, value)
    - Calls callback after specified time
    - NOT a loop - called once
    - Must re-register to continue

Pattern:
    void myTimer(int value)
    {
        // Update animation variables
        angle += 5.0f;
        
        glutPostRedisplay();  // Trigger redraw
        
        // Re-register for next frame
        glutTimerFunc(30, myTimer, 0);
    }

Frame rate calculation:
    30ms = 1000/30 = ~33 FPS
    16ms = 1000/16 = ~60 FPS


BOUNDARY CHECKING:
──────────────────

Wrap-around (cart reappears on other side):
    if (cartX > rightBoundary)
        cartX = leftBoundary;

Bounce (reverse direction):
    if (cartX > rightBoundary)
    {
        cartX = rightBoundary;
        cartDirection = -cartDirection;
    }

Stop at boundary:
    if (cartX > rightBoundary)
        cartX = rightBoundary;

================================================================================
COMMON HIERARCHICAL PATTERNS
================================================================================

PATTERN 1: Simple Parent-Child
    glPushMatrix()
        glTranslatef(parentX, parentY, 0)
        DrawParent()
        
        glPushMatrix()
            glTranslatef(childOffsetX, childOffsetY, 0)
            DrawChild()
        glPopMatrix()
    glPopMatrix()


PATTERN 2: Rotating Child
    glPushMatrix()
        glTranslatef(parentX, parentY, 0)
        DrawParent()
        
        glPushMatrix()
            glTranslatef(jointX, jointY, 0)  // Joint position
            glRotatef(angle, 0, 0, 1)        // Rotation
            DrawChild()
        glPopMatrix()
    glPopMatrix()


PATTERN 3: Multiple Children
    glPushMatrix()
        glTranslatef(parentX, parentY, 0)
        DrawParent()
        
        glPushMatrix()
            glTranslatef(child1X, child1Y, 0)
            DrawChild1()
        glPopMatrix()
        
        glPushMatrix()
            glTranslatef(child2X, child2Y, 0)
            DrawChild2()
        glPopMatrix()
    glPopMatrix()


PATTERN 4: Deep Hierarchy (Grandchildren)
    glPushMatrix()
        glTranslatef(parentX, parentY, 0)
        DrawParent()
        
        glPushMatrix()
            glTranslatef(childX, childY, 0)
            DrawChild()
            
            glPushMatrix()
                glTranslatef(grandchildX, grandchildY, 0)
                DrawGrandchild()
            glPopMatrix()
        glPopMatrix()
    glPopMatrix()

================================================================================
*/
