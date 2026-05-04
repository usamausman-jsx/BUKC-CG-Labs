/*
================================================================================
    LAB 12: CAMERA FLYBY OPERATIONS
    Computer Graphics Course - Bahria University Karachi
================================================================================

OBJECTIVES:
    1. Implement camera flyby operations
    2. Understand pitch, yaw, and roll rotations
    3. Interactive camera control

NEW CONCEPTS:
    - Pitch: Rotation around X-axis (look up/down)
    - Yaw: Rotation around Y-axis (look left/right)
    - Roll: Rotation around Z-axis (tilt head left/right)
    - Camera orientation using angles
    - First-person camera control

================================================================================
AIRCRAFT ROTATIONS (CAMERA ANALOGY)
================================================================================

Think of the camera as an airplane:

PITCH:
    - Nose up/down
    - Rotation around X-axis (horizontal, left-right)
    - Look up at sky or down at ground
    - Angle between viewing direction and horizontal plane

YAW:
    - Turn left/right
    - Rotation around Y-axis (vertical, up-down)
    - Change direction of flight
    - Compass heading

ROLL:
    - Wing tip up/down
    - Rotation around Z-axis (forward-backward)
    - Bank/tilt to the side
    - Horizon tilts

Visual:
         Pitch (up/down)
              ?
              ¦
    Yaw ?-----?-----? Yaw
   (left)     ¦      (right)
              ?
         Pitch (down)
         
    Roll (tilt): Entire view rotates clockwise/counter-clockwise

================================================================================
*/

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>

using namespace std;

#define PI 3.14159265358979323846

int windowWidth = 800;
int windowHeight = 600;

// Camera position
float cameraX = 0.0f;
float cameraY = 2.0f;
float cameraZ = 5.0f;

// Camera orientation (in degrees)
float pitch = 0.0f;  // Up/down rotation
float yaw = 0.0f;    // Left/right rotation
float roll = 0.0f;   // Tilt rotation

// Camera look direction (calculated from pitch/yaw)
float lookX, lookY, lookZ;

// Movement speed
float moveSpeed = 0.2f;
float rotateSpeed = 2.0f;


// CALCULATE LOOK DIRECTION FROM ANGLES
// ============================================================================
void UpdateCameraVectors()
{
    /*
    Convert pitch and yaw angles to look-at point
    
    Spherical to Cartesian coordinates:
    x = r * cos(pitch) * sin(yaw)
    y = r * sin(pitch)
    z = r * cos(pitch) * cos(yaw)
    
    Where r = 1 (unit vector for direction)
    */
    
    float pitchRad = pitch * PI / 180.0f;
    float yawRad = yaw * PI / 180.0f;
    
    lookX = cameraX + cos(pitchRad) * sin(yawRad);
    lookY = cameraY + sin(pitchRad);
    lookZ = cameraZ + cos(pitchRad) * cos(yawRad);
}


// HELPER: Draw ground with grid
// ============================================================================
void DrawGround()
{
    glColor3f(0.3, 0.5, 0.3);
    glBegin(GL_QUADS);
        glVertex3f(-20, 0, -20);
        glVertex3f( 20, 0, -20);
        glVertex3f( 20, 0,  20);
        glVertex3f(-20, 0,  20);
    glEnd();
    
    // Grid lines
    glColor3f(0.2, 0.4, 0.2);
    glBegin(GL_LINES);
        for (int i = -20; i <= 20; i += 2)
        {
            glVertex3f(i, 0.01f, -20);
            glVertex3f(i, 0.01f,  20);
            glVertex3f(-20, 0.01f, i);
            glVertex3f( 20, 0.01f, i);
        }
    glEnd();
}


// HELPER: Draw reference objects
// ============================================================================
void DrawReferenceObjects()
{
    // Center red cube
    glPushMatrix();
        glTranslatef(0, 1, 0);
        glColor3f(1, 0, 0);
        glutSolidCube(2.0);
    glPopMatrix();
    
    // Four corner pyramids
    GLfloat colors[4][3] = {
        {0, 1, 1},  // Cyan
        {1, 1, 0},  // Yellow
        {1, 0, 1},  // Magenta
        {0, 0, 1}   // Blue
    };
    
    float positions[4][2] = {
        {-10, -10},
        { 10, -10},
        { 10,  10},
        {-10,  10}
    };
    
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
            glTranslatef(positions[i][0], 0, positions[i][1]);
            glColor3fv(colors[i]);
            glRotatef(-90, 1, 0, 0);
            glutSolidCone(1.0, 2.0, 16, 16);
        glPopMatrix();
    }
}


// HELPER: Draw teapot
// ============================================================================
void DrawTeapot()
{
    glPushMatrix();
        glTranslatef(0, 1.5, 0);
        glColor3f(0.8, 0.6, 0.2);  // Gold-ish
        glutSolidTeapot(1.0);
    glPopMatrix();
}


