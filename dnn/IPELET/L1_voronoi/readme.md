
# L1 Voronoi diagram

## 1. Introduction
The Ipe extensible drawing editor is a tool used by computational geometry researchers to produce 2D figures for inclusion in articles or presentations.
In the latest version ipe-7.2.20, there are already functions to draw a Voronoi diagram(e.g order 1~3 Voronoi diagram, furthest Voronoi diagram and so on).
However, there is no function to draw Voronoi diagram with respect to L1(Manhattan) distance.
It is a reason why we start the project which draw a L1 Voronoi diagram using C++ ipelet framwork of Ipe.
This function can draw a L1 Voronoi diagram of a point set given by user.

## 2. Algorithm and Analysis
The algorithm to compute L1 Voronoi diagram is based on the following basic property.
![image](https://user-images.githubusercontent.com/17876333/93000489-c726d600-f563-11ea-92ea-65644f1ba93f.png)
<br>
Using this property, we can compute each Voronoi region for all sites. When there is a large bounding box containing all points, we can represent each ![vor_p_i](https://user-images.githubusercontent.com/17876333/98764058-58101580-241e-11eb-8134-ef2d1a3660f9.png)
 as bounded simple polygon.
Thus we can compute each Voronoi region using simple polygon intersection algorithm.
Specifically, there is a popular clipping algorithm, so called ` Weiler Atherton Polygon Clipping Algorithm` <sup>[[1]](#footnote_1)</sup>
In manhattan environment, there are lots of degenerate case to compute Voronoi region. For example two edges can be overlapped when we intersect them. Thus we write the algorithm which can deal with these degenerate situations referring to the paper. <sup>[[2]](#footnote_1)</sup>
Note that the time to compute the intersection between two simple polygon is <img src="https://latex.codecogs.com/svg.latex?O(nm)"> in `Weiler Atherton method`. Since the complexity of each ![CodeCogsEqn (1)](https://user-images.githubusercontent.com/17876333/98764254-c3f27e00-241e-11eb-8830-06aa14c5d67b.png) is <img src = "https://latex.codecogs.com/svg.latex?O(1)"> , the time to compute ![image](https://user-images.githubusercontent.com/17876333/93000517-f4738400-f563-11ea-8cb2-e9ead90c5924.png) is <br>
<img src = "https://latex.codecogs.com/svg.latex?\sum_j{(|Vor_{%3C%20j}(p_i)|%20*O(1))}%20=%20O(n^2)"><br> Thus total time complexity for n Voronoi regions is <img src = "https://latex.codecogs.com/svg.latex?O(n^3)"><br>
Although it has too big complexity compared to other efficient Voronoi algorithms, it is simple to implement and there is no delay to draw Voronoi diagram for small n. 

## 3. Build and Use
### 3.1. (Window)
 1) Install a mingw(gcc for window). https://sourceforge.net/projects/mingw-w64/files/<br>
 Check the gcc version in a prompt of command (gcc -v)
 2) Check the path of ipe folder which is installed in your computer. (e.g C:\ipe-7.2.20)
 3) Open a prompt of command(cmd) at `L1_Voronoi\Window` folder and type a "make IPE=`the above path`". (e.g make IPE=C:\ipe-7.2.20)<br>
 Then L1 Voronoi codes are built automatically and it outputs "L1_Voronoi.dll" file.
 4) Move L1_Voronoi.dll and L1_Voronoi.lua files into ipelets folder. (e.g C:\ipe-7.2.20\ipelets)<br>
 Then the L1_Voronoi function appears in the ipelets tab.

### 3.2. (Mac)
 1) Install gcc and GNU make.<br>
 Check the gcc, make version in a terminal (gcc -v, make -v)
 2) Check the path of ipe folder which is installed in your computer. (e.g /Applications/Ipe.app)
 3) Open a terminal at `L1_Voronoi/Mac` folder and type a "make IPE=`the above path`". (e.g make IPE=/Applications/Ipe.app)<br>
 Then L1 Voronoi codes are built automatically and it outputs "L1_Voronoi.so" file.
 4) Move L1_Voronoi.so and L1_Voronoi.lua files into ipelets folder. (e.g /Applications/Ipe.app/Contents/Resources/ipelets)<br>
 Then the L1_Voronoi function appears in the ipelets tab.

### 3.3 (Linux)
To be updated.

## 4. Example
![L1_Voronoi](https://user-images.githubusercontent.com/17876333/92321277-9d514900-f063-11ea-943c-154da99940a0.png)


<a name="footnote_1">[1]</a>: https://en.wikipedia.org/wiki/Weiler%E2%80%93Atherton_clipping_algorithm <br>
<a name="footnote_2">[2]</a>: Foster, Erich & Hormann, Kai & Popa, Romeo. (2019). Clipping Simple Polygons with Degenerate Intersections. Computers & Graphics: X. 2. 100007. 10.1016/j.cagx.2019.100007.  

*This project is supported by the Institute of Information & communications Technology Planning & Evaluation(IITP) grant funded by the Korea government(MSIT) (No. 2017-0-00905, Software Star Lab (Optimal Data Structure and Algorithmic Applications in Dynamic Geometric Environment)) and (No. 2019-0-01906, Artificial Intelligence Graduate School Program(POSTECH))
