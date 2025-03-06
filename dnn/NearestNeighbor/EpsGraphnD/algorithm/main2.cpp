#include <string>
#include <iostream>
#include "CPolytope.h"
#include "errorTest.h"
#include "Point.h"
#include <vector>
#include <iomanip>

// #include "../data_generation/data_generation.h"

   int main(int argc, char* argv[]) {

    // input / ouput 경로
    std::string dir = argv[1];

    // 쿼리 속도 체크(T), 쿼리 오차 체크(E), 2차원 visualize(V)
    std::string testType = argv[2];

    // 입력으로 사용할 데이터 셋의 인덱스
    int startID = std::stoi(argv[3]);
    int endID = std::stoi(argv[4]);

    // input site 수
    int numSites = std::stoi(argv[5]);
    // query 수
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