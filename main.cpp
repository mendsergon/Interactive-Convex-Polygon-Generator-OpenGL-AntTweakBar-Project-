#include <GL/glut.h>
#include <AntTweakBar.h>
#include <vector> 
#include <iostream>
#include <algorithm> 
#include <cmath>
#include <random>
#include <ctime>
#include "convex_polygon_functions.h"


// Global variables for number of sides and the polygon's points
int currentNumSides = 3;
std::vector<Point> convextPolygon;

// Main Function
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

    // Setup GLUI
    setupAntTweakBar();

    // Generate the initial polygon
    generateConvexPolygon(3);

    glutIdleFunc([](){ glutPostRedisplay(); });


    // Start main loop
    glutMainLoop(); 

    return 0;
}