// INITIALIZATION
// ============================================================================
void myInit()
{
    glClearColor(0.5, 0.7, 1.0, 1.0);  // Sky blue
    glEnable(GL_DEPTH_TEST);
    
    // Enable basic lighting for better 3D perception
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    
    GLfloat light_pos[] = {5.0f, 10.0f, 5.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    
    // Projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)windowWidth/(double)windowHeight, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    
    // Initial camera direction
    UpdateCameraVectors();
}


// DISPLAY FUNCTION
// ============================================================================
void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Calculate up vector considering roll
    float rollRad = roll * PI / 180.0f;
    float upX = sin(rollRad);
    float upY = cos(rollRad);
    float upZ = 0.0f;
    
    // Set camera with gluLookAt
    gluLookAt(
        cameraX, cameraY, cameraZ,  // Eye position
        lookX, lookY, lookZ,         // Look-at point
        upX, upY, upZ                // Up vector (affected by roll)
    );
    
    // Draw scene
    DrawGround();
    DrawReferenceObjects();
    DrawTeapot();
    
    glutSwapBuffers();
}


// KEYBOARD CALLBACK
// ============================================================================
void myKeyboard(unsigned char key, int x, int y)
{
    // Calculate forward/right vectors for movement
    float forwardX = sin(yaw * PI / 180.0f);
    float forwardZ = cos(yaw * PI / 180.0f);
    float rightX = sin((yaw + 90) * PI / 180.0f);
    float rightZ = cos((yaw + 90) * PI / 180.0f);
    
    switch(key)
    {
        // Movement (WASD)
        case 'w':
        case 'W':
            // Move forward
            cameraX += forwardX * moveSpeed;
            cameraZ += forwardZ * moveSpeed;
            break;
            
        case 's':
        case 'S':
            // Move backward
            cameraX -= forwardX * moveSpeed;
            cameraZ -= forwardZ * moveSpeed;
            break;
            
        case 'a':
        case 'A':
            // Strafe left
            cameraX -= rightX * moveSpeed;
            cameraZ -= rightZ * moveSpeed;
            break;
            
        case 'd':
        case 'D':
            // Strafe right
            cameraX += rightX * moveSpeed;
            cameraZ += rightZ * moveSpeed;
            break;
            
        case 'q':
        case 'Q':
            // Move up
            cameraY += moveSpeed;
            break;
            
        case 'e':
        case 'E':
            // Move down
            cameraY -= moveSpeed;
            break;
            
        // Reset camera
        case ' ':
            cameraX = 0.0f;
            cameraY = 2.0f;
            cameraZ = 5.0f;
            pitch = 0.0f;
            yaw = 0.0f;
            roll = 0.0f;
            cout << "Camera reset" << endl;
            break;
            
        case 27:  // ESC
            exit(0);
            break;
    }
    
    UpdateCameraVectors();
    glutPostRedisplay();
}


// SPECIAL KEYS CALLBACK
// ============================================================================
void mySpecialKeys(int key, int x, int y)
{
    switch(key)
    {
        // PITCH (up/down look)
        case GLUT_KEY_UP:
            pitch += rotateSpeed;
            if (pitch > 89.0f) pitch = 89.0f;  // Limit to avoid flip
            cout << "Pitch: " << pitch << "°" << endl;
            break;
            
        case GLUT_KEY_DOWN:
            pitch -= rotateSpeed;
            if (pitch < -89.0f) pitch = -89.0f;  // Limit to avoid flip
            cout << "Pitch: " << pitch << "°" << endl;
            break;
            
        // YAW (left/right look)
        case GLUT_KEY_LEFT:
            yaw += rotateSpeed;
            if (yaw >= 360.0f) yaw -= 360.0f;
            cout << "Yaw: " << yaw << "°" << endl;
            break;
            
        case GLUT_KEY_RIGHT:
            yaw -= rotateSpeed;
            if (yaw < 0.0f) yaw += 360.0f;
            cout << "Yaw: " << yaw << "°" << endl;
            break;
            
        // ROLL (tilt) - using Page Up/Down
        case GLUT_KEY_PAGE_UP:
            roll += rotateSpeed;
            if (roll >= 360.0f) roll -= 360.0f;
            cout << "Roll: " << roll << "°" << endl;
            break;
            
        case GLUT_KEY_PAGE_DOWN:
            roll -= rotateSpeed;
            if (roll < 0.0f) roll += 360.0f;
            cout << "Roll: " << roll << "°" << endl;
            break;
    }
    
    UpdateCameraVectors();
    glutPostRedisplay();
}


