#pragma once
#include"Point.h"
#include "ShortestPathTree.h"
#include "polygon_decomposition.h"
#include <limits>
int line_of_sight_id;
vector<int> funnel_path_point(vector<int> chain1, vector<int> chain2, Point p);
pair<vector<int>,Point> funnel_path(vector<int> chain1, vector<int> chain2);
pair<vector<int>, Point> shortest_path_line(Point p1, Point p2, SPT* spt);

bool isVisible(int from, int to);
Point computeEndpoint(int lineFrom, int lineTo);
Point foot_of_max_perpendicular(int p, Point origin, Point dest);

enum TYPE {
	tERROR,
	tDEFAULT,
	tPATH,
	tBOUNDARY,
	tBEND_add, 
	tBEND_del,
	tBEND_BOUNDARY_PATH
};
enum ROT {
	DEFAULT,
	CW,
	CCW
};

bool is_tangent_slope(double slope, double from, double to, ROT direction);

class LINE {
protected:
	Point endP[2];
	double slope;
	TYPE type;
	int v;
	vector<vector<int>> path; //stores path from s or t to the line of sight
	Point foot[2];
	double length[2] = { 0,0 };
	bool type1 = false;
public:
	LINE() {
		path.push_back(vector<int>());
		path.push_back(vector<int>());
	}
	LINE(int _v, double _slope, vector<int> path1, vector<int> path2)
	{
		type = tDEFAULT;
		v = _v;
		slope = _slope;
		path.push_back(vector<int>());
		path.push_back(vector<int>());
		path[0] = path1, path[1] = path2;
		for (int idx = 0; idx < 2; idx++) {
			for (int i = 0; i < path[idx].size() - 1; i++)
				length[idx] += dist(path[idx][i], path[idx][i + 1]);
		//length doesn't include path to foot in this case....
		}
	}
	bool getType1(void) {
		return type1;
	}
	void setType1(bool to)
	{
		type1 = to;
	}
	double getLength(void)
	{
		return length[0] + length[1];
	}
	double getLength(int idx) {
		if(idx==0||idx==1)
			return length[idx];
		else return 0;
	}
	void computeEndpointWithSlope(void)
	{
		//all bend events have to be tangent to V so the while loop shouldn't run forever
		Point V = point_list[v];
		double slice = 1;
		while (1)
		{
			Point p1(V.get_x() + slice, V.get_y() + slope * slice);
			if (point_state.find_triangle(p1) == -1)
				slice /= 2;
			else
			{
				point_list.push_back(p1);
				endP[0] = computeEndpoint(point_list.size() - 1, v);
				endP[1] = computeEndpoint(v, point_list.size() - 1);
				point_list.pop_back();
				break;
			}
		}
	}
	void setLength_withFoot()
	{
		for (int i = 0; i < 2; i++) {
			foot[i] = foot_of_max_perpendicular(path[i].back(), endP[0], endP[1]);
			length[i] += dist(point_list[path[i].back()], foot[i]);
		}
	}
	double getLength_noFoot(int idx)
	{
		double sum=0;
		if (idx == 0 || idx == 1)
		{
			for (int i = 0; i < path[idx].size() - 1; i++)
				sum += dist(path[idx][i], path[idx][i + 1]);
		}
		return sum;
	}
	//returns sum of distances from each _s and _t to the line in question
	double getDistanceSum(void)
	{
		double sum = 0;
		for (int i = 0; i < path[0].size() - 1; i++)
		{
			sum += dist(path[0][i], path[0][i + 1]);
		}
		for (int i = 0; i < path[1].size() - 1; i++)
		{
			sum += dist(path[1][i], path[1][i + 1]);
		}
		

		return sum;
	}
	Point* getEndpoints() {
		return endP;
	}
	int getV()
	{
		return v;
	}
	TYPE getType()
	{
		return type;
	}
	double getSlope()
	{
		return slope;
	}
	void setPathS(vector<int> path_)
	{
		path[0] = path_;
	}
	void setPath(int idx, pair<vector<int>, Point> res)
	{
		if (idx == 0 || idx == 1)
		{
			path[idx] = res.first;
			foot[idx] = res.second;
		}
	}
	vector<int> getPath(int idx)
	{
		switch (idx)
		{
		case 0:
			return path[0];
		case 1: 
			return path[1];
		default:
			return vector<int>();
		}
	}
	vector<Point> getShortestPath(int idx)
	{
		vector<Point> sPath;
		if (idx == 0 || idx == 1)
		{
			for (int i = 0; i < path[idx].size(); i++)
				sPath.push_back(point_list[path[idx][i]]);
			
			sPath.push_back(foot[idx]);
		}
		return sPath;
	}
	double Dist_from_u_to_line(int idx,double _slope)
	{
		Point _v = point_list[v];
		Point u = point_list[path[idx].back()];
		double d = abs(_slope * u.get_x() - slope * _v.get_x() + _v.get_y()-u.get_y());
		return d;
	}
};

