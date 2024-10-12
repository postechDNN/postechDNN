#pragma once
#include "Object.h"
#include "./Util/SVP.h"
#include <vector>

class DNN_DS
{
// Function
public:
	DNN_DS();
	~DNN_DS();
	long long int get_virtual_memory();
	long long int get_physical_memory();
	long long int get_execution_time();

// Data structure pointers 
public:
	Object object2D;

	SVP svp;


private:
	MEMORYSTATUSEX memory;
	long long int exe_time;
};

