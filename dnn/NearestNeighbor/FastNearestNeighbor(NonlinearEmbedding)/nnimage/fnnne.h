#include <stdio.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define REAL float
//#define INTTYPE unsigned int
#define INTTYPE int
#define SEED_K 10

using namespace std;

struct _result{
	REAL sqdist;
	int idx;
};

struct _qidx{
	int c,h,w;
	REAL sig;
};
bool qcmp(_qidx &a,_qidx &b);

class _image{
public:
	char*** p;//pixel. p[RGB][height][width]
	void alloc(int width,int height);
};

class _fnnne{
public:
	INTTYPE*** int_image;
	INTTYPE*** sqint_image;

	REAL* omean;
	REAL* osig;
	REAL** mean;
	REAL** sig;
	REAL**** mean4;
	REAL**** sig4;
	REAL**** mean16;
	REAL**** sig16;
	
	_image* data;
	_image* query;

	int qnum;
	int dnum;
	int width;
	int height;
	_fnnne();
	void fnn_preprocessing(_image* data,int datanum,int width,int height);
	void set_meansig();
	_result fnn_query(_image* q);
	void release();
};