class PATH : public LINE {
	int v2;

public:
	PATH(int _v2, int _v, SPT** spt) {
		type = tPATH;
		v = _v;
		v2 = _v2;
		endP[0] = computeEndpoint(v, v2);
		endP[1] = computeEndpoint(v2, v);
		slope = computeSlope(endP[0], endP[1]);
		
		for (int i = 0; i < 2; i++) {
			pair<vector<int>, Point> res = shortest_path_line(endP[i], point_list[(i==0)?v2:v], spt[i]);
			path[i] = res.first;
			foot[i] = res.second;
			for (int j = 0; j < path[i].size()-1; j++)
				length[i] += dist(path[i][j], path[i][j + 1]);
			point_list.push_back(foot[i]);
			length[i] += dist(point_list.size() - 1, path[i].back());
			point_list.pop_back();
		}
	}
	int getV2()
	{
		return v2;
	}
	
};

class BOUNDARY : public LINE {
	int boundary_point;

public:
	BOUNDARY(int _v, int _v2, SPT** spt,bool is_s) {
		type = tBOUNDARY;
		v = _v;
		boundary_point = _v2;
		endP[0] = computeEndpoint(v, boundary_point);//point_list[_v2];
		endP[1] = computeEndpoint(boundary_point, v);
		slope = computeSlope(point_list[_v], point_list[_v2]);
	
		for (int i = 0; i < 2; i++) {
			pair<vector<int>, Point> res = shortest_path_line(endP[(is_s)?1-i:i], point_list[v], spt[i]);// shortest_path_line(endP[0], endP[1], spt[i]);
			path[i] = res.first;
			foot[i] = res.second;
			for (int j = 0; j < path[i].size() - 1; j++)
				length[i] += dist(path[i][j], path[i][j + 1]);
			point_list.push_back(foot[i]);
			length[i] += dist(point_list.size() - 1, path[i].back());
			point_list.pop_back();
		}
	}
	
};

