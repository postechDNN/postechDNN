#pragma once
#include <memory>
#include <queue>
#include <fstream>
#include "../RBTree/Red Black Tree.h"
using namespace std;

float sweep_t;

class Site {
private:
	//if vertical site -> slope == FLT_MAX, formula : x = c;
	//y = slope * t + c || x = c
	float slope;
	float c;
	pair<float, float> l_endpoint; //for vertical site, l_endpoint is upper endpoint
	pair<float, float> r_endpoint; //for vertical site, r_endpoint is lower endpoint
public:
	Site(float l_x, float l_y, float r_x, float r_y) {
		l_endpoint = make_pair(l_x, l_y);
		r_endpoint = make_pair(r_x, r_y);
		if (l_x == r_x) {
			//vertical
			slope = FLT_MAX;
			c = l_x;
			if (l_y < r_y) {
				l_endpoint = make_pair(r_x, r_y);
				r_endpoint = make_pair(l_x, l_y);
			}
		}
		else {
			slope = (l_y - r_y) / (l_x - r_x);
			c = l_endpoint.second - slope * l_endpoint.first;
		}
	}
	pair<float, float> get_l_endpoint() {
		return l_endpoint;
	}
	pair<float, float> get_r_endpoint() {
		return r_endpoint;
	}
	float get_slope() {
		return slope;
	}
	float get_c() {
		return c;
	}
	float get_y(float x = sweep_t) {
		float x_l = l_endpoint.first;
		float y_l = l_endpoint.second;
		float y = slope * (x - x_l) + y_l;
		return y;
	}
	/*float get_y() {
		return get_y(sweep_t);
	}*/
	bool is_vertical() {
		return l_endpoint.first == r_endpoint.first;
	}
};

class Vertex {
private:
	pair<float, float> vertex;
public:
	Vertex() {
		vertex = make_pair(-FLT_MAX, -FLT_MAX);
	}
	Vertex(float x, float y) {
		vertex = make_pair(x, y);
	}
	Vertex(pair<float, float> v) {
		vertex = v;
	}
	pair<float, float> get_vertex() {
		return vertex;
	}
	Vertex & operator=(const Vertex &vertex2) {
		vertex = vertex2.vertex;
		return *this;
	}
	bool operator!=(const Vertex &vertex2) {
		return vertex.first != vertex2.vertex.first || vertex.second != vertex2.vertex.second;
	}
};

enum Position {
	leftOrRignt, leftend, rightend
};
class Edge {
private:
	float slope = -FLT_MAX;
	Vertex * endpoint1 = nullptr;
	Vertex * endpoint2 = nullptr;
	Position position = leftOrRignt;
	bool real;
public:
	Edge() {

	}
	/*Edge(Vertex * e1, bool real_) {
		endpoint1 = e1;
		real = real_;
	}*/
	Edge(Vertex * e1, Position l, float s, bool real_) {
		slope = s;
		position = l;
		endpoint1 = e1;
		real = real_;
	}
	Edge(Vertex * e1, Vertex * e2, bool real_) {
		endpoint1 = e1;
		endpoint2 = e2;
		real = real_;
	}
	void set_real(bool real_) {
		real = real_;
	}
	Edge & operator=(const Edge &e2) {
		slope = e2.slope;
		endpoint1 = e2.endpoint1;
		endpoint2 = e2.endpoint2;
		return *this;
	}
	pair<float, float> get_endpoint1() {
		if (endpoint1 == nullptr) {
			if (slope == 1)
				return make_pair(-FLT_MAX, -FLT_MAX);
			else
				return make_pair(-FLT_MAX, FLT_MAX);
		}
		return endpoint1->get_vertex();
	}
	pair<float, float> get_endpoint2() {
		if (endpoint2 == nullptr) {
			if (slope == 1)
				return make_pair(FLT_MAX, FLT_MAX);
			else
				return make_pair(FLT_MAX, -FLT_MAX);
		}
		return endpoint2->get_vertex();
	}
	Vertex * get_vertex1() {
		return endpoint1;
	}
	Vertex * get_vertex2() {
		return endpoint2;
	}
	bool is_real() {
		return real;
	}
	Position get_position() {
		return position;
	}
	float get_slope() {
		return slope;
	}
	void set_endpoint2(Vertex * p2) {
		this->endpoint2 = p2;
	}
};

//site out 때 뒤로 나가는 45 ray의 경우 p_de / p_in이 아닌,
//자기자신 line이 2사분면에 있는 경우 : 자기자신 line과 site out 때 끝점의 bisector == +1 ray (wavekind == pl)
//										site out 때 끝점과 끝점에서 뒤로 나가는 수평선의 bisector == -1 ray (wavekind == pl)
//자기자신 line이 4사분면에 있는 경우 :  자기자신 line과 site out 때 끝점의 bisector == -1 ray (wavekind == pl)
//										site out 때 끝점과 끝점에서 뒤로 나가는 수평선의 bisector == +1 ray (wavekind == pl)
enum WaveKind {
	none_wave,
	pp, //point_element1, point_element2
	pl, //point_element1, line_element1
	ll, //line_element1, line_element2
	l
};
enum PointKind {
	nothing, p_up, p_down, p_right
};

class Wave {
private:
	WaveKind wavekind;
	float y_slope;
	float y_c;
	float x_slope;
	float x_c;

	pair<float, float> point_element1; //pp1,  pl, p_de, p_in 
	PointKind pointkind1;
	pair<float, float> point_element2; //pp2
	PointKind pointkind2;
	Site * line_element1;//pl, ll1, l
	Site * line_element2;//ll2
	Vertex * l_vertex;//if l_vertex != nullptr then this wave will be voronoi edge starts from the vertex that has l_vertex_index of vertex vector
					  //start from v1 / v2 / spike event
protected:
public:
	//pp
	Wave() {
		wavekind = none_wave;
		y_slope = -FLT_MAX;
		y_c = -FLT_MAX;
		x_slope = -FLT_MAX;
		x_c = -FLT_MAX;
		point_element1 = make_pair<float, float>(-FLT_MAX, -FLT_MAX);
		pointkind1 = nothing;
		point_element2 = make_pair<float, float>(-FLT_MAX, -FLT_MAX);
		pointkind2 = nothing;
		line_element1 = nullptr;
		line_element2 = nullptr;
		l_vertex = nullptr;
	}
	/*Wave(WaveKind wavekind_, pair<float, float> p1, PointKind pk1, pair<float, float> p2, PointKind pk2, Site * l1, Site * l2, float y_slope_, float y_c_, float x_slope_, float x_c_) {
		if (wavekind_ == pp) Wave(wavekind_, p1, pk1, p2, pk2, y_slope_, y_c_, x_slope_, x_c_);
		else if (wavekind_ == pl) Wave(wavekind_, p1, pk1, l1, y_slope_, y_c_, x_slope_, x_c_);
		else if (wavekind_ == ll) Wave(wavekind_, l1, l2, y_slope_, y_c_, x_slope_, x_c_);
		else Wave(wavekind_, l1, y_slope_, y_c_, x_slope_, x_c_);
	}*/
	//pp
	Wave(WaveKind wavekind_, pair<float, float> p1, PointKind pk1, pair<float, float> p2, PointKind pk2, float y_slope_, float y_c_, float x_slope_, float x_c_) {
		wavekind = wavekind_;
		y_slope = y_slope_;
		y_c = y_c_;
		x_slope = x_slope_;
		x_c = x_c_;
		point_element1 = p1;
		pointkind1 = pk1;
		point_element2 = p2;
		pointkind2 = pk2;
		l_vertex = nullptr;

		line_element1 = nullptr;
		line_element2 = nullptr;
	}
	//pl
	Wave(WaveKind wavekind_, pair<float, float> p1, PointKind pk1, Site * l1, float y_slope_, float y_c_, float x_slope_, float x_c_) {
		wavekind = wavekind_;
		y_slope = y_slope_;
		y_c = y_c_;
		x_slope = x_slope_;
		x_c = x_c_;
		point_element1 = p1;
		pointkind1 = pk1;
		line_element1 = l1;
		l_vertex = nullptr;

		point_element2 = make_pair<float, float>(-FLT_MAX, -FLT_MAX);
		pointkind2 = nothing;
		line_element2 = nullptr;
	}
	//ll
	Wave(WaveKind wavekind_, Site * l1, Site * l2, float y_slope_, float y_c_, float x_slope_, float x_c_) {
		wavekind = wavekind_;
		y_slope = y_slope_;
		y_c = y_c_;
		x_slope = x_slope_;
		x_c = x_c_;
		line_element1 = l1;
		line_element2 = l2;
		l_vertex = nullptr;

		point_element1 = make_pair<float, float>(-FLT_MAX, -FLT_MAX);
		pointkind1 = nothing;
		point_element2 = make_pair<float, float>(-FLT_MAX, -FLT_MAX);
		pointkind2 = nothing;
	}
	//l
	Wave(WaveKind wavekind_, Site * l1, float y_slope_, float y_c_, float x_slope_, float x_c_) {
		wavekind = wavekind_;
		y_slope = y_slope_;
		y_c = y_c_;
		x_slope = x_slope_;
		x_c = x_c_;
		line_element1 = l1;
		l_vertex = nullptr;

		point_element1 = make_pair<float, float>(-FLT_MAX, -FLT_MAX);
		pointkind1 = nothing;
		point_element2 = make_pair<float, float>(-FLT_MAX, -FLT_MAX);
		pointkind2 = nothing;
		line_element2 = nullptr;
	}
	void set_l_vertex(Vertex * v) {
		l_vertex = v;
	}
	bool operator== (const Wave &wave2)
	{
		bool same = this->wavekind == wave2.wavekind;
		same &= (this->y_slope == wave2.y_slope) && (this->y_c == wave2.y_c) && (this->x_slope == wave2.x_slope) && (this->x_c == wave2.x_c);
		same &= this->point_element1 == wave2.point_element1;
		same &= this->pointkind1 == wave2.pointkind1;
		same &= this->point_element2 == wave2.point_element2;
		same &= this->pointkind2 == wave2.pointkind2;
		same &= this->line_element1 == wave2.line_element1;
		same &= this->line_element2 == wave2.line_element2;
		same &= this->l_vertex == wave2.l_vertex;
		return same;
	}
	bool operator<= (const Wave &wave2)
	{
		float y = this->y_slope * sweep_t + this->y_c;
		float y2 = wave2.y_slope * sweep_t + wave2.y_c;

		return y <= y2;
	}
	bool operator>= (const Wave &wave2)
	{
		float y = this->y_slope * sweep_t + this->y_c;
		float y2 = wave2.y_slope * sweep_t + wave2.y_c;

		return y >= y2;
	}
	bool operator> (const Wave &wave2)
	{
		float y = this->y_slope * (sweep_t) + this->y_c;
		float y2 = wave2.y_slope * (sweep_t) + wave2.y_c;

		if (y == y2) {
			y = this->y_slope * (sweep_t + 1) + this->y_c;
			y2 = wave2.y_slope * (sweep_t + 1) + wave2.y_c;
		}

		return y > y2;
	}
	bool compare(Wave key1, Wave key2) {
		return key1 > key2;
	}
	bool compare_insert(const Wave &wave2) {
		float y = this->y_slope * (sweep_t)+this->y_c;
		float y2 = wave2.y_slope * (sweep_t)+wave2.y_c;

		if (y == y2) {
			y = this->y_slope * (sweep_t + 1) + this->y_c;
			y2 = wave2.y_slope * (sweep_t + 1) + wave2.y_c;
		}

		return y > y2;
	}
	bool compare_delete(const Wave &wave2) {
		float y = this->y_slope * (sweep_t)+this->y_c;
		float y2 = wave2.y_slope * (sweep_t)+wave2.y_c;

		if (y == y2) {
			y = this->y_slope * (sweep_t - 1) + this->y_c;
			y2 = wave2.y_slope * (sweep_t - 1) + wave2.y_c;
		}

		return y > y2;
	}
	Wave & operator=(const Wave &wave2) {
		wavekind = wave2.wavekind;
		y_slope = wave2.y_slope;
		y_c = wave2.y_c;
		x_slope = wave2.x_slope;
		x_c = wave2.x_c;
		point_element1 = wave2.point_element1;
		pointkind1 = wave2.pointkind1;
		point_element2 = wave2.point_element2;
		pointkind2 = wave2.pointkind2;
		line_element1 = wave2.line_element1;
		line_element2 = wave2.line_element2;
		l_vertex = wave2.l_vertex;
		return *this;
	}

