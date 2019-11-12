#include "fnnne.h"
#include <algorithm>

bool qcmp(_qidx &a,_qidx &b){
	return a.sig > b.sig;
}

void _image::alloc(int width,int height){
	p = new char**[3];
	for(int c=0 ; c<3 ; c++){
		p[c] = new char*[height];
		for(int y=0 ; y<height ; y++){
			p[c][y] = new char[width];
		}
	}
}

_fnnne::_fnnne(){
}

void _fnnne::release(){
	for(int n=0 ; n<dnum ; n++){
		for(int c=0 ; c<3 ; c++){
			for(int h=0 ; h<2 ; h++){
				delete [] mean4[n][c][h];
				delete [] sig4 [n][c][h];
			}
			for(int h=0 ; h<4 ; h++){
				delete [] mean16[n][c][h];
				delete [] sig16 [n][c][h];
			}
			delete [] mean4[n][c];
			delete [] sig4 [n][c];
			delete [] mean16[n][c];
			delete [] sig16 [n][c];

		}
		delete [] mean4[n];
		delete [] sig4[n];
		delete [] mean16[n];
		delete [] sig16[n];
	}
	delete [] mean4;
	delete [] sig4;
	delete [] mean16;
	delete [] sig16;
	delete [] mean;
	delete [] sig;
}

void _fnnne::fnn_preprocessing(_image* data,int datanum,int width,int height){
	this->data = data;
	this->dnum = datanum;
	this->width = width;
	this->height = height;

	this->set_meansig();
}

