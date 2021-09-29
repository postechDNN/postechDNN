#pragma once
#include "LOS.h"
using namespace std;

LINE* minSumLine;
LINE* minMaxLine;

class EVENTS {
	int next_line_id;
	vector<vector<LINE*>> Queue;
	vector<int> shortest_path;
	vector<ROT> rotation; // true for CW, false for CCW
	SPT* spt[2]; //[0]: spt_s (s as root) , [1]: spt_t (t as root)
public:
	EVENTS() {}
	EVENTS(vector<int> _shortest_path, SPT* _spt_s, SPT* _spt_t)
	{
		next_line_id = 0;
		shortest_path = _shortest_path;
		for (int i = 0; i < shortest_path.size()-1; i++)
		{
			Queue.push_back(vector<LINE *> ());
		}
		spt[0] = _spt_s;
		spt[1] = _spt_t;
	}
	vector<int> get_shortest_path()
	{
		return shortest_path;
	}
	vector<vector<LINE*>> getQueue() {
		return Queue;
	}
	void sort_by_slope();
	void compute_path_events();
	void compute_boundary_events();
	void compute_bend_events();
	void compute_min_line(void);
	void compute_min_max(void);
	void compute_min_sum(void);
};

//sorts from small -> big
bool compare_slope(LINE* a, LINE* b)
{
	return (a->getSlope() < b->getSlope());
}


/* adds a LOS to the queue vector for every edge in the shortest path (s,t) */
void EVENTS::compute_path_events()
{
	for (int i = 0; i < shortest_path.size()-1; i++)
	{
		int prev = shortest_path[i];
		int cur = shortest_path[i + 1];
		
		PATH* line = new PATH(prev, cur,spt);
		Queue[i].push_back(line);
	}

	rotation.push_back(DEFAULT);
	for (int i = 1; i < shortest_path.size() - 1; i++)
	{
		double angle = calculate_angle_between(shortest_path[i], shortest_path[i + 1], shortest_path[i - 1], shortest_path[i]);
		//atan2 결과 양수이면 ccw 음수면 CW 0이면 전 결과 그대로
		if (angle > 0)
			rotation.push_back(CCW);
		else if (angle < 0)
			rotation.push_back(CW);
		else
			rotation.push_back(rotation.back());
	}
}

/* determines whether the line (*not vector) (CUR, P) is tangent to the path (PREV~CUR~NEXT) at vertex CUR */
bool is_tangent(int prev, int cur, int next, int p)
{
	float first = calculate_angle(prev, cur);
	float second = calculate_angle(cur, next);

	float p_cur = calculate_angle(p, cur);
	float cur_p = calculate_angle(cur, p);

	float angle1 = normalize_angle(first - p_cur);
	float angle2 = normalize_angle(p_cur - second);

	if (angle1*angle2 > 0)
		return true;
	
	angle1 = normalize_angle(first - cur_p);
	angle2 = normalize_angle(cur_p - second);

	if(angle1*angle2 > 0)
		return true;
	
	return false;
}

/* computes all boundary events
	sorts the boundary events by slope after inserting into the queue */
void EVENTS::compute_boundary_events()
{
	//search the tree for candidates 
	for (int i = 1; i < shortest_path.size() - 1; i++)
	{
		int prev = shortest_path[i - 1];
		int cur = shortest_path[i];
		int next = shortest_path[i + 1];

		//find the vertex in the spt and the direct children will be the candidate
		SPTnode* parent_s = spt[0]->get_node(cur);
		SPTnode* parent_t = spt[1]->get_node(cur);
		if (parent_s == NULL || parent_t ==NULL )
		{
			printf("couldn't find node in tree\n");
			exit(-1);
		}

		vector<SPTnode*> candidates(parent_s->get_children());
		int s_size = candidates.size();
		vector<SPTnode*> t_kids = parent_t->get_children();
		candidates.insert(candidates.end(), t_kids.begin(),t_kids.end());
		
		//then check the tangent thing...
		for (int j = 0; j < candidates.size(); j++)
		{
			int vertex_id = candidates[j]->get_id();
			if (vertex_id != next && vertex_id != shortest_path[i - 1])
			{
				if (is_tangent(prev,cur,next, vertex_id))
				{
					BOUNDARY* boundary = new BOUNDARY(cur, vertex_id,spt,(j<s_size));
					Queue[i - 1].push_back(boundary);
				}
			}
		}
	}
	sort_by_slope();
}

