The purpose of the project was to develop a program in C++ and OpenGL for the
creation of arbitrary convex polygons, with the possibility of dynamic modification
of the number of sides by the user in real time. The program calculates
the polygons with mathematical calculations, without using predefined commands
OpenGL, except for points and lines. The program generates random points
and calculates the convex contour of these points using the method
"monotone-chain". A basic calculation is the calculation of the "cross prod-
uct” of two vectors, to check the direction of rotation and ensure
the correct order of points. The use of the library was initially planned
GLUI, but due to inability to install AntTweakBar was chosen for management
of the polygon parameters by the user.

For a more detailed description read: ConvexPolygons_English_Paper.pdf
