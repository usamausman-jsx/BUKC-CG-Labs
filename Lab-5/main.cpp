/*
================================================================================
    LAB 05: DRAWING CIRCLES AND ARCS
    Computer Graphics Course - Bahria University Karachi
================================================================================

OBJECTIVES:
    1. Draw circles using parametric equations
    2. Draw arcs (partial circles)
    3. Create complex shapes using circles and arcs

NEW CONCEPTS IN THIS LAB:
    - Parametric circle equation: x = cx + r*cos(?), y = cy + r*sin(?)
    - Angle iteration to approximate curves
    - Arc drawing with start/end angles
    - Number of segments vs smoothness tradeoff
    - Combining arcs to create complex shapes (clouds, teardrops)
    - Mathematical transformations (teardrop formula)

================================================================================
CIRCLE MATHEMATICS
================================================================================

CARTESIAN EQUATION:
    (x - cx)² + (y - cy)² = r²
    
    Where:
        (cx, cy) = center of circle
        r        = radius
    
    Problem: Solving for y gives ± (messy for drawing)

PARAMETRIC EQUATION (Better for drawing!):
    x = cx + r × cos(?)
    y = cy + r × sin(?)
    
    Where:
        ? ranges from 0 to 2p (0° to 360°)
    
    Advantage: Easy to iterate, generates points in order


WHY PARAMETRIC IS BETTER:
    - Single equation for each coordinate
    - ? acts as a "parameter" that traces the circle
    - Easy to control direction (clockwise/counter-clockwise)
    - Simple to draw partial circles (arcs)


VISUAL UNDERSTANDING:
    
    ? = 0°:    x = cx + r×1,  y = cy + r×0  ? (cx+r, cy)   [right]
    ? = 90°:   x = cx + r×0,  y = cy + r×1  ? (cx, cy+r)   [top]
    ? = 180°:  x = cx + r×-1, y = cy + r×0  ? (cx-r, cy)   [left]
    ? = 270°:  x = cx + r×0,  y = cy + r×-1 ? (cx, cy-r)   [bottom]


NUMBER OF SEGMENTS:
    Circle is approximated by N line segments
    
    Few segments (N=6):    Hexagon (blocky)
    More segments (N=20):  Recognizable circle
    Many segments (N=50):  Smooth circle
    Too many (N=1000):     Smooth but slow
    
    Recommendation: 50-100 segments for most circles

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


// INITIALIZATION
// ============================================================================
void myInit()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);  // White background
    glColor3f(0.0, 0.0, 0.0);           // Black drawing color
    glPointSize(2.0);
    glLineWidth(2.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, windowWidth, 0.0, windowHeight);
}


// FUNCTION: Draw Circle (Outline)
// ============================================================================
void DrawCircle(float cx, float cy, float radius, int numSegments)
{
    /*
    PARAMETERS:
        cx, cy       : Center coordinates
        radius       : Radius of the circle
        numSegments  : Number of line segments to approximate circle
                       (more segments = smoother circle)
    
    ALGORITHM:
        1. Loop through angles from 0 to 2p
        2. For each angle, compute (x, y) using parametric equation
        3. Connect consecutive points with lines (GL_LINE_LOOP)
    
    GL_LINE_LOOP vs GL_LINE_STRIP:
        LINE_LOOP:  Automatically connects last point back to first (closed)
        LINE_STRIP: Leaves gap between last and first (open)
        For circles, use LINE_LOOP!
    */
    
    glBegin(GL_LINE_LOOP);  // GL_LINE_LOOP closes the shape automatically
        for (int i = 0; i < numSegments; i++)
        {
            // Calculate angle for this segment
            float theta = 2.0f * PI * (float)i / (float)numSegments;
            /*
            ANGLE CALCULATION:
                - Full circle = 2p radians
                - Divide by numSegments to get angle step
                - Multiply by i to get current angle
            
            Example: 4 segments (square)
                i=0: ? = 0
                i=1: ? = p/2  (90°)
                i=2: ? = p    (180°)
                i=3: ? = 3p/2 (270°)
                Loop ends, GL_LINE_LOOP connects back to i=0
            */
            
            // Calculate x and y coordinates
            float x = radius * cosf(theta);  // X offset from center
            float y = radius * sinf(theta);  // Y offset from center
            
            // Add center coordinates to get absolute position
            glVertex2f(x + cx, y + cy);
        }
    glEnd();
}


