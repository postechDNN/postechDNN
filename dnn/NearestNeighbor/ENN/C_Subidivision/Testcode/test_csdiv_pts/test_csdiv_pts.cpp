#include "test_csdiv_pts.h"

bool test_csdiv_pts_file(std::string fname){
    std::cout << "test conforoming subdivision with filename="<<fname<<std::endl;
    std::ifstream fin(fname);
    std::string line;
    int n,alpha;
    std::vector<Point> pts;
    if(fin.is_open()){
        fin >> n >> alpha;
        double x,y;
        for(int i = 0 ; i< n ; i++){
            fin >>x >> y;
            pts.push_back(Point(x,y));
        }
        C_Subdivision csubdiv(pts);
        std::cout<<"number of points: "<<n << " alpha: "<<alpha<<std::endl;
        std::cout<<"scale_factor: "<<csubdiv.scale_factor<<std::endl;
        std::cout<<"translation_x_factor: "<<csubdiv.tr_x_factor<<std::endl;
        std::cout<<"translation_y_factor: "<<csubdiv.tr_y_factor<<std::endl;
        DCEL dcel = csubdiv.build_alpha_subdivision(alpha);

        bool pass = true;
        std::cout <<"Strong alpha-conforming subdivision space test\n";
        if(!check_strong_csdiv(pts,dcel,alpha))
            pass = false;

        std::cout <<"\nAdditional properties of alpha-conforming subdivision test\n";
        if(!check_add_properties(pts,dcel,alpha))
            pass = false;
        return pass;
    }
    else{
        std::cout <<"error: file is not opened\n";
        return false;
    }
}

bool test_csdiv_pts(){
    std::string folder_name = "Testcode/test_csdiv_pts/testcase/";
    std::vector<std::string> fnames= {"t1.txt","t2.txt","t3.txt","t4.txt","t5.txt"};
    std::vector<bool> passes(fnames.size());
    int i = 0;
    bool ret = true;
    for(auto fname :fnames){
        if(test_csdiv_pts_file(folder_name+fname))
            passes[i] = true;
        else
            passes[i] = false;
        i++;
    }
    i= 0;
    for(auto fname: fnames){
        std::cout <<"file name("<<fname<<"): ";
        if(passes[i]) std::cout <<"PASS\n";
        else std::cout <<"FAIL\n";  
        i++;
        ret = ret & passes[i];
    }
    return ret;
}