void EVENTS::sort_by_slope()
{
	for (int i = 0; i < Queue.size()-1; i++)
	{
		double curS = Queue[i].front()->getSlope();
		double nextS = Queue[i + 1].front()->getSlope();
		bool type1 = false;
		std::sort(Queue[i].begin(), Queue[i].end(), compare_slope);
		vector<LINE*> newQueue;
		int curIdx = 0;
		for (; curIdx < Queue[i].size(); curIdx++)
		{
			if (curS == Queue[i][curIdx]->getSlope())
				break;
		}
		//this is for the tBEND_BOUNDARY_PATH events
		if (curIdx != Queue[i].size() - 1 && Queue[i][curIdx + 1]->getSlope() == curS)
			type1 = true;

		int nxtIdx = Queue[i].size();
		
		ROT rot = rotation[i+1];
		vector<LINE*>::iterator it = Queue[i].begin() + curIdx + 1;//points to place right after path event

		if (rot == CW) {
			newQueue.insert(newQueue.begin(), it, Queue[i].end());//path event 직후부터 끝까지 넣음
			newQueue.insert(newQueue.end(), Queue[i].begin(), it);//처음부터 path event 까지 넣음
			reverse(newQueue.begin(), newQueue.end());
			if (type1)
			{
				newQueue.insert(newQueue.begin() + 1, newQueue.back());
				newQueue.pop_back();
			}
			Queue[i] = newQueue;
		
		}
		else if (rot == CCW)
		{
			Queue[i].insert(Queue[i].end(), Queue[i].begin(), Queue[i].begin() + curIdx);
			Queue[i].erase(Queue[i].begin(), Queue[i].begin() + curIdx);
		}
	}
}

void EVENTS::compute_bend_events()
{
	int s_size = shortest_path.size();
	vector<int> prev = Queue[0][0]->getPath(0), cur, cur_t = Queue[0][0]->getPath(1), prev_t;
	double slope_prev = Queue[0][0]->getSlope(), slope_cur;
	for (int i = 0; i < Queue.size(); i++)
	{
		vector<LINE*> tempQueue;
		for (int j = 0; j < Queue[i].size(); j++)
		{
			cur = Queue[i][j]->getPath(0);
			prev_t = Queue[i][j]->getPath(1);
			slope_cur = Queue[i][j]->getSlope();
			int idx = (j == 0) ? i : i + 1;
			int rot = shortest_path[idx];
			//inspect paths with root _s
			int idx_prev = 0, idx_cur = 0;
			while (idx_cur != cur.size() || idx_prev != prev.size())
			{
				if (idx_cur != cur.size() && idx_prev != prev.size() && cur[idx_cur] == prev[idx_prev])
					idx_cur++, idx_prev++;
				else
				{
					if (idx_cur < cur.size()) {
						vector<int>::iterator it = find(prev.begin(), prev.end(), cur[idx_cur]);
						if (it == prev.end()) //type 1 (ii)
						{
							BEND* bend = new BEND(rot, cur[idx_cur - 1], cur[idx_cur], spt,0,true);
							idx_cur++;
							if (bend->getType() != tERROR && is_tangent_slope(bend->getSlope(),slope_prev,slope_cur,rotation[idx]))
							{
								if (j == 0)
									Queue[i - 1].push_back(bend);
								else
									tempQueue.push_back(bend);
							}
						}
					}
					if (idx_prev < prev.size()) {
						vector<int>::iterator it = find(cur.begin(), cur.end(), prev[idx_prev]);
						if (it == cur.end()) //type 2
						{
							BEND* bend = new BEND(rot, prev[idx_prev - 1], prev[idx_prev], spt,0,false);
							idx_prev++;
							if (bend->getType() != tERROR && is_tangent_slope(bend->getSlope(),slope_prev,slope_cur,rotation[idx]))
							{
								if (j == 0)
									Queue[i - 1].push_back(bend);
								else
									tempQueue.push_back(bend);
							}
						}
					}

				}
			}
			//bend events in terms of paths with root _t
			idx_prev = 0, idx_cur = 0;
			while (idx_cur != cur_t.size() || idx_prev != prev_t.size())
			{
				if (idx_cur != cur_t.size() && idx_prev != prev_t.size() && cur_t[idx_cur] == prev_t[idx_prev])
					idx_cur++,idx_prev++;
				else
				{
					if (idx_cur < cur_t.size()) {
						vector<int>::iterator it = find(prev_t.begin(), prev_t.end(), cur_t[idx_cur]);
						if (it == prev_t.end()) //type 1 (ii)
						{
							BEND* bend = new BEND(rot, cur_t[idx_cur - 1], cur_t[idx_cur], spt,1,false);
							idx_cur++;
							if (bend->getType() != tERROR && is_tangent_slope(bend->getSlope(), slope_prev, slope_cur, rotation[idx]))
							{
								if (j == 0)
									Queue[i - 1].push_back(bend);
								else
									tempQueue.push_back(bend);
							}
						}
					}
					if (idx_prev < prev_t.size()) {
						vector<int>::iterator it = find(cur_t.begin(), cur_t.end(), prev_t[idx_prev]);
						if (it == cur_t.end()) //type 2
						{
							BEND* bend = new BEND(rot, prev_t[idx_prev - 1], prev_t[idx_prev], spt,1,true);
							idx_prev++;
							if (bend->getType() != tERROR && is_tangent_slope(bend->getSlope(), slope_prev, slope_cur, rotation[idx]))
							{
								if (j == 0)
									Queue[i - 1].push_back(bend);
								else
									tempQueue.push_back(bend);
							}
						}
					}
				}
			}

			prev = cur;
			cur_t = prev_t;
			slope_prev = slope_cur;
		}
		Queue[i].insert(Queue[i].end(), tempQueue.begin(), tempQueue.end());
	}

	
	ROT before = DEFAULT;
	for (int i = 1; i < Queue.size(); i++)
	{
		ROT current = rotation[i];
		if (before!=DEFAULT && before != current)
		{
			Queue[i - 1][0]->setType1(true);
		}
		before = current;
	}

	sort_by_slope();
}


