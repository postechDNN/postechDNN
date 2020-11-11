
# L1 voronoi diagram

## 1. Introduction
The Ipe extensible drawing editor is a tool used by computational geometry researchers to produce 2D figures for inclusion in articles or presentations.
In the latest version ipe-7.2.20, there are already functions to draw a voronoi diagram(e.g order 1~3 voronoi diagram, furthest voronoi diagram and so on).
However, there is no function to draw voronoi diagram with respect to L1(Manhattan) distance.
It is a reason why we start the project which draw a L1 voronoi diagram using C++ ipelet framwork of Ipe.
This function can draw a L1 voronoi diagram of a point set given by user.

## 2. Algorithm and Analysis
The algorithm to compute L1 voronoi diagram is based on the following basic property.
![image](https://user-images.githubusercontent.com/17876333/93000489-c726d600-f563-11ea-92ea-65644f1ba93f.png)
<br>
Using this property, we can compute each voronoi region for all sites. When there is a large bounding box containing all points, we can represent each <img src = "https://latex.codecogs.com/svg.latex?B(p,q)"> as bounded simple polygon.
Thus we can compute each voronoi region using simple polygon intersection algorithm.
Specifically, there is a popular clipping algorithm, so called ` Weiler Atherton Polygon Clipping Algorithm` <sup>[[1]](#footnote_1)</sup>
In manhattan environment, there are lots of degenerate case to compute voronoi region. For example two edges can be overlapped when we intersect them. Thus we write the algorithm which can deal with these degenerate situations referring to the paper. <sup>[[2]](#footnote_1)</sup>
Note that the time to compute the intersection between two simple polygon is <img src="https://latex.codecogs.com/svg.latex?O(nm)"> in `Weiler Atherton method`. Since the complexity of each <img src = "https://latex.codecogs.com/svg.latex?B(p,q)"> is <img src = "https://latex.codecogs.com/svg.latex?O(1)"> , the time to compute ![image](https://user-images.githubusercontent.com/17876333/93000517-f4738400-f563-11ea-8cb2-e9ead90c5924.png) is <br>
<img src = "https://latex.codecogs.com/svg.latex?\sum_j{(|Vor_{%3C%20j}(p_i)|%20*O(1))}%20=%20O(n^2)"><br> Thus total time complexity for n voronoi regions is <img src = "https://latex.codecogs.com/svg.latex?O(n^3)"><br>
Although it has too big complexity compared to other efficient voronoi algorithms, it is simple to implement and there is no delay to draw voronoi diagram for small n. 

<!--- ## 3. Build and Use
### 3.1 (Window)
#### 1) Install a mingw(gcc for window). https://sourceforge.net/projects/mingw-w64/files/
Check the folder where mingw is installed. (e.g C:\Program Files\mingw-w64)
#### 2) Put all ipe*.h files into the include folder of mingw
(e.g C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\lib\gcc\x86_64-w64-mingw32\8.1.0\include)
<br>ipe*.h header files can be found in the include folder of Ipe
(e.g C:\ipe-7.2.17\include)
#### 3) Modify the Makefile if a location of your ipe.exe is different.
Open a Makefile in the folder and find the following part. (-LC:/ipe-7.2.17/bin)
<br>Modify this part by a location of your ipe.exe. (-L"your location")
#### 4) Open a prompt of command(cmd) at this folder and type a "make".
Then L1 voronoi codes are built automatically and it outputs "L1_voronoi.dll" file.
#### 5) Move L1_voronoi.dll and L1_voronoi.lua files into ipelets folder. (e.g C:\ipe-7.2.17\ipelets)
Then L1_voronoi function appears in the ipelets tab.

### 3.2 (Linux)
-->
## 3. Example
![L1_voronoi](https://user-images.githubusercontent.com/17876333/92321277-9d514900-f063-11ea-943c-154da99940a0.png)


<a name="footnote_1">[1]</a>: https://en.wikipedia.org/wiki/Weiler%E2%80%93Atherton_clipping_algorithm <br>
<a name="footnote_2">[2]</a>: Foster, Erich & Hormann, Kai & Popa, Romeo. (2019). Clipping Simple Polygons with Degenerate Intersections. Computers & Graphics: X. 2. 100007. 10.1016/j.cagx.2019.100007.  

*This project is supported by the Institute of Information & communications Technology Planning & Evaluation(IITP) grant funded by the Korea government(MSIT) (No. 2017-0-00905, Software Star Lab (Optimal Data Structure and Algorithmic Applications in Dynamic Geometric Environment)) and (No. 2019-0-01906, Artificial Intelligence Graduate School Program(POSTECH))