	pair<float, float> get_wave_vertex(float t = sweep_t) {
		float x_t, y_t;
		x_t = x_slope * t + x_c;
		y_t = y_slope * t + y_c;
		return make_pair(x_t, y_t);
	}
	WaveKind get_wavekind() { return wavekind; }
	pair<float, float> get_p1() { return point_element1; }
	pair<float, float> get_p2() { return point_element2; }
	PointKind get_pointkind1() { return pointkind1; }
	PointKind get_pointkind2() { return pointkind2; }
	Site * get_l1() { return line_element1; }
	Site * get_l2() { return line_element2; }
	Vertex * get_v() { return l_vertex; }
	float get_slope() {
		if (x_slope == 0) return FLT_MAX;
		else return y_slope / x_slope;
	}
	//return intersection center and x pos of sweep line
	pair<pair<float, float>, float> check_intersection(Wave &wave2) {
		float y1_slope = y_slope;
		float y1_c = y_c;
		float y2_slope = wave2.y_slope;
		float y2_c = wave2.y_c;
		float x1_slope = x_slope;
		float x1_c = x_c;
		float x2_slope = wave2.x_slope;
		float x2_c = wave2.x_c;

		if (y1_slope == y2_slope ) { 
			if (y1_c != y2_c)
				return make_pair(make_pair(-FLT_MAX, -FLT_MAX), -FLT_MAX);
			else {
				float t = (x2_c - x1_c) / (x1_slope - x2_slope);
				float y = y_slope * t + y_c;
				float x = x_slope * t + x_c;
				pair<float, float> center = make_pair(x, y);
				return make_pair(center, t);
			}
		}
		if (x1_slope == x2_slope) {
			if (x1_c != x2_c)
				return make_pair(make_pair(-FLT_MAX, -FLT_MAX), -FLT_MAX);
			else {
				float t = (y2_c - y1_c) / (y1_slope - y2_slope);
				float y = y_slope * t + y_c;
				float x = x_slope * t + x_c;
				pair<float, float> center = make_pair(x, y);
				return make_pair(center, t);
			}
		}

		float t_y = (y2_c - y1_c) / (y1_slope - y2_slope);
		float t_x = (x2_c - x1_c) / (x1_slope - x2_slope);
		if (t_y == t_x && t_y > sweep_t) {
			//spike event occur
			float t = t_y;
			float y = y_slope * t + y_c;
			float x = x_slope * t + x_c;
			pair<float, float> center = make_pair(x, y);
			return make_pair(center, t);
		}
		
		return make_pair(make_pair(-FLT_MAX, -FLT_MAX), -FLT_MAX);
	}

};

//typedef shared_ptr<Wave> Waveptr;

//typedef shared_ptr<Site> Siteptr;

enum EventKind {
	SITE_IN,
	SITE_OUT,
	SITE_VER,
	SPIKE
};

class Event {
private:
	EventKind eventkind;
	float event_xPos;
	//site event
	Site * site_event_owner;
	//spike event
	pair<float, float> spike_point;
	Wave spike_wave1;
	Wave spike_wave2;
public:
	//site event
	Event(EventKind site_or_spike_, float event_xPos_, Site * site_event_owner_) {
		eventkind = site_or_spike_;
		event_xPos = event_xPos_;
		site_event_owner = site_event_owner_;
		spike_point = make_pair(-FLT_MAX, -FLT_MAX);
		spike_wave1 = Wave();
		spike_wave2 = Wave();
	}
	//spike event
	Event(EventKind site_or_spike_, float event_xPos_, pair<float, float> spike_p, Wave spike_wave1_, Wave spike_wave2_) {
		eventkind = site_or_spike_;
		event_xPos = event_xPos_;
		site_event_owner = nullptr;
		spike_point = spike_p;
		spike_wave1 = spike_wave1_;
		spike_wave2 = spike_wave2_;
	}
	//Overload the > operator.
	bool operator> (const  Event &event2) const
	{
		if (this->event_xPos == event2.event_xPos) {
			return this->eventkind != SPIKE && event2.eventkind == SPIKE;
		}
		/*if (site_or_spike == SPIKE && event2.site_or_spike != SPIKE && event_xPos == event2.event_xPos)
			return true;*/
		else
			return this->event_xPos > event2.event_xPos;
	}
	EventKind get_eventKind() {
		return eventkind;
	}
	float get_xPos() {
		return event_xPos;
	}
	Site * get_owner() {
		return site_event_owner;
	}
	pair<float, float> get_spike_point() {
		return spike_point;
	}
	pair<Wave, Wave> get_spike_waves() {
		return make_pair(spike_wave1, spike_wave2);
	}
	/*
	Wave * get_top_wave() {
		return top_spike_wave;
	}
	Wave * get_bottom_wave() {
		return bottom_spike_wave;
	}*/
};

class WaveFront : public RBT<Wave> {
public:
	pair<float, float> get_wavefront_vertex(Node<Wave> *& node) {
		return node->key.get_wave_vertex();
	}
	bool compare(Wave key1, Wave key2) {
		float key1_y = key1.get_wave_vertex().second;
		float key2_y = key2.get_wave_vertex().second;
		if (key1_y >= key2_y) return true;
		else return false;
	}
	Node<Wave> * find_same_or_biggest_small(float v2_y) {
		Node<Wave> * tmp = tail; //start from biggest
		while (tmp->pre != nullptr && tmp->pre != head) {
			Wave wave = tmp->pre->key;
			float wave_vertex_y = wave.get_wave_vertex().second;
			if (wave_vertex_y <= v2_y)
				return tmp->pre;
			tmp = tmp->pre;
		}
		return nullptr;//all keys > input key
	}
	Node<Wave> * find_same_or_smallest_big(float v1_y) {
		Node<Wave> * tmp = head; //start from smallest
		while (tmp->next != nullptr && tmp->next != head) {
			Wave wave = tmp->next->key;
			float wave_vertex_y = wave.get_wave_vertex().second;
			if (wave_vertex_y >= v1_y)
				return tmp->next;
			tmp = tmp->next;
		}
		return nullptr;//all keys < input key
	}
	void predict_spike_event(Wave & wave, priority_queue<Event, vector<Event>, greater<vector<Event>::value_type>> *& EventQ) {
		insert_bst(wave);
		Node<Wave> * tmp;
		tmp = root;
		while (tmp != leaf) { //tmp != nullptr
			if (tmp->key == wave) {
				Node<Wave> * pre, *next;
				pre = tmp->pre;
				next = tmp->next;
				pair<pair<float, float>, float> pre_spike = wave.check_intersection(pre->key);
				pair<pair<float, float>, float> next_spike = wave.check_intersection(next->key);
				pair<pair<float,float>,float> none_spike = make_pair(make_pair(-FLT_MAX, -FLT_MAX), -FLT_MAX);
				if (pre_spike != none_spike) {
					EventQ->push(Event(SPIKE, pre_spike.second, pre_spike.first, tmp->key, pre->key));
				}
				if (next_spike != none_spike) {
					EventQ->push(Event(SPIKE, next_spike.second, next_spike.first, next->key, tmp->key));
				}
				break;
			}
			if (compare(tmp->key, wave)) {
				tmp = tmp->left_child;
			}
			else {
				tmp = tmp->right_child;
			}
		}
	}
	Node<Wave> * search_spike(Wave key) {
		Node<Wave> * tmp = root;
		while (tmp != leaf) {
			if (tmp->key == key)
				return tmp;
			else {
				//tmp->key > key ? tmp = tmp->left_child : tmp = tmp->right_child;
				compare_spike(tmp->key, key) ? tmp = tmp->left_child : tmp = tmp->right_child;
			}
		}
		return nullptr;
	}
	bool compare_spike(Wave key1, Wave key2) {
		float y1 = key1.get_wave_vertex().second;
		float y2 = key2.get_wave_vertex().second;

		if (y1 == y2) {
			y1 = key1.get_wave_vertex(sweep_t - 1).second;
			y2 = key2.get_wave_vertex(sweep_t - 1).second;
		}

		return y1 > y2;
	}
	friend class Voronoi;
};
//typedef shared_ptr<WaveFront> WaveFrontptr;