double dist(double slope, int u, int v)
{
	Point U = point_list[u];
	Point V = point_list[v];

	double distance = abs(slope * U.get_x() - U.get_y() - slope * V.get_x() + V.get_y());
	distance /= sqrt(slope * slope + 1);
	return distance;
}


double getSlopeMinSum(double bound1, double bound2, ROT dir, int v, int u, int u_, double* sum)
{
	double minSlope = bound1;
	/* need to check three possible cases
	(i) boundary slopes
	(ii) absolute value turn slopes
	(iii) local optimum/maximum slopes */

	if (u == -1 || u_ == -1 || v == -1)
		printf("whatthe");
	double minSumDist = dist(bound1, u, v) + dist(bound1, u_, v);

	double distance = dist(bound2, u, v) + dist(bound2, u_, v);
	if (minSumDist > distance)
	{
		minSlope = bound2;
		minSumDist = distance;
	}

	//(ii) case
	Point U = point_list[u];
	Point U_ = point_list[u_];
	Point V = point_list[v];
	double c1 = U.get_x() - V.get_x();
	double c2 = V.get_y() - U.get_y();
	double c_1 = U_.get_x() - V.get_x();
	double c_2 = V.get_y() - U_.get_y();

	for (int i = 0; i < 2; i++)
	{
		double tempSlope = (i == 0) ? (-c2 / c1) : (-c_2 / c_1);
		if (is_tangent_slope(tempSlope, bound1, bound2, dir))
		{
			distance = dist(tempSlope, u, v) + dist(tempSlope, u_, v);
			if (minSumDist > distance)
			{
				minSlope = tempSlope;
				minSumDist = distance;
			}
		}
	}

	//(iii) case
	double tempSlope = (c1 + c_1) / (c2 + c_2);
	if (is_tangent_slope(tempSlope, bound1, bound2, dir) && (c1 * tempSlope + c2) * (c_1 * tempSlope + c_2) > 0)
	{
		distance = dist(tempSlope, u, v) + dist(tempSlope, u_, v);
		if (minSumDist > distance)
		{
			minSlope = tempSlope;
			minSumDist = distance;
		}
	}
	tempSlope = (c1 - c_1) / (c2 - c_2);
	if (is_tangent_slope(tempSlope, bound1, bound2, dir) && (c1 * tempSlope + c2) * (c_1 * tempSlope + c_2) < 0)
	{
		distance = dist(tempSlope, u, v) + dist(tempSlope, u_, v);
		if (minSumDist > distance)
		{
			minSlope = tempSlope;
			minSumDist = distance;
		}
	}

	*sum = minSumDist;
	return minSlope;
}

