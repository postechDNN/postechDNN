#pragma once
#include <vector>
#include "../../Examples/ShortestPathSimplePolygon/Point.h"


class Environment {

	
	public :
		vector<Point> env;

		Environment(vector<Point>* inputEnv = NULL,bool Example=false) {
			if (Example) {

			}
			return;
			if (inputEnv == NULL) return;
			for (int i = 0; i < inputEnv->size(); i++) {
				env.push_back((*inputEnv)[i]);
			}

		};

		Environment(Environment* e) {
			for (int i = 0; i < e->env.size(); i++) env.push_back(e->env[i]);
		}

	bool MoveEnv(int Num, const Point & p) {
		if (env.size() < Num) return false;
		env[Num] = p;
		return true;
	}

};


class DynamicNN {
	vector <Point> plist;
	Environment * env ;

public :
	DynamicNN(Environment* env = NULL, vector<Point>* pointSet = NULL);

	void AddPoint(const Point& p) {
		plist.push_back(p);
	}

	bool delPoint(int Num) {
		if (plist.size() < Num) return false;
		else plist.erase(plist.begin() + Num);
	}

	bool MoveEnv(int Num, const Point& p);

	const vector<Point>& getPointList() {
		return plist;
	}

	void KNNindex(Point& p, vector<int>* result, int k = 1);

	void KNNpoint(Point& p, vector<Point> * result,int k = 1) {
		vector<int> iresult;
		KNNindex(p, &iresult , k);
		for (int i = 0; i < k; i++)
			result->push_back(plist[iresult[i]]);
	}

};