// MAIN FUNCTION
// ============================================================================
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("LAB 12 - Camera Flyby Demo");
    
    myInit();
    glutDisplayFunc(myDisplay);
    glutKeyboardFunc(myKeyboard);
    glutSpecialFunc(mySpecialKeys);
    
    cout << "\n=== LAB 12 - Camera Flyby Operations ===" << endl;
    cout << "MOVEMENT:" << endl;
    cout << "  W/S - Move forward/backward" << endl;
    cout << "  A/D - Strafe left/right" << endl;
    cout << "  Q/E - Move up/down" << endl;
    cout << "\nROTATION:" << endl;
    cout << "  Arrow Up/Down - PITCH (look up/down)" << endl;
    cout << "  Arrow Left/Right - YAW (turn left/right)" << endl;
    cout << "  Page Up/Down - ROLL (tilt head)" << endl;
    cout << "\nCONTROLS:" << endl;
    cout << "  SPACE - Reset camera" << endl;
    cout << "  ESC - Exit" << endl;
    cout << "=========================================\n" << endl;
    
    glutMainLoop();
}


/*
================================================================================
EXERCISE SOLUTIONS
================================================================================
*/


/*
================================================================================
  EXERCISE 1: Teapot with Camera Flyby
================================================================================

Already implemented in main demo above!
Features:
- Teapot at center
- Full pitch/yaw/roll control
- WASD movement
- Arrow keys for rotation
*/


/*
================================================================================
  EXERCISE 2: Animated Checkerboard with Bouncing Balls
================================================================================
*/

// Add to globals:
/*
struct Ball {
    float x, y, z;
    float radius;
    float velocity;
    float color[3];
};

Ball balls[3] = {
    {-3.0f, 2.0f, 0.0f, 0.5f, 0.05f, {1.0f, 0.0f, 0.0f}},  // Red
    { 0.0f, 3.0f, 0.0f, 0.7f, 0.03f, {0.0f, 1.0f, 0.0f}},  // Green
    { 3.0f, 2.5f, 0.0f, 0.6f, 0.04f, {0.0f, 0.0f, 1.0f}}   // Blue
};

void DrawCheckerboard()
{
    const int size = 20;
    for (int i = -size; i < size; i++)
    {
        for (int j = -size; j < size; j++)
        {
            if ((i + j) % 2 == 0)
                glColor3f(1, 1, 1);  // White
            else
                glColor3f(0, 0, 0);  // Black
                
            glBegin(GL_QUADS);
                glVertex3f(i, 0, j);
                glVertex3f(i+1, 0, j);
                glVertex3f(i+1, 0, j+1);
                glVertex3f(i, 0, j+1);
            glEnd();
        }
    }
}

void UpdateBalls()
{
    for (int i = 0; i < 3; i++)
    {
        balls[i].y += balls[i].velocity;
        
        // Bounce on ground
        if (balls[i].y - balls[i].radius <= 0.0f)
        {
            balls[i].y = balls[i].radius;
            balls[i].velocity = -balls[i].velocity;
        }
        
        // Bounce on ceiling (invisible at y=5)
        if (balls[i].y + balls[i].radius >= 5.0f)
        {
            balls[i].y = 5.0f - balls[i].radius;
            balls[i].velocity = -balls[i].velocity;
        }
    }
}

void DrawBalls()
{
    for (int i = 0; i < 3; i++)
    {
        glPushMatrix();
            glTranslatef(balls[i].x, balls[i].y, balls[i].z);
            glColor3fv(balls[i].color);
            glutSolidSphere(balls[i].radius, 32, 32);
        glPopMatrix();
    }
}

void myDisplay_Ex2()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Camera with flyby
    float rollRad = roll * PI / 180.0f;
    gluLookAt(
        cameraX, cameraY, cameraZ,
        lookX, lookY, lookZ,
        sin(rollRad), cos(rollRad), 0
    );
    
    DrawCheckerboard();
    DrawBalls();
    
    glutSwapBuffers();
}

void myTimer_Ex2(int value)
{
    UpdateBalls();
    glutPostRedisplay();
    glutTimerFunc(30, myTimer_Ex2, 0);
}
*/


/*
================================================================================
KEY CONCEPTS SUMMARY
================================================================================

PITCH, YAW, ROLL:
    Pitch: Look up/down (X-axis rotation)
    Yaw: Turn left/right (Y-axis rotation)
    Roll: Tilt head (Z-axis rotation)

SPHERICAL TO CARTESIAN:
    lookX = cameraX + cos(pitch) * sin(yaw)
    lookY = cameraY + sin(pitch)
    lookZ = cameraZ + cos(pitch) * cos(yaw)

MOVEMENT RELATIVE TO VIEW:
    Forward: Move along (sin(yaw), 0, cos(yaw))
    Right: Move along (sin(yaw+90°), 0, cos(yaw+90°))
    Up: Move along (0, 1, 0)

FIRST-PERSON CAMERA:
    - Camera position in world
    - Calculate look direction from angles
    - Move relative to current orientation
    - Update look-at point each frame

LIMITS:
    Pitch: -89° to +89° (prevent gimbal lock)
    Yaw: 0° to 360° (wrap around)
    Roll: 0° to 360° (wrap around)

================================================================================
*/