/* given two boundary lines, find the slope rotating around v that minimizes MAX(dist from u, dist from u') */
double getSlopeMinMax(LINE* prev, LINE* cur, ROT dir, int v, int u, int u_, double* Max)
{
	double bias1 = prev->getLength_noFoot(0), bias2 = prev->getLength_noFoot(1);
	double bound1 = prev->getSlope(), bound2 = cur->getSlope();

	double minDist, minSlope;

	Point U = point_list[u];
	Point U_ = point_list[u_];
	Point V = point_list[v];
	double c1 = U.get_x() - V.get_x();
	double c2 = V.get_y() - U.get_y();
	double c_1 = U_.get_x() - V.get_x();
	double c_2 = V.get_y() - U_.get_y();

	//check the two boundary values
	double first = max(dist(bound1, u, v) + bias1, dist(bound1, u_, v) + bias2);
	double second = max(dist(bound2, u, v) + bias1, dist(bound2, u_, v) + bias2);
	if (first >= second)
		minSlope = bound2, minDist = second;
	else
		minSlope = bound1, minDist = first;

	vector<double> candidate_slopes;

	//check for possible intersection points
	double beta = bias1 - bias2;
	double A = pow((c1 - c_1), 2) - pow(beta, 2);
	double B = 2*(c1 * c2 + c_1 * c_2 - c1 * c_2 - c_1 * c2);
	double C = pow((c2 - c_2), 2) - pow(beta, 2);
	double D = pow(B, 2) - 4 * A * C;
	if (D > 0)
	{
		double plus = (-B + sqrt(D)) / 2 / A;
		double minus = (-B - sqrt(D)) / 2 / A;
		candidate_slopes.push_back(plus);
		candidate_slopes.push_back(minus);
	}
	else if (D == 0)
		candidate_slopes.push_back((-B + sqrt(D)) / 2 / A);
	
	A = pow((c1 + c_1), 2) - pow(beta, 2);
	B = 2 * (c1 * c2 + c_1 * c_2 + c1 * c_2 + c_1 * c2);
	C = pow((c2 + c_2), 2) - pow(beta, 2);
	if (D > 0)
	{
		double plus = (-B + sqrt(D)) / 2 / A;
		double minus = (-B - sqrt(D)) / 2 / A;
		candidate_slopes.push_back(plus);
		candidate_slopes.push_back(minus);
	}
	else if (D == 0)
		candidate_slopes.push_back((-B + sqrt(D)) / 2 / A);

	//check for possible local min/maxes
	candidate_slopes.push_back(-c1 / c2);
	candidate_slopes.push_back(-c_1 / c_2);

	//go through the candidates and check if max is min
	for (int i = 0; i < candidate_slopes.size(); i++)
	{
		double slp = candidate_slopes[i];
		//make sure slope is in range
		if (is_tangent_slope(slp, bound1, bound2, dir)) {
			double dst = max(bias1 + dist(slp, u, v), bias2 + dist(slp, u_, v));
			if (dst < minDist)
				minDist = dst, minSlope = slp;
		}
	}

	*Max = minDist;
	return minSlope;
}

double getSlopeMinSum(LINE * prev, LINE * cur, ROT dir, int v, int u, int u_, double* sum)
{
	double bound1 = prev->getSlope();
	double bound2 = cur->getSlope();
	//case (i) : boundaries
	double minDist = min(prev->getLength(), cur->getLength());
	double minSlope = (minDist == bound1) ? bound1 : bound2;
	
	Point U = point_list[u];
	Point U_ = point_list[u_];
	Point V = point_list[v];
	double c1 = U.get_x() - V.get_x();
	double c2 = V.get_y() - U.get_y();
	double c_1 = U_.get_x() - V.get_x();
	double c_2 = V.get_y() - U_.get_y();

	//case (ii) : absolute value boundaries
	for (int i = 0; i < 2; i++)
	{
		double tempSlope = (i == 0) ? (-c2 / c1) : (-c_2 / c_1);
		if (is_tangent_slope(tempSlope, bound1, bound2, dir))
		{
			double distance = dist(tempSlope, u, v) + dist(tempSlope, u_, v);
			if (minDist > distance)
				minSlope = tempSlope, minDist = distance;
		}
	}

	//case (iii) local optimum/minimum candidates
	double tempSlope = (c1 + c_1) / (c2 + c_2);
	if (is_tangent_slope(tempSlope, bound1, bound2, dir) && (c1 * tempSlope + c2) * (c_1 * tempSlope + c_2) > 0)
	{
		double distance = dist(tempSlope, u, v) + dist(tempSlope, u_, v);
		if (minDist > distance)
		{
			minSlope = tempSlope;
			minDist = distance;
		}
	}
	tempSlope = (c1 - c_1) / (c2 - c_2);
	if (is_tangent_slope(tempSlope, bound1, bound2, dir) && (c1 * tempSlope + c2) * (c_1 * tempSlope + c_2) < 0)
	{
		double distance = dist(tempSlope, u, v) + dist(tempSlope, u_, v);
		if (minDist > distance)
		{
			minSlope = tempSlope;
			minDist = distance;
		}
	}
	*sum = minDist;
	return minSlope;
}

