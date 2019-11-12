#include "MaxDom.h"
#include <time.h>
#include <sys/timeb.h>

int main() {
	int p, q, k;
	Q* myQ;
	vector<Q*> Qs;
	P* myP;
	vector<P*> Ps;
	vector<int> Ts;
	int r;
	pair<int, vector<pair<int, int>>> ans;
	
	srand(time(NULL));

	struct _timeb timebuffer; 
	char *timeline;

	/*
	for random case; can change k, q, p
	
	*/

	k = 20;

	q = 100;

	myQ = new Q;

	Qs.push_back(myQ);
	for (int i = 1; i < q; i++) {
		myQ = new Q;
		r = (rand() << 15) | rand();
		myQ->x = r%100000;
		r = (rand() << 15) | rand();
		myQ->y = r%100000;
		Qs.push_back(myQ);
	}

	p = 500;

	for (int i = 0; i < p; i++) {
		myP = new P;
		r = (rand() << 15) | rand();
		myP->x = r%100000;
		r = (rand() << 15) | rand();
		myP->y = r%100000;
		myP->w = rand()%100 - 40.0;
		Ps.push_back(myP);
	}

	_ftime(&timebuffer);
	timeline = ctime(&(timebuffer.time));
	printf("The time is %.19s.%hu %s\n", timeline, timebuffer.millitm, &timeline[20]);

	ans = MaxDom(k, Ps, Qs, 0);

	cout << ans.first << endl;
	int ans_x;
	int ans_y;
	
	ans_x = ans.second[0].first;
	ans_y = ans.second[0].second;
	cout << ans.second[0].first << " " << ans.second[0].second << endl;

	for (int i = 1; i < ans.second.size(); i++) {
		if (ans.second[i].first != ans_x && ans.second[i].second != ans_y) {
			cout << ans.second[i].first << " " << ans.second[i].second << endl;
		}
	}

	cout << endl;

	_ftime(&timebuffer);
	timeline = ctime(&(timebuffer.time));
	printf("The time is %.19s.%hu %s\n", timeline, timebuffer.millitm, &timeline[20]);

	/*
		for structed case, insert k, q, Qs, p, Ps in order
	*/

	cin >> k;

	cin >> q;

	myQ = new Q;

	cin >> myQ->x >> myQ->y;
	Qs.push_back(myQ);

	for (int i = 1; i < q; i++) {
		myQ = new Q;
		cin >> myQ->x >> myQ->y;
		Qs.push_back(myQ);
	}

	cin >> p;

	for (int i = 0; i < p; i++) {
		myP = new P;
		cin >> myP->x >> myP->y >> myP->w;
		Ps.push_back(myP);
	}

	_ftime(&timebuffer);
	timeline = ctime(&(timebuffer.time));
	printf("\nThe time is %.19s.%hu %s", timeline, timebuffer.millitm, &timeline[20]);

	ans = MaxDom(k, Ps, Qs, 0);
	ans_x = ans.second[0].first;
	ans_y = ans.second[0].second;

	cout << endl << ans.first << endl;

	cout << ans.second[0].first << " " << ans.second[0].second << endl;
	
	for (int i = 1; i < ans.second.size(); i++) {
		if (ans.second[i].first != ans_x && ans.second[i].second != ans_y) {
			ans_x = ans.second[i].first;
			ans_y = ans.second[i].second;
			cout << ans_x << " " << ans_y << endl;
		}
	}
	
	cout << endl;
	
	_ftime(&timebuffer);
	timeline = ctime(&(timebuffer.time));
	printf("The time is %.19s.%hu %s", timeline, timebuffer.millitm, &timeline[20]);

	getchar();
	getchar();
	
}