class Voronoi {
private:
	vector<Site *> site_vector;
	vector<Vertex *> vertex_vector;
	vector<Edge *> edge_vector;
	WaveFront * wavefrontptr;
	//WaveFront wavefront;
	priority_queue<Event, vector<Event>, greater<Event>> * EventQ;
public:
	Voronoi(vector<Site *> sv, vector<Vertex *> vv, vector<Edge *> ev) {
		site_vector = sv;
		vertex_vector = vv;
		edge_vector = ev;
		wavefrontptr = new WaveFront();
		EventQ = new priority_queue<Event, vector<Event>, greater<vector<Event>::value_type>>;
	}
	~Voronoi() {
		delete wavefrontptr;
		delete EventQ;
		vector<Vertex *>::iterator v_it;
		for (v_it = vertex_vector.begin(); v_it != vertex_vector.end(); v_it++) {
			delete (*v_it);
		}
		vector<Edge *>::iterator e_it;
		for (e_it = edge_vector.begin(); e_it != edge_vector.end(); e_it++) {
			delete (*e_it);
		}
		vector<Site *>::iterator s_it;
		for (s_it = site_vector.begin(); s_it != site_vector.end(); s_it++) {
			delete (*s_it);
		}
	}
	pair<float, float> find_v1(pair<float, float> p1) {
		//find v1
		pair<float, float> v1;
		float x_in = p1.first;
		float y_in = p1.second;

		Node<Wave> *wave1, *wave2;
		wave1 = wavefrontptr->root;
		Node<Wave> *leaf = wavefrontptr->leaf;
		if (wave1 != leaf) {
			if (wave1->left_child == leaf && wave1->right_child == leaf) {
				//only one wavefront vertex exist in RBT
				pair<float, float> wave_vertex = wavefrontptr->get_wavefront_vertex(wave1);
				float x1 = wave_vertex.first;
				float y1 = wave_vertex.second;
				float y1_ = x_in + y_in - x1;
				if (y1_ <= y1) {
					//v1 exist
					float v1_x = (x_in + y_in + x1 - y1) / (float)2;
					float v1_y = (x_in + y_in - x1 + y1) / (float)2;
					v1 = make_pair(v1_x, v1_y);
				}
				else {
					//v1 is infinity value
					v1 = make_pair(-FLT_MAX, FLT_MAX);
				}
			}
			else {
				//more than one wave vertex
				if (wave1->next == wavefrontptr->tail) {
					wave2 = wave1;
					wave1 = wave2->pre;
				}
				else {
					wave2 = wave1->next;
				}
				while (1) {
					pair<float, float>wave_vertex2 = wavefrontptr->get_wavefront_vertex(wave2);
					pair<float, float>wave_vertex1 = wavefrontptr->get_wavefront_vertex(wave1);

					float x2 = wave_vertex2.first;
					float y2 = wave_vertex2.second;
					float x1 = wave_vertex1.first;
					float y1 = wave_vertex1.second;
					float y2_ = x_in + y_in - x2;
					float y1_ = x_in + y_in - x1;
					float x_tmp, y_tmp;
					if (y2 < y1) {
						x_tmp = x2;
						x2 = x1;
						x1 = x_tmp;
						y_tmp = y2;
						y2 = y1;
						y1 = y_tmp;
					}
					if (y1_ >= y1 && y2_ <= y2 && !(x_in == x1 && y_in == y1) && !(x_in == x2 && y_in == y2)) {
						if (x2 == x1) {
							v1 = make_pair(x1, -x1 + x_in + y_in);
							break;
						}
						float b = (y2 - y1) / (x2 - x1);
						if (b == -1) {
							v1 = make_pair(x1, y1);
							break;
						}
						float v1_x = (b * x1 - y1 + x_in + y_in) / (b + 1);
						float v1_y = ((-b) * x1 + y1 + b * x_in + b * y_in) / (b + 1);
						v1 = make_pair(v1_x, v1_y);
						break;
					}
					else if (y1_ < y1) {
						if (wave1->pre == wavefrontptr->head) {
							float v1_x = (x_in + y_in + x1 - y1) / 2.0;
							float v1_y = (x_in + y_in - x1 + y1) / 2.0;
							v1 = make_pair(v1_x, v1_y);
							break;
						}
						else {
							wave2 = wave1;
							wave1 = wave2->pre;
						}
					}
					else {//y2_>y2
						if (wave2->next == wavefrontptr->tail) {
							v1 = make_pair(-FLT_MAX, FLT_MAX);
							break;
						}
						else {
							wave1 = wave2;
							wave2 = wave1->next;
						}
					}
				}
			}

		}
		else {
			v1 = make_pair(-FLT_MAX, FLT_MAX);
		}

		return v1;
	}
	pair<float, float> find_v2(pair<float, float>p2) {
		//find v2
		pair<float, float> v2;
		float x_in = p2.first;
		float y_in = p2.second;

		Node<Wave> *wave1, *wave2;
		wave1 = wavefrontptr->root;
		Node<Wave> *leaf = wavefrontptr->leaf;
		if (wave1 != leaf) {
			if (wave1->left_child == leaf && wave1->right_child == leaf) {
				//only one wave front vertex
				pair<float, float> wave_vertex = wavefrontptr->get_wavefront_vertex(wave1);
				float x1 = wave_vertex.first;
				float y1 = wave_vertex.second;
				float y1_ = (-x_in) + y_in + x1;
				if (y1_ >= y1) {
					//v1 exist
					float v2_x = (x1 + y1 + x_in - y_in) / (float)2;
					float v2_y = (x1 + y1 - x_in + y_in) / (float)2;
					v2 = make_pair(v2_x, v2_y);
				}
				else {
					//v1 is infinity value
					v2 = make_pair(-FLT_MAX, -FLT_MAX);
				}
			}
			else {
				//more than one wave vertex
				if (wave1->next == wavefrontptr->tail) {
					wave2 = wave1;
					wave1 = wave2->pre;
				}
				else {
					wave2 = wave1->next;
				}
				while (1) {
					pair<float, float>wave_vertex2 = wavefrontptr->get_wavefront_vertex(wave2);
					pair<float, float>wave_vertex1 = wavefrontptr->get_wavefront_vertex(wave1);
					float x2 = wave_vertex2.first;
					float y2 = wave_vertex2.second;
					float x1 = wave_vertex1.first;
					float y1 = wave_vertex1.second;
					float y2_ = (-x_in) + y_in + x2;
					float y1_ = (-x_in) + y_in + x1;
					float x_tmp, y_tmp;
					if (y2 < y1) {
						x_tmp = x2;
						x2 = x1;
						x1 = x_tmp;
						y_tmp = y2;
						y2 = y1;
						y1 = y_tmp;
					}
					if (y1_ >= y1 && y2_ <= y2 && !(x_in == x1 && y_in == y1) && !(x_in == x2 && y_in == y2)) {
						float b, v2_x, v2_y;
						if (x2 == x1) {
							v2 = make_pair(x1, x1 - x_in + y_in);
							break;
						}
						b = (y2 - y1) / (x2 - x1);
						if (b == 1) {
							v2 = make_pair(x2, y2);
							break;
						}
						v2_x = (b * x1 - y1 - x_in + y_in) / (b - 1);
						v2_y = (b * x1 - y1 - b * x_in + b * y_in) / (b - 1);
						v2 = make_pair(v2_x, v2_y);
						break;
					}
					else if (y1_ <= y1) {
						if (wave1->pre == wavefrontptr->head) {
							v2 = make_pair(-FLT_MAX, -FLT_MAX);
							break;
						}
						else {
							wave2 = wave1;
							wave1 = wave2->pre;
						}
					}
					else {//y2_>y2
						if (wave2->next == wavefrontptr->tail) {
							float v2_x = (x2 + y2 + x_in - y_in) / 2.0;
							float v2_y = (x2 + y2 - x_in + y_in) / 2.0;
							v2 = make_pair(v2_x, v2_y);
							break;
						}
						else {
							wave1 = wave2;
							wave2 = wave1->next;
						}
					}
				}
			}

		}
		else {
			v2 = make_pair(-FLT_MAX, -FLT_MAX);
		}

		return v2;
	}

