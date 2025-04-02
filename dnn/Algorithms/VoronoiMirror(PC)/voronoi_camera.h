#ifndef VORONOI_CAMERA_VORONOI_CAMERA_H
#define VORONOI_CAMERA_VORONOI_CAMERA_H

#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <cmath>
#include <ctime>
#include <list>
#include <vector>
#include <stdio.h>

#define VORONOITIME 10

#define MAIN_SCREEN_WD 2560
#define MAIN_SCREEN_HT 1440
#define SUB_SCREEN_WD 960
#define SUB_SCREEN_HT 540
// S8 1920 1080, P2 1280 960
#define wdth 1280
#define ht 720


#define MAXSEG 6000
#define INITSEG 150
#define MINSQDISTANCE 25
#define FACE_ADDSPEED 0.08
#define FACE_DELSPEED 0.15
#define ADDSPEED 0.1 // (ADDSPEED) of the whole screen in one step
#define DELSPEED 0.1 // (DELSPEED) of the whole screen in one step

#define GRIDXNUM 10
#define GRIDYNUM 8
#define MAXGRIDNUM 600
#define MINGRIDNUM 2

#define WHITELINE false
#define MOVEINTERVAL 5
#define MOVERANGE 2.0

#define TONERED 1.0
#define TONEGRN 1.0
#define TONEBLU 1.0

using namespace std;
using namespace cv;

extern list <int> deleted_pool;
extern int grid[GRIDXNUM][GRIDYNUM];
extern int sec;
extern CascadeClassifier face_cascade;

struct vertices
{
	long double x_cor;
	long double y_cor;
};

struct sites
{
	int active;
	int allowed;
	uchar old_col[3];
	uchar new_col[3];
	long double x_cor;
	long double y_cor;
	vector<int> list; // vertex list
	vector<int> near_site; // neighbor

	int area; // # of pixels in this site
	int grid_x;
	int grid_y;

	int create_count;
	int move_count;
	int disform_x;
	int disform_y;
};

double unifRand();

double unifRand(double a, double b);

bool comp(long double a, long double b);

void read_data(vector <sites> &, vector <vertices> &);
void write_data(vector <sites> &, vector <vertices> &);

void display_sites_info(vector <sites> &);

void bisector_intersection(long double, long double, long double, long double, long double, long double, long double, long double *, long double *);

int locate(vector <sites> &, long double, long double);

int add_point(vector <sites> &, vector <vertices> &, long double, long double);

int delete_site(int index, vector <sites> &, vector <vertices> &);

bool progress(vector <sites> &, vector <vertices> &, long double, long double, long double, long double, int, long double &, long double &, int &, int);

long double det(long double x1, long double x2, long double x3, long double y1, long double y2, long double y3, long double z1, long double z2, long double z3);

void draw(Mat &, vector <sites> &, vector <vertices> &, Mat &, int, bool);

int convertToVoronoi(Mat &, Mat &);

void reallocate_vertices(vector <sites> &, vector <vertices> &);

//int initializerInC();

int get_grid_num(long double, long double);

void add_to_grid(long double, long double);

void delete_from_grid(long double, long double);

vector<Rect> detectFace(Mat &frame);

#endif //VORONOI_CAMERA_VORONOI_CAMERA_H