class BEND : public LINE {
	int orthogonalP[2];
public:
	BEND( PATH* reference)
	{
		type = tBEND_BOUNDARY_PATH;
		v = reference->getV2();
		
		endP[0] = point_list[reference->getV()];//reference->getEndpoints()[0];
		endP[1] = point_list[v];// reference->getEndpoints()[1];
		slope = reference->getSlope();
	}
	BEND(int _v, int orth1, int orth2, double slope1, double slope2, ROT dir)
	{
		type = tBEND_add;
		v = _v;
		orthogonalP[0] = orth1, orthogonalP[1] = orth2;
		Point V = point_list[v];

		Point _foot = foot_of_perpendicular(_v, point_list[orth1], point_list[orth2]);
		double tempSlope = computeSlope(V, _foot);

		point_list.push_back(_foot);

		if (!is_tangent_slope(tempSlope, slope1, slope2, dir) && isVisible(_v,point_list.size()-1))
		{
			point_list.pop_back();
			vector<Point> candidates;
			
			tempSlope = computeSlope(V, point_list[orth2]);
			if (is_tangent_slope(tempSlope, slope1, slope2, dir))
				candidates.push_back(point_list[orth2]);

			Point end = computeEndpoint(orth1, orth2);
			tempSlope = computeSlope(V, end);
			if (is_tangent_slope(tempSlope, slope1, slope2, dir))
				candidates.push_back(end);

			Point newP;
			for (int i = 0; i < 2; i++) {
				Point dummy(V.get_x() + 1, V.get_y() + ((i == 0) ? slope1 : slope2));
				newP = line_intersection(point_list[orth1], point_list[orth2], point_list[v], dummy);
				if (newP.get_x() != numeric_limits<double>::infinity())
				{
					point_list.push_back(newP);
					if (isVisible(v, point_list.size()-1))
						candidates.push_back(newP);
					point_list.pop_back();

				}
			}
			double min = std::numeric_limits<double>::infinity();
			int idx =0;
			for (int i = 0; i < candidates.size(); i++)
			{
				double distance = dist(V, candidates[i]);
				if (distance < min)
				{
					min = distance;
					idx = i;
				}
			}
			point_list.push_back(candidates[idx]);
		}

		endP[0] = computeEndpoint(v, point_list.size() - 1);
		endP[1] = computeEndpoint(point_list.size() - 1, v);
		slope = computeSlope(endP[0], endP[1]);
		point_list.pop_back();

	}
	BEND(int _v, int orth1, int orth2, SPT** spt, int idx, bool type1)
	{
		if (orth1 == _v || orth2 == _v)
			type = tERROR;
		else {
			//foot on extended segment (orth1, orth2) assuming path from v isn't hindered
			Point _foot = foot_of_max_perpendicular(_v, point_list[orth1], point_list[orth2]);
			point_list.push_back(_foot);
			//check if v and _foot are visible to each other, if not, abort (type Error)
			orthogonalP[0] = orth1;
			orthogonalP[1] = orth2;
			v = _v;

			if (isVisible(_v, point_list.size() - 1))
			{	
				type = (type1)? tBEND_add:tBEND_del;

				endP[0] = computeEndpoint(v, point_list.size() - 1);
				endP[1] = computeEndpoint(point_list.size() - 1, v);
				//set shortest path from s/t to line
				//for the side that doesn't involve the "orthogonality", compute manually 
				pair<vector<int>, Point> res = shortest_path_line(endP[0], endP[1], spt[!idx]);
				path[!idx] = res.first, foot[!idx] = res.second;
				//fpr the side involving the orthogonality, we don't have to do the whole thing again
				path[idx] = spt[idx]->retrieve_shortest_path(orth1);
				path[idx].push_back(orth2);
				foot[idx] = foot_of_max_perpendicular(orth2, endP[0], endP[1]);

				for (int i = 0; i < 2; i++)
				{
					for (int j = 0; j < path[i].size() - 1; j++)
						length[i] += dist(path[i][j], path[i][j + 1]);
					point_list.push_back(foot[i]);
					length[i] += dist(point_list.size() - 1, path[i].back());
					point_list.pop_back();
				}
				slope = computeSlope(endP[0], endP[1]);
			}
			else
			{
				type = tBEND_add;//tERROR;
				
				endP[0] = computeEndpoint(v, orth2);
				endP[1] = computeEndpoint(orth2, v);

				pair<vector<int>, Point> res;
				for (int i = 0; i < 2; i++)
				{
					res = shortest_path_line(endP[0], endP[1], spt[i]);
					path[i] = res.first;
					foot[i] = res.second;
				}
				if (path[idx].back() == orth2)
					path[idx].pop_back();

				for (int i = 0; i < 2; i++)
				{
					for (int j = 0; j < path[i].size() - 1; j++)
						length[i] += dist(path[i][j], path[i][j + 1]);
					point_list.push_back(foot[i]);
					length[i] += dist(point_list.size() - 1, path[i].back());
					point_list.pop_back();
				}
				slope = computeSlope(endP[0], endP[1]);
			}
			point_list.pop_back();
			
		}
	}
	void setPath(int idx, vector<int> pth)
	{
		if (idx == 0 || idx == 1)
			path[idx] = pth;
	}
};

bool is_tangent_slope(double slope, double from, double to, ROT direction) {

	if (slope == from || slope == to)
		return false;
	bool inBetween = (direction == CW) == (from > to);
	if (inBetween)
		return (from - slope) * (to - slope) < 0;
	else
		return (from - slope) * (to - slope) > 0;
}

/* finds (i) the triangle and (ii) edge the vector (FROM,TO) from START penetrates through first
   START must be either FROM or TO */
