#include <string>
#include <iostream>
#include "CPolytope.h"
#include "Point.h"
#include <vector>
#include <iomanip>

void print_halfplane(HalfPlane& hp, int dim) {
    std::cout << "HalfPlane equation: ";
    for (int i = 0; i < dim; ++i) {
        std::cout << std::fixed << std::setprecision(3) << hp.getCoeff(i) << "x" << (i + 1) << " + ";
    }
    std::cout << hp.getCoeff(dim) << " <= 0";
    if (hp.is_boundary) {
        std::cout << " (Boundary)";
    }
    std::cout << std::endl;
}

// Function to print intersection results
void test_intersection(CPolytope& polytope, Point p, Point q, int test_num) {
    bool intersects = polytope.is_intersect(p, q);
    std::cout << "Test Intersection " << test_num << ": ";
    std::cout << "Line Segment [(";
    for (int i = 0; i < p.getsize(); ++i) {
        std::cout << p.getx(i);
        if (i < p.getsize() - 1) std::cout << ", ";
    }
    std::cout << ") to (";
    for (int i = 0; i < q.getsize(); ++i) {
        std::cout << q.getx(i);
        if (i < q.getsize() - 1) std::cout << ", ";
    }
    std::cout << ")] ";
    if (intersects) {
        std::cout << "intersects the polytope." << std::endl;
    }
    else {
        std::cout << "does NOT intersect the polytope." << std::endl;
    }
}

int main() {
    // Existing Test Cases for HalfPlanes
    // ... [Your existing HalfPlane test cases here] ...

    // --------------------------------------------------------------------------------
    // New Test Cases for CPolytope Intersection
    // --------------------------------------------------------------------------------

    // Example: 2D Square Polytope
    {
        std::cout << "=== 2D Square Polytope Intersection Tests ===" << std::endl;

        // Define vertices of the square (counter-clockwise order)
        std::vector<Point> vertices2D = {
            Point({0.0, 0.0}),
            Point({1.0, 0.0}),
            Point({1.0, 1.0}),
            Point({0.0, 1.0})
        };

        // Define facets (edges) as pairs of vertex indices
        std::vector<std::vector<int>> facets2D = {
            {0, 1}, // Edge from Vertex 0 to Vertex 1
            {1, 2}, // Edge from Vertex 1 to Vertex 2
            {2, 3}, // Edge from Vertex 2 to Vertex 3
            {3, 0}  // Edge from Vertex 3 to Vertex 0
        };

        // Create the 2D square polytope
        CPolytope square2D(2, vertices2D, facets2D);

        // Define line segments to test
        std::vector<std::pair<Point, Point>> line_segments2D = {
            { Point({-0.5, 0.5}), Point({0.5, 0.5}) },   // Intersects (enters)
            { Point({0.5, 0.5}), Point({1.5, 1.5}) },    // Intersects (exits)
            { Point({0.2, 0.2}), Point({0.8, 0.8}) },    // Completely inside
            { Point({1.5, 1.5}), Point({2.0, 2.0}) },    // Completely outside
            { Point({0.0, 0.5}), Point({1.0, 0.5}) },    // Along an edge
            { Point({0.5, -0.5}), Point({0.5, 1.5}) },   // Passes through two edges
            { Point({-1.0, -1.0}), Point({-0.1, -0.1}) } // Completely outside
        };

        // Perform intersection tests
        int test_num = 1;
        for (const auto& segment : line_segments2D) {
            test_intersection(square2D, segment.first, segment.second, test_num++);
        }

        std::cout << std::endl;
    }

    // Example: 3D Cube Polytope
    {
        std::cout << "=== 3D Cube Polytope Intersection Tests ===" << std::endl;

        // Define vertices of the cube
        std::vector<Point> vertices3D = {
            Point({0.0, 0.0, 0.0}), // 0
            Point({1.0, 0.0, 0.0}), // 1
            Point({1.0, 1.0, 0.0}), // 2
            Point({0.0, 1.0, 0.0}), // 3
            Point({0.0, 0.0, 1.0}), // 4
            Point({1.0, 0.0, 1.0}), // 5
            Point({1.0, 1.0, 1.0}), // 6
            Point({0.0, 1.0, 1.0})  // 7
        };

        // Define facets (faces) as lists of vertex indices (counter-clockwise order)
        std::vector<std::vector<int>> facets3D = {
            {0, 1, 2, 3}, // Bottom face
            {4, 5, 6, 7}, // Top face
            {0, 1, 5, 4}, // Front face
            {1, 2, 6, 5}, // Right face
            {2, 3, 7, 6}, // Back face
            {3, 0, 4, 7}  // Left face
        };

        // Create the 3D cube polytope
        CPolytope cube3D(3, vertices3D, facets3D);

        // Define line segments to test
        std::vector<std::pair<Point, Point>> line_segments3D = {
            { Point({-0.5, 0.5, 0.5}), Point({0.5, 0.5, 0.5}) }, // Intersects (enters)
            { Point({0.5, 0.5, 0.5}), Point({1.5, 1.5, 1.5}) },  // Intersects (exits)
            { Point({0.2, 0.2, 0.2}), Point({0.8, 0.8, 0.8}) },  // Completely inside
            { Point({1.5, 1.5, 1.5}), Point({2.0, 2.0, 2.0}) },  // Completely outside
            { Point({0.0, 0.5, 0.5}), Point({1.0, 0.5, 0.5}) },  // Along an edge
            { Point({0.5, -0.5, 0.5}), Point({0.5, 1.5, 0.5}) }, // Passes through two edges
            { Point({-1.0, -1.0, -1.0}), Point({-0.1, -0.1, -0.1}) } // Completely outside
        };

        // Perform intersection tests
        int test_num = 1;
        for (const auto& segment : line_segments3D) {
            test_intersection(cube3D, segment.first, segment.second, test_num++);
        }

        std::cout << std::endl;
    }

    return 0;
}
