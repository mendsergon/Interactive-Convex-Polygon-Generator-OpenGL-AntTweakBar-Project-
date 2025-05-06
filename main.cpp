#include <GL/glut.h> // GLUT library
#include <vector> // Needed to store points for the polygon
#include <iostream>
#include <algorithm> // To use sort
#include <cmath> 
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

    convextPolygon.clear(); // Clear any existing polygon

    // Generate a set of unique points;
    std::vector<Point> points;
    int attempts = 0;
    const int maxAttempts = 1000; // Limit retry attempts to avoid infinite loop
    while (points.size() < n && attempts < maxAttempts) 
    {
        attempts++;
        Point p;
        p.x = rand() % 600 + 20; // Random X coordinate
        p.y = rand() % 440 + 20; // Random Y coordinate
        
        // Check if the point is already in the vector
        bool isDuplicate = false;
        for (const auto& existingPoint : points)
        {
            if (existingPoint.x == p.x && existingPoint.y == p.y)
            {
                isDuplicate = true;
                break;
            }
        }

        if (!isDuplicate)
        {
            points.push_back(p); // Add unique point
        }
        else
        {
            std::cout << "Duplicate found, retrying: (" << p.x << ", " << p.y << ")\n";
        }
    }

    // If number of attempts exeeded the max limit, print an error
    if (attempts >= maxAttempts)
    {
        std::cerr << "Error: Could not generate " << n << " unique points after " << maxAttempts << " attempts" << std::endl;
    }

    // Find the pivot
    Point pivot = points[0];
    for (const auto& p : points)
    {
        if (p.y < pivot.y || (p.y == pivot.y && p.x < pivot.x))
        {
            pivot = p;
        }
    }

    // Sort points by polar angle with respect to pivot
    std::sort(points.begin(), points.end(), [&pivot](const Point& p1, const Point& p2)
    {
        float angle1 = atan2(p1.y - pivot.y, p1.x - pivot.x);
        float angle2 = atan2(p2.y - pivot.y, p2.x - pivot.x);
        return angle1 < angle2;
    });

    // Create the convex hull using Graham's Scan
    std::vector<Point> hull;
    for (const auto& point : points)
    {
        // While the current point causes a right turn, remove the last point
        while (hull.size() >= 2 && crossProduct(hull[hull.size() - 2], hull.back(), point) <= 0)
        {
            hull.pop_back();
        }
        hull.push_back(point);
    }

    // Ensure the polygon has exactly 'n' sides
    while(hull.size() < n) 
    {
        generateConvexPolygon(n);
        return;
    }

    // If hull has more than 'n' sides
    if (hull.size() > n)
    {
        hull.resize(n);
    }

    // Hull contains the points that form the convex polygon
    convextPolygon = hull;
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
    generateConvexPolygon(100);

    // Start main loop
    glutMainLoop(); 
}