void getFirstTri(int start, int from, int to, int* edge, int* tri)
{
	int t = -1;
	int e = -1;
	int id = point_list[start].get_id();
	if (id != -1) //polygon vertex
	{
		vector<int> candidateTri = vertex_triangle_list[id];
		for (int i = 0; i < candidateTri.size(); i++)
		{
			t = candidateTri[i];
			vector<int> vertices = polygon_list[t];
			int v[2];
			for (int j = 0; j < 3; j++)
			{
				if (vertices[j] == id) {
					v[0] = vertices[(j + 1) % 3];
					v[1] = vertices[(j + 2) % 3];
					break;
				}
			}
			bool valid = check_penetration(from,to,id,v[0], v[1]);
			if (valid) {
				vector<int> edges = triangle_edge_list[t];
				for (int j = 0; j < 3; j++)
				{
					if (diagonal_with_edge_list[edges[j]].check_same_edge(v[0], v[1]))
					{
						e = edges[j];
						break;
					}
				}
				break;
			}
			t = -1;
		}
	}
	else //test point and temporary points
	{
		t = point_state.find_triangle(point_list[start]);
		vector<int> vertices = polygon_list[t];
		int v[2];
		for (int j = 0; j < 3; j++)
		{
			int first = vertices[j];
			int second = vertices[(j + 1) % 3];
			bool penetrate = check_penetration(from, to, start, first, second);
			bool onSameSide = all_left_or_right(vector<int>(from, to), first, second);
			if (penetrate && onSameSide)
			{
				v[0] = first;
				v[1] = second;
				break;
			}
		}

		vector<int> edges = triangle_edge_list[t];
		for (int j = 0; j < 3; j++)
		{
			if (diagonal_with_edge_list[edges[j]].check_same_edge(v[0], v[1]))
			{
				e = edges[j];
				break;
			}
		}
	}
	*tri = t;
	*edge = e;
}

int getFinalEdge(int start, int from, int to)
{
	int edge = -1;
	int tri = -1;
	getFirstTri(start, from, to, &edge, &tri);
	if (edge == -1)
		return edge;
	if (tri == -1)
		printf("error finding triangle in getFinalEdge()\n");

	while (edge < diagonal_list.size())
	{
		int new_tri = -1;
		int new_edge = -1;
		int v[3];
		int* t = diagonal_list[edge].get_triangle();
		if (t[0] == tri)
			new_tri = t[1];
		else
			new_tri = t[0];

		vector<int> vertices = polygon_list[new_tri];
		for (int i = 0; i < 3; i++)
		{
			if (diagonal_list[edge].check_same_point(vertices[i]) == -1) {
				v[0] = vertices[i];//the vertex not included in the EDGE
				v[1] = vertices[(i + 1) % 3];
				v[2] = vertices[(i + 2) % 3];
				break;
			}
		}
		tri = new_tri;
		vector<int> edges = triangle_edge_list[new_tri];

		bool penetrateLeft = check_penetration(from,to,start, v[0], v[1]);
		int otherPoint = (penetrateLeft) ? v[1] : v[2];

		for (int i = 0; i < 3; i++)
		{
			if (diagonal_with_edge_list[edges[i]].check_same_edge(v[0], otherPoint))
			{
				new_edge = edges[i];
				break;
			}
		}

		if (new_edge != -1)
			edge = new_edge;
	}

	return edge;
}
bool isVisible(int from, int to)
{
	int edge = getFinalEdge(from, from, to);
	if (edge == -1)
		return false;

	int v1 = diagonal_with_edge_list[edge].get_origin();
	int v2 = diagonal_with_edge_list[edge].get_dest();
	//reached polygon boundary
	Point* p = get_line_intersection(from, to, v1, v2);
	if (p == NULL) 
		return false;
	
	double px = p->get_x();
	//check if endPoint is equal to TO (loose check)
	double err = 0.00001;
	if (abs(point_list[to].get_x()-px)<err)
		return true;

	if ((point_list[from].get_x() - px) * (point_list[to].get_x() - px) <= 0)
		return false;
	else return true;// false;
}
Point computeEndpoint(int lineFrom, int lineTo)
{
	int edge = getFinalEdge(lineTo, lineFrom, lineTo);
	if (edge == -1)
		return point_list[lineTo];

	int v1 = diagonal_with_edge_list[edge].get_origin();
	int v2 = diagonal_with_edge_list[edge].get_dest();
	//reached polygon boundary
	Point* p = get_line_intersection(lineFrom, lineTo, v1, v2);
	if (p != NULL)
		return *p;
	else
		return Point(-1, -1);
}

