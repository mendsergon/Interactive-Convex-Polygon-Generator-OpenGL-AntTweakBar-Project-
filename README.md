## Interactive Convex Polygon Generator (OpenGL + AntTweakBar Project)

### Project Summary

This C++ project generates and renders **random 2D convex polygons** using **OpenGL** and allows interactive control over the polygon's number of sides through an **AntTweakBar GUI**. The program uses **computational geometry algorithms** for polygon construction and a **scanline filling algorithm** for rasterization. Users can observe both the polygon's outline and filled area, along with vertex positions, dynamically updated as parameters change.

---

### Core Features

* Generate convex polygons with **user-specified number of sides** (3–1000).
* Interactive GUI slider to dynamically modify polygon sides.
* Efficient polygon filling using **scanline rasterization** with **Edge Tables (ET)** and **Active Edge Tables (AET)**.
* Draw polygon outlines and vertices in distinct colors.
* Randomized generation ensures convexity and uniqueness of each polygon.
* Real-time updates via **GLUT** event handling and **AntTweakBar** controls.

---

### Key Methods and Algorithms

#### 1. **Convex Hull Computation**

* Uses **Andrew’s monotone-chain algorithm** to compute the minimal convex polygon containing a set of points.
* Steps:

  1. **Lexicographic sorting** of points by (x, y).
  2. Construction of **lower and upper hulls** using **cross product** to determine left/right turns.
  3. Concatenation of hulls yields the convex polygon in counter-clockwise order.

**Mathematical foundation:**
Cross product ( \vec{OA} \times \vec{OB} = (A_x - O_x)(B_y - O_y) - (A_y - O_y)(B_x - O_x) ) determines turn direction: positive → left, negative → right.

---

#### 2. **Polygon Generation with Edge Subdivision**

* Generates **random points** within the canvas and computes their convex hull.
* If the hull has fewer vertices than desired, edges are **subdivided** proportionally:
  [
  P_{\text{new}} = (1-t) \cdot P_i + t \cdot P_{i+1}, \quad t \in \left(0,1\right)
  ]
* Ensures exactly ( n ) vertices while maintaining convexity.

---

#### 3. **Scanline Polygon Filling**

* Implements **scanline rasterization** for efficient polygon filling.
* **Edge Table (ET):** Stores edges sorted by minimum y-coordinate.
* **Active Edge Table (AET):** Tracks edges intersecting the current scanline.
* **Filling procedure:** For each scanline, intersections of edges with the scanline are computed; pixels between pairs of intersections are filled.

**Incremental update:** Edge intersection x-values are updated for each scanline using slope:
[
x_{\text{next}} = x_{\text{current}} + \frac{\Delta x}{\Delta y}
]

---

#### 4. **Interactive GUI**

* Uses **AntTweakBar** to adjust polygon sides dynamically.
* GUI events are routed to **GLUT callbacks**, allowing **real-time visualization**.

---

#### 5. **Rendering**

* **Vertices:** Rendered as blue points.
* **Outline:** Rendered in black using **GL_LINE_LOOP**.
* **Filled area:** Rendered with red color using scanline filling algorithm.

---

### Skills Demonstrated

* **Computational geometry:** convex hulls, cross-product for turn determination.
* **Polygon rasterization:** scanline algorithm with edge tables.
* **OpenGL rendering:** points, lines, and polygons.
* **GUI programming:** real-time interactive controls using AntTweakBar.
* **Randomized procedural generation:** reproducible convex polygons with controlled vertex count.
* **Event-driven programming:** integrating GLUT callbacks with GUI updates.
* **C++ programming:** modular design, STL usage, and vector manipulation.

---

### File Overview

| File Name                      | Description                                                                                            |
| ------------------------------ | ------------------------------------------------------------------------------------------------------ |
| **main.cpp**                   | Entry point: initializes OpenGL, GUI, generates polygon, and runs main loop.                           |
| **convex_polygon_functions.h** | Implements polygon generation, convex hull, scanline filling, cross-product math, and GUI integration. |

---

### How to Compile and Run

1. Install **OpenGL**, **GLUT**, and **AntTweakBar** libraries.
2. Compile with a C++ compiler:

```bash
g++ main.cpp -o convex_polygon -lGL -lGLU -lglut -lAntTweakBar
```

3. Run the program:

```bash
./convex_polygon
```

4. Use the **GUI slider** to change the number of polygon sides in real time. Observe filled polygons, outlines, and vertices.

---
