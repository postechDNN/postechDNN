#include"dPoint.h"
#include<vector>

dPoint::dPoint(){
	this->d=3;
	this->cds=new std::vector<double>();
	this->cds->push_back(0.);
	this->cds->push_back(0.);
	this->cds->push_back(0.);
}

dPoint::dPoint(double a, double b, double c){
	this->d=3;
	this->cds=new std::vector<double>();
	this->cds->push_back(a);
	this->cds->push_back(b);
	this->cds->push_back(c);
}

dPoint::dPoint(std::vector<double>* _cds){
	this->d=_cds->size();
	this->cds=new std::vector<double>();
	for(int i=0;i<this->d;i++)
		this->cds->push_back((*_cds)[i]);
}

dPoint::dPoint(dPoint* _dP){
	this->d=_dP->getd();
	this->cds=new std::vector<double>();
	for(int i=0;i<this->d;i++)
		this->cds->push_back(_dP->getc(i));
}

dPoint::~dPoint(){
	delete(this->cds);
}

bool dPoint::operator==(dPoint _dP){
	if(this->d!=_dP.getd())
		return false;
	for(int i=0;i<this->d;i++)
		if((*this->cds)[i]!=_dP.getc(i))
			return false;
	return true;
}

int dPoint::getd(){
	return this->d;
}

double dPoint::getx(){
	return (*this->cds)[0];
}

double dPoint::gety(){
	return (*this->cds)[1];
}

double dPoint::getz(){
	return (*this->cds)[2];
}

double dPoint::getc(int i){
	return (*this->cds)[i];
}