#include <string>
#include <iostream>
#include "CPolytope.h"
#include "errorTest.h"
#include "Point.h"
#include <vector>
#include <iomanip>

// #include "../data_generation/data_generation.h"

   int main(int argc, char* argv[]) {
    // 아래 switch문 메뉴 선택
    int inVal = std::stoi(argv[1]); // *(argv[1]) - '0';
    // 최종 데이터 저장될 디렉토리
    auto argv2 = argv[2];
    std::string dir = argv[2];
    // 쿼리 속도 체크(T), 쿼리 오차 체크(F)
    bool speedFlag = true;
    if (std::string(argv[3]) == "F") speedFlag = false;
    // grid step size
    std::string epsilonStr = argv[4];
    double epsilon = std::stod(epsilonStr);
    // 입력으로 사용할 데이터 셋의 인덱스
    int useDataSetId = std::stoi(argv[5]);
    // 입력할 polytope의 수
    int numPolytopes = std::stoi(argv[6]);
    std::cout << "read arguments" << std::endl;
    //cout << "menu\n";
    //cout << "0. auto test\n"; // data generation\n";
    //cout << "1. generate point sites\n";
    //cout << "2. read point sites\n";
    //cout << "3. read polytopes\n";
    //cout << "4. kNN query\n";
    //cout << "Enter: ";

    // manual 입력

    inVal = 0;
    // dir = "C:\\Users\\HWI\\Documents\\epsGraphTest";
	dir = "C:\\Users\\HWI\\Documents\\StarLab\\epsGraphTest";
	// Data\\000\\polytopes\\0"
    speedFlag = false;
    useDataSetId = 0;

    switch (inVal) {
    case 0:
        // return;
        // autoTest(dir, epsilon, speedFlag, useDataSetId);
        autoTestConvex(dir, epsilon, speedFlag, useDataSetId);
        break;
    case 1:
        // generatePoints();
        break;
    case 2:
        // readPoints();
        break;
    case 3:
        // readPolytopes();
        break;
    case 4:
        break;
    }
    std::cout << "escaped from switch statement";
    return 0;
}

/*

int main2() {
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

        // std::vector<Point> vertices2D;
        // std::vector<double> v1 = {0.0, 0.0}, v2 = { 1.0, 0.0 }, v3 = { 1.0, 1.0 }, v4 = { 0.0, 1.0 };

        Point p1(v1), p2(v2), p3(v3), p4(v4);

        vertices2D.push_back(p1); vertices2D.push_back(p2);
        vertices2D.push_back(p3); vertices2D.push_back(p4);

        // vertices2D.push_back(Point({ 0.0, 0.0 }));
        // vertices2D.push_back(Point({ 1.0, 0.0 }));
        // vertices2D.push_back(Point({ 1.0, 1.0 }));
        // vertices2D.push_back(Point({ 0.0, 1.0 }));

        // Define facets (edges) as pairs of vertex indices
        std::vector<std::vector<int>> facets2D = {
            {0, 1}, // Edge from Vertex 0 to Vertex 1
            {1, 2}, // Edge from Vertex 1 to Vertex 2
            {2, 3}, // Edge from Vertex 2 to Vertex 3
            {3, 0}  // Edge from Vertex 3 to Vertex 0
        };

        // Create the 2D square polytope
        CPolytope square2D(2, vertices2D, facets2D);

        std::vector<double> sv1 = { -0.5, 0.5 }, sv2 = { 0.5, 0.5 };// , v3 = { 1.0, 1.0 }, v4 = { 0.0, 1.0 }, v2 = { 1.0, 0.0 }, v3 = { 1.0, 1.0 }, v4 = { 0.0, 1.0 };
        
        // std::vector<double> v1 = { 0.0, 0.0 }, v2 = { 1.0, 0.0 }, v3 = { 1.0, 1.0 }, v4 = { 0.0, 1.0 }, v2 = { 1.0, 0.0 }, v3 = { 1.0, 1.0 }, v4 = { 0.0, 1.0 };

        // Define line segments to test
        std::vector<std::pair<Point, Point>> line_segments2D = {
            // {sv1, sv2}
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
*/