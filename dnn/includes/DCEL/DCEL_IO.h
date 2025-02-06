#pragma once

#include "DCEL/DCEL.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace DCEL_IO{
    enum{__DEBUG_MODE__ = 0};
    DCEL text_to_DCEL(std::string);
    void print_DCEL(DCEL&);
    //void DCEL_to_text(std::string);
    //void printVertexTab(DCEL& );
    //void printHedgeTab(DCEL &);
    //void printFaceTab(DCEL &);
}