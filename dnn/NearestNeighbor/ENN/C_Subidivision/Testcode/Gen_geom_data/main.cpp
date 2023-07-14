#include "Gen_geom_data.h"
#include "../../DCEL/Point.h"
#include "../../DCEL/Polygon.h"

int main(){
    Gen_geom_data gen_geom(Point(0,0), Point(1000,1000));
    std::vector<Point> pts = gen_geom.gen_points_uniform(10);
    return 0;
}