void _fnnne::set_meansig(){
	//alloc memory
	omean  = new REAL[dnum];
	osig   = new REAL[dnum];
	
	mean   = new REAL*[dnum];
	sig    = new REAL*[dnum];
	for(int n=0 ; n<dnum ; n++){
		mean[n] = new REAL[3];
		sig [n] = new REAL[3];
	}

	mean4  = new REAL***[dnum];
	sig4   = new REAL***[dnum];
	for(int n=0 ; n<dnum ; n++){
		mean4[n] = new REAL**[3];
		sig4 [n] = new REAL**[3];
		for(int c=0 ; c<3 ; c++){
			mean4[n][c] = new REAL*[2];
			sig4 [n][c] = new REAL*[2];
			for(int d=0 ; d<2 ; d++){
				mean4[n][c][d] = new REAL[2];
				sig4 [n][c][d] = new REAL[2];
			}
		}
	}

	mean16 = new REAL***[dnum];
	sig16  = new REAL***[dnum];
	for(int n=0 ; n<dnum ; n++){
		mean16[n] = new REAL**[3];
		sig16 [n] = new REAL**[3];
		for(int c=0 ; c<3 ; c++){
			mean16[n][c] = new REAL*[4];
			sig16 [n][c] = new REAL*[4];
			for(int d=0 ; d<4 ; d++){
				mean16[n][c][d] = new REAL[4];
				sig16 [n][c][d] = new REAL[4];
			}
		}
	}


	//set sum, sqsum
	//int pp = SIZEP/4;
	int pw = width/4;
	int ph = height/4;
	for(int n=0 ; n<dnum ; n++){
		//mean16,sig16
		for(int c=0 ; c<3 ; c++){
		for(int h=0 ; h<4 ; h++){
		for(int w=0 ; w<4 ; w++){
			mean16[n][c][h][w] = 0;
			sig16 [n][c][h][w] = 0;
			for(int y=0 ; y<ph ; y++){
			for(int x=0 ; x<pw ; x++){
				mean16[n][c][h][w] += ((unsigned char)data[n].p[c][ph*h+y][pw*w+x]);
				sig16 [n][c][h][w] += ((unsigned char)data[n].p[c][ph*h+y][pw*w+x])
					                 *((unsigned char)data[n].p[c][ph*h+y][pw*w+x]);
			}}
		}}}

		//mean4,sig4
	
		for(int c=0 ; c<3 ; c++){
		for(int h=0 ; h<2 ; h++){
		for(int w=0 ; w<2 ; w++){
			mean4[n][c][h][w] = mean16[n][c][h*2+0][w*2+0]
			                   +mean16[n][c][h*2+0][w*2+1]
							   +mean16[n][c][h*2+1][w*2+0]
							   +mean16[n][c][h*2+1][w*2+1];
			sig4 [n][c][h][w] = sig16 [n][c][h*2+0][w*2+0]
			                   +sig16 [n][c][h*2+0][w*2+1]
							   +sig16 [n][c][h*2+1][w*2+0]
							   +sig16 [n][c][h*2+1][w*2+1];
		}}}

		//mean,sig
		for(int c=0 ; c<3 ; c++){
			mean[n][c] = mean4[n][c][0][0]
			            +mean4[n][c][0][1]
						+mean4[n][c][1][0]
						+mean4[n][c][1][1];
			sig [n][c] = sig4 [n][c][0][0]
			            +sig4 [n][c][0][1]
						+sig4 [n][c][1][0]
						+sig4 [n][c][1][1];
		}
		omean[n] = mean[n][0]+mean[n][1]+mean[n][2];
		osig [n] = sig [n][0]+sig [n][1]+sig [n][2];
	}

	//set mean,sig
	int dim;
	for(int n=0 ; n<dnum ; n++){
		//mean16,sig16
		dim = width*height/16;
		for(int c=0 ; c<3 ; c++){
		for(int h=0 ; h<4 ; h++){
		for(int w=0 ; w<4 ; w++){
			mean16[n][c][h][w]/=dim;
			sig16 [n][c][h][w] = sig16[n][c][h][w]/dim - mean16[n][c][h][w]*mean16[n][c][h][w];
			if(sig16[n][c][h][w]<=0) sig16[n][c][h][w]=0;
			else sig16[n][c][h][w] = sqrt(sig16[n][c][h][w]);
		}}}

		//mean4,sig4
		dim *= 4;
		for(int c=0 ; c<3 ; c++){
		for(int h=0 ; h<2 ; h++){
		for(int w=0 ; w<2 ; w++){
			mean4[n][c][h][w]/=dim;
			sig4 [n][c][h][w] = sig4[n][c][h][w]/dim - mean4[n][c][h][w]*mean4[n][c][h][w];
			if(sig4[n][c][h][w]<=0) sig4[n][c][h][w]=0;
			else sig4[n][c][h][w] = sqrt(sig4[n][c][h][w]);
		}}}

		//mean,sig
		dim *= 4;
		for(int c=0 ; c<3 ; c++){
			mean[n][c] /= dim;
			sig [n][c]  = sig[n][c]/dim - mean[n][c]*mean[n][c];
			if(sig[n][c]<=0) sig[n][c] = 0;
			else sig[n][c]= sqrt(sig[n][c]);
		}

		//omean,osig
		dim *=3;
		omean[n] /= dim;
		osig [n]  = osig[n]/dim - omean[n]*omean[n];
		if(osig[n]<=0) osig[n] = 0;
		else osig[n] = sqrt(osig[n]);
	}
}

