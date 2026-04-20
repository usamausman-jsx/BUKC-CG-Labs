/*
================================================================================
    LAB 09: ILLUMINATION AND SHADING
    Computer Graphics Course - Bahria University Karachi
================================================================================

OBJECTIVES:
    1. Understand how real-world lighting is approximated in OpenGL
    2. Render illuminated objects by defining light sources
    3. Define material properties for realistic appearance
    4. Use the Phong illumination model

NEW CONCEPTS IN THIS LAB:
    - Lighting system in OpenGL
    - Phong illumination model (ambient + diffuse + specular)
    - Light sources (position, color, intensity)
    - Material properties (how surfaces reflect light)
    - Surface normals (orientation matters!)
    - GL_LIGHTING, GL_LIGHT0-7

================================================================================
WHY LIGHTING MATTERS
================================================================================

WITHOUT LIGHTING:
    - Objects are flat colors
    - No sense of depth or shape
    - Everything looks 2D
    - No realism

WITH LIGHTING:
    - Objects have shading (light and dark areas)
    - 3D shape is visible
    - Materials look realistic (metal, plastic, rubber)
    - Highlights show shininess


REAL-WORLD LIGHTING:
    When light hits a surface:
    1. Some light is absorbed (determines color)
    2. Some light is reflected (makes surface visible)
    3. Direction of reflection depends on surface properties
       - Rough surfaces: diffuse (scatter evenly)
       - Smooth surfaces: specular (mirror-like)

================================================================================
PHONG ILLUMINATION MODEL
================================================================================

PHONG MODEL: Surface color = Ambient + Diffuse + Specular

AMBIENT LIGHT:
    - Indirect light (light that bounced around the room)
    - Same brightness everywhere
    - Prevents pure black shadows
    - Minimum illumination level
    Example: Dark room with light under the door

DIFFUSE REFLECTION:
    - Matte surfaces (paper, clay, unpolished wood)
    - Light scattered equally in all directions
    - Surface looks same brightness from all viewing angles
    - Depends on angle between surface and light
    Example: Chalk, matte paint

SPECULAR REFLECTION:
    - Shiny surfaces (metal, polished plastic, water)
    - Mirror-like reflection
    - Creates bright highlights
    - Depends on viewing angle
    Example: Shiny apple, chrome, wet surface


FORMULA (simplified):
    I = I_ambient + I_diffuse + I_specular
    
    I_ambient  = K_ambient × Light_ambient
    I_diffuse  = K_diffuse × Light_diffuse × (N · L)
    I_specular = K_specular × Light_specular × (R · V)^shininess
    
    Where:
    N = surface normal (perpendicular to surface)
    L = light direction (surface to light)
    V = view direction (surface to eye)
    R = reflection direction
    K = material reflection coefficients

================================================================================
*/

// HEADER FILES
// ============================================================================
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>

using namespace std;

// Window dimensions
int windowWidth = 800;
int windowHeight = 600;

// Lighting control flags
bool ambientOn = true;
bool diffuseOn = true;
bool specularOn = true;


// MATERIAL PRESETS
// ============================================================================
struct Material {
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess;
};

// Predefined materials from lab manual
Material gold = {
    {0.24725f, 0.1995f, 0.0745f, 1.0f},   // Ambient
    {0.75164f, 0.60648f, 0.22648f, 1.0f}, // Diffuse
    {0.628281f, 0.555802f, 0.366065f, 1.0f}, // Specular
    51.2f  // Shininess
};

Material silver = {
    {0.19225f, 0.19225f, 0.19225f, 1.0f},
    {0.50754f, 0.50754f, 0.50754f, 1.0f},
    {0.773911f, 0.773911f, 0.773911f, 1.0f},
    89.6f
};

Material chrome = {
    {0.25f, 0.25f, 0.25f, 1.0f},
    {0.4f, 0.4f, 0.4f, 1.0f},
    {0.774597f, 0.774597f, 0.774597f, 1.0f},
    76.8f
};

Material blackPlastic = {
    {0.0f, 0.0f, 0.0f, 1.0f},
    {0.01f, 0.01f, 0.01f, 1.0f},
    {0.50f, 0.50f, 0.50f, 1.0f},
    32.0f
};

Material ruby = {
    {0.1745f, 0.01175f, 0.01175f, 1.0f},
    {0.61424f, 0.04136f, 0.04136f, 1.0f},
    {0.727811f, 0.626959f, 0.626959f, 1.0f},
    76.8f
};


// HELPER: Apply material
// ============================================================================
void ApplyMaterial(Material mat)
{
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat.ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat.diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat.specular);
    glMaterialf(GL_FRONT, GL_SHININESS, mat.shininess);
}