// FUNCTION: Draw Filled Circle
// ============================================================================
void DrawFilledCircle(float cx, float cy, float radius, int numSegments)
{
    /*
    FILLED CIRCLE using GL_POLYGON or GL_TRIANGLE_FAN
    
    GL_POLYGON: Works for convex shapes (like circles)
    GL_TRIANGLE_FAN: More explicit triangle-based approach
    */
    
    glBegin(GL_POLYGON);  // GL_POLYGON fills the interior
        for (int i = 0; i < numSegments; i++)
        {
            float theta = 2.0f * PI * (float)i / (float)numSegments;
            float x = radius * cosf(theta);
            float y = radius * sinf(theta);
            glVertex2f(x + cx, y + cy);
        }
    glEnd();
}


// FUNCTION: Draw Arc (Partial Circle)
// ============================================================================
void DrawArc(float cx, float cy, float radius, float startAngle, float endAngle, int numSegments)
{
    /*
    PARAMETERS:
        startAngle, endAngle : Angles in DEGREES
        
    ARC: A portion of a circle
    
    Example:
        startAngle = 0°, endAngle = 180° ? semicircle (right half)
        startAngle = 90°, endAngle = 270° ? semicircle (top half)
        startAngle = 0°, endAngle = 90° ? quarter circle (top-right)
    
    IMPORTANT: Angles in DEGREES (easier for users) but convert to RADIANS for sin/cos
    */
    
    // Convert angles from degrees to radians
    float startRad = startAngle * PI / 180.0f;
    float endRad = endAngle * PI / 180.0f;
    
    // Calculate angle increment
    float angleStep = (endRad - startRad) / (float)numSegments;
    
    glBegin(GL_LINE_STRIP);  // GL_LINE_STRIP (not LOOP) for open arc
        for (int i = 0; i <= numSegments; i++)
        {
            float theta = startRad + (float)i * angleStep;
            float x = radius * cosf(theta);
            float y = radius * sinf(theta);
            glVertex2f(x + cx, y + cy);
        }
    glEnd();
}


// FUNCTION: Draw Filled Arc (Pie Slice)
// ============================================================================
void DrawFilledArc(float cx, float cy, float radius, float startAngle, float endAngle, int numSegments)
{
    /*
    Creates a PIE SLICE (filled arc with lines to center)
    Like a pizza slice or Pac-Man shape
    */
    
    float startRad = startAngle * PI / 180.0f;
    float endRad = endAngle * PI / 180.0f;
    float angleStep = (endRad - startRad) / (float)numSegments;
    
    glBegin(GL_TRIANGLE_FAN);
        // First vertex at center
        glVertex2f(cx, cy);
        
        // Then vertices along the arc
        for (int i = 0; i <= numSegments; i++)
        {
            float theta = startRad + (float)i * angleStep;
            float x = radius * cosf(theta);
            float y = radius * sinf(theta);
            glVertex2f(x + cx, y + cy);
        }
    glEnd();
}


// FUNCTION: Draw Teardrop Shape
// ============================================================================
void DrawTeardrop(float cx, float cy, float radius, int numSegments)
{
    /*
    TEARDROP FORMULA (from lab manual):
        x = cx + ((0.5 * (1 - cos(?))) * sin(?)) * radius
        y = cy + cos(?) * radius
    
    This creates a heart-like teardrop shape
    ? ranges from 0 to 2p
    */
    
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < numSegments; i++)
        {
            float theta = 2.0f * PI * (float)i / (float)numSegments;
            
            // Teardrop parametric equations
            float x = cx + ((0.5f * (1.0f - cosf(theta))) * sinf(theta)) * radius;
            float y = cy + cosf(theta) * radius;
            
            glVertex2f(x, y);
        }
    glEnd();
}


