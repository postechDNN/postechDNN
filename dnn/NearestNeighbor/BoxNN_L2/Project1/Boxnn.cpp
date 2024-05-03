#include "Boxnn.h"
#include <queue>
/*
int main() {
	std::vector<std::pair<Point, Point> > boxes;
	std::vector<Point> pts;
	Point* b11 = new Point(5, 10.);
	Point* b12 = new Point(5, 10.);
	std::pair<Point, Point> b1 = std::make_pair(b11,b12);
	boxes.push_back(b1);
	Point* b21 = new Point(5, 10.);
	Point* b22 = new Point(5, 10.);
	std::pair<Point, Point> b2 = std::make_pair(b21,b22);
	boxes.push_back(b2);
	Point* p1 = new Point(5, 5.);
	pts.push_back(p1);
	Point* p2 = new Point(5, 5.);
	pts.push_back(p2);
	Point* p3 = new Point(5, 5.);
	pts.push_back(p3);
	Point* q1 = new Point(5, 5.);
    // Point* b11 = new Point({-1., -1.});
	// Point* b12 = new Point({1.,1.});
	// std::pair<Point, Point> b1 = std::make_pair(b11,b12);
    // boxes.push_back(b1);
    // Point* p1 = new Point({2.,0.});
    // pts.push_back(p1);
    // Point* q1 = new Point({-2.,0.});
	BoxNN B(boxes,pts, q1);
    B.Dijkstra();
    // for (int i=0;i<B.tot_num;i++){
    //     std::vector<int> v = B.int2grid(i);
    //     std::cout<<i<<": ";
    //     for (int j=0;j<B.n;j++){
    //         std::cout<<v[j]<<" ";
    //     }
    //     int check = B.grid2int(v);
    //     std::cout<<check;
    //     std::cout<<" " << std::endl;
    //     std::cout<<B.dist[i]<<std::endl;
    // }
    B.print_knn(3);
	return 0;
}*/

