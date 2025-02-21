/*
Input: n = the number of vertices, m = the number of queries
1. Make a random planar graph
2. Convert the graph into DCEL
3. for each query
    pick a face randomly and pick a random point in the face.
    record the pair of query point and key of the face where the query point lies.
*/

void make_testcase_pts_loc();