// FUNCTION: Draw Cloud (using multiple arcs)
// ============================================================================
void DrawCloud(float cx, float cy, float size)
{
    /*
    A cloud is made of several overlapping circles/arcs
    This creates a fluffy, irregular shape
    */
    
    // Draw several overlapping filled circles with white color
//    glColor3f(1.0, 1.0, 1.0);  // White fill
    
    
    // Draw outline
    glColor3f(0.5, 0.5, 0.5);  // Gray outline
    DrawCircle(cx - size*0.5, cy, size*0.6, 50);
    DrawCircle(cx, cy + size*0.2, size*0.7, 50);
    DrawCircle(cx + size*0.5, cy, size*0.6, 50);
    DrawCircle(cx + size*0.2, cy - size*0.2, size*0.5, 50);
    DrawCircle(cx - size*0.2, cy - size*0.2, size*0.5, 50);
    
    
    glColor3f(0.8, 0.5, 0.5);
    
    DrawFilledCircle(cx - size*0.5, cy, size*0.6, 50);
    DrawFilledCircle(cx, cy + size*0.2, size*0.7, 50);
    DrawFilledCircle(cx + size*0.5, cy, size*0.6, 50);
    DrawFilledCircle(cx + size*0.2, cy - size*0.2, size*0.5, 50);
    DrawFilledCircle(cx - size*0.2, cy - size*0.2, size*0.5, 50);
}


// DISPLAY FUNCTION - DEMO: Various Circle Examples
// ============================================================================
void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    // ========================================================================
    // EXAMPLE 1: Basic Circle (outline)
    // ========================================================================
    glColor3f(1.0, 0.0, 0.0);  // Red
    DrawCircle(150, 450, 50, 50);  // Center (150,450), radius 50, 50 segments
    
    
    // ========================================================================
    // EXAMPLE 2: Filled Circle
    // ========================================================================
    glColor3f(0.0, 0.0, 1.0);  // Blue
    DrawFilledCircle(400, 450, 50, 50);
    
    
    // ========================================================================
    // EXAMPLE 3: Concentric Circles
    // ========================================================================
    glColor3f(0.0, 0.0, 0.0);  // Black
    for (int i = 1; i <= 5; i++)
    {
        DrawCircle(650, 450, i * 10, 50);  // Radii: 10, 20, 30, 40, 50
    }
    
    
    // ========================================================================
    // EXAMPLE 4: Arc (semicircle)
    // ========================================================================
    glColor3f(0.0, 0.5, 0.0);  // Dark green
    DrawArc(150, 300, 50, 0, 180, 50);  // 0° to 180° = top semicircle
    
    
    // ========================================================================
    // EXAMPLE 5: Arc (quarter circle)
    // ========================================================================
    glColor3f(0.5, 0.0, 0.5);  // Purple
    DrawArc(400, 300, 50, 0, 90, 50);  // 0° to 90° = top-right quarter
    
    
    // ========================================================================
    // EXAMPLE 6: Filled Arc (Pac-Man)
    // ========================================================================
    glColor3f(1.0, 1.0, 0.0);  // Yellow
    DrawFilledArc(650, 300, 50, 30, 330, 50);  // 30° to 330° = Pac-Man mouth
    
    
    // ========================================================================
    // EXAMPLE 7: Teardrop
    // ========================================================================
    glColor3f(0.0, 0.5, 1.0);  // Light blue
    DrawTeardrop(150, 150, 50, 100);
    
    
    // ========================================================================
    // EXAMPLE 8: Cloud
    // ========================================================================
    DrawCloud(400, 150, 50);
    
    
    // ========================================================================
    // EXAMPLE 9: Different segment counts (smoothness comparison)
    // ========================================================================
    glColor3f(0.0, 0.0, 0.0);
    
    DrawCircle(580, 150, 30, 6);   // Hexagon (6 segments)
    DrawCircle(630, 150, 30, 12);  // 12-sided polygon
    DrawCircle(680, 150, 30, 50);  // Smooth circle (50 segments)
    
    
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
    glutCreateWindow("LAB 05 - Circles and Arcs Demo");
    glutDisplayFunc(myDisplay);
    myInit();
    
    cout << "=== LAB 05 DEMO - Circles and Arcs ===" << endl;
    cout << "Row 1 (top):    Circle, Filled Circle, Concentric Circles" << endl;
    cout << "Row 2 (middle): Semicircle, Quarter Arc, Pac-Man" << endl;
    cout << "Row 3 (bottom): Teardrop, Cloud, Smoothness Comparison" << endl;
    cout << "========================================\n" << endl;
    
    glutMainLoop();
}


