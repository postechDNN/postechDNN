# $d$-dimensional Approximated Nearest Neighbor Search

This project is an implementation of the $d$-dimensinal approximated nearest neighbor search algorithm with box obstacles. 
In this project, axis-aligned $d$-dimensional boxes are obstacles and the algorithm find the nearest site among the point sites from the query point.
Description of the algorithm can be found [here](https://github.com/postechDNN/postechDNN/tree/BoxNN/dnn/NearestNeighbor/BoxNN_L2/Project1/description.pdf).

## How to use?

The algorithm is maintained with the class **BoxGraphL2**. **BoxGraphL2** supports these kinds of queries below.
- **BoxGraphL2(int, vector<Point> sites&, vector<Box> boxes&, double epsilon)**: Construct the query structure using the vector of $d$-dimensional points **sites**, the vector of $d$-dimensional boxes **boxes**, and the approximation ratio **epsilon**.
- **void add_Box(Box b)**: Add a box **b** and modify **BoxGraphL2**.
- **void del_Box(int i)**: Delete the box with index **i** and modify **BoxGraphL2**.
- **void add_vert(Point p)**: Add a site point **p** and modify **BoxGraphL2**.
- **void del_vert(int i)**: Delete the site with index **i** and modify **BoxGraphL2**.
- **tuple<Point, int, double>  query(Point p)**: Find the nearest site **q** from **p** and return **q**, the index of **q** and the distance from p to **q**.

The classes **Point** and **Box** can be constructed as follows.
- **Point(std::vector<double> xs)**: Construct the point with coordinate **xs**.
- **Box(int n,  vector<double> min, vector<double> max)**: Construct the **n**-dimentional box with the range from **min** to **max** for each axis. 

## Before use it 
### Setup the project
none
### Dependent project
none