void EVENTS::compute_min_max(void) {
	//find v
	int v, i;
	double slope_prev, slope_next;
	bool former = (Queue[0][0]->getLength(0) >= Queue[0][0]->getLength(1));
	for (i = 0; i < Queue.size(); i++)
	{
		LINE* line = Queue[i][0];
		if (former) {
			if (line->getLength(0) <= line->getLength(1)) {
				v = line->getV();
				slope_prev = line->getSlope();
				slope_next = Queue[i + 1][0]->getSlope();
				break;
			}
		}
		else {
			if (line->getLength(1) <= line->getLength(0)) {
				v = line->getV();
				slope_prev = Queue[--i][0]->getSlope();
				slope_next = line->getSlope();
				break;
			}
		}
	}

	LINE* start = Queue[i][0], *end = Queue[i][0], *prev = Queue[i][0];
	double minMax = std::numeric_limits<double>::infinity();
	vector<double> tempSum;
	LINE* reference;

	//check all the events within slope_prev~slope_next
	for (int count=0, j=1; count<Queue[i].size()+1;j++,count++)
	{
		if (j == Queue[i].size())
			i++, j = 0;
		
		end = Queue[i][j];
		TYPE type_start = start->getType();
		TYPE type_end = end->getType();

		//this we need to fix later
		if (type_end == tBOUNDARY)
			continue;

		//set reference for u, u', v
		int idx = (j == 0) ? i : i + 1;
		if (type_start == tBEND_del || start->getType1())
			reference = end;
		else
			reference = start;
		int u = reference->getPath(0).back();
		int u_ = reference->getPath(1).back();
		int v = reference->getV();
		if (type_end == tPATH)
			v = start->getV();

		double sum;
		double slope = getSlopeMinMax(start, end, rotation[idx], v, u, u_, &sum);
		LINE* temp = new LINE(v, slope, reference->getPath(0), reference->getPath(1));
		if (sum < minMax)
			minMax = sum, minMaxLine = temp;

		start = end;
	}
	if (minMaxLine == NULL)
		printf("what the fuck");
	else
		minMaxLine->computeEndpointWithSlope();
	minMaxLine->setLength_withFoot();
	Queue[--i].push_back(minMaxLine);
	printf("stop here dudes");
}

void EVENTS::compute_min_sum(void)
{
	LINE* start = Queue[0][0], * end = Queue[0][0], * prev = Queue[0][0];
	double minSum = std::numeric_limits<double>::infinity();
	vector<double> tempSum;
	LINE* reference;
	for (int i = 0; i < Queue.size(); i++)
	{
		for (int j = 0; j < Queue[i].size(); j++)
		{
			end = Queue[i][j];
			TYPE type_start = start->getType();
			TYPE type_end = end->getType();

			if (type_end == tBOUNDARY)
				continue;

			int idx = (j == 0) ? i : i + 1;
			if (type_start == tBEND_del || start->getType1())
				reference = end;
			else
				reference = start;
			
			int u = reference->getPath(0).back();
			int u_ = reference->getPath(1).back();
			int v = reference->getV();
			if (type_end == tPATH)
				v = start->getV();

			double sum;
			double slope = getSlopeMinSum(start, end, rotation[idx], v, u, u_, &sum);
			//double slope = getSlopeMinSum(start, cur, rotation[idx], refV, refU, refU_, &sum);
			LINE * temp = new LINE(v, slope, reference->getPath(0), reference->getPath(1)); //need to adjust this later for bend_del cases
			sum += temp->getDistanceSum();
			tempSum.push_back(sum);
			if (sum < minSum) {
				minSum = sum; minSumLine = temp;
			}
			start = end;
		}
	}

	minSumLine->computeEndpointWithSlope();
	printf("stop here");
}

void EVENTS::compute_min_line(void)
{
	double minSum = std::numeric_limits<double>::infinity();
	double minMax = std::numeric_limits<double>::infinity();

	for (int i = 0; i < Queue.size(); i++)
	{
		for (int j = 0; j<Queue[i].size(); j++)
		{
			LINE* line = Queue[i][j];

			double tempSum = line->getLength();
			double tempMax = max(line->getLength(0), line->getLength(1));

			if (tempSum < minSum)
			{
				minSumLine = line;
				minSum = tempSum;
			}
			if (tempMax < minMax)
			{
				minMaxLine = line;
				minMax = tempMax;
			}
		}
	}
}