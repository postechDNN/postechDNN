// nnimage.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "fnnne.h"

void load_data(char* dataname,char* queryname,_image*& data, _image*& query,int datanum,int qnum,int width,int height);
_result ibrute_nnsearch(_image* data, _image* query,int datanum,int width,int height);

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));

	FILE* ofile = fopen("out.txt","w");

	//load data	
	_fnnne fnn;
	char datafilename[] = "data_batch_1.bin";
	char queryfilename[] = "data_batch_6.bin";
	int datanum = 10000;
	int qnum = 100;
	int image_width = 16;
	int image_height = 16;
	_image* data;
	_image* query;
	load_data(datafilename,queryfilename,data,query,datanum,qnum,image_width,image_height);
	printf("cifar data load done\n");

	_result* fnnres = new _result[qnum];
	_result* brtres  = new _result[qnum];
	
	//fnnseed preprocessing
	printf("FNN prep...\n");
	fnn.fnn_preprocessing(data,datanum,image_width,image_height);

	//fnnseed query
	printf("FNN query...\n");
	for(int i=0 ; i<qnum ; i++){
		fnnres[i] = fnn.fnn_query(&query[i]);
	}
	
	printf("Release memory...\n");
	fnn.release();
	printf("FNN done.\n");

	//opt brute query
	printf("optBRT query...\n");
	for(int i=0 ; i<qnum ; i++){
		brtres[i] = ibrute_nnsearch(data, &query[i],datanum,image_width,image_height);
	}
	printf("optBRT done.\n");

	printf("FNN: %d %g\n",fnnres[0].idx,fnnres[0].sqdist);
	printf("BRT: %d %g\n",brtres[0].idx,brtres[0].sqdist);

	//write result on file
	for(int q=0 ; q<qnum ; q++){
		fprintf(ofile,"FNNidx:%d FNNdist:%g BRTidx:%d BRTdist%g\n",fnnres[q].idx,fnnres[q].sqdist,brtres[q].idx,brtres[q].sqdist);
	}
	fclose(ofile);
	return 0;
}


void load_data(char* dataname,char* queryname,_image*& data, _image*& query,int datanum,int qnum,int width,int height){
	char buf[12289];//64*64*3+1=12289
	
	FILE* dfile=NULL;
	FILE* qfile=NULL;
	int check;

	//alloc data	
	data = new _image[datanum];
	for(int n=0 ; n<datanum ; n++){
		data[n].alloc(width,height);
	}
	query = new _image[qnum];
	for(int q=0 ; q<qnum ; q++){
		query[q].alloc(width,height);
	}

	//file open
	qfile = fopen(queryname,"rb");
	if(qfile==NULL){
		printf("dfile open error: %s\n",queryname);
	}

	int cnt;
	for(int f=0 ; f<1 ; f++){

		dfile = fopen(dataname,"rb");
	
		if(dfile==NULL){
			printf("data file open error: %s\n",dataname);
			exit(1);
		}

		//set data
		for(int n=0 ; n<datanum ; n++){
			check = fread(buf,1,width*height*3+1,dfile);
			if(check != width*height*3+1){
				printf("data fread check error: %d/%d\n",check,width*height*3+1);
				exit(1);
			}

			cnt=1;
			for(int c=0 ; c<3 ; c++){
			for(int y=0 ; y<height ; y++){
			for(int x=0 ; x<width ; x++){				
				data[10000*f+n].p[c][y][x] = buf[cnt];
				cnt++;
			}}}
		}
		fclose(dfile);
	}

	//set query
	for(int n=0 ; n<qnum ; n++){
		check = fread(buf,1,width*height*3+1,qfile);
		if(check != width*height*3+1){
			printf("query fread check error: %d/%d\n",check,width*height*3+1);
			exit(1);
		}

		cnt=1;
		for(int c=0 ; c<3 ; c++){
		for(int y=0 ; y<height ; y++){
		for(int x=0 ; x<width ; x++){
			query[n].p[c][y][x] = buf[cnt];
			cnt++;
		}}}
	}
}

_result ibrute_nnsearch(_image* data, _image* query,int datanum,int width,int height){
	//bruteforcing
	REAL diff, min_diff,a,b;
	_result res;
	
	min_diff = FLT_MAX;
	for(int n=0 ; n<datanum ; n++){
		//compute patch dist
		diff = 0.0;
		for(int c=0 ; c<3 ; c++){
		for(int y=0 ; y<height ; y++){
		for(int x=0 ; x<width ; x++){
			//for each pixel
			a = (unsigned char)data[n].p[c][y][x];
			b = (unsigned char)(query->p[c][y][x]);
			diff += (a-b)*(a-b);
			if(diff>=min_diff){
				goto BRT_LABEL;
			}
		}}}
BRT_LABEL:
		//keep min dist
		if(diff < min_diff){
			min_diff = diff;
			res.sqdist= min_diff;
			res.idx = n;
		}
	}
	return res;
}
