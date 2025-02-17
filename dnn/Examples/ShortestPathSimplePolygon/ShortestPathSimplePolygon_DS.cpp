#include "pch.h"
#include "ShortestPathSimplePolygon_DS.h"
#include "fstream"
#include "windows.h"
#include "chrono"


//#include "../../Examples/ShortestPathSimplePolygon/Edge.h"


using namespace std;
#define DIV 1024



DNN_DS::DNN_DS() {
	//this->Graph = new EPS::Eps_Graph_3D();
	memory.dwLength = sizeof(memory);
	exe_time = 0;
}

DNN_DS::~DNN_DS() {

}

long long int DNN_DS::get_virtual_memory()
{
	return memory.ullTotalVirtual / DIV;
}

long long int DNN_DS::get_physical_memory()
{
	return memory.ullTotalPhys / DIV;
}

long long int DNN_DS::get_execution_time()
{
	return exe_time;
}

