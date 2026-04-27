/*
================================================================================
    LAB 10: TEXTURE MAPPING
    Computer Graphics Course - Bahria University Karachi
================================================================================

OBJECTIVES:
    1. Understand texture mapping concepts
    2. Apply procedural textures (checkerboard)
    3. Load and apply bitmap images as textures
    4. Map textures to 3D objects

NEW CONCEPTS IN THIS LAB:
    - Texture mapping (gluing images to geometry)
    - Texture coordinates (s, t) vs geometric coordinates (x, y, z)
    - glTexImage2D() - defining textures
    - glTexCoord2f() - mapping texture to vertices
    - Texture parameters (wrap, filter)
    - Procedural textures vs image textures

================================================================================
WHY TEXTURE MAPPING?
================================================================================

WITHOUT TEXTURES:
    - Objects are solid colors or gradients
    - To draw a brick wall: thousands of small brick polygons
    - Complex patterns require complex geometry
    - Not realistic

WITH TEXTURES:
    - Single polygon can show detailed pattern
    - Photograph of real wall mapped to single quad
    - Complex appearance with simple geometry
    - Highly realistic results


REAL-WORLD ANALOGY:
    Think of texture mapping like wrapping a gift:
    - Geometry = the box shape
    - Texture = the wrapping paper pattern
    - Texture coordinates = how you align the paper


APPLICATIONS:
    - Realistic surfaces (wood, stone, fabric)
    - Decals and labels
    - Sky boxes
    - Terrain (grass, dirt, sand)
    - Character skins

================================================================================
TEXTURE COORDINATES
================================================================================

GEOMETRIC COORDINATES (x, y, z):
    Define the 3D position of vertices in world space
    Example: (100, 200, 0)

TEXTURE COORDINATES (s, t):
    Define position in the texture image
    Range: 0.0 to 1.0 (normalized)
    (0, 0) = bottom-left of texture
    (1, 1) = top-right of texture
    
    s = horizontal (like x)
    t = vertical (like y)


MAPPING EXAMPLE:
    Quad with texture:
    
    Geometry:              Texture coords:
    (0,1)-----(1,1)       (0,1)-----(1,1)
      ¦         ¦           ¦    ¦¦¦  ¦
      ¦         ¦           ¦  ¦¦¦¦¦  ¦
    (0,0)-----(1,0)       (0,0)-----(1,0)
    
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(0, 0);  // Bottom-left
        glTexCoord2f(1, 0); glVertex2f(1, 0);  // Bottom-right
        glTexCoord2f(1, 1); glVertex2f(1, 1);  // Top-right
        glTexCoord2f(0, 1); glVertex2f(0, 1);  // Top-left
    glEnd();

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

// Texture IDs
GLuint checkerTexture;
GLuint brickTexture;


// CREATE PROCEDURAL CHECKERBOARD TEXTURE
// ============================================================================
void MakeCheckerTexture()
{
    /*
    Procedural texture: Generated algorithmically (not from file)
    
    Creates 8×8 checkerboard pattern:
    - 64×64 pixels total
    - Each square is 8×8 pixels
    - Alternating black (0) and white (255)
    */
    
    const int texSize = 64;
    GLubyte image[texSize][texSize][3];  // RGB image
    
    for (int i = 0; i < texSize; i++)
    {
        for (int j = 0; j < texSize; j++)
        {
            // Determine if this pixel is in a black or white square
            int c = (((i & 8) == 0) ^ ((j & 8) == 0)) * 255;
            /*
            Explanation:
            (i & 8) checks if bit 3 is set ? changes every 8 pixels
            Same for (j & 8)
            XOR (^) alternates the pattern
            Multiply by 255 to get black (0) or white (255)
            */
            
            image[i][j][0] = (GLubyte)c;  // Red
            image[i][j][1] = (GLubyte)c;  // Green
            image[i][j][2] = (GLubyte)c;  // Blue
        }
    }
    
    // Generate texture ID
    glGenTextures(1, &checkerTexture);
    /*
    glGenTextures(n, &id)
    - Generates 'n' unique texture IDs
    - Stores ID in 'id' variable
    */
    
    // Bind texture (make it current)
    glBindTexture(GL_TEXTURE_2D, checkerTexture);
    /*
    glBindTexture(target, id)
    - Makes texture 'id' the current texture
    - All subsequent texture operations apply to this texture
    - Like selecting a layer in Photoshop
    */
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    /*
    TEXTURE FILTERING:
    GL_NEAREST: Use closest pixel (blocky, sharp)
    GL_LINEAR:  Interpolate between pixels (smooth, blurry)
    
    MIN_FILTER: When texture is smaller than polygon
    MAG_FILTER: When texture is larger than polygon
    */
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    /*
    TEXTURE WRAPPING:
    GL_REPEAT: Tile the texture (0.0-1.0, 1.0-2.0, etc.)
    GL_CLAMP:  Clamp to edge (values > 1.0 use edge pixel)
    
    WRAP_S: Horizontal wrapping
    WRAP_T: Vertical wrapping
    */
    
    // Upload texture data to GPU
    glTexImage2D(
        GL_TEXTURE_2D,     // Target
        0,                 // Mipmap level (0 = base)
        GL_RGB,            // Internal format
        texSize,           // Width
        texSize,           // Height
        0,                 // Border (must be 0)
        GL_RGB,            // Format of pixel data
        GL_UNSIGNED_BYTE,  // Data type
        image              // Pixel data
    );
}