BoxNN::BoxNN(std::vector<std::pair<Point,Point>> _boxes, std::vector<Point> _pts, Point _q){
    n = _q.n;
    boxes = _boxes;
    pts = _pts;
    query = _q;
    make_valid();
    init_grid();
}
void BoxNN::init_grid(){
    std::cout<<"dimension: "<<n<<std::endl;
    grid_val = std::vector<std::vector<double>>(n,std::vector<double>(1,0));
    for (int i=0;i<n; i++){
        grid_val[i][0] = query.getxs()[i];
    }
    for (int i=0;i<pts.size();i++){
        for (int j=0;j<n;j++){
            grid_val[j].push_back(pts[i].getxs()[j]);
        }
    }
    for (int i=0;i<boxes.size();i++){
        for (int j=0;j<n;j++){
            grid_val[j].push_back(boxes[i].first.getxs()[j]);
            grid_val[j].push_back(boxes[i].second.getxs()[j]);
        }
    }
    tot_num = 1;
    for (int i=0;i<n;i++){
        sort(grid_val[i].begin(), grid_val[i].end());
        grid_val[i].erase(unique(grid_val[i].begin(), grid_val[i].end()),grid_val[i].end());
        ind_num.push_back(grid_val[i].size());
        tot_num*=grid_val[i].size();
    }

    //vector<Box> boxes;
    //struct Box {
    //    vector<std::pair<double, double>> coordinates;
    //};

    visited = std::vector<bool>(tot_num, false);
    valid = std::vector<bool>(tot_num, true);
    dist = std::vector<double>(tot_num, std::numeric_limits<double>::infinity());
    // valid grid 값별로 확인하기
for (int i=0;i<tot_num;i++){
    std::vector<int> v = int2grid(i);
    // std::cout<<i;
    for (auto box:boxes){
        if (valid[i]==false){
            break;
        }
        std::vector<double> p1 = box.first.getxs();
        std::vector<double> p2 = box.second.getxs();
        for (int k=0;k<n;k++){
            if (grid_val[k][v[k]]<=std::min(p1[k],p2[k])) break;
            if (grid_val[k][v[k]]>=std::max(p1[k],p2[k])) break;
            if (k==n-1) valid[i] = false;
        }    
    }
    }
}
bool BoxNN::is_valid(Point _p){
    std::vector<double> xs = _p.getxs();
    for (auto box:boxes){
        std::vector<double> p1 = box.first.getxs();
        std::vector<double> p2 = box.second.getxs();
        for (int k=0;k<n;k++){
            if (xs[k]<=std::min(p1[k],p2[k])) break;
            if (xs[k]>=std::max(p1[k],p2[k])) break;
            if (k==n-1) return false;
        }    
    }
    return true;
}
void BoxNN::make_valid(){
}
int BoxNN::grid2int(std::vector<int> _v){
    int i = _v[0];
    for (int j=1;j<n;j++){
        i*=ind_num[j];
        i+=_v[j];
    }
    return i;
}
std::vector<int> BoxNN::int2grid(int i){
    std::vector<int> v(n,0);
    for (int j=n-1;j>=0;j--){
        v[j] = i%ind_num[j];
        i = i/ind_num[j];
    }
    return v;
}
void BoxNN::Dijkstra(){
    std::vector<int> q_grid;
    for (int i=0;i<n;i++){
        for (int j=0;j<ind_num[i];j++){
            if (query.getxs()[i]==grid_val[i][j]){
                q_grid.push_back(j);
                break;
            }
        }
    }
    int q_ind = grid2int(q_grid);
    // std::cout<<q_ind<<std::endl;
    // visited[q_ind] = true;
    // dist[q_ind] = 0;
    std::priority_queue<std::pair<double, int>> q = {};
	q.push(std::make_pair(0.,q_ind));
    while (!q.empty()){
        //std::cout<<q.size()<<"top: "<<q.top().first<<" "<<q.top().second<<std::endl;
        if (visited[q.top().second]==true){
            q.pop();
            continue;
        }
        std::pair<double, int> t = q.top();
        q.pop();
        visited[t.second]=true;
        dist[t.second]=0-t.first;
        double cur_dist = t.first;
        std::vector<int> n_grid = int2grid(t.second);
        for (int i=0;i<n;i++){
            if (n_grid[i]!=0){
                std::vector<int> v2(n_grid);
                v2[i]-=1;
                int v2_int = grid2int(v2);
                std::vector<double> xs(n,0.);
                for (int j=0;j<n;j++){
                    if(i==j) xs[j] = (grid_val[i][n_grid[i]]+grid_val[i][n_grid[i]-1])/2.;
                    else xs[j] = grid_val[j][n_grid[j]];
                }
                if (valid[v2_int]==true &&visited[v2_int]==false&&is_valid(Point(xs))==true){
                    //std::cout<<cur_dist-grid_val[i][n_grid[i]]+grid_val[i][n_grid[i]-1]<<" "<<v2_int<<std::endl;
                    q.push(std::make_pair(cur_dist-grid_val[i][n_grid[i]]+grid_val[i][n_grid[i]-1],v2_int));
                }
            }
            if (n_grid[i]!=ind_num[i]-1){
                std::vector<int> v2(n_grid);
                v2[i]+=1;
                int v2_int = grid2int(v2);
                std::vector<double> xs(n,0.);
                for (int j=0;j<n;j++){
                    if(i==j) xs[j] = (grid_val[i][n_grid[i]]+grid_val[i][n_grid[i]+1])/2.;
                    else xs[j] = grid_val[j][n_grid[j]];
                }
                if (valid[v2_int]&&visited[v2_int]==false&&is_valid(Point(xs))==true){
                    //std::cout<<cur_dist-grid_val[i][n_grid[i]+1]+grid_val[i][n_grid[i]]<<" "<<v2_int<<std::endl;
                    q.push(std::make_pair(cur_dist-grid_val[i][n_grid[i]+1]+grid_val[i][n_grid[i]],v2_int));
                }
            }
        }
        
    }
}
void BoxNN::print_knn(int k){
    //int k = std::min(k, pts.size());
    std::priority_queue<std::pair<double, int>> q = {};
    for (auto pt:pts){
        std::vector<double> xs = pt.getxs();
        std::vector<int> ind(n,0);
        for (int i=0;i<n;i++){
            for (int j=0;j<grid_val[i].size();j++){
                if (xs[i]==grid_val[i][j]) ind[i] = j;
            }
        }
        int ind_i = grid2int(ind);
        q.push(std::make_pair(-dist[ind_i], ind_i));
    }
    for (int i=0;i<k;i++){
        std::pair<double, int> p = q.top();
        std::vector<int> ind = int2grid(p.second);
        std::vector<double> pt(n, 0.);
        std::cout<<i+1<<"th pt: { ";
        for (int j=0;j<n;j++){
            pt[j] = grid_val[j][ind[j]];
            std::cout<<grid_val[j][ind[j]]<<" ";
        }
        std::cout<<"}, distance: "<<-p.first<<std::endl;
        q.pop();
    }
}