	void create() {
		ofstream voronoi("voronoi_diagram.txt");
		vector<Site *>::iterator it;
		//WaveFront wavefront;
		//priority_queue<Event, vector<Event>, greater<Event>> EventQ;

		//insert site event
		for (it = site_vector.begin(); it != site_vector.end(); it++) {
			if ((*it)->get_l_endpoint().first == (*it)->get_r_endpoint().first) { // vertical site
				EventQ->push(Event(SITE_VER, (*it)->get_l_endpoint().first, *it));
			}
			else {
				EventQ->push(Event(SITE_IN, (*it)->get_l_endpoint().first, *it));
				EventQ->push(Event(SITE_OUT, (*it)->get_r_endpoint().first, *it));
			}
		}
		vector <Site *>::iterator s_it;
		voronoi << site_vector.size() << endl;
		for (s_it = site_vector.begin(); s_it != site_vector.end(); s_it++) {
			voronoi << (*s_it)->get_l_endpoint().first << " " << (*s_it)->get_l_endpoint().second << " "
				<< (*s_it)->get_r_endpoint().first << " " << (*s_it)->get_r_endpoint().second << endl;
		}
		//event handling
		while (!EventQ->empty()) {
			Event e = EventQ->top();
			EventQ->pop();
			sweep_t = e.get_xPos();
			//spike event handling
			if (e.get_eventKind() == SPIKE) {
				if (check_invalid_spike(e)) continue;
				spike(e);
			}
			//site event handling
			else if (e.get_eventKind() == SITE_IN) {
				site_in(e);
			}
			else if (e.get_eventKind() == SITE_OUT) {
				site_out(e);
			}
			else { //e.get_eventKind() == SITE_VER
				site_ver(e);
			}
		}
	
		Node<Wave> * tmp = wavefrontptr->head->next;
		while (tmp != wavefrontptr->tail) {
			if (tmp->key.get_v() != nullptr) {
				float slope = tmp->key.get_slope();
				if (slope != FLT_MAX)
					edge_vector.push_back(new Edge(tmp->key.get_v(), leftend, slope, 1));
			}
			tmp = tmp->next;
		}

		vector<Vertex *>::iterator v_it;
		vector<Edge *>::iterator e_it;	

		for (e_it = edge_vector.begin(); e_it != edge_vector.end(); e_it++) {
			if ((*e_it)->is_real()) {
				if ((*e_it)->get_position() == leftend) {
					/*if ((*e_it)->get_slope() == 1) {
						pair<float, float> p1 = (*e_it)->get_endpoint1();
						Vertex * v = new Vertex(p1.first + 500.0, p1.second + 500.0);
						(*e_it)->set_endpoint2(v);
					}
					else if ((*e_it)->get_slope() == -1) {
						pair<float, float> p1 = (*e_it)->get_endpoint1();
						Vertex * v = new Vertex(p1.first + 500.0, p1.second - 500.0);
						(*e_it)->set_endpoint2(v);
					}*/
					pair<float,float> p = (*e_it)->get_endpoint1();
					Vertex * v = new Vertex(p.first + 500.0, p.second + 500 * (*e_it)->get_slope());
					vertex_vector.push_back(v);
					(*e_it)->set_endpoint2(v);
				}
				else if ((*e_it)->get_position() == rightend) {
					/*if ((*e_it)->get_slope() == 1) {
						pair<float, float> p1 = (*e_it)->get_endpoint1();
						Vertex * v = new Vertex(p1.first - 500.0, p1.second - 500.0);
						(*e_it)->set_endpoint2(v);
					}
					else if ((*e_it)->get_slope() == -1) {
						pair<float, float> p1 = (*e_it)->get_endpoint1();
						Vertex * v = new Vertex(p1.first - 500.0, p1.second + 500.0);
						(*e_it)->set_endpoint2(v);
					}*/
					pair<float, float> p = (*e_it)->get_endpoint1();
					Vertex * v = new Vertex(p.first - 500.0, p.second - 500 * (*e_it)->get_slope());
					vertex_vector.push_back(v);
					(*e_it)->set_endpoint2(v);
				}
				//voronoi << (*e_it)->get_endpoint1().first << " " << (*e_it)->get_endpoint1().second << " " << (*e_it)->get_endpoint2().first << " " << (*e_it)->get_endpoint2().second << endl;
			}
			else
				voronoi << "unreal" << endl;
		}
	
		voronoi << vertex_vector.size() << endl;
		for (v_it = vertex_vector.begin(); v_it != vertex_vector.end(); v_it++) {
			voronoi << (*v_it)->get_vertex().first << " " << (*v_it)->get_vertex().second << endl;
		}

		voronoi << edge_vector.size() << endl;
		for (e_it = edge_vector.begin(); e_it != edge_vector.end(); e_it++) {
			Vertex * v1 = (*e_it)->get_vertex1();
			Vertex * v2 = (*e_it)->get_vertex2();
			int index1 = -1;
			int index2 = -1;
			int count = 0;
			for (v_it = vertex_vector.begin(); v_it != vertex_vector.end(); v_it++) {
				if ((*v_it) == v1) {
					index1 = count;
				}
				if ((*v_it) == v2) {
					index2 = count;
				}
				if (index1 != -1 && index2 != -1)
					break;
				count++;
			}
			//voronoi << (*e_it)->get_endpoint1().first << " " << (*e_it)->get_endpoint1().second << " " << (*e_it)->get_endpoint2().first << " " << (*e_it)->get_endpoint2().second << endl;
			voronoi << index1 << " " << index2 << endl;
		}

	}
	void site_in(Event &e) {
		Site * s = e.get_owner();
		pair<float, float> l_endpoint = s->get_l_endpoint();
		//find v1, v2 and add vetex & edge, delete wave between v1 & v2
		if (!wavefrontptr->is_empty()) 
			site_in_find_v1v2(s, l_endpoint, l_endpoint);

		float slope = s->get_slope();
		float c = s->get_c();
		//new wave on site
		Wave w1(l, s, slope, c, 1, 0);
		wavefrontptr->predict_spike_event(w1, EventQ);
		//new wave by 45 rays
		if (slope > 0) add_new_wave_45_dec_ray(l_endpoint, s);
		else if (slope < 0) add_new_wave_45_inc_ray(l_endpoint, s);
	}
	void site_in_find_v1v2(Site * s, pair<float,float> &l_point, pair<float,float> &r_point) {
		Vertex * l_ptr = new Vertex(l_point);
		Vertex * r_ptr = new Vertex(r_point);
		pair< float, float > v1_result;
		pair< float, float > v2_result;
		pair<float, float> v1_element_p = make_pair(-FLT_MAX, -FLT_MAX);
		Site * v1_element_l = nullptr;
		pair<float, float> v2_element_p = make_pair(-FLT_MAX, -FLT_MAX);
		Site * v2_element_l = nullptr;
		//find v1, v2
		v1_result = find_v1(l_point);
		v2_result = find_v2(r_point);
		//add voronoi edge & vertex
		Node<Wave> * tmp;
		Node<Wave> * first = wavefrontptr->find_same_or_biggest_small(v2_result.second);
		Node<Wave> * last = wavefrontptr->find_same_or_smallest_big(v1_result.second);
		Vertex * pre_vertex = nullptr;
		Vertex * v1 = nullptr;
		Vertex * v2 = nullptr;
		tmp = first;
		if (tmp == nullptr) {//case: no v2
			if (wavefrontptr->head->next != last) {
				tmp = wavefrontptr->head->next;
				pre_vertex = new Vertex(tmp->key.get_wave_vertex());
				edge_vector.push_back(new Edge(pre_vertex, rightend, 1, 1));
				vertex_vector.push_back(pre_vertex);
				if (tmp->key.get_v() != nullptr)
					edge_vector.push_back(new Edge(tmp->key.get_v(), pre_vertex, 1));
				//else edge_vector.push_back(new Edge(pre_vertex, 0));
			}
			else
				tmp = wavefrontptr->head;
		}
		else if (tmp->key.get_wave_vertex() == v2_result) {//v2 == wave vertex
			find_common_element(tmp->key, tmp->pre->key, v2_element_p, v2_element_l);
			v2 = new Vertex(v2_result);
			pre_vertex = v2;
			vertex_vector.push_back(pre_vertex);
			if (tmp->key.get_v() != nullptr)
				edge_vector.push_back(new Edge(tmp->key.get_v(), pre_vertex, 1));
			//else edge_vector.push_back(new Edge(pre_vertex, 0));
		}
		else {//v2 == between 2 wave vertex
			if (tmp->next->key.get_wavekind() != none_wave)
				find_common_element(tmp->key, tmp->next->key, v2_element_p, v2_element_l);
			else {
				if (tmp->key.get_wavekind() == pl)
					v2_element_p = tmp->key.get_p1();
				else if (tmp->key.get_wavekind() == l)
					v2_element_l = tmp->key.get_l1();
			}
			v2 = new Vertex(v2_result);
			pre_vertex = v2;
			vertex_vector.push_back(pre_vertex);
		}
		tmp = tmp->next;
		while (tmp != wavefrontptr->tail && tmp != last) {
			Vertex * cur_vertex = new Vertex(tmp->key.get_wave_vertex());
			vertex_vector.push_back(cur_vertex);
			edge_vector.push_back(new Edge(pre_vertex, cur_vertex, 1));
			if (tmp->key.get_v() != nullptr)
				edge_vector.push_back(new Edge(tmp->key.get_v(), cur_vertex, 1));
			//else edge_vector.push_back(new Edge(cur_vertex, 0));
			tmp = tmp->next;
			pre_vertex = cur_vertex;
			//if (tmp != wavefront.tail && tmp != last)
			//	pre_vertex = cur_vertex;
			//else break;
		}
		if (tmp == wavefrontptr->tail) { //case: no v1
			edge_vector.push_back(new Edge(pre_vertex, rightend, -1.0, 1));//-1 ray
		}
		else if (tmp->key.get_wave_vertex() == v1_result) {//wave vertex == v1
			find_common_element(tmp->key, tmp->next->key, v1_element_p, v1_element_l);
			v1 = new Vertex(v1_result);;
			Vertex * cur_vertex = v1;
			vertex_vector.push_back(cur_vertex);
			edge_vector.push_back(new Edge(pre_vertex, cur_vertex, 1));
			if (tmp->key.get_v() != nullptr)
				edge_vector.push_back(new Edge(tmp->key.get_v(), cur_vertex, 1));
			//else edge_vector.push_back(new Edge(cur_vertex, 0));
		}
		else {//v1 between two wave vertex
			if (tmp->pre->key.get_wavekind() != none_wave)
				find_common_element(tmp->key, tmp->pre->key, v1_element_p, v1_element_l);
			else {
				if (tmp->key.get_wavekind() == pl)
					v1_element_p = tmp->key.get_p1();
				else if (tmp->key.get_wavekind() == l)
					v1_element_l = tmp->key.get_l1();
			}
			v1 = new Vertex(v1_result);;
			Vertex * cur_vertex = v1;
			vertex_vector.push_back(cur_vertex);
			if (pre_vertex == nullptr)
				edge_vector.push_back(new Edge(cur_vertex, rightend, -1.0, 1));
			else
				edge_vector.push_back(new Edge(pre_vertex, cur_vertex, 1));
		}

		//delete wave from wavefront
		tmp = first;
		if (tmp == nullptr) 
			tmp = wavefrontptr->head;
		else if (first->key.get_wave_vertex() == v2_result) 
			tmp = tmp->pre;
		while (tmp->next != wavefrontptr->tail && tmp->next != last) {
			wavefrontptr->delete_bst(tmp->next->key);
		}
		if (tmp->next != wavefrontptr->tail && last->key.get_wave_vertex() == v1_result) 
			wavefrontptr->delete_bst(tmp->next->key);


		//vertex_vector.push_back(l_ptr);//site in point도 vertex vector에 추가??????????????????????
		//if (l_ptr->get_vertex().first != r_ptr->get_vertex().first)
		//	vertex_vector.push_back(r_ptr);
		//new wave by v1, v2
		if (v1_result != make_pair(-FLT_MAX, FLT_MAX)) {
			//v1 exist
			if (v1_element_p != make_pair(-FLT_MAX, -FLT_MAX)) {
				add_new_wave_v1_in(v1_element_p, s, v1);
				//edge_vector.push_back(new Edge(v1, l_ptr, 0));
			}
			else {
				add_new_wave_v1_in(v1_element_l, s, v1);
				//edge_vector.push_back(new Edge(l_ptr, -1, 0));
			}
		}
		if (v2_result != make_pair(-FLT_MAX, -FLT_MAX)) {
			//v2 exist
			if (v2_element_p != make_pair(-FLT_MAX, -FLT_MAX)) {
				add_new_wave_v2_in(v2_element_p, s, v2);
				//edge_vector.push_back(new Edge(v2, r_ptr, 0));
			}
			else {
				add_new_wave_v2_in(v2_element_l, s, v2);
				//edge_vector.push_back(new Edge(r_ptr, 1, 0));
			}
		}
	}
	void site_out(Event &e) {
		//find v1 || v2
		Site * s = e.get_owner();
		float slope_s = s->get_slope();
		pair<float, float> r_endpoint = s->get_r_endpoint();
		float x1 = r_endpoint.first;
		float y1 = r_endpoint.second;

		if (slope_s > 0) {
			//add vertex & edge & new wave from v1
			site_out_up(r_endpoint);
			//new wave by 45 rays
			float start_x = s->get_l_endpoint().first;
			Site * horizon = new Site(start_x, r_endpoint.second, r_endpoint.first, r_endpoint.second);
			site_vector.push_back(horizon);
			add_new_wave_45_inc_ray(r_endpoint, s);
			add_new_wave_45_dec_ray(r_endpoint, s);
		}
		else if (slope_s < 0) {
			site_out_down(r_endpoint);
			float start_x = s->get_l_endpoint().first;
			Site * horizon = new Site(start_x, r_endpoint.second, r_endpoint.first, r_endpoint.second);
			site_vector.push_back(horizon);
			add_new_wave_45_inc_ray(r_endpoint, s);
			add_new_wave_45_dec_ray(r_endpoint, s);
		}
		else {
			site_out_horizon(r_endpoint);//slope_s == 0
			add_new_wave_45_inc_ray(r_endpoint, s);
			add_new_wave_45_dec_ray(r_endpoint, s);
		}
		//delete site wave
		wavefrontptr->delete_bst(Wave(l, s, s->get_slope(), s->get_c(), 1, 0));
			
	}
	void site_out_up(pair<float,float> r_endpoint) {
		//find v1
		pair<float,float> v1_result = find_v1(r_endpoint); 
		pair<float, float> v1_element_p = make_pair(-FLT_MAX, -FLT_MAX);
		Site * v1_element_l = nullptr;
		Vertex * v1 = new Vertex(v1_result);
		//add v1 to vertex
		if (v1_result != make_pair(-FLT_MAX, FLT_MAX))
			vertex_vector.push_back(v1);
		Node<Wave> * last = wavefrontptr->find_same_or_smallest_big(v1_result.second);
		//last == nullptr || last->key.get_wave_vertex() == v1
		if (last != nullptr) {
			Wave wave = last->key;
			find_common_element(wave, /*last->next->key*/last->pre->key, v1_element_p, v1_element_l);
			if (wave.get_v() != nullptr)
				edge_vector.push_back(new Edge(wave.get_v(), v1, 1));
			wavefrontptr->delete_bst(wave);
			//add new wave from v1 and check spike event
			if (v1_element_p != make_pair(-FLT_MAX, -FLT_MAX)) {
				if (wave.get_wavekind() == pp) {
					wave.get_p1() == v1_element_p ? add_new_wave_v1_out(wave.get_p2(), r_endpoint, v1) : add_new_wave_v1_out(wave.get_p1(), r_endpoint, v1);
				}
				else {//pl
					Site * site = wave.get_l1();
					add_new_wave_v1_out(site, r_endpoint, v1);
				}
			}
			else {//v1_element_l != nullptr
				if (wave.get_wavekind() == pl) {
					add_new_wave_v1_out(wave.get_p1(), r_endpoint, v1);
				}
				else if (wave.get_wavekind() == ll) {
					Site * site = nullptr;
					wave.get_l1() == v1_element_l ? site = wave.get_l2() : site = wave.get_l1();
					add_new_wave_v1_out(site, r_endpoint, v1);
				}
			}
		}
	}
	void site_out_down(pair<float, float> r_endpoint) {
		//find v2
		pair<float, float> v2_result = find_v2(r_endpoint);
		pair<float, float> v2_element_p = make_pair(-FLT_MAX, -FLT_MAX);
		Site * v2_element_l = nullptr;
		Vertex * v2 = new Vertex(v2_result);
		//add v2 to vertex
		if (v2_result != make_pair(-FLT_MAX, -FLT_MAX))
			vertex_vector.push_back(v2);
		Node<Wave> * first = wavefrontptr->find_same_or_biggest_small(v2_result.second);
		//first == nullptr || first->key.get_wave_vertex() == v2
		if (first != nullptr) {
			Wave wave = first->key;
			find_common_element(wave, /*first->pre->key*/first->next->key, v2_element_p, v2_element_l);
			if (wave.get_v() != nullptr)
				edge_vector.push_back(new Edge(wave.get_v(), v2, 1));
			wavefrontptr->delete_bst(wave);
			//add new wave from v2 and check spike event
			if (v2_element_p != make_pair(-FLT_MAX, -FLT_MAX)) {
				if (wave.get_wavekind() == pp) {
					wave.get_p1() == v2_element_p ? add_new_wave_v2_out(wave.get_p2(), r_endpoint, v2) : add_new_wave_v2_out(wave.get_p1(), r_endpoint, v2);
				}
				else {//pl
					Site * site = wave.get_l1();
					add_new_wave_v2_out(site, r_endpoint, v2);
				}
			}
			else {//v2_element_l != nullptr
				if (wave.get_wavekind() == pl) {
					add_new_wave_v2_out(wave.get_p1(), r_endpoint, v2);
				}
				else if (wave.get_wavekind() == ll) {
					Site * site = nullptr;
					wave.get_l1() == v2_element_l ? site = wave.get_l2() : site = wave.get_l1();
					add_new_wave_v2_out(site, r_endpoint, v2);
				}
			}
		}
	}
	void site_out_horizon(pair<float, float> r_endpoint) {
		site_out_up(r_endpoint);
		site_out_down(r_endpoint);
	}
	void site_ver(Event &e) {
		Site * s = e.get_owner();
		pair<float, float> l_endpoint = s->get_l_endpoint();
		pair<float, float> r_endpoint = s->get_r_endpoint();
		if (!wavefrontptr->is_empty()) 
			site_in_find_v1v2(s, l_endpoint, r_endpoint);

		add_new_wave_45_inc_ray(l_endpoint, s);
		add_new_wave_45_dec_ray(r_endpoint, s);
	}
	void spike(Event &e) {//if wavekind == l, then spike event will not occur
		//TODO
		pair<float, float> spike_point = e.get_spike_point();
		pair<Wave, Wave> spike_waves = e.get_spike_waves();
		Wave spike_wave1 = spike_waves.first;
		Wave spike_wave2 = spike_waves.second;
		WaveKind wavekind1 = spike_wave1.get_wavekind();
		WaveKind wavekind2 = spike_wave2.get_wavekind();
		pair<float, float> common_element_p = make_pair(-FLT_MAX, -FLT_MAX);
		Site * common_element_l = nullptr;

		//for new wave
		WaveKind wavekind;
		float y_slope;
		float y_c;
		float x_slope;
		float x_c;
		pair<float, float> point_element1 = make_pair(-FLT_MAX, -FLT_MAX);
		pair<float, float> point_element2 = make_pair(-FLT_MAX, -FLT_MAX);
		PointKind pointkind1 = nothing;
		PointKind pointkind2 = nothing;
		Site * line_element1 = nullptr;
		Site * line_element2 = nullptr;
		Vertex * l_vertex = new Vertex(spike_point);

		//new voronoi vertex and edges
		vertex_vector.push_back(l_vertex);
		if (spike_wave1.get_v() != nullptr)
			edge_vector.push_back(new Edge(spike_wave1.get_v(), l_vertex, 1));
		if (spike_wave2.get_v() != nullptr)
			edge_vector.push_back(new Edge(spike_wave2.get_v(), l_vertex, 1));

		//delete intermediate wave
		/*Node<Wave> * node1 = wavefrontptr->search_spike(spike_wave1);
		Node<Wave> * node2 = wavefrontptr->search_spike(spike_wave2);
		if (node1->next == node2->pre)
			wavefrontptr->delete_bst(node1->next->key);
		else if (node1->pre == node2->next)
			wavefrontptr->delete_bst(node1->pre->key);*/
		wavefrontptr->delete_bst(spike_wave1);
		wavefrontptr->delete_bst(spike_wave2);


		//define two wave element of new wave
		find_common_element(spike_wave1, spike_wave2, common_element_p, common_element_l);

		if (common_element_l != nullptr) {
			if (wavekind1 == pl) {
				point_element1 = spike_wave1.get_p1();
				pointkind1 = spike_wave1.get_pointkind1();
				if (wavekind2 == pl) {
					point_element2 = spike_wave2.get_p1();
					pointkind2 = spike_wave2.get_pointkind1();
					wavekind = pp;
				}
				else {
					spike_wave2.get_l1() == common_element_l ? line_element1 = spike_wave2.get_l2() : line_element1 = spike_wave2.get_l1();//wavekind2 == ll
					wavekind = pl;
				}
			}
			else if (wavekind1 == ll) {
				spike_wave1.get_l1() == common_element_l ? line_element1 = spike_wave1.get_l2() : line_element1 = spike_wave1.get_l1();
				if (wavekind2 == pl) {
					point_element1 = spike_wave2.get_p1();
					pointkind1 = spike_wave2.get_pointkind1();
					wavekind = pl;
				}
				else {
					spike_wave2.get_l1() == common_element_l ? line_element2 = spike_wave2.get_l2() : line_element2 = spike_wave2.get_l1();//wavekind2 == ll
					wavekind = ll;
				}
			}
		}
		else {//common_element_p != make_pair(-FLT_MAX, -FLT_MAX)
			if (wavekind1 == pp) {
				if (spike_wave1.get_p1() == common_element_p) {
					point_element1 = spike_wave1.get_p2();
					pointkind1 = spike_wave1.get_pointkind2();
				}
				else {
					point_element1 = spike_wave1.get_p1();
					pointkind1 = spike_wave1.get_pointkind1();
				}
				if (wavekind2 == pp) { 
					if (spike_wave2.get_p1() == common_element_p) {
						point_element2 = spike_wave2.get_p2();
						pointkind2 = spike_wave2.get_pointkind2();
					}
					else {
						point_element2 = spike_wave2.get_p1();
						pointkind2 = spike_wave2.get_pointkind1();
					}
					wavekind = pp;
				}
				else {//wavekind2 == pl
					if (spike_wave2.get_p1() == spike_wave2.get_l1()->get_r_endpoint() && !spike_wave2.get_l1()->is_vertical()) {
						point_element2 = spike_wave2.get_p1();
						pointkind2 = p_right;
						wavekind = pp;
					}
					else {
						line_element1 = spike_wave2.get_l1();
						wavekind = pl;
					}
				}
			}
			else {//wavekind1 == pl
				if (spike_wave1.get_p1() == spike_wave1.get_l1()->get_r_endpoint() && !spike_wave1.get_l1()->is_vertical()) {
					point_element1 = spike_wave1.get_p1();
					pointkind1 = p_right;
					if (wavekind2 == pp) {
						if (spike_wave2.get_p1() == common_element_p) {
							point_element2 = spike_wave2.get_p2();
							pointkind2 = spike_wave2.get_pointkind2();
						}
						else {
							point_element2 = spike_wave2.get_p1();
							pointkind2 = spike_wave2.get_pointkind1();
						}
						wavekind = pp;
					}
					else {//wavekind2 == pl
						if (spike_wave2.get_p1() == spike_wave2.get_l1()->get_r_endpoint() && !spike_wave2.get_l1()->is_vertical()) {
							point_element2 = spike_wave2.get_p1();
							pointkind2 = p_right;
							wavekind = pp;
						}
						else {
							line_element2 = spike_wave2.get_l1();
							wavekind == pl;
						}
					} 
				}
				else {
					line_element1 = spike_wave1.get_l1();
					if (wavekind2 == pp) {
						if (spike_wave2.get_p1() == common_element_p) {
							point_element1 = spike_wave2.get_p2();
							pointkind1 = spike_wave2.get_pointkind2();
						}
						else {
							point_element1 = spike_wave2.get_p1();
							pointkind1 = spike_wave2.get_pointkind1();
						}
						wavekind = pl;
					}
					else {//wavekind2 == pl
						if (spike_wave2.get_p1() == spike_wave2.get_l1()->get_r_endpoint() && !spike_wave2.get_l1()->is_vertical()) {
							point_element1 = spike_wave2.get_p1();
							pointkind1 = p_right;
							wavekind = pl;
						}
						else {
							line_element2 = spike_wave2.get_l1();
							wavekind = ll;
						}
					}
				}
			}
		}
	
		float a1, b1, c1, a2, b2, c2;
		if (wavekind == pp) {
			if (pointkind1 == p_up) {
				a1 = -1.0;
				b1 = 1.0;
				c1 = point_element1.second;
			}
			else if (pointkind1 == p_down) {
				a1 = 1.0;
				b1 = -1.0;
				c1 = point_element1.second;
			}
			else if (pointkind1 == p_right) {
				a1 = FLT_MAX;
				b1 = 1.0 / 2.0;
				c1 = point_element1.first / 2.0;
			}
			if (pointkind2 == p_up) {
				a2 = -1.0;
				b2 = 1.0;
				c2 = point_element2.second;
			}
			else if (pointkind2 == p_down) {
				a2 = 1.0;
				b2 = -1.0;
				c2 = point_element2.second;
			}
			else if (pointkind2 == p_right) {
				a2 = FLT_MAX;
				b2 = 1.0 / 2.0;
				c2 = point_element2.first / 2.0;
			}
		}
		else if (wavekind == pl) {
			if (pointkind1 == p_up) {
				a1 = -1.0;
				b1 = 1.0;
				c1 = point_element1.second;
			}
			else if (pointkind1 == p_down) {
				a1 = 1.0;
				b1 = -1.0;
				c1 = point_element1.second;
			}
			else if (pointkind1 == p_right) {
				a1 = FLT_MAX;
				b1 = 1.0 / 2.0;
				c1 = point_element1.first / 2.0;
			}
			float event_y = spike_point.second;
			float site_y = line_element1->get_y(spike_point.first);
			float site_slope = line_element1->get_slope();
			if (site_slope == FLT_MAX) {
				a2 = FLT_MAX;
				b2 = 1.0 / 2.0;
				c2 = line_element1->get_c() / 2.0;
			}
			else if (site_slope > 0) {
				if (event_y > site_y)
					a2 = -1.0;
				else 
					a2 = 2.0 * site_slope + 1.0;
				b2 = site_slope - a2;
				c2 = line_element1->get_c();
			}
			else if (site_slope < 0) {
				if (event_y > site_y)
					a2 = 2.0 * site_slope - 1.0;
				else 
					a2 = 1.0;
				b2 = site_slope - a2;
				c2 = line_element1->get_c();
			}
			else {//site_slope == 0
				if (event_y > site_y) 
					a2 = -1.0;
				else 
					a2 = 1.0;
				b2 = site_slope - a2;
				c2 = line_element1->get_c();
			}
		}
		else {//ll
			float event_y = spike_point.second;
			float site1_y = line_element1->get_y(spike_point.first);
			float site1_slope = line_element1->get_slope();
			if (site1_slope == FLT_MAX) {
				a1 = FLT_MAX;
				b1 = 1.0 / 2.0;
				c1 = line_element1->get_c() / 2.0;
			}
			else if (site1_slope > 0) {
				if (event_y > site1_y)
					a1 = -1.0;
				else
					a1 = 2.0 * site1_slope + 1.0;
				b1 = site1_slope - a1;
				c1 = line_element1->get_c();
			}
			else if (site1_slope < 0) {
				if (event_y > site1_y)
					a1 = 2.0 * site1_slope - 1.0;
				else
					a1 = 1.0;
				b1 = site1_slope - a1;
				c2 = line_element1->get_c();
			}
			else {//site_slope == 0
				if (event_y > site1_y)
					a1 = -1.0;
				else
					a1 = 1.0;
				b1 = site1_slope - a1;
				c1 = line_element1->get_c();
			}

			float site2_y = line_element2->get_y(spike_point.first);
			float site2_slope = line_element2->get_slope();
			if (site2_slope == FLT_MAX) {
				a2 = FLT_MAX;
				b2 = 1.0 / 2.0;
				c2 = line_element2->get_c() / 2.0;
			}
			else if (site2_slope > 0) {
				if (event_y > site2_y)
					a2 = -1.0;
				else
					a2 = 2.0 * site2_slope + 1.0;
				b2 = site2_slope - a2;
				c2 = line_element2->get_c();
			}
			else if (site2_slope < 0) {
				if (event_y > site2_y)
					a2 = 2.0 * site2_slope - 1.0;
				else
					a2 = 1.0;
				b2 = site2_slope - a2;
				c2 = line_element2->get_c();
			}
			else {//site_slope == 0
				if (event_y > site2_y)
					a2 = -1.0;
				else
					a2 = 1.0;
				b2 = site2_slope - a2;
				c2 = line_element2->get_c();
			}
		}
	
		pair<pair<float, float>, pair<float, float>> new_xy = find_intersection_of_2lines(a1, b1, c1, a2, b2, c2);
		y_slope = new_xy.first.first;
		y_c = new_xy.first.second;
		x_slope = new_xy.second.first;
		x_c = new_xy.second.second;

		//Wave new_wave = Wave(wavekind, point_element1, pointkind1, point_element2, pointkind2, line_element1, line_element2, y_slope, y_c, x_slope, x_c);
		Wave new_wave;
		if (wavekind == pp)
			new_wave = Wave(wavekind, point_element1, pointkind1, point_element2, pointkind2, y_slope, y_c, x_slope, x_c);
		else if (wavekind == pl)
			new_wave = Wave(wavekind, point_element1, pointkind1, line_element1, y_slope, y_c, x_slope, x_c);
		else if (wavekind == ll)
			new_wave = Wave(wavekind, line_element1, line_element2, y_slope, y_c, x_slope, x_c);
		else //wavekind == l
			new_wave = Wave(wavekind, line_element1, y_slope, y_c, x_slope, x_c);
		new_wave.set_l_vertex(l_vertex);
		wavefrontptr->predict_spike_event(new_wave, EventQ);
	}
	bool check_invalid_spike(Event &e) {
		//spike event 만드는 두 wave 중 하나가 wavefront bst에 없으면 invalid ->return true
		pair<Wave, Wave> spike_waves = e.get_spike_waves();
		Wave spike_wave1 = spike_waves.first;
		Wave spike_wave2 = spike_waves.second;
		if (wavefrontptr->search_spike(spike_wave1) == nullptr)
			return true;
		if (wavefrontptr->search_spike(spike_wave2) == nullptr)
			return true;
		return false;
	}
	void add_new_wave_45_inc_ray(pair<float, float> p, Site * l) {
		Wave w1(pl, p, p_right, l, 1.0 / 2.0, p.second - p.first / 2.0, 1.0 / 2.0, p.first / 2.0);
		wavefrontptr->predict_spike_event(w1, EventQ);
	}	
	void add_new_wave_45_dec_ray(pair<float, float> p, Site * l) {
		Wave w1(pl, p, p_right, l, -1.0 / 2.0, p.second + p.first / 2.0, 1.0 / 2.0, p.first / 2.0);
		wavefrontptr->predict_spike_event(w1, EventQ);
	}
	//find common element of two wave
	void find_common_element(Wave & key1, Wave & key2, pair<float, float> &element_p, Site *& element_l) {
		WaveKind wavekind1 = key1.get_wavekind();
		WaveKind wavekind2 = key2.get_wavekind();

		if (wavekind1 == pp) {
			if (wavekind2 == pp) {
				pair<float, float> p11 = key1.get_p1();
				pair<float, float> p12 = key1.get_p2();
				pair<float, float> p21 = key2.get_p1();
				pair<float, float> p22 = key2.get_p2();
				p11 == p21 || p11 == p22 ? element_p = p11 : element_p = p12;
			}
			else if (wavekind2 == pl) element_p = key2.get_p1();
		}
		else if (wavekind1 == pl) {
			if (wavekind2 == pp) element_p = key1.get_p1();
			else if (wavekind2 == pl) {
				pair<float, float> p1 = key1.get_p1();
				Site * l1 = key1.get_l1();
				pair<float, float> p2 = key2.get_p1();
				if (p1 == p2) element_p = p1;
				else element_l = l1;
			}
			else if (wavekind2 == ll) element_l = key1.get_l1();
			else if (wavekind2 == l) element_l = key1.get_l1();
		}
		else if (wavekind1 == ll) {
			if (wavekind2 == pl) element_l = key2.get_l1();
			else if (wavekind2 == ll) {
				Site * l11 = key1.get_l1();
				Site * l12 = key1.get_l2();
				Site * l21 = key2.get_l1();
				Site * l22 = key2.get_l2();
				l11 == l21 || l11 == l22 ? element_l = l11 : element_l = l12;
			}
			else if (wavekind2 == l) element_l = key2.get_l1();
		}
		else element_l = key1.get_l1();//wavekind1 == l
	}
	//site: y = ax + b || x = b, element_l: y = mx + n or x = x1
	/*void add_new_wave_v1_in(Site *& element_l, Site *& site) {
		float a = site->get_slope();
		float b = site->get_c();
		float m = element_l->get_slope();
		float n = element_l->get_c();
		pair<pair<float, float>, pair<float, float>> new_xy;
		if (a != FLT_MAX && a >= 0) {
			Wave wave;
			if (m < 0) new_xy = find_intersection_of_2lines(-1 + 2 * m, 1 - m, n, -1, a + 1, b);
			else if (m > 0 && m != FLT_MAX) new_xy = find_intersection_of_2lines(1 + 2 * m, -1 - m, n, -1, a + 1, b);
			else new_xy = find_intersection_of_2lines(1 / 2, n / 2, -1, a + 1, b);//m == FLT_MAX
			if (a == 0)
				wave = Wave(pl, site->get_l_endpoint(), p_up, element_l, new_xy.first.first, new_xy.first.second, new_xy.second.first, new_xy.second.second);
			else
				wave = Wave(ll, element_l, site, new_xy.first.first, new_xy.first.second, new_xy.second.first, new_xy.second.second);
			wavefrontptr->predict_spike_event(wave, EventQ);
		}
		else {
			if (m < 0) new_xy = find_intersection_of_2lines(-1 + 2 * m, 1 - m, n, -1, 1, a * sweep_t + b);
			else if (m > 0 && m != FLT_MAX) new_xy = find_intersection_of_2lines(1 + 2 * m, -1 - m, n, -1, 1, a * sweep_t + b);
			else new_xy = find_intersection_of_2lines(1 / 2, n / 2, -1, 1, a * sweep_t + b);//m == FLT_MAX
			Wave wave(pl, site->get_l_endpoint(), p_up, element_l, new_xy.first.first, new_xy.first.second, new_xy.second.first, new_xy.second.second);
			wavefrontptr->predict_spike_event(wave, EventQ);
		}
	}*/
	//site: y = ax + b || x = b, element_l: y = mx + n or x = x1
	void add_new_wave_v1_in(Site *& element_l, Site *& site, Vertex * v1) {
		float a = site->get_slope();
		float b = site->get_c();
		float m = element_l->get_slope();
		float n = element_l->get_c();
		float a1, b1, c1, a2, b2, c2;
		Wave wave;
		WaveKind wk = none_wave;
		PointKind pk = nothing;
		if (a >= 0 && a != FLT_MAX) {
			a1 = -1; b1 = a + 1; c1 = b;
			wk = ll;
		}
		else { //a < 0 || a == FLT_MAX
			if (a == FLT_MAX) {
				a1 = -1; b1 = 1; c1 = site->get_r_endpoint().second;
			}
			else {
				a1 = -1; b1 = 1; c1 = a * sweep_t + b;
			}
			wk = pl;
			pk = p_up;
		}
		if (m == FLT_MAX) {
			a2 = FLT_MAX; b2 = 1.0 / 2.0; c2 = n / 2.0;
		}
		else if (m > 0) {
			a2 = 2.0 * m + 1.0; b2 = -1.0 - m; c2 = n;
		}
		else { //m < 0
			if (element_l->get_r_endpoint().first >= sweep_t && site->get_y() < element_l->get_y()) {
				a2 = 1.0; b2 = m - 1.0; c2 = n;
			}
			else {
				a2 = 2.0 * m - 1.0; b2 = 1.0 - m; c2 = n;
			}
		}
		pair<pair<float, float>, pair<float, float>> new_xy = find_intersection_of_2lines(a1, b1, c1, a2, b2, c2);
		if (wk == ll)
			wave = Wave(ll, element_l, site, new_xy.first.first, new_xy.first.second, new_xy.second.first, new_xy.second.second);
		else
			wave = Wave(pl, site->get_l_endpoint(), pk, element_l, new_xy.first.first, new_xy.first.second, new_xy.second.first, new_xy.second.second);
		wave.set_l_vertex(v1);
		wavefrontptr->predict_spike_event(wave, EventQ);
	}
	//site: y = ax + b || x = b, element p: (x1, y1)
	void add_new_wave_v1_in(pair<float, float> element_p, Site *& site, Vertex * v1) {
		float a = site->get_slope();
		float b = site->get_c();
		float x1 = element_p.first;
		float y1 = element_p.second;
		bool under_standard = (a * sweep_t + b) < (y1 - sweep_t + x1); 
		pair<pair<float, float>, pair<float, float>> new_xy;
		if (a != FLT_MAX && a > 0) {
			Wave wave;
			PointKind pk;
			if (under_standard) {
				new_xy = find_intersection_of_2lines(1, -1, y1, -1, a + 1, b);
				pk = p_down;
			}
			else {
				new_xy = find_intersection_of_2lines(1 / 2.0, x1 / 2.0, -1, a + 1, b);
				pk = p_right;
			}
			if (a == 0)
				wave = Wave(pp, element_p, pk, site->get_l_endpoint(), p_up, new_xy.first.first, new_xy.first.second, new_xy.second.first, new_xy.second.second);
			else
				wave = Wave(pl, element_p, pk, site, new_xy.first.first, new_xy.first.second, new_xy.second.first, new_xy.second.second);
			wave.set_l_vertex(v1);
			wavefrontptr->predict_spike_event(wave, EventQ);
		}
		else {
			PointKind pk;
			if (under_standard) {
				if (a == FLT_MAX)
					new_xy = find_intersection_of_2lines(1, -1, y1, -1, 1, site->get_r_endpoint().second);
				else
					new_xy = find_intersection_of_2lines(1, -1, y1, -1, 1, a*sweep_t + b);
				pk = p_down;
			}
			else {
				if (a== FLT_MAX)
					new_xy = find_intersection_of_2lines(1 / 2.0, x1 / 2.0, -1, 1, site->get_r_endpoint().second);
				else 
					new_xy = find_intersection_of_2lines(1 / 2.0, x1 / 2.0, -1, 1, a*sweep_t + b);
				pk = p_right;
			}
			Wave wave(pp, element_p, pk, site->get_l_endpoint(), p_up, new_xy.first.first, new_xy.first.second, new_xy.second.first, new_xy.second.second);
			wave.set_l_vertex(v1);
			wavefrontptr->predict_spike_event(wave, EventQ);
		}
	}
	//site: y = ax + b || x = b, element_l: y = mx + n or x = x1
	void add_new_wave_v2_in(Site *& element_l, Site *& site, Vertex * v2) {
		float a = site->get_slope();
		float b = site->get_c();
		float m = element_l->get_slope();
		float n = element_l->get_c();
		float a1, b1, c1, a2, b2, c2;
		Wave wave;
		WaveKind wk = none_wave;
		PointKind pk = nothing;
		if (a > 0) {
			if (a == FLT_MAX) {//site_ver인 경우
				a1 = 1; b1 = -1; c1 = site->get_r_endpoint().second;
			}
			else {
				a1 = 1; b1 = -1; c1 = a * sweep_t + b;
			}
			wk = pl;
			pk = p_down;
		}
		else { //a <= 0
			a1 = 1; b1 = a - 1; c1 = b;
			wk = ll;
		}
		if (m == FLT_MAX) {
			a2 = FLT_MAX; b2 = 1 / 2.0; c2 = n / 2.0;
		}
		else if (m > 0) {
			if (element_l->get_r_endpoint().first >= sweep_t && site->get_y() > element_l->get_y()) {
				a2 = -1; b2 = m + 1; c2 = n;
			}
			else {
				a2 = 2 * m + 1; b2 = -1 - m; c2 = n;
			}
		}
		else { //m < 0
			a2 = 2 * m - 1; b2 = 1 - m; c2 = n;
		}
		pair<pair<float, float>, pair<float, float>> new_xy = find_intersection_of_2lines(a1, b1, c1, a2, b2, c2);
		if (wk == ll)
			wave = Wave(ll, element_l, site, new_xy.first.first, new_xy.first.second, new_xy.second.first, new_xy.second.second);
		else if (site->get_slope() == FLT_MAX)
			wave = Wave(pl, site->get_r_endpoint(), pk, element_l, new_xy.first.first, new_xy.first.second, new_xy.second.first, new_xy.second.second);
		else
			wave = Wave(pl, site->get_l_endpoint(), pk, element_l, new_xy.first.first, new_xy.first.second, new_xy.second.first, new_xy.second.second);
		wave.set_l_vertex(v2);
		wavefrontptr->predict_spike_event(wave, EventQ);
	}
	void add_new_wave_v2_in(pair<float, float> element_p, Site *& site, Vertex * v2) {
		float a = site->get_slope();
		float b = site->get_c();
		float x1 = element_p.first;
		float y1 = element_p.second;
		bool over_standard = (a * sweep_t + b) > (y1 - sweep_t + x1);
		pair<pair<float, float>, pair<float, float>> new_xy;
		if (a >= 0) { 
			PointKind pk;
			if (over_standard) {
				if (a == FLT_MAX)
					new_xy = find_intersection_of_2lines(-1, 1, y1, 1, -1, site->get_r_endpoint().second);
				else
					new_xy = find_intersection_of_2lines(-1, 1, y1, 1, -1, a * sweep_t + b);
				pk = p_up;
			}
			else {
				if (a == FLT_MAX)
					new_xy = find_intersection_of_2lines(1 / 2.0, x1 / 2.0, 1, -1, site->get_r_endpoint().second);
				else
					new_xy = find_intersection_of_2lines(1 / 2.0, x1 / 2.0, 1, -1, a * sweep_t + b);
				pk = p_right;
			}
			Wave wave(pp, element_p, pk, site->get_l_endpoint(), p_down, new_xy.first.first, new_xy.first.second, new_xy.second.first, new_xy.second.second);
			wave.set_l_vertex(v2);
			wavefrontptr->predict_spike_event(wave, EventQ);
		}
		else if (a < 0) {
			PointKind pk;
			Wave wave;
			if (over_standard) {
				new_xy = find_intersection_of_2lines(-1, 1, y1, 1, a - 1, b);
				pk = p_up;
			}
			else {
				new_xy = find_intersection_of_2lines(1 / 2.0, x1 / 2.0, 1, a - 1, b);
				pk = p_right;
			}
			if (a == 0)
				wave = Wave(pp, element_p, pk, site->get_l_endpoint(), p_down, new_xy.first.first, new_xy.first.second, new_xy.second.first, new_xy.second.second);
			else
				wave = Wave(pl, element_p, pk, site, new_xy.first.first, new_xy.first.second, new_xy.second.first, new_xy.second.second);
			wave.set_l_vertex(v2);
			wavefrontptr->predict_spike_event(wave, EventQ);
		}
	}
	//site: (xp, yp), element p: (x1,y1)
	void add_new_wave_v1_out(pair<float, float> element_p, pair<float, float> site_endpoint, Vertex * v1) {
		float x1 = element_p.first;
		float y1 = element_p.second;
		float xp = site_endpoint.first;
		float yp = site_endpoint.second;
		bool under_standard = yp < (-sweep_t + x1 + y1);
		pair<pair<float, float>, pair<float, float>> new_xy;
		PointKind pk;
		if (under_standard) {
			new_xy = find_intersection_of_2lines(1, -1, y1, -1, 1, yp);
			pk = p_down;
		}
		else {
			new_xy = find_intersection_of_2lines(1.0 / 2.0, x1 / 2.0, -1, 1, yp);
			pk = p_right;
		}
		Wave wave(pp, element_p, pk, site_endpoint, p_up, new_xy.first.first, new_xy.first.second, new_xy.second.first, new_xy.second.second);
		wave.set_l_vertex(v1);
		wavefrontptr->predict_spike_event(wave, EventQ);
	}
	//site: (xp, yp), element l: y = mx + n || x = n
	void add_new_wave_v1_out(Site *& element_l, pair<float, float> site_endpoint, Vertex * v1) {
		float xp = site_endpoint.first;
		float yp = site_endpoint.second;
		float m = element_l->get_slope();
		float n = element_l->get_c();
		pair<pair<float, float>, pair<float, float>> new_xy;
		if (m == FLT_MAX) new_xy = find_intersection_of_2lines(1.0 / 2.0, n / 2.0, -1, 1, yp);
		else if (m > 0) new_xy = find_intersection_of_2lines(1 + 2.0 * m, -1 - m, n, -1, 1, yp);
		else new_xy = find_intersection_of_2lines(1, m - 1, n, -1, 1, yp);
		Wave wave(pl, site_endpoint, p_up, element_l, new_xy.first.first, new_xy.first.second, new_xy.second.first, new_xy.second.second);
		wave.set_l_vertex(v1);
		wavefrontptr->predict_spike_event(wave, EventQ);
	}
	//site: (xp, yp), element p: (x1,y1)
	void add_new_wave_v2_out(pair<float, float> element_p, pair<float, float> site_endpoint, Vertex * v2) {
		float x1 = element_p.first;
		float y1 = element_p.second;
		float xp = site_endpoint.first;
		float yp = site_endpoint.second;
		bool over_standard = yp > (sweep_t - x1 + y1);
		pair<pair<float, float>, pair<float, float>> new_xy;
		PointKind pk;
		if (over_standard) {
			new_xy = find_intersection_of_2lines(-1, 1, y1, 1, -1, yp);
			pk = p_up;
		}
		else {
			new_xy = find_intersection_of_2lines(1.0 / 2.0, x1 / 2.0, 1, -1, yp);
			pk = p_right;
		}
		Wave wave(pp, element_p, pk, site_endpoint, p_down, new_xy.first.first, new_xy.first.second, new_xy.second.first, new_xy.second.second);
		wave.set_l_vertex(v2);
		wavefrontptr->predict_spike_event(wave, EventQ);
	}
	//site: (xp, yp), element l: y = mx + n || x = n
	void add_new_wave_v2_out(Site *& element_l, pair<float, float> site_endpoint, Vertex * v2) {
		float xp = site_endpoint.first;
		float yp = site_endpoint.second;
		float m = element_l->get_slope();
		float n = element_l->get_c();
		pair<pair<float, float>, pair<float, float>> new_xy;
		if (m == FLT_MAX) new_xy = find_intersection_of_2lines(1.0 / 2.0, n / 2.0, 1, -1, yp);
		else if (m < 0) new_xy = find_intersection_of_2lines(2 * m - 1, 1 - m, n, 1, -1, yp);
		else new_xy = find_intersection_of_2lines(-1, m + 1, n, 1, -1, yp);
		Wave wave(pl, site_endpoint, p_down, element_l, new_xy.first.first, new_xy.first.second, new_xy.second.first, new_xy.second.second);
		wave.set_l_vertex(v2);
		wavefrontptr->predict_spike_event(wave, EventQ);
	}