// CREATE PROCEDURAL BRICK TEXTURE
// ============================================================================
void MakeBrickTexture()
{
    const int texWidth = 128;
    const int texHeight = 64;
    GLubyte image[texHeight][texWidth][3];
    
    // Brick colors
    GLubyte brickColor[3] = {180, 80, 40};    // Red-brown
    GLubyte mortarColor[3] = {200, 200, 200}; // Light gray
    
    for (int i = 0; i < texHeight; i++)
    {
        for (int j = 0; j < texWidth; j++)
        {
            // Determine if this is mortar or brick
            bool isMortar = false;
            
            // Horizontal mortar lines (every 16 pixels)
            if (i % 16 == 0 || i % 16 == 1)
                isMortar = true;
            
            // Vertical mortar lines (staggered pattern)
            int row = i / 16;
            int offset = (row % 2) * 32;  // Offset every other row
            if ((j + offset) % 64 == 0 || (j + offset) % 64 == 1)
                isMortar = true;
            
            if (isMortar)
            {
                image[i][j][0] = mortarColor[0];
                image[i][j][1] = mortarColor[1];
                image[i][j][2] = mortarColor[2];
            }
            else
            {
                // Add slight variation to bricks
                int variation = (i * j) % 20 - 10;
                image[i][j][0] = brickColor[0] + variation;
                image[i][j][1] = brickColor[1] + variation;
                image[i][j][2] = brickColor[2] + variation;
            }
        }
    }
    
    glGenTextures(1, &brickTexture);
    glBindTexture(GL_TEXTURE_2D, brickTexture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 
                 0, GL_RGB, GL_UNSIGNED_BYTE, image);
}


// INITIALIZATION
// ============================================================================
void myInit()
{
    glClearColor(0.3, 0.3, 0.4, 1.0);
    glEnable(GL_DEPTH_TEST);
    
    // Create textures
    MakeCheckerTexture();
    MakeBrickTexture();
    
    // Setup projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)windowWidth/(double)windowHeight, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}


// DRAW TEXTURED QUAD
// ============================================================================
void DrawTexturedQuad(GLuint textureID, float size)
{
    /*
    Draw a square with texture applied
    
    KEY: Match texture coords to vertices
    Each glTexCoord2f() applies to the NEXT glVertex*()
    */
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glBegin(GL_QUADS);
        // Bottom-left corner
        glTexCoord2f(0.0, 0.0);  // Texture: bottom-left
        glVertex3f(-size, -size, 0.0);
        
        // Bottom-right corner
        glTexCoord2f(1.0, 0.0);  // Texture: bottom-right
        glVertex3f(size, -size, 0.0);
        
        // Top-right corner
        glTexCoord2f(1.0, 1.0);  // Texture: top-right
        glVertex3f(size, size, 0.0);
        
        // Top-left corner
        glTexCoord2f(0.0, 1.0);  // Texture: top-left
        glVertex3f(-size, size, 0.0);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
}


// DRAW TEXTURED CUBE
// ============================================================================
void DrawTexturedCube(float size)
{
    /*
    Cube with same texture on all faces
    Each face needs its own texture coordinates
    */
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, checkerTexture);
    
    glBegin(GL_QUADS);
    
    // Front face
    glTexCoord2f(0, 0); glVertex3f(-size, -size,  size);
    glTexCoord2f(1, 0); glVertex3f( size, -size,  size);
    glTexCoord2f(1, 1); glVertex3f( size,  size,  size);
    glTexCoord2f(0, 1); glVertex3f(-size,  size,  size);
    
    // Back face
    glTexCoord2f(0, 0); glVertex3f(-size, -size, -size);
    glTexCoord2f(1, 0); glVertex3f(-size,  size, -size);
    glTexCoord2f(1, 1); glVertex3f( size,  size, -size);
    glTexCoord2f(0, 1); glVertex3f( size, -size, -size);
    
    // Top face
    glTexCoord2f(0, 0); glVertex3f(-size,  size, -size);
    glTexCoord2f(1, 0); glVertex3f(-size,  size,  size);
    glTexCoord2f(1, 1); glVertex3f( size,  size,  size);
    glTexCoord2f(0, 1); glVertex3f( size,  size, -size);
    
    // Bottom face
    glTexCoord2f(0, 0); glVertex3f(-size, -size, -size);
    glTexCoord2f(1, 0); glVertex3f( size, -size, -size);
    glTexCoord2f(1, 1); glVertex3f( size, -size,  size);
    glTexCoord2f(0, 1); glVertex3f(-size, -size,  size);
    
    // Right face
    glTexCoord2f(0, 0); glVertex3f( size, -size, -size);
    glTexCoord2f(1, 0); glVertex3f( size,  size, -size);
    glTexCoord2f(1, 1); glVertex3f( size,  size,  size);
    glTexCoord2f(0, 1); glVertex3f( size, -size,  size);
    
    // Left face
    glTexCoord2f(0, 0); glVertex3f(-size, -size, -size);
    glTexCoord2f(1, 0); glVertex3f(-size, -size,  size);
    glTexCoord2f(1, 1); glVertex3f(-size,  size,  size);
    glTexCoord2f(0, 1); glVertex3f(-size,  size, -size);
    
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
}


