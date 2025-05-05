#include <GL/glut.h> // GLUT library
#include <vector> // Needed to store points for the polygon
#include <iostream>
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
void display();
void onNumSidesChanged(int newSides);
void setupGLUI();

// Function to generate the polygon with 'n' sides
void generateConvexPolygon(int n) 
{

}

// Function to display the polygon
void display() 
{
    glClearColor(1, 1, 1, 0); // Sets color to clear screen
    glClear(GL_COLOR_BUFFER_BIT); // Clears screen
    glColor3f(1, 0, 0); // Chooses color for next draw
    glFlush(); // Execute pending commands
}

// Callback function for when the slider is changed
void onNumSidesChanged(int newSides) 
{

}

// Function to set up GLUI controls
void setupGLUI() 
{

}

int main(int argc, char **argv)
{
    glutInit(&argc, argv); // Enables the use  of GLUT library's functions 
    glutInitWindowPosition(50, 50); // Defines where the window will pop up on the screen
    glutInitWindowSize(640, 480); // Defines window size of the application with pixels
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB); // Defines display settings
    glutCreateWindow("Convex Polygons"); // Creates window of application
    glMatrixMode(GL_PROJECTION); // Sets the register we wish to modify
    gluOrtho2D(0, 50, 0, 50); // Defines parallel view
    glutDisplayFunc(display); // Sets the function that will be called every time it is required to map the scene
    glutMainLoop(); // Enables event listener loop
}