_result _fnnne::fnn_query(_image* q){
	REAL l_bound, min_diff, ais, bis;
	REAL qom=0.0, qos=0.0;
	REAL qm[3]={0.0,}, qs[3]={0.0,};
	REAL qm4 [3][2][2]={0.0, }, qs4 [3][2][2]={0.0, };
	REAL qm16[3][4][4]={0.0, }, qs16[3][4][4]={0.0, };
	REAL ltemp[3][4][4];
	_qidx qidx[48];
	REAL dim = width*height;
	REAL odim = dim*3.0;
	REAL dim4 = dim/4.0;
	REAL dim16 = dim/16.0;
	
	int SW16 = width/4;
	int SH16 = height/4;
	int SW4 = width/2;
	int SH4 = height/2;

	_result nnres;

	//double dbg_check = 0.0;
	//double dbg_visit = 0.0;

	//calc query sum,sqsum
	for(int c=0 ; c<3 ; c++){
	for(int h=0 ; h<4 ; h++){
	for(int w=0 ; w<4 ; w++){
		//qm16
		qm16[c][h][w] = 0;
		qs16[c][h][w] = 0;
		for(int y=0 ; y<SH16 ; y++){
		for(int x=0 ; x<SW16 ; x++){
			qm16[c][h][w] += ((unsigned char)q->p[c][h*SH16+y][w*SW16+x]);
			qs16[c][h][w] += ((unsigned char)q->p[c][h*SH16+y][w*SW16+x])
				            *((unsigned char)q->p[c][h*SH16+y][w*SW16+x]);
		}}
	}}}
		//qm4
	for(int c=0 ; c<3 ; c++){
	for(int h=0 ; h<2 ; h++){
	for(int w=0 ; w<2 ; w++){
		qm4[c][h][w] = qm16[c][h*2+0][w*2+0]
		              +qm16[c][h*2+0][w*2+1]
					  +qm16[c][h*2+1][w*2+0]
					  +qm16[c][h*2+1][w*2+1];
		qs4[c][h][w] = qs16[c][h*2+0][w*2+0]
		              +qs16[c][h*2+0][w*2+1]
					  +qs16[c][h*2+1][w*2+0]
					  +qs16[c][h*2+1][w*2+1];
	}}}
		//qm
	for(int c=0 ; c<3 ; c++){
		qm[c] = qm4[c][0][0]
		       +qm4[c][0][1]
			   +qm4[c][1][0]
			   +qm4[c][1][1];
		qs[c] = qs4[c][0][0]
		       +qs4[c][0][1]
			   +qs4[c][1][0]
			   +qs4[c][1][1];
	}
		//oqm
	qom = qm[0] + qm[1] +qm[2];
	qos = qs[0] + qs[1] +qs[2];

	//calc query mean,sig
	for(int c=0 ; c<3 ; c++){
	for(int h=0 ; h<4 ; h++){
	for(int w=0 ; w<4 ; w++){
		qm16[c][h][w]/=dim16;
		qs16[c][h][w] = qs16[c][h][w]/dim16 - qm16[c][h][w]*qm16[c][h][w];
		if(qs16[c][h][w]<=0) qs16[c][h][w] = 0;
		else qs16[c][h][w] = sqrt(qs16[c][h][w]);
	}}}
	for(int c=0 ; c<3 ; c++){
	for(int h=0 ; h<2 ; h++){
	for(int w=0 ; w<2 ; w++){
		qm4[c][h][w]/=dim4;
		qs4[c][h][w] = qs4[c][h][w]/dim4 - qm4[c][h][w]*qm4[c][h][w];
		if(qs4[c][h][w]<=0) qs4[c][h][w] = 0;
		else qs4[c][h][w] = sqrt(qs4[c][h][w]);
	}}}
	for(int c=0 ; c<3 ; c++){
		qm[c]/= dim;
		qs[c] = qs[c]/dim - qm[c]*qm[c];
		if(qs[c]<=0) qs[c] = 0;
		else qs[c] = sqrt(qs[c]);
	}
	qom/= odim;
	qos = qos/odim - qom*qom;
	if(qos<=0) qos = 0;
	else qos = sqrt(qos);



	//sort query wrt sig
	int cnt=0;
	for(int c=0 ; c<3 ; c++){
	for(int h=0 ; h<4 ; h++){
	for(int w=0 ; w<4 ; w++){
		qidx[cnt].sig = qs16[c][h][w];
		qidx[cnt].h = h;
		qidx[cnt].w = w;
		qidx[cnt].c = c;
		cnt++;
	}}}
	sort(qidx,qidx+48,qcmp);

	//find seed
	min_diff = FLT_MAX;
	int sn;
	for(int k=0 ; k<SEED_K ; k++){
		sn = rand()%dnum;

		//calc true dist for sn
		l_bound = 0;
		for(int c=0 ; c<3 ; c++)
		for(int y=0 ; y<height ; y++){
		for(int x=0 ; x<width ; x++){
			ais = (unsigned char)data[sn].p[c][y][x];
			bis = (unsigned char)q->p[c][y][x];
			l_bound += (ais-bis)*(ais-bis);
			if(l_bound>=min_diff){
				goto QLABEL;
			}
		}}
QLABEL:
		//keep min dist
		if(l_bound < min_diff){
			min_diff = l_bound;
			nnres.idx = sn;
			nnres.sqdist = min_diff;
		}
	}

///////////////////////////////////intensive routine////////////////////////////////
	int sh,sw,sc;
	for(int n=0 ; n<dnum ; n++){
		//check 1th lower bound
		l_bound = dim*((qom-omean[n])*(qom-omean[n])
					  +(qos- osig[n])*(qos- osig[n]));

		//l_bound already larger then min_dist
		if(l_bound>=min_diff){
			continue;
		}
		
		//check 2th lower bound
		l_bound = 0.0;
		l_bound+= ( (qm[0]-mean[n][0])*(qm[0]-mean[n][0])
			       +(qs[0]-sig [n][0])*(qs[0]-sig [n][0]) );
		l_bound+= ( (qm[1]-mean[n][1])*(qm[1]-mean[n][1])
			       +(qs[1]-sig [n][1])*(qs[1]-sig [n][1]) );
		l_bound+= ( (qm[2]-mean[n][2])*(qm[2]-mean[n][2])
			       +(qs[2]-sig [n][2])*(qs[2]-sig [n][2]) );
		l_bound*= dim;

		//l_bound already larger then min_dist
		if(l_bound>=min_diff){
			continue;
		}

		//check 3th lower bound
		l_bound = 0.0;
		for(int c=0 ; c<3 ; c++){
			l_bound+= ( (qm4[c][0][0]-mean4[n][c][0][0])*(qm4[c][0][0]-mean4[n][c][0][0])
					   +(qs4[c][0][0]- sig4[n][c][0][0])*(qs4[c][0][0]- sig4[n][c][0][0]) );
			l_bound+= ( (qm4[c][0][1]-mean4[n][c][0][1])*(qm4[c][0][1]-mean4[n][c][0][1])
					   +(qs4[c][0][1]- sig4[n][c][0][1])*(qs4[c][0][1]- sig4[n][c][0][1]) );
			l_bound+= ( (qm4[c][1][0]-mean4[n][c][1][0])*(qm4[c][1][0]-mean4[n][c][1][0])
					   +(qs4[c][1][0]- sig4[n][c][1][0])*(qs4[c][1][0]- sig4[n][c][1][0]) );
			l_bound+= ( (qm4[c][1][1]-mean4[n][c][1][1])*(qm4[c][1][1]-mean4[n][c][1][1])
					   +(qs4[c][1][1]- sig4[n][c][1][1])*(qs4[c][1][1]- sig4[n][c][1][1]) );
		}
		l_bound*= dim4;

		//l_bound already larger then min_dist
		if(l_bound>=min_diff){
			continue;
		}

		//check 4th lower bound
		l_bound=0.0;
		for(int c=0 ; c<3 ; c++){
		for(int h=0 ; h<4 ; h++){
		for(int w=0 ; w<4 ; w++){
			ltemp[c][h][w] = dim16*( (qm16[c][h][w]-mean16[n][c][h][w])*(qm16[c][h][w]-mean16[n][c][h][w])
								    +(qs16[c][h][w]- sig16[n][c][h][w])*(qs16[c][h][w]- sig16[n][c][h][w]) );
			l_bound += ltemp[c][h][w];
		}}}
		//l_bound already larger then min_dist
		if(l_bound>=min_diff){
			continue;
		}

		//refine bound
		for(int i=0 ; i<48 ; i++){
			sc = qidx[i].c;
			sh = qidx[i].h;
			sw = qidx[i].w;

			l_bound -= ltemp[sc][sh][sw];

			for(int y=0 ; y<SH16 ; y++){
			for(int x=0 ; x<SW16 ; x++){
				ais = (unsigned char)data[n].p[sc][sh*SH16+y][sw*SW16+x];
				bis =      (unsigned char)q->p[sc][sh*SH16+y][sw*SW16+x];
				l_bound += (ais-bis)*(ais-bis);
					
				if(l_bound>=min_diff){
					goto DLABEL;
				}
			}}
		}
DLABEL:
		//keep min dist
		if(l_bound < min_diff){
			min_diff = l_bound;
			nnres.idx = n;
			nnres.sqdist = min_diff;
		}
	}
///////////////////////////////////intensive routine end////////////////////////////////
	return nnres;
}