// DISPLAY FUNCTION
// ============================================================================
void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    gluLookAt(0, 2, 6,    // Eye
              0, 0, 0,    // Look at
              0, 1, 0);   // Up
    
    // Draw checkerboard quad (left)
    glPushMatrix();
        glTranslatef(-2.5, 0, 0);
        DrawTexturedQuad(checkerTexture, 1.0);
    glPopMatrix();
    
    // Draw brick quad (right)
    glPushMatrix();
        glTranslatef(2.5, 0, 0);
        DrawTexturedQuad(brickTexture, 1.0);
    glPopMatrix();
    
    // Draw textured cube (center, below)
    glPushMatrix();
        glTranslatef(0, -2.5, 0);
        glRotatef(45, 0, 1, 0);  // Rotate to see multiple faces
        DrawTexturedCube(0.8);
    glPopMatrix();
    
    glutSwapBuffers();
}


// MAIN FUNCTION
// ============================================================================
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("LAB 10 - Texture Mapping Demo");
    glutDisplayFunc(myDisplay);
    myInit();
    
    cout << "\n=== LAB 10 DEMO - Texture Mapping ===" << endl;
    cout << "Left: Checkerboard texture (procedural)" << endl;
    cout << "Right: Brick texture (procedural)" << endl;
    cout << "Bottom: Textured cube" << endl;
    cout << "\nBoth textures generated algorithmically!" << endl;
    cout << "======================================\n" << endl;
    
    glutMainLoop();
}


/*
================================================================================
TEXTURE MAPPING PIPELINE
================================================================================

STEP 1: CREATE TEXTURE DATA
    - From file (BMP, PNG, JPEG)
    - Procedural (algorithm)
    - From framebuffer

STEP 2: GENERATE TEXTURE ID
    GLuint texID;
    glGenTextures(1, &texID);

STEP 3: BIND TEXTURE
    glBindTexture(GL_TEXTURE_2D, texID);

STEP 4: SET TEXTURE PARAMETERS
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

STEP 5: UPLOAD TO GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 
                 0, GL_RGB, GL_UNSIGNED_BYTE, data);

STEP 6: ENABLE TEXTURING
    glEnable(GL_TEXTURE_2D);

STEP 7: DRAW WITH TEXTURE COORDINATES
    glBegin(GL_QUADS);
        glTexCoord2f(s, t); glVertex3f(x, y, z);
        ...
    glEnd();

STEP 8: DISABLE TEXTURING
    glDisable(GL_TEXTURE_2D);


COMMON MISTAKES:
----------------

1. Forgetting glEnable(GL_TEXTURE_2D)
   ? Texture won't appear

2. Wrong texture coordinate order
   ? Texture appears flipped or rotated

3. Not binding texture before drawing
   ? Wrong texture or no texture

4. Texture coordinates outside 0-1 with GL_CLAMP
   ? Texture stretched at edges

5. Forgetting to generate mipmaps for GL_LINEAR_MIPMAP_LINEAR
   ? Black texture


TEXTURE PARAMETERS EXPLAINED:
------------------------------

MIN_FILTER (texture smaller than polygon):
    GL_NEAREST - Blocky, pixelated (fast)
    GL_LINEAR - Smooth, blurred (slower)
    GL_LINEAR_MIPMAP_LINEAR - Best quality (needs mipmaps)

MAG_FILTER (texture larger than polygon):
    GL_NEAREST - Blocky, sharp pixels
    GL_LINEAR - Smooth interpolation

WRAP_S/WRAP_T (coordinates outside 0-1):
    GL_REPEAT - Tile the texture
    GL_CLAMP - Use edge pixels
    GL_CLAMP_TO_EDGE - Clamp exactly to edge


TEXTURE COORDINATE TIPS:
-------------------------

Standard mapping (full texture):
    (0,0) ? (1,0) ? (1,1) ? (0,1)

Flip vertically:
    (0,1) ? (1,1) ? (1,0) ? (0,0)

Flip horizontally:
    (1,0) ? (0,0) ? (0,1) ? (1,1)

Repeat 2×2:
    (0,0) ? (2,0) ? (2,2) ? (0,2)

Use only part of texture (top-left quarter):
    (0,0.5) ? (0.5,0.5) ? (0.5,1) ? (0,1)

================================================================================
*/