/*
================================================================================
UNDERSTANDING ANGLE MEASUREMENTS
================================================================================

DEGREES vs RADIANS:
    Degrees: 0° to 360° (full circle)
    Radians: 0 to 2p (full circle)
    
    Conversion:
        radians = degrees × (p / 180)
        degrees = radians × (180 / p)

COMMON ANGLES:
    Degrees    Radians    Position on Circle
    ---------------------------------------
    0°         0          Right (3 o'clock)
    90°        p/2        Top (12 o'clock)
    180°       p          Left (9 o'clock)
    270°       3p/2       Bottom (6 o'clock)
    360°       2p         Back to right


CLOCKWISE vs COUNTER-CLOCKWISE:
    Standard (counter-clockwise): 0° ? 90° ? 180° ? 270°
    Clockwise: Use negative angles or reverse start/end


ARC EXAMPLES:
    DrawArc(cx, cy, r, 0, 180, n)     ? Top semicircle
    DrawArc(cx, cy, r, 180, 360, n)   ? Bottom semicircle
    DrawArc(cx, cy, r, 0, 90, n)      ? Top-right quarter
    DrawArc(cx, cy, r, 90, 180, n)    ? Top-left quarter
    DrawArc(cx, cy, r, 180, 270, n)   ? Bottom-left quarter
    DrawArc(cx, cy, r, 270, 360, n)   ? Bottom-right quarter

================================================================================
SMOOTHNESS: NUMBER OF SEGMENTS
================================================================================

TRADEOFF:
    More segments = Smoother curve BUT slower rendering
    Fewer segments = Faster BUT blocky appearance

GUIDELINES:
    3-6 segments:   Polygon shapes (triangle, hexagon)
    8-12 segments:  Rough circle, fast rendering
    20-30 segments: Acceptable circle for small radii
    50-100 segments: Smooth circle (recommended for most uses)
    200+ segments:  Very smooth, only needed for large radii or zoom

FORMULA FOR SEGMENT COUNT:
    A rough guideline: numSegments ˜ 2 × p × radius / pixelTolerance
    
    Example:
        radius = 50 pixels, tolerance = 2 pixels
        numSegments ˜ 2 × 3.14 × 50 / 2 ˜ 157
        
    But 50-100 is usually good enough!


ADAPTIVE SEGMENTATION (advanced):
    Adjust segments based on radius:
        int numSegments = max(20, (int)(2 * PI * radius / 3));
    
    Small circles get fewer segments, large circles get more.

================================================================================
COMBINING SHAPES: TIPS
================================================================================

CLOUDS:
    - Use 3-5 overlapping circles
    - Vary sizes (different radii)
    - Slightly offset centers
    - Draw filled first, then outlines

FLOWERS:
    - Center circle for middle
    - 5-8 circles around edge for petals
    - Use arcs for half-petals

RAINBOW:
    - Multiple concentric arcs with different colors
    - Red (outermost), Orange, Yellow, Green, Blue, Indigo, Violet (innermost)

MOUNTAINS:
    - Use triangles for basic shape
    - Add arcs for rounded peaks
    - Layer multiple mountains at different Z-depths (or Y positions)

RAIN DROPS:
    - Use teardrop function
    - Draw multiple at different positions
    - Animate by moving Y coordinate down over time

================================================================================
NEXT STEPS
================================================================================

Now you're ready for the exercises:

Exercise 1: Concentric circles (multiple circles, same center, different radii)
Exercise 2: Cloud using arcs (overlapping circles)
Exercise 3: Teardrop shape (special parametric equation)
Exercise 4: Complete scene (mountains, rainbow, clouds, rain)

See Lab05_All_Exercise_Solutions.cpp for complete implementations!

================================================================================
*/
