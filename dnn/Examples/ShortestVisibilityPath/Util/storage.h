#pragma once
/*
enum event_type {
	Path,
	BOUNDARY_S,
	BOUNDARY_T,
	Bend
};
class LOS {
	int id;
	event_type type;
	int p[2]; //PATH: the two vertices of the shortest path pi(s,t)
	//BOUNDARY: p[0] will be the rotation vertex and p[1] will be the other polygon vertex 

	Point endpoint[2]; // the points where the extended lines meet with the polygon boundary
					   //PATH: [0] and [1] will both be used since path events can be extended bothways
					   //BOUDNARY: only [0] will be used to extend the line p[1]->p[0]
					   //float slope;
	float path_event_angle; //the angle between the previous path event (used to sort the boundary events)
	int rotation_vertex;

	vector<int> sp_line[2];//[0] : pi_s_l, [1] : pi_t_l
	//vector<int> pi_s_l;//shortest path from s to l (only the polygon vertices registered in point_list)
	//vector<int> pi_t_l;//shortest path from t to l (only the polygon vertices registered in point_list)
	Point foot[2]; //foot[0] indicates the foot of perpendicular in pi_s_l and foot[1] :  pi_t_l
	bool foot_is_P_vertex;
	int edge1[2]; //vertices of the edge that 'endpoint1 (endpoint2)' passes through
	int edge2[2];
public:
	LOS(int _id, int _p1, int _p2, int rot_vertex, float angle, event_type _type)
	{
		id = _id;
		p[0] = _p1;
		p[1] = _p2;
		type = _type;
		rotation_vertex = rot_vertex;
		//slope calculation
		//slope = compute_slope(p1, p2);
		path_event_angle = angle;
	}
	int get_p1() { return p[0]; }
	int get_p2() { return p[1]; }
	Point get_endpoint(bool idx) { return endpoint[idx]; }
	event_type get_type() { return type; }
	vector<int> get_pi_s_l() { return sp_line[0]; }
	vector<int> get_pi_t_l() { return sp_line[1]; }

	int get_rotation_vertex()
	{
		return rotation_vertex;
	}
	float get_path_angle()
	{
		return path_event_angle;
	}

	void set_pi_s_l(vector<int> pi)
	{
		sp_line[0] = pi;
	}
	void set_pi_t_l(vector<int> pi)
	{
		sp_line[1] = pi;
	}
	void set_foot_bool(bool is_polygon_vertex)
	{
		foot_is_P_vertex = is_polygon_vertex;
	}
	void set_path_angle(float _angle)
	{
		path_event_angle = _angle;
	}
	void set_endpoint(int idx, Point p)
	{
		if (idx == 0 || idx == 1)
			endpoint[idx] = p;
	}
	void compute_shortest_path_to_los(vector<int> shortest_path, SPT * *spt);
	bool compute_other_endpoint(bool is_type);
	vector<Point> get_shortest_path_to_line(bool s);
	Point* get_endpoint(int from, int to, int tri, int vertex1, int vertex2, bool first);
	vector<int> compute_shortest_path_line_nonP_vertex(Point vertex, SPT * spt, int* e);
	void extend_path_event();
};


vector<Point> LOS::get_shortest_path_to_line(bool s)
{
	vector<Point> sp;
	vector<int>* line = s ? &sp_line[0] : &sp_line[1];
	for (int i = 0; i < line->size(); i++)
	{
		sp.push_back(point_list[line->at(i)]);
	}

	sp.push_back(s?foot[0]:foot[1]);


	return sp;
}

// this is for the shortest path in the funnel i guess ? 
void get_remaining_path(vector<int> chain1, vector<int> chain2, vector<int>* final_path, Point* Foot)
{
	int apex = chain1[0];
	Point foot = foot_of_perpendicular(apex, point_list[chain1.back()], point_list[chain2.back()]);

	if (chain1.size() == 1 || chain2.size() == 1)
	{
		*Foot = foot;
		return;
	}
	int foot_idx = point_list.size();
	point_list.push_back(foot);

	bool direct = check_penetration(apex, foot_idx, apex, chain1[1], chain2[1]);


	if (direct) {
		*Foot = foot;
		point_list.pop_back();
		return;
	}

	vector<int> main_chain = (calculate_angle_between_positive(apex, chain1[1], apex, foot_idx) > calculate_angle_between_positive(apex, chain2[1], apex, foot_idx))
		? chain2 : chain1;
	bool side = is_left(main_chain[1], apex, foot_idx);/////////////not sure about the order of the arguments
	point_list.pop_back();

	for (int i = 1; i < main_chain.size() - 1; i++)
	{
		apex = main_chain[i];
		final_path->push_back(apex);
		//only for the boundary case
		foot = foot_of_perpendicular(apex, point_list[chain1.back()], point_list[chain2.back()]);
		point_list.push_back(foot);
		if (side != is_left(main_chain[i + 1], apex, foot_idx))
		{
			//correct foot
			*Foot = foot;
			point_list.pop_back();
			return;
		}
		point_list.pop_back();
	}

	//no foot of perpendicular
	//final_path->pop_back();
	*Foot = point_list[main_chain.back()];
	return;
}


vector<int> LOS::compute_shortest_path_line_nonP_vertex(Point vertex, SPT* spt, int* e)
{
	vector<int> shortest_path;
	vector<int> _e1 = spt->retrieve_shortest_path(e[0]);
	vector<int> _e2 = spt->retrieve_shortest_path(e[1]);

	int size = _e1.size() <= _e2.size() ? _e1.size() : _e2.size();
	int idx = 0;
	for (; idx < size; idx++)
	{
		if (_e1[idx] != _e2[idx])
			break;
	}

	//the common path for the two points of the edge (e1, e2) -> inserted from the beginning to the apex
	shortest_path.insert(shortest_path.end(), _e1.begin(), _e1.begin() + idx);


	//the different part
	vector<int> temp1(_e1.begin() + idx-1, _e1.end());
	vector<int> temp2(_e2.begin() + idx-1, _e2.end());

	if (temp1.size() == 1 || temp2.size() == 1)
		return shortest_path;
	int v_id = point_list.size();
	point_list.push_back(vertex);
	bool success = check_penetration(shortest_path.back(), v_id, temp1[0], temp1[1], temp2[1]);
	//point_list.pop_back();
	if (success) {
		point_list.pop_back();
		return shortest_path;
	}


	vector<int>* main_chain;
	int apex = temp1[0];
	double temp1A = calculate_angle_between_positive(apex, v_id, apex, temp1[1]);
	double temp2A = calculate_angle_between_positive(apex, v_id, apex, temp2[1]);
	if(temp1A > temp2A)
	//if(calculate_angle_between_positive(apex,v_id,apex,temp1[1])>calculate_angle_between_positive(apex, v_id,apex,temp2[1]))
		main_chain = &temp2;
	else
		main_chain = &temp1;

	point_list.pop_back();
	bool left = is_Left(point_list[main_chain->at(1)], point_list[apex], vertex);

	for (int i = 1; i < main_chain->size() - 1; i++)
	{
		apex = main_chain->at(i);
		shortest_path.push_back(apex);
		if (left!=is_Left(point_list[main_chain->at(i + 1)], point_list[apex], vertex))
		{
			return shortest_path;
		}
	}

	printf("this should be an error\n");
	exit(40);
}

void LOS::compute_shortest_path_to_los(vector<int> shortest_path, SPT** spt)
{
	if (type == Path)
	{
		//pi_s_l should be the shortest path to line p[0] and endpoint[0]
		//pi_t_l should be the shortest path to line p[1] and endpoint[1]
		for (int i = 0; i < 2; i++)
		{
			vector<int> to_p = spt[i]->retrieve_shortest_path(p[i]);
			point_list.push_back(endpoint[i]);
			vector<int> to_endp = compute_shortest_path_line_nonP_vertex(endpoint[i], spt[i], i==0?edge1:edge2);
			to_endp.push_back(point_list.size() - 1);

			int idx = 0;
			for (;idx<to_p.size() && idx<to_endp.size();idx++)
			{
				if (to_p[idx] != to_endp[idx])
					break;
			}

			sp_line[i].insert(sp_line[i].end(), to_p.begin(), to_p.begin() + idx);
			vector<int> temp1(to_p.begin() + idx - 1, to_p.end());
			vector<int> temp2(to_endp.begin() + idx - 1, to_endp.end());
			get_remaining_path(temp1, temp2, &sp_line[i], &foot[i]);
			point_list.pop_back();
		}
	}
	else
	{
		//vector<int>* other, * endpoint_;
		bool is_s = (type == BOUNDARY_S);

		vector<int> to_v = spt[!is_s]->retrieve_shortest_path(rotation_vertex);
		point_list.push_back(endpoint[0]);
		vector<int> to_other_endpoint = compute_shortest_path_line_nonP_vertex(endpoint[0], spt[!is_s], edge1);
		to_other_endpoint.push_back(point_list.size() - 1);
		int idx = 0;
		for (; idx < to_v.size() && idx < to_other_endpoint.size(); idx++)
		{
			if (to_v[idx] != to_other_endpoint[idx])
				break;
		}
		sp_line[!is_s].insert(sp_line[!is_s].end(), to_v.begin(), to_v.begin()+idx);
		vector<int> temp1(to_v.begin() + idx - 1, to_v.end());
		vector<int> temp2(to_other_endpoint.begin() + idx - 1, to_other_endpoint.end());
		get_remaining_path(temp1, temp2, &sp_line[!is_s],&foot[!is_s]);
		point_list.pop_back();

		to_v = spt[is_s]->retrieve_shortest_path(rotation_vertex);
		vector<int> to_endpoint2 = spt[is_s]->retrieve_shortest_path(p[1]);
		idx = 0;
		for (; idx < to_v.size() && idx < to_endpoint2.size(); idx++)
		{
			if (to_v[idx] != to_endpoint2[idx])
				break;
		}
		sp_line[is_s].insert(sp_line[is_s].end(), to_v.begin(), to_v.begin() + idx);
		temp1 = vector<int>(to_v.begin() + idx - 1, to_v.end());
		temp2 = vector<int>(to_endpoint2.begin() + idx - 1, to_endpoint2.end());
		get_remaining_path(temp1, temp2, &sp_line[is_s], &foot[is_s]);
	}

	return;
}

// computes the shortest path from the root of the spt to the line of sight

void LOS::compute_shortest_path_to_los(bool spt_s, vector<int> point_to_apex, vector<int> chain1, vector<int> chain2)
{
	//Inserts into the shortest path all the vertices from the root to the common apex
	vector<int> shortest_path(point_to_apex);
	vector<int>* dest = spt_s ? &pi_s_l : &pi_t_l;
	int apex = chain1[0];

	if (chain1.front() != chain2.front())
	{
		printf("not a valid chain (compute_shortest_path_to_los)\n");
		exit(35);
	}
	else if (chain1.size() == 1 || chain2.size() == 1) // the apex is the foot
	{
		shortest_path.pop_back(); //remove the apex (it's going to be the foot instead);
		foot = point_list[apex];
		*dest = shortest_path;
		return;
	}

	//calculate the foot of perpendicular
	Point foot = (type == PATH) ? foot_of_perpendicular(apex, point_list[endpoint1], point_list[endpoint2]) : foot_of_perpendicular(apex, point_list[endpoint1], other_endpoint);
	int foot_idx = point_list.size();
	point_list.push_back(foot);

	bool direct = check_penetration(apex, foot_idx, apex, chain1[1], chain2[1]);
	if (direct)
	{
		this->foot = foot;
		point_list.pop_back();
		*dest = shortest_path;
		return;
	}
	vector<int> main_chain = (calculate_angle_between_positive(apex, chain1[1], apex, foot_idx) > calculate_angle_between_positive(apex, chain2[1], apex, foot_idx))
		? chain2 : chain1;
	bool side = is_left(apex, foot_idx, main_chain[1]);
	for (int i = 1; i < main_chain.size() - 1; i++)
	{
		apex = main_chain[i];
		shortest_path.push_back(apex);
		point_list.pop_back();
		//foot = recalculate new foot of perpendicular
		foot = (type == PATH) ? foot_of_perpendicular(apex, point_list[endpoint1], point_list[endpoint2]) : foot_of_perpendicular(apex, point_list[endpoint1], other_endpoint);
		point_list.push_back(foot);

		//check if our foot is correct
		if (side != is_left(apex, foot_idx, main_chain[i + 1]))
		{
			//chosen
			point_list.pop_back();
			*dest = shortest_path;
			return;
		}
	}

	// no foot of perpendicular
	point_list.pop_back();
	foot = point_list[main_chain.back()];
	*dest = shortest_path;
	return;
}

// Returns whether the line (start, foot) doesn't overlap with the chain 
bool check_valid_foot(vector<int> chain, bool left, int start, Point foot)
{
	vector<int>::iterator it = find(chain.begin(), chain.end(), start);

	if (it + 1 == chain.end())
	{
		printf("this shouldn't be happening\n");
		exit(47);
	}

	if (left != is_Left(point_list[(*it)], foot, point_list[*(it + 1)]))
		return true;
	else
		return false;
}
//returns the index of the triangle in polygon_list that (endpoint, rotation) penentrates through
   //@rotation : the rotation vertex
  // @endpoint : the other vertex of the boundary event
 //  @vertex : the name of the int[2] array that contains the indices of the two other triangle vertices
 
int choose_triangle(int rotation, int endpoint, int* vertex)
{
	//list of all triangles adjacent to vertex `rotation'
	vector<int> candidates = point_state.find_all_triangles(point_list[rotation]);
	int triangle;

	vertex[0] = -1;
	vertex[1] = -1;
	//find the triangle that (endpoint,rotation) penentrates through!!


	if (rotation == v_num + 3 || rotation == v_num + 4)
	{
		triangle = candidates.front();

		vector<int> v = polygon_list[triangle];

		for (int i = 0; i < 3; i++)
		{
			if (check_penetration(endpoint, rotation, rotation, v[i], v[(i + 1) % 3]))
			{
				vertex[0] = v[i];
				vertex[1] = v[(i + 1) % 3];
				break;
			}
		}

		if (vertex[0] == -1)
		{
			printf(" vertex should have been set by now\n");
			exit(67);
		}

		return triangle;
	}


	for (int i = 0; i < candidates.size(); i++)
	{
		triangle = candidates[i];
		for (int j = 0; j < 3; j++)
		{
			if (polygon_list[triangle][j] == rotation)
			{
				vertex[0] = polygon_list[triangle][(j + 1) % 3];
				vertex[1] = polygon_list[triangle][(j + 2) % 3];
				break;
			}
		}

		if (vertex[0] == -1 || vertex[1] == -1)
			break;

		if (check_penetration(endpoint, rotation, rotation, vertex[0], vertex[1]))
			return triangle;
	}

	return -1;
}


Point * LOS::get_endpoint(int from, int to, int tri, int vertex1, int vertex2,bool first)
{
	Triangle triangle = t_list[tri];
	int* diag_list = triangle.get_d_list();
	int* p_list = triangle.get_p_list();
	int other_vertex = -1;
	int chosen_vertex = -1;
	int diag = -1;
	int new_tri = -1;;

	for (int i = 0; i < 3; i++)
	{
		if (p_list[i] != vertex1 && p_list[i] != vertex2)
		{
			other_vertex = p_list[i];
			break;
		}
	}

	if (check_penetration(from, to, to, vertex1, other_vertex))
		chosen_vertex = vertex1;
	else if (check_penetration(from, to, to, vertex2, other_vertex))
		chosen_vertex = vertex2;
	else
		return NULL;

	//check if polygon edge
	int diff = abs(chosen_vertex - other_vertex);
	if (diff == 1 || diff == (v_num - 1)) {
		int* edge = (type != Path) ? edge1 : (first ? edge1 : edge2);//)first ? edge1 : edge2;
		edge[0] = chosen_vertex;
		edge[1] = other_vertex;
		return get_line_intersection(from, to, chosen_vertex, other_vertex);
	}
	else
	{
		//find new triangle
		for (int i = 0; i < 3; i++)
		{
			if (diag_list[i] != -1 && diagonal_list[diag_list[i]].check_same_point(chosen_vertex) != -1 && diagonal_list[diag_list[i]].check_same_point(other_vertex) != -1)
				diag = diag_list[i];
		}

		if (diag == -1)
			return NULL;

		if (diagonal_list[diag].get_triangle()[0] == tri)
			new_tri = diagonal_list[diag].get_triangle()[1];
		else
			new_tri = diagonal_list[diag].get_triangle()[0];

		return get_endpoint(from, to, new_tri, chosen_vertex, other_vertex,first);
	}
}


bool LOS::compute_other_endpoint(bool is_type)
{
	int vertex[2] = { -1, -1 };
	//int* edge = first ? edge1 : edge2;
	bool idx = 1;

	do {

		idx = !idx;
		int tri = choose_triangle(p[idx], p[!idx], vertex);
		if (tri == -1)
			return false;

		int diff = abs(vertex[0] - vertex[1]);
		if (diff == 1 || diff == (v_num - 1))
		{
			endpoint[idx] = *get_line_intersection(p[idx], p[!idx], vertex[0], vertex[1]);
			int* edge = idx == 0 ? edge1 : edge2;// is_type ? (idx == 0 ? edge1 : edge2) : (edge1);// idx == 0 ? edge2 : edge1;
			edge[0] = vertex[0];
			edge[1] = vertex[1];
		}
		else
		{
			int new_tri = -1;
			int* d_list = t_list[tri].get_d_list();
			int diag = -1;
			for (int i = 0; i < 3; i++)
			{
				if (d_list[i] != -1 && diagonal_list[d_list[i]].check_same_point(vertex[0]) != -1 && diagonal_list[d_list[i]].check_same_point(vertex[1]) != -1)
				{
					diag = d_list[i];
					break;
				}
			}
			if (diag == -1)
				return false;

			if (diagonal_list[diag].get_triangle()[0] == tri)
				new_tri = diagonal_list[diag].get_triangle()[1];
			else
				new_tri = diagonal_list[diag].get_triangle()[0];

			//boundary types only get idx==false (TYPEs get false-true in order)
			Point * ptr = get_endpoint(p[!idx], p[idx], new_tri, vertex[0], vertex[1],!idx);
			if (ptr == NULL)
				return false;
			this->endpoint[idx] = *ptr;
		}
	} while (is_type && idx==false);
	//Point res = computeEndpoint(p[0], p[1]);
	//Point res2 = computeEndpoint(p[1], p[0]);
	return true;
}


Point compute_bend_event_endpoint(int p1, int p2, int rotation_vertex)
{
	Point foot = foot_of_perpendicular(rotation_vertex, point_list[p1], point_list[p2]);
	int foot_tri = point_state.find_triangle(foot);
	int vertex[2];

	int tri = choose_triangle(p2, p1, vertex);
	if (tri == foot_tri)
		return foot;

	while (!is_polygon_edge(vertex[0],vertex[1]))
	{
		//set the new diag (vertex[0], vertex[1])
		int* diag_list = t_list[tri].get_d_list();
		int diag = -1;
		for (int i = 0; i < 3; i++)
		{
			int d = diag_list[i];
			if (d != -1 && diagonal_list[d].check_same_edge(vertex[0], vertex[1]))
			{
				diag = d;
				break;
			}
		}
		//find opposite triangle to diag
		tri = opposite_tri(tri, diag);
		if (tri == foot_tri)
			return foot;

		//getting the other endpoint
		Triangle t = t_list[tri];
		int* p_list = t.get_p_list();
		int other_p;
		for (int i = 0; i < 3; i++)
		{
			if (p_list[i] != vertex[0] && p_list[i] != vertex[1])
			{
				other_p = p_list[i];
				break;
			}
		}

		//setting vertex[0] and vertex[1] -> the next diag
		if (check_penetration(p1, p2, p2, vertex[0], other_p))
		{
			vertex[1] = other_p;
		}
		else if (check_penetration(p1, p2, p2, vertex[1], other_p))
		{
			vertex[0] = other_p;
		}
	}

	//diag should be the polygon edge
	return *get_line_intersection(p1, p2,vertex[0],vertex[1]);

}
LOS* add_bend_event(LOS* path_event, int rotation_vertex, bool first)
{
	//get the foot of perpendicular from the rotation vertex to line(p1,p2)
	int p1 = path_event->get_p1();
	int p2 = path_event->get_p2();
	Point foot = foot_of_perpendicular(rotation_vertex, point_list[p1], point_list[p2]);

	//if the foot is in the polygon boundary
	int valid = point_state.find_triangle(foot);
	if (valid != -1)
	{
		LOS los(-1, rotation_vertex, -1, rotation_vertex, -1, Bend);
		los.set_endpoint(0, foot);
		return &los;
		//los connects points rotation_vertex and foot
	}
	else //not inside the polygon ->we find the intersection with the polygon boundary
	{
		//guess what!? we already computed it!! it's part of the path event
		foot = path_event->get_endpoint(first);
		LOS los(-1, rotation_vertex, -1, rotation_vertex, -1, Bend);
		los.set_endpoint(0, foot);
		return &los;
	}

}*/