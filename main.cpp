#include <GL/glut.h> // GLUT library
#include <vector> // Needed to store points for the polygon
#include <iostream>
#include <algorithm> // To use sort
#include <cmath>
#include <random>
#include <ctime>
#include "libs/glui/include/GL/glui.h" // GLUI library

struct  Point // Store x and y coordinates of each point of the polygon
{
    float x, y;
};

// Global variables for number of sides and the polygon's points
int currentNumSides = 5;
std::vector<Point> convextPolygon;

// GLUI objects
GLUI *glui;
GLUI_Scrollbar * slider;
float sliderValue = 50.0f;

// Function declarations
void generateConvexPolygon(int n);
float crossProduct(const Point& o, const Point& a, const Point& b);
void getBoundingBox(const std::vector<Point>& Polygon, float& minX, float& maxX, float& minY, float& maxY);
bool isInside(const Point& p, const Point& edgeStart, const Point& edgeEnd);
void fillPolygon(const std::vector<Point>& Polygon);
void display();
void onNumSidesChanged(int newSides);
void setupGLUI();

// Function to generate the polygon with 'n' sides
void generateConvexPolygon(int n) 
{
    if (n < 3)
    {
        std::cerr << "Error: A polygon requires at least 3 points." << std::endl;
        convextPolygon.clear();
        return;
    }

    // Generate n unique random points
    std::vector<Point> Points;
    Points.reserve(n);
    std::mt19937 rng((unsigned)std::time(nullptr));
    std::uniform_int_distribution<int> dx(20, 619), dy(20, 459);
    while ((int)Points.size() < n)
    {
        Point p{float(dx(rng)), float(dy(rng))};
        bool dup = false;
        for (auto &q : Points)
        {
            if (q.x == p.x && q.y == p.y)
            {
                dup = true;
                break;
            } 
        }
        if (!dup) Points.push_back(p);
    }

    // Compute centroid
    Point centroid{ 0.0f, 0.0f};
    for (auto &p : Points)
    {
        centroid.x += p.x;
        centroid.y += p.y;
    }
    centroid.x /= n;
    centroid.y /= n;

    // Sort by polar angle around centroid
    std::sort(Points.begin(), Points.end(), [&](const Point &A, const Point &B)
    {
        float a1 = std::atan2(A.y - centroid.y, A.x - centroid.x);
        float a2 = std::atan2(B.y - centroid.y, B.x - centroid.x);
        return a1 < a2;
    });

    // Convexity check
    bool allLeft = true;
    for (int i =0; i < n; ++i)
    {
        if (crossProduct(Points[i], Points[(i + 1) % n], Points[(i + 2) % n]) <= 0)
        {
            allLeft = false;
            break;
        }
    }
    if (!allLeft)
    {
        std::reverse(Points.begin(), Points.end());
    }

    // Store result as convext polygon
    convextPolygon = std::move(Points);
}

// Compute the cross product of the two vectors to check the turn direction
float crossProduct(const Point& o, const Point& a, const Point& b)
{
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

// Find the bounding box
void getBoundingBox(const std::vector<Point>& Polygon, float& minX, float& maxX, float& minY, float& maxY)
{
    minX = maxX = Polygon[0].x;
    minY = maxY = Polygon[0].y;

    for (const Point& p : Polygon)
    {
        if (p.x < minX) minX = p.x;
        if (p.x > maxX) maxX = p.x;
        if (p.y < minY) minY = p.y;
        if (p.y > maxY) maxY = p.y;
    }
}

// Determine if a pixel is inside the polygon or not
bool isInside(const Point& p, const Point& edgeStart, const Point& edgeEnd)
{
    const float epsilon = 0.01f; // small tolerance
    return (edgeEnd.x - edgeStart.x) * (p.y - edgeStart.y) - (edgeEnd.y - edgeStart.y) * (p.x - edgeStart.x) >= 0;
}

// Loop through each pixel within the bounding box of the convex polygon and check if it is inside the polygon
void fillPolygon(const std::vector<Point>& Polygon)
{
    // Get bounding box
    float minX, maxX, minY, maxY;
    getBoundingBox(Polygon, minX, maxX, minY, maxY);

    // Cast bounds to int for pixel stepping
    int iMinX = static_cast<int>(std::floor(minX));
    int iMaxX = static_cast<int>(std::ceil(maxX));
    int iMinY = static_cast<int>(std::floor(minY));
    int iMaxY = static_cast<int>(std::ceil(maxY));

    // Loop through each pixel
    for (int y = iMinY; y <= iMaxY; ++y)
    {
        for (int x = iMinX; x <= iMaxX; ++x)
        {
            // Sample at pixel center
            Point p = { x+ 0.5f, y + 0.5f};

            bool inside = true;

            for (size_t i = 0; i < Polygon.size(); ++i)
            {
                const Point& start = Polygon[i];
                const Point& end = Polygon[(i + 1) % Polygon.size()];

                if (!isInside(p, start, end))
                {
                    inside = false;
                    break;
                }
            }

            if (inside)
            {
                // Draw pixel
                glBegin(GL_POINTS);
                glVertex2f(p.x, p.y);
                glEnd();
            }
        }
    }
}

// Callback function for when the slider is changed
void onNumSidesChanged(int newSides) 
{

}

// Function to set up GLUI controls
void setupGLUI() 
{

}

// Function to display the polygon
void display() 
{
    // Set background to white and clear the screen
    glClearColor(1, 1, 1, 0); 
    glClear(GL_COLOR_BUFFER_BIT); 

    // Set polygon color (red)
    glColor3f(1.0f, 0.8f, 0.8f); 
    glPointSize(1.5f);

    // Fill the polygon manually
    fillPolygon(convextPolygon);


    // Draw polygon outline (black)
    glColor3f(0.0f, 0.0f, 0.0f); // Black edges
    glLineWidth(1.5f);
    glBegin(GL_LINE_LOOP);
    for (const Point& p : convextPolygon)
    {
        glVertex2f(p.x, p.y);
    }
    glEnd();

    // Draw polygon vertices (blue dots)
    glColor3f(0.0f, 0.0f, 1.0f); // Blue points
    glPointSize(6.0);
    glBegin(GL_POINTS);
    for (const Point& p : convextPolygon)
    {
        glVertex2f(p.x, p.y);
    }
    glEnd();

    glFlush(); // Execute pending commands
}

int main(int argc, char **argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);  
    glutInitWindowPosition(50, 50); 
    glutInitWindowSize(640, 480); 
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB); 
    glutCreateWindow("Convex Polygons"); 

    // Set up OpenGL projection
    glMatrixMode(GL_PROJECTION); 
    gluOrtho2D(0, 640, 0, 480); 

    // Set up display callback
    glutDisplayFunc(display); 

    // Generate the initial polygon
    generateConvexPolygon(40);

    // Start main loop
    glutMainLoop(); 
}