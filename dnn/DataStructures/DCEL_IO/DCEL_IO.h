#pragma once
#include "../DCEL/DCEL.h"

class DCEL_IO : public DCEL{
public:
	DCEL_IO(FILE*);
	void DCELtotext(FILE*);
	void printVertexTab();
	void printHedgeTab();
	void printFaceTab();
    ~DCEL_IO();
};