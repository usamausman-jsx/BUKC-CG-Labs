/*
================================================================================
    LAB 13: INTEGRATION PROJECT - 3D INTERACTIVE SCENE
    Computer Graphics Course - Bahria University Karachi
================================================================================

OBJECTIVE:
    Create a complete 3D interactive scene combining ALL concepts from Labs 1-12

FEATURES TO INCLUDE:
    ? 3D Objects (primitives, hierarchical models)
    ? Transformations (translate, rotate, scale)
    ? Lighting and Materials (Phong model)
    ? Textures (procedural)
    ? Camera Control (flyby with pitch/yaw/roll)
    ? Animation (moving objects)
    ? Keyboard/Mouse Interaction

PROJECT: INTERACTIVE 3D PARK SCENE
    - Ground with texture
    - Trees (hierarchical modeling)
    - Animated windmill
    - Moving cart
    - Sun with lighting
    - First-person camera to explore

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

int windowWidth = 1024;
int windowHeight = 768;

// Camera (flyby from Lab 12)
float cameraX = 0.0f, cameraY = 3.0f, cameraZ = 15.0f;
float pitch = 0.0f, yaw = 0.0f, roll = 0.0f;
float lookX, lookY, lookZ;

// Animation
float windmillAngle = 0.0f;
float cartX = -20.0f;
float sunAngle = 45.0f;

// Textures
GLuint grassTex, brickTex, woodTex;

// Controls
bool animationOn = true;
bool lightingOn = true;


// UPDATE CAMERA
// ============================================================================
void UpdateCamera()
{
    float pitchRad = pitch * PI / 180.0f;
    float yawRad = yaw * PI / 180.0f;
    
    lookX = cameraX + cos(pitchRad) * sin(yawRad);
    lookY = cameraY + sin(pitchRad);
    lookZ = cameraZ + cos(pitchRad) * cos(yawRad);
}


// CREATE TEXTURES
// ============================================================================
void CreateTextures()
{
    // Grass texture
    const int size = 64;
    GLubyte grass[size][size][3];
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
        {
            int c = ((i & 4) == 0) ^ ((j & 4) == 0);
            grass[i][j][0] = c ? 100 : 80;
            grass[i][j][1] = c ? 180 : 150;
            grass[i][j][2] = c ? 80 : 60;
        }
    
    glGenTextures(1, &grassTex);
    glBindTexture(GL_TEXTURE_2D, grassTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, grass);
}


// DRAW GROUND
// ============================================================================
void DrawGround()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, grassTex);
    
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-50, 0, -50);
        glTexCoord2f(20, 0); glVertex3f(50, 0, -50);
        glTexCoord2f(20, 20); glVertex3f(50, 0, 50);
        glTexCoord2f(0, 20); glVertex3f(-50, 0, 50);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
}


// DRAW TREE (Hierarchical)
// ============================================================================
void DrawTree(float x, float z)
{
    glPushMatrix();
        glTranslatef(x, 0, z);
        
        // Trunk
        glColor3f(0.5, 0.3, 0.1);
        glPushMatrix();
            glTranslatef(0, 1.5, 0);
            glScalef(0.3, 3, 0.3);
            glutSolidCube(1.0);
        glPopMatrix();
        
        // Leaves (3 spheres)
        glColor3f(0.2, 0.6, 0.2);
        glPushMatrix();
            glTranslatef(0, 3.5, 0);
            glutSolidSphere(1.2, 16, 16);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0, 4.5, 0);
            glutSolidSphere(1.0, 16, 16);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0, 5.3, 0);
            glutSolidSphere(0.7, 16, 16);
        glPopMatrix();
    glPopMatrix();
}


// DRAW WINDMILL (from Lab 8)
// ============================================================================
void DrawWindmill()
{
    // Stand
    glColor3f(0.6, 0.4, 0.2);
    glPushMatrix();
        glTranslatef(0, 2, 0);
        glScalef(0.4, 4, 0.4);
        glutSolidCube(1.0);
    glPopMatrix();
    
    // Fan
    glPushMatrix();
        glTranslatef(0, 4, 0);
        glRotatef(windmillAngle, 0, 0, 1);
        
        glColor3f(0.9, 0.9, 1.0);
        for (int i = 0; i < 4; i++)
        {
            glPushMatrix();
                glRotatef(i * 90, 0, 0, 1);
                glTranslatef(0, 0.5, 0);
                glScalef(0.2, 1.5, 0.05);
                glutSolidCube(1.0);
            glPopMatrix();
        }
    glPopMatrix();
}


// DRAW CART (from Lab 8)
// ============================================================================
void DrawCart()
{
    // Body
    glColor3f(0.7, 0.4, 0.2);
    glPushMatrix();
        glScalef(2, 0.8, 1);
        glutSolidCube(1.0);
    glPopMatrix();
    
    // Wheels
    glColor3f(0.3, 0.3, 0.3);
    glPushMatrix();
        glTranslatef(-0.7, -0.5, 0.6);
        glRotatef(90, 0, 0, 1);
        glutSolidTorus(0.1, 0.3, 16, 16);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.7, -0.5, 0.6);
        glRotatef(90, 0, 0, 1);
        glutSolidTorus(0.1, 0.3, 16, 16);
    glPopMatrix();
}


// DRAW SUN
// ============================================================================
void DrawSun()
{
    glPushMatrix();
        glTranslatef(0, 20, -20);
        glColor3f(1.0, 1.0, 0.5);
        glutSolidSphere(3.0, 32, 32);
    glPopMatrix();
}


// INITIALIZATION
// ============================================================================
void myInit()
{
    glClearColor(0.5, 0.7, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    
    CreateTextures();
    
    // Lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    
    GLfloat light_pos[] = {10, 20, 10, 0};
    GLfloat light_amb[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat light_diff[] = {1.0f, 1.0f, 0.9f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)windowWidth/(double)windowHeight, 0.1, 200.0);
    glMatrixMode(GL_MODELVIEW);
    
    UpdateCamera();
}


// DISPLAY
// ============================================================================
void myDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Camera
    float rollRad = roll * PI / 180.0f;
    gluLookAt(cameraX, cameraY, cameraZ, lookX, lookY, lookZ, 
              sin(rollRad), cos(rollRad), 0);
    
    // Scene
    DrawGround();
    DrawSun();
    
    // Trees
    DrawTree(10, 10);
    DrawTree(-10, 10);
    DrawTree(15, -5);
    DrawTree(-12, -8);
    
    // Windmill
    glPushMatrix();
        glTranslatef(-15, 0, 0);
        DrawWindmill();
    glPopMatrix();
    
    // Cart
    glPushMatrix();
        glTranslatef(cartX, 0.5, 5);
        DrawCart();
    glPopMatrix();
    
    glutSwapBuffers();
}


// ANIMATION
// ============================================================================
void myTimer(int value)
{
    if (animationOn)
    {
        windmillAngle += 2.0f;
        if (windmillAngle >= 360.0f) windmillAngle -= 360.0f;
        
        cartX += 0.1f;
        if (cartX > 30.0f) cartX = -30.0f;
    }
    
    glutPostRedisplay();
    glutTimerFunc(30, myTimer, 0);
}


// KEYBOARD
// ============================================================================
void myKeyboard(unsigned char key, int x, int y)
{
    float forwardX = sin(yaw * PI / 180.0f);
    float forwardZ = cos(yaw * PI / 180.0f);
    
    switch(key)
    {
        case 'w': cameraX += forwardX * 0.5f; cameraZ += forwardZ * 0.5f; break;
        case 's': cameraX -= forwardX * 0.5f; cameraZ -= forwardZ * 0.5f; break;
        case 'a': cameraX -= forwardZ * 0.5f; cameraZ += forwardX * 0.5f; break;
        case 'd': cameraX += forwardZ * 0.5f; cameraZ -= forwardX * 0.5f; break;
        case 'q': cameraY += 0.5f; break;
        case 'e': cameraY -= 0.5f; break;
        
        case ' ': animationOn = !animationOn; break;
        case 'l': 
            lightingOn = !lightingOn;
            if (lightingOn) glEnable(GL_LIGHTING);
            else glDisable(GL_LIGHTING);
            break;
        
        case 27: exit(0); break;
    }
    
    UpdateCamera();
    glutPostRedisplay();
}


void mySpecialKeys(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP: pitch += 2.0f; if (pitch > 89) pitch = 89; break;
        case GLUT_KEY_DOWN: pitch -= 2.0f; if (pitch < -89) pitch = -89; break;
        case GLUT_KEY_LEFT: yaw += 2.0f; break;
        case GLUT_KEY_RIGHT: yaw -= 2.0f; break;
    }
    
    UpdateCamera();
    glutPostRedisplay();
}


// MAIN
// ============================================================================
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("LAB 13 - Integration Project: 3D Park");
    
    myInit();
    glutDisplayFunc(myDisplay);
    glutKeyboardFunc(myKeyboard);
    glutSpecialFunc(mySpecialKeys);
    glutTimerFunc(30, myTimer, 0);
    
    cout << "\n=== LAB 13 - INTEGRATION PROJECT ===" << endl;
    cout << "3D Interactive Park Scene" << endl;
    cout << "\nMOVEMENT (WASD/QE):" << endl;
    cout << "  W/S - Forward/Backward" << endl;
    cout << "  A/D - Left/Right" << endl;
    cout << "  Q/E - Up/Down" << endl;
    cout << "\nCAMERA (Arrow Keys):" << endl;
    cout << "  Up/Down - Pitch" << endl;
    cout << "  Left/Right - Yaw" << endl;
    cout << "\nCONTROLS:" << endl;
    cout << "  SPACE - Toggle animation" << endl;
    cout << "  L - Toggle lighting" << endl;
    cout << "  ESC - Exit" << endl;
    cout << "\nExplore the park!" << endl;
    cout << "=====================================\n" << endl;
    
    glutMainLoop();
}

/*
================================================================================
CONCEPTS INTEGRATED:
================================================================================

From Lab 1-3: Primitives, viewports, coordinates
From Lab 4: Keyboard and mouse interaction
From Lab 5: Circles (wheel construction)
From Lab 6: N/A (algorithms not needed for high-level)
From Lab 7: Transformations (translate, rotate, scale)
From Lab 8: Hierarchical modeling (tree, windmill, cart)
From Lab 9: Lighting and materials (Phong model)
From Lab 10: Texture mapping (ground)
From Lab 11: Camera settings (gluLookAt, gluPerspective)
From Lab 12: Camera flyby (pitch, yaw, first-person control)

EXTENSION IDEAS:
- Add more objects (house, fence, pond)
- Add day/night cycle
- Add particle effects (rain, snow)
- Add collision detection
- Add shadows
- Load models from files

================================================================================
*/