// INITIALIZATION
// ============================================================================
void myInit()
{
    glClearColor(0.1, 0.1, 0.15, 1.0);  // Dark blue-gray background
    
    // Enable depth testing (CRITICAL for 3D!)
    glEnable(GL_DEPTH_TEST);
    /*
    Depth testing ensures objects in front hide objects behind
    Without this, objects draw in random order (looks wrong!)
    */
    
    // Enable lighting
    glEnable(GL_LIGHTING);
    /*
    GL_LIGHTING: Master switch for lighting system
    Without this, objects show their raw colors (no shading)
    */
    
    glEnable(GL_LIGHT0);
    /*
    OpenGL provides 8 lights: GL_LIGHT0 through GL_LIGHT7
    Each must be enabled individually
    GL_LIGHT0 is the most commonly used
    */
    
    // Set up light 0
    GLfloat light_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};  // White light
    GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light_position[] = {1.0f, 1.0f, 1.0f, 0.0f};
    /*
    Light position: (x, y, z, w)
    w = 0: Directional light (infinitely far away, like sun)
    w = 1: Positional light (has specific location, like lamp)
    
    Directional light: All light rays are parallel
    Positional light: Light rays emanate from a point
    */
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    // Enable color material (allows glColor to set material)
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
    // Set up projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)windowWidth/(double)windowHeight, 1.0, 100.0);
    /*
    gluPerspective: Creates perspective projection
    Parameters:
        45.0 = field of view (degrees)
        aspect ratio = width/height
        1.0 = near clipping plane
        100.0 = far clipping plane
    */
    
    glMatrixMode(GL_MODELVIEW);
}


// UPDATE LIGHTING (called when toggling components)
// ============================================================================
void UpdateLighting()
{
    GLfloat amb[] = {
        ambientOn ? 0.2f : 0.0f,
        ambientOn ? 0.2f : 0.0f,
        ambientOn ? 0.2f : 0.0f,
        1.0f
    };
    GLfloat diff[] = {
        diffuseOn ? 1.0f : 0.0f,
        diffuseOn ? 1.0f : 0.0f,
        diffuseOn ? 1.0f : 0.0f,
        1.0f
    };
    GLfloat spec[] = {
        specularOn ? 1.0f : 0.0f,
        specularOn ? 1.0f : 0.0f,
        specularOn ? 1.0f : 0.0f,
        1.0f
    };
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
}


// DISPLAY FUNCTION
// ============================================================================
void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /*
    Clear both color buffer AND depth buffer
    Depth buffer tracks which pixels are in front
    */
    
    glLoadIdentity();
    
    // Position camera
    gluLookAt(0.0, 0.0, 5.0,   // Eye position
              0.0, 0.0, 0.0,   // Look at point
              0.0, 1.0, 0.0);  // Up vector
    
    
    // ========================================================================
    // DEMO: 5 Objects with Different Materials
    // ========================================================================
    
    // Gold Teapot (center)
    glPushMatrix();
        glTranslatef(0.0, 0.0, 0.0);
        ApplyMaterial(gold);
        glutSolidTeapot(0.5);
    glPopMatrix();
    
    // Silver Sphere (left)
    glPushMatrix();
        glTranslatef(-2.0, 0.0, 0.0);
        ApplyMaterial(silver);
        glutSolidSphere(0.5, 32, 32);
    glPopMatrix();
    
    // Chrome Sphere (right)
    glPushMatrix();
        glTranslatef(2.0, 0.0, 0.0);
        ApplyMaterial(chrome);
        glutSolidSphere(0.5, 32, 32);
    glPopMatrix();
    
    // Black Plastic Torus (top)
    glPushMatrix();
        glTranslatef(0.0, 1.5, 0.0);
        ApplyMaterial(blackPlastic);
        glutSolidTorus(0.2, 0.4, 32, 32);
    glPopMatrix();
    
    // Ruby Cone (bottom)
    glPushMatrix();
        glTranslatef(0.0, -1.5, 0.0);
        glRotatef(-90, 1, 0, 0);
        ApplyMaterial(ruby);
        glutSolidCone(0.5, 1.0, 32, 32);
    glPopMatrix();
    
    
    glutSwapBuffers();
}


// KEYBOARD CALLBACK
// ============================================================================
void myKeyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'a':
        case 'A':
            ambientOn = !ambientOn;
            cout << "Ambient light: " << (ambientOn ? "ON" : "OFF") << endl;
            UpdateLighting();
            break;
            
        case 'd':
        case 'D':
            diffuseOn = !diffuseOn;
            cout << "Diffuse reflection: " << (diffuseOn ? "ON" : "OFF") << endl;
            UpdateLighting();
            break;
            
        case 's':
        case 'S':
            specularOn = !specularOn;
            cout << "Specular reflection: " << (specularOn ? "ON" : "OFF") << endl;
            UpdateLighting();
            break;
            
        case 'q':
        case 'Q':
        case 27:
            exit(0);
            break;
    }
    
    glutPostRedisplay();
}


