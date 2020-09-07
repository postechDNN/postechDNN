# $L_1$ Nearest Neighbor Search in Rectangular Domain (L1NNS)
## What is '$L_1$ Nearest Neighbor Search in Rectangular Domain'?
In the 2D-plane, there are rectangles behaving obstacles and data represented by points. A rectangular domain is a region induced by rectangles with sufficiently large bounding box as an outer boundary. For given a query, $L_1$ Nearest Neighbor Search is to find the nearest data from query under $L_1$ distance (equivalently, rectilinear distance).

## Sample
Sample execute file L1NNS.exe is available. It supports GUI using openGL.
Left click "Query" button (in present red colored button)
	- Set up query point (left click)
	- If there is no data in rectangular domain, it does not work
	- Highlight the query and its nearest data
	- Also support 2-NNS, 3-NNS, and Farthest-NS (right click and choose)
Initial state or left click "Insertion" button (in present green colored button)
	- Insertion of point (left click) and rectangle (left drag and drop)
	- Overlap is impossible
Left click "Deletion" button (in present blue colored button)
	- Deletion of point and rectangle (left click)
	- A rectangle is deleted by clicking an interior of the rectangle 
	- The nearest data from pointed location is deleted 
Left click "Exit" button (in present black colored button)
	- Exit the program

## Functions
RectangularDomain::RectangularDomain(vector<Rect> rectlist)
	- with rectlist (that vector also contain a set of points), a rectangular domain that all obstacles are in rectlist is constructed. This construction includes preprocessing to compute the $L_1$ distance.
Point_Distance RectangularDomain::NNS(Point query)
	- If a rectangular domain D exists, this function can be called as D.NNS(query); It returns a pair of point which is the nearest neighbor and distance to the nearest neighbor from query.
Point_Distance RectangularDomain::kNNS(Point query, int k)
	- If a rectangular domain D exists, this function can be called as D.kNNS(query, k); It returns vector of k points which are k-nearest neighbor and distances to those k-nearest neighbors from query.
Point_Distance RectangularDomain::FNS(Point query)
	- If a rectangular domain D exists, this function can be called as D.FNS(query); It returns a pair of point which is the farthest neighbor and distance to the farthest neighbor from query.