	//y = a1 * x + b1 * t + c1, y = a2 * x + b2 * t + c2 => x = x_slope * t + x_c, y = y_slope * t + y_c
	//a1 != a2
	pair<pair<float, float>, pair<float, float>> find_intersection_of_2lines(float a1, float b1, float c1, float a2, float b2, float c2) {
		if (a1 == a2) return make_pair(make_pair(-FLT_MAX, -FLT_MAX), make_pair(-FLT_MAX, -FLT_MAX));
		else if (a1 == FLT_MAX) return find_intersection_of_2lines(b1, c1, a2, b2, c2);
		else if (a2 == FLT_MAX) return find_intersection_of_2lines(b2, c2, a1, b1, c1);
		float x_slope, x_c, y_slope, y_c;
		x_slope = (b2 - b1) / (a1 - a2);
		x_c = (c2 - c1) / (a1 - a2);
		y_slope = (a1 * b2 - a2 * b1) / (a1 - a2);
		y_c = (a1 * c2 - a2 * c1) / (a1 - a2);
		return make_pair(make_pair(y_slope, y_c), make_pair(x_slope, x_c));
	}

	//x = b1 * t + c1, y = a2 * x + b2 * t + c2 => x = x_slope * t + x_c, y = y_slope * t + y_c
	pair<pair<float, float>, pair<float, float>> find_intersection_of_2lines(float b1, float c1, float a2, float b2, float c2) {
		float x_slope, x_c, y_slope, y_c;
		x_slope = b1;
		x_c = c1;
		y_slope = a2 * b1 + b2;
		y_c = a2 * c1 + c2;
		return make_pair(make_pair(y_slope, y_c), make_pair(x_slope, x_c));
	}

};
