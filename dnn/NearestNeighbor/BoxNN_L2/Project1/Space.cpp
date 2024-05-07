
#include "Space.h"
#include "Box.h"

Space::Space(int n, vector<Point> vert, vector<Box> boxes) {
    this->d = n;
    this->vertices = vert;
    this->Boxes = boxes;
    cal_rmin();
    this->rmin = 0.1;
    gen_SteinerPoint();
    
}
/*


Space::Space(const Space& p)
{
    this->d=p.d;
    this->vertices = p.vert;
    this->Boxes = p.boxes;
    cal_rmin();
    this->rmin = 0.1;
    gen_SteinerPoint();
}


*/

Space::~Space(){
}

void  Space::set_Space(int, vector<Point>, vector<Box>) {

}


template<typename T>
std::vector<std::vector<T>> Cartesian_Product(const std::vector<std::vector<T>>& items, int a, int b) {
    
    std::vector<std::vector<double>> new_items;
    for (int i = 0; i < items.size(); ++i) {
        if (i != a && i != b) {
            new_items.push_back(items[i]);
        }
    }

    int length = new_items.size();
    std::vector<int> indexes(length, 0);

    std::vector<std::vector<double>> result;

    while (true) {
        std::vector<double> arr(length);
        for (int i = 0; i < length; i++) {
            arr[i] = new_items[i][indexes[i]];
        }
        result.push_back(arr);

        int row = length - 1;
        indexes[row]++;

        while (indexes[row] == new_items[row].size()) {
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

    std::vector<vector<double>> combinations;//���� ouput�� index�� �����ϴ� �迭

    std::vector<bool> v(this->d);
    std::fill(v.end() - 2, v.end(), true);

    do {
        vector<double> temp;
        for (auto i = 0; i < d; ++i) {
            if (v[i]) {
                //combinations.push_back({ i + 1, i + 2 });//ũ�Ⱑ 2�̱� ������, dC2��
                temp.push_back(i);
            }
        }
        combinations.push_back(temp);
    } while (std::next_permutation(v.begin(), v.end()));
    std::vector<std::pair<double, double>> comb;
    for (auto &c : combinations) {
        comb.emplace_back(c[0], c[1]);
    }
    return comb;

};

std::vector<std::vector<double>> Space::gen_SteinerPoint() {

    std::cout << "1" << endl;
    std::vector<std::pair<int, int>> combination = this->Combination();
    
    // 
    std::vector<std::vector<double>> answer_1;

    // 최종 정답
    std::vector<std::vector<double>> answer_3;

    for (auto box : Boxes) {
        std::vector<std::vector<double> > Range = box.generate_epsilon(this->rmin, this->epsilon);
        //decltype(Range) new_Range;

       // std::copy(Range.begin(), Range.end(), std::back_inserter(new_Range));


        for (auto space : combination) {
            std::vector<std::vector<double>> fixed_Range = Range;

            int a = space.first;
            int b = space.second;

            std::vector<std::vector<double>> answer = Cartesian_Product(fixed_Range, a, b);
            //.insert(answer_1.end(), answer.begin(), answer.end());

            //double min_a = box.min[a];
            double min_a = box.min[a];
            double min_b = box.min[b];
            double max_a = box.max[a];
            double max_b = box.max[b];

            std::vector<std::vector<double>> answer_2;

            answer_2 = { {min_a,min_b},{min_a,max_b},{max_a,min_b},{max_a,max_b} };

            // for (int i = 0; i < answer_1)

            for (auto& k : answer_2) {

                std::vector<std::vector<double>> answer_1 = answer; //

                for (int i = 0; i < answer_1.size(); i++) {
                   
                    answer_1[i].insert(answer_1[i].begin()+a, k[0]);
                    answer_1[i].insert(answer_1[i].begin()+b, k[1]);
                    answer_3.push_back(answer_1[i]);

                }

            }
        }
    }
    
    return answer_3;

};


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

void Space::visibility_graph() {
    this->adj_list.clear();
    for (long long int i = 0; i < size(this->vertices); i++) {
        vector<pair<long long int, double>> temp;
        for (long long int j = 0; j < size(this->vertices); j++) {
            bool intersect = false;
            for (auto box : this->Boxes) {
                if (box.intersect(vertices[i], vertices[j])) { intersect = true; break; }
            }
            if (intersect) { break; }
            temp.push_back(make_pair(j,vertices[i].dist(vertices[j])));
        }
        this->adj_list.push_back(temp);
    }   
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


//v.insert(it + 2, v2.begin(), v2.end());
//front���� end �ٲٸ� ok
//for (int i = 0; i < space.size(); i++) {
//answer.insert(answer.begin() + a, temp_1.begin(), temp_1.end());
//answer.insert(answer.begin() + b, temp_1.end(), temp_1.end() + 1);
//std::vector<std::vector<double>> answer = Cartesian_Product(Range);
//answer.insert(answer.begin(), Range.begin(), Range.end());
//return answer;

/*
//std::vector<std::vector<int>> answer = MyProduct(variableVectors);
std::vector<std::vector<double>> Space::gen_SteinerPoint() {
    cout << "1" << endl;
    std::vector<std::pair<double, double>> combination=this->Combination();

    cout << "1" << endl;
    for (auto box : Boxes) {
        std::vector<std::vector<double> > Range = box.generate_epsilon(this->rmin, this->epsilon);

        for (auto space : combination) {
            int a = space.first - 1;
            int b = space.second - 1;
            double min_a = Range[a].front()-1;
            double min_b = Range[b].front()-1;
            vector<double> temp_1 = { min_a, min_b };

            Range.erase(Range.begin()+a-1);
            Range.erase(Range.begin()+b-1);

            std::vector<std::vector<double>> answer = Cartesian_Product(Range);


            Range.insert(Range.begin() + a-1, temp_1.begin()-1, temp_1.end()-1);
            Range.insert(Range.begin() + b-1, temp_1.end()-1, temp_1.end());
            return answer;

        };

    };

};


vector<vector<double>> Space::deep_copy() {

    Box b(3, { 0,0,0 }, {1,1,1});
    vector<vector<double>> vec =b.generate_epsilon(this->rmin, this->epsilon);
    vector<vector<double>> new_range;

    vector<double> arr;
    int NS = d;
    double* new_range = (double*)calloc(NS, sizeof(double));

        for (int i = 0; i < d; i++) {

            std::copy(Range.begin(), Range.end(), std::back_inserter(y));

            new_range.push_back(arr);
        };


}

*/

/*
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



                    //std::vector<double> temp_1 = { min_a, min_b };

            // Remove Range[a] and Range[b]
            //fixed_Range.erase(fixed_Range.begin() + a);
            //fixed_Range.erase(fixed_Range.begin() + b);

            // Calculate Cartesian Product for newRang

*/
