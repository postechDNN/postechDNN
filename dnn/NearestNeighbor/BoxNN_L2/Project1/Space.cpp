#include "Space.h"
#include "Box.h"

Space::Space(int n, vector<Point> vert, vector<Box> boxes) {
    this->d = n;
    this->vertices = vert;
    this->Boxes = boxes;
    this->rmin = 0.1;
    gen_SteinerPoint();
    cal_rmin();
}

Space::~Space(){
}

void  Space::set_Space(int, vector<Point>, vector<Box>) {

}


template<typename T>
std::vector<std::vector<T>> Cartesian_Product(const std::vector<std::vector<T>>& items) {
    int length = items.size();
    std::vector<int> indexes(length, 0);

    std::vector<std::vector<T>> result;

    while (true) {
        std::vector<T> arr(length);
        for (int i = 0; i < length; ++i) {
            arr[i] = items[i][indexes[i]];
        }
        result.push_back(arr);

        int row = length - 1;
        indexes[row]++;
        while (indexes[row] == items[row].size()) {
            if (row == 0)
                return result;
            indexes[row] = 0;
            row--;
            indexes[row]++;
        }
    }
}


//std::vector<std::pair<double, double>>Space:: Combination(int d) {
std::vector<std::pair<double, double>> Space:: Combination() {

    std::vector<std::pair<double, double>> combinations;//���� ouput�� index�� �����ϴ� �迭

    std::vector<bool> v(this->d);
    std::fill(v.end() - 2, v.end(), true);

    do {
        for (auto i = 0; i < d; ++i) {
            if (v[i]) {
                combinations.push_back({ i + 1, i + 2 });//ũ�Ⱑ 2�̱� ������, dC2��
            }
        }
    } while (std::next_permutation(v.begin(), v.end()));

    return combinations;

};


//std::vector<std::vector<int>> answer = MyProduct(variableVectors);
std::vector<std::vector<double>> Space::gen_SteinerPoint() {
    cout << "1" << endl;
    std::vector<std::pair<double, double>> combination=this->Combination();

    cout << "1" << endl;
    for (auto box : Boxes) {
        std::vector<std::vector<double> > Range = box.generate_epsilon(this->rmin, this->epsilon);

        for (auto space : combination) {
            int a = space.first - 1;
            //Range[a];
            for (int i = 0; i < Range[a].size(); i++) {
                double k = Range[a].front();
                Range[a][i] = k;
            };

            int b = space.second - 1;
            //Range[b];
            for (int i = 0; i < Range[b].size(); i++) {
                double k = Range[b].front();
                Range[b][i] = k;

            };

            std::vector<std::vector<double>> answer = Cartesian_Product(Range);
            //answer.insert(answer.begin(), Range.begin(), Range.end());
            return answer;
        };
        
    };
        
};


/*
vector<Point>& a = box.min;
vector<Point>& b = box.max;
*/

// std::vector<double> 
// auto Range = generate_epsilon(min, max);
// i->combinations;
// Range[i]=
//variableVectors.push_back(vectors[i]);

    //vector<Point> Box::vertices;
    //x.Boxgenste(rmin)

/*
   std::vector<double> min_values(arr.size());
    std::vector<double> max_values(arr.size());


double min_i = *std::min_element(arr[i].begin(), arr[i].end());
double max_i = *std::max_element(arr[i].begin(), arr[i].end());
double min_j = *std::min_element(arr[j].begin(), arr[j].end());
double max_j = *std::max_element(arr[j].begin(), arr[j].end());

*/
void Space::cal_rmin() {
    std::vector<double> results;
    double sum = 0.0;
    double diff;

    // Box vs Box
    for (int i = 0; i < Boxes.size(); i ++) {
        for (int j = i + 1; j < Boxes.size(); j ++) {
            sum = 0.0;

            for (int k = 0; k < d; k ++) {
                double a_ik = Boxes[i].min[k];
                double b_ik = Boxes[i].max[k];

                double a_jk = Boxes[j].min[k];
                double b_jk = Boxes[j].max[k];

                if (std::max(a_ik, a_jk) <= std::min(b_ik, b_jk)) {
                    diff = 0.0;
                } else {
                    if (b_ik < a_jk) {
                        diff = b_ik - a_jk;
                    } else if (b_jk < a_ik) {
                        diff = b_jk - a_ik;
                    }
                }

                sum += diff * diff;
            }

            results.push_back(sqrt(sum));
        }
    }


    // Point vs Point
    for (int i = 0; i < vertices.size(); i ++) {
        for (int j = i + 1; j < vertices.size(); j ++) {
            sum = 0.0;

            for (int k = 0; k < d; k ++) {
                double x_k = vertices[i].xs[k];
                double y_k = vertices[j].xs[k];

                diff = x_k - y_k;

                sum += diff * diff;
            }

            results.push_back(sqrt(sum));
        }
    }


    // Box vs Point
    for (int i = 0; i < Boxes.size(); i ++) {
        for (int j = 0; j < vertices.size(); j ++) {
            sum = 0.0;

            for (int k = 0; k < d; k ++) {
                double a_ik = Boxes[i].min[k];
                double b_ik = Boxes[i].max[k];

                double a_jk = vertices[j].xs[k];
                double b_jk = vertices[j].xs[k];

                if (std::max(a_ik, a_jk) <= std::min(b_ik, b_jk)) {
                    diff = 0.0;
                } else {
                    if (b_ik < a_jk) {
                        diff = b_ik - a_jk;
                    } else if (b_jk < a_ik) {
                        diff = b_jk - a_ik;
                    }
                }

                sum += diff * diff;
            }

            results.push_back(sqrt(sum));
        }
    }


    double rmin = *std::min_element(results.begin(), results.end());

    cout << "rmin: " << rmin << endl;
};

void Space::add_Box(Box) {

}
void Space::del_Box(int) {

}
void Space::add_vert(Point) {

}
void Space::del_vert(int) {

}
void Space::Dijkstra(Point query) {

}
void Space::print_knn(int) {

}