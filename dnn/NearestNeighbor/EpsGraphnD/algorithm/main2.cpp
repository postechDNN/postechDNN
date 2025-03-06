#include <string>
#include <iostream>
#include "CPolytope.h"
#include "errorTest.h"
#include "Point.h"
#include <vector>
#include <iomanip>

// #include "../data_generation/data_generation.h"

   int main(int argc, char* argv[]) {

    // input / ouput ���
    std::string dir = argv[1];

    // ���� �ӵ� üũ(T), ���� ���� üũ(E), 2���� visualize(V)
    std::string testType = argv[2];

    // �Է����� ����� ������ ���� �ε���
    int startID = std::stoi(argv[3]);
    int endID = std::stoi(argv[4]);

    // input site ��
    int numSites = std::stoi(argv[5]);
    // query ��
    int numQueries = std::stoi(argv[6]);

    if (testType == "S") {
        std::cout << "query speed test start\n\n";
        querySpeedTest(dir, startID, endID, numSites, numQueries);
    } 
    else if (testType == "E") {
        std::cout << "distance error test start\n\n";
        distanceSumTest(dir, startID, endID, numSites, numQueries);
    } 
    else if (testType == "V") {
        std::cout << "visualization test start\n\n";
        visualize(dir);
    }
    else { throw std::invalid_argument("test type should be T, F, or V"); }

    return 0;
}