Point foot_of_max_perpendicular(int p, Point origin, Point dest)
{
	Point foot = foot_of_perpendicular(p, origin, dest);
	
	point_list.push_back(origin);
	point_list.push_back(dest);
	Point a = computeEndpoint(point_list.size() - 2, point_list.size() - 1);
	Point b = computeEndpoint(point_list.size() - 1, point_list.size() - 2);
	double ax = a.get_x();
	double bx = b.get_x();
	double footx = foot.get_x();
	point_list.pop_back();
	point_list.pop_back();
	

	if ((ax - footx) * (bx - footx) > 0) //foot outside segment
	{
		if (abs(ax - footx) > abs(bx - footx))
			return b;
		else
			return a;
	}
	else
		return foot;
}

/* path within the funnel to a point */
vector<int> funnel_path_point(vector<int> chain1, vector<int> chain2, Point p)
{
	vector<int> funnel_path;
	
	if (chain1.size() == 1 || chain2.size() == 1)
		return funnel_path;
	
	point_list.push_back(p);
	double first = calculate_angle_between(chain1[0], chain1[1], point_list.size() - 1);
	double second = calculate_angle_between(chain2[0], chain2[1],point_list.size() - 1);
	
	if (first * second < 0) {
		point_list.pop_back();
		return funnel_path;
	}
	else if (first == 0)
	{
		funnel_path.push_back(chain1[1]);
		point_list.pop_back();
		return funnel_path;
	}
	else if (second == 0)
	{
		funnel_path.push_back(chain2[1]);
		point_list.pop_back();
		return funnel_path;
	}

	vector<int>* main_chain;
	double prev = 0;
	if (abs(first) < abs(second))
	{
		main_chain = &chain1;
		prev = first;
	}
	else
	{
		main_chain = &chain2;
		prev = second;
	}

	bool check = false;
	for (int i = 1; i < main_chain->size() - 1; i++)
	{
		funnel_path.push_back((*main_chain)[i]);
		double angle = calculate_angle_between((*main_chain)[i], (*main_chain)[i + 1], point_list.size() - 1);
		if (angle * prev < 0)
		{
			check = true;
			break;
		}
		prev = angle;
	}

	if (!check)
		funnel_path.push_back(main_chain->back());
	point_list.pop_back();
	return funnel_path;
}
pair<vector<int>,Point> funnel_path(vector<int> chain1, vector<int> chain2)
{
	vector<int> funnel_path;
	Point foot = foot_of_perpendicular(chain1.front(), point_list[chain1.back()], point_list[chain2.back()]);
	point_list.push_back(foot);

	double first = calculate_angle_between(chain1[0], chain1[1], point_list.size() - 1);
	double second = calculate_angle_between(chain2[0], chain2[1], point_list.size() - 1);
	
	//there may be precision problems here
	if (first * second < 0 )
	{
		point_list.pop_back();
		return pair<vector<int>,Point>(funnel_path,foot);
	}
	

	vector<int>* main_chain;
	double prev = 0;
	if (abs(first) < abs(second))
	{
		main_chain = &chain1;
		prev = first;
	}
	else//(abs(first)>abs(second))
	{
		main_chain = &chain2;
		prev = second;
	}

	bool perpendicular = false;
	for (int i = 1; i < main_chain->size()-1; i++)
	{
		double angle = calculate_angle_between((*main_chain)[i], (*main_chain)[i + 1], (*main_chain)[0], point_list.size() - 1);
		funnel_path.push_back((*main_chain)[i]);
		if (angle * prev <= 0) {
			perpendicular = true;
			break;
		}
		prev = angle;
	}

	if (perpendicular)
	{
		foot = (funnel_path.empty()) ? foot : foot_of_perpendicular(funnel_path.back(), point_list[chain1.back()], point_list[chain2.back()]);
	}
	else
	{
		foot = point_list[main_chain->back()];
	}
	point_list.pop_back();
	return pair<vector<int>, Point>(funnel_path, foot);
}