// MAIN FUNCTION
// ============================================================================
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    /*
    GLUT_DOUBLE: Double buffering (smooth animation)
    GLUT_RGB: Color mode
    GLUT_DEPTH: Depth buffer (for 3D rendering)
    */
    
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("LAB 09 - Illumination and Shading Demo");
    glutDisplayFunc(myDisplay);
    glutKeyboardFunc(myKeyboard);
    myInit();
    
    cout << "\n=== LAB 09 DEMO - Illumination and Shading ===" << endl;
    cout << "5 objects with different materials:" << endl;
    cout << "  Center: Gold Teapot" << endl;
    cout << "  Left: Silver Sphere" << endl;
    cout << "  Right: Chrome Sphere" << endl;
    cout << "  Top: Black Plastic Torus" << endl;
    cout << "  Bottom: Ruby Cone" << endl;
    cout << "\nKeyboard controls:" << endl;
    cout << "  A - Toggle ambient light" << endl;
    cout << "  D - Toggle diffuse reflection" << endl;
    cout << "  S - Toggle specular reflection" << endl;
    cout << "  Q - Quit" << endl;
    cout << "\nTry turning off each component to see its effect!" << endl;
    cout << "===============================================\n" << endl;
    
    glutMainLoop();
}


/*
================================================================================
UNDERSTANDING OPENGL LIGHTING
================================================================================

LIGHTING EQUATION (Phong model):
    Final_Color = Ambient + Diffuse + Specular
    
    Ambient  = Material_ambient × Light_ambient
    Diffuse  = Material_diffuse × Light_diffuse × max(N·L, 0)
    Specular = Material_specular × Light_specular × max(R·V, 0)^shininess


WHAT EACH COMPONENT DOES:
--------------------------

AMBIENT:
    - Base illumination (indirect light)
    - Same everywhere on object
    - Prevents pure black areas
    - Turn OFF ? See only directly lit areas

DIFFUSE:
    - Main shading (defines shape)
    - Bright where light hits directly
    - Dark on sides away from light
    - Turn OFF ? Object loses 3D appearance

SPECULAR:
    - Shiny highlights
    - Bright spot where reflection angle = viewing angle
    - Defines surface shininess
    - Turn OFF ? Object looks matte


MATERIAL PROPERTIES:
--------------------

AMBIENT COEFFICIENT:
    How much ambient light is reflected
    Low = dark, High = bright base color

DIFFUSE COEFFICIENT:
    How much directional light is scattered
    Usually matches object's color
    High = bright when lit, Low = dark

SPECULAR COEFFICIENT:
    How much creates shiny highlights
    High = very shiny, Low = matte
    Usually white or light-colored

SHININESS (exponent):
    How focused the highlight is
    Low (1-10): Dull, wide highlight
    Medium (10-50): Moderate shine
    High (50-128): Very focused, mirror-like


LIGHT PROPERTIES:
-----------------

POSITION:
    (x, y, z, w)
    w=0: Directional (sun-like)
    w=1: Positional (lamp-like)

AMBIENT:
    Indirect light contribution
    Usually dim (0.2, 0.2, 0.2)

DIFFUSE:
    Main light color
    Usually bright (1.0, 1.0, 1.0)

SPECULAR:
    Highlight color
    Usually same as diffuse


SURFACE NORMALS:
----------------

NORMAL = Vector perpendicular to surface

Why normals matter:
    - Determine how much light hits surface
    - Face toward light ? bright
    - Face away from light ? dark
    - Perpendicular to light ? medium

OpenGL primitives (sphere, teapot, etc.) have automatic normals
Custom geometry needs glNormal3f() calls


DEPTH TESTING:
--------------

glEnable(GL_DEPTH_TEST)
    - Tracks which objects are in front
    - Hides objects behind others
    - Essential for 3D rendering
    
glClear(GL_DEPTH_BUFFER_BIT)
    - Clear depth buffer each frame
    - Prevents ghosting from previous frame


DOUBLE BUFFERING:
-----------------

glutInitDisplayMode(GLUT_DOUBLE)
    - Two framebuffers: front and back
    - Render to back buffer
    - Swap when complete
    - Prevents flickering
    
glutSwapBuffers()
    - Swap front and back buffers
    - Use instead of glFlush() for double buffering

================================================================================
COMMON LIGHTING PATTERNS
================================================================================

PATTERN 1: Basic Setup
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    
    GLfloat light_pos[] = {1, 1, 1, 0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);


PATTERN 2: White Light
    GLfloat white[] = {1, 1, 1, 1};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);


PATTERN 3: Colored Light
    GLfloat red[] = {1, 0, 0, 1};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, red);


PATTERN 4: Material Setup
    GLfloat mat_ambient[] = {0.2, 0.2, 0.2, 1};
    GLfloat mat_diffuse[] = {0.8, 0.8, 0.8, 1};
    GLfloat mat_specular[] = {1, 1, 1, 1};
    GLfloat mat_shininess = 50.0;
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

================================================================================
*/