/* Retrieves shortest path from the root of the SPT to a random point */
vector<int> shortest_path_random_point(Point p, SPT* spt)
{
	vector<int> shortest_path;
	if (p.get_id() != -1)
	{
		shortest_path = spt->retrieve_shortest_path(p.get_id());
		shortest_path.pop_back();
		return shortest_path;
	}
	int tri = point_state.find_triangle(p);
	int tri_spt_root = point_state.find_triangle(point_list[spt->get_root()]);
	if (tri == tri_spt_root)
	{
		shortest_path.push_back(spt->get_root());
		return shortest_path;
	}
	vector<int> points = polygon_list[tri];

	vector<vector<int>> path;
	for (int i = 0; i < 3; i++)
	{
		path.push_back(vector<int>());
		path[i] = spt->retrieve_shortest_path(points[i]);
	}

	//extract common path
	int idx = 0;
	int path1_size = path[0].size();
	int path2_size = path[1].size();
	int path3_size = path[2].size();

	int min_size = min(min(path1_size, path2_size), path3_size);
	while (idx < min_size)
	{
		int path1 = path[0][idx];
		int path2 = path[1][idx];
		int path3 = path[2][idx];

		if (path1 == path2 && path1 == path3)
		{
			idx++;
			shortest_path.push_back(path1);
		}
		else
			break;
	}

	int apex = path[0][idx - 1];
	if (apex == points[0] || apex == points[1] || apex == points[2])
		return shortest_path; //point not included
	//all three vertices share common parent
	if (idx == path1_size - 1 && idx == path2_size - 1 && idx == path3_size - 1)
		return shortest_path;

	point_list.push_back(p);
	int i = 0;
	bool check = false;
	for (i = 0; i < 3; i++)
	{
		bool isSeparatingDiagonal = is_left(points[(i + 1) % 3], points[(i + 2) % 3], points[i]) != is_left(points[(i + 1) % 3], points[(i + 2) % 3], apex);
		if (isSeparatingDiagonal && path[(i + 1) % 3][idx] != path[(i + 2) % 3][idx]) {
			check = true;
			break;
		}

	}
	point_list.pop_back();

	if (!check)
	{
		printf("error in shortest_path_random_point()\n");
	}
	vector<int> chain1 = spt->retrieve_shortest_path(points[(i + 1) % 3]);
	vector<int> chain2 = spt->retrieve_shortest_path(points[(i + 2) % 3]);

	chain1.erase(chain1.begin(), chain1.begin() + idx - 1);
	chain2.erase(chain2.begin(), chain2.begin() + idx - 1);

	vector<int> funnel = funnel_path_point(chain1, chain2, p);
	shortest_path.insert(shortest_path.end(), funnel.begin(), funnel.end());

	return shortest_path;
}
pair<vector<int>, Point> shortest_path_line(Point p1, Point p2, SPT* spt)
{
	vector<int> shortest_path;
	Point foot;
	int idx = 0;

	vector<int> first = shortest_path_random_point(p1, spt);
	vector<int> second = shortest_path_random_point(p2, spt);
	int minSize = min(first.size(), second.size());

	for (;idx<minSize;idx++)
	{
		if (first[idx] == second[idx])
			shortest_path.push_back(first[idx]);
		else
			break;
	}

	//trimming the two chains
 	first.erase(first.begin(), first.begin() + idx - 1);
	second.erase(second.begin(), second.begin() + idx - 1);
	point_list.push_back(p1);
	point_list.push_back(p2);
	first.push_back(point_list.size() - 2);
	second.push_back(point_list.size() - 1);

	//getting shortest path within the funnel
	pair<vector<int>,Point> funnel = funnel_path(first, second);

	//restoring order
	point_list.pop_back();
	point_list.pop_back();

	shortest_path.insert(shortest_path.end(), funnel.first.begin(), funnel.first.end());

	return pair<vector<int>, Point>(shortest_path, funnel.second);
}
