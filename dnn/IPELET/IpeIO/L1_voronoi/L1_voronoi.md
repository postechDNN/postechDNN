# User Manual

## 1. Introduction
The Ipe extensible drawing editor is a tool used by computational geometry researchers to produce 2D figures for inclusion in articles or presentations.
In the latest version ipe-7.2.20, there are already functions to draw a voronoi diagram(e.g order 1~3 voronoi diagram, furthest voronoi diagram and so on).
However, there is no function to draw voronoi diagram with respect to L-1(Manhattan) distance.
It is a reason why we start the project which draw a L-1 voronoi diagram using C++ ipelet framwork of Ipe.
This function can draw a L-1 voronoi diagram of a point set given by user.

## 2. Build and Use
### 2.1 (Window)
#### 1) Install a mingw(gcc for window). https://sourceforge.net/projects/mingw-w64/files/
Check the folder where mingw is installed. (e.g C:\Program Files\mingw-w64)
#### 2) Put all ipe*.h files into the include folder of mingw
(e.g C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\lib\gcc\x86_64-w64-mingw32\8.1.0\include)
<br>ipe*.h header files can be found in the include folder of Ipe
(e.g C:\ipe-7.2.17\include)
#### 3) Modify the Makefile if a location of your ipe.exe is different.
Open a Makefile in the folder and find the following part. (-LC:/ipe-7.2.17/bin)
Modify this part by a location of your ipe.exe. (-L"your location")
#### 4) Open a prompt of command(cmd) at this folder and type a "make".
Then L1 voronoi codes are built automatically and it outputs "L1_voronoi.dll" file.
#### 5) Move L1_voronoi.dll and L1_voronoi.lua files into ipelets folder. (e.g C:\ipe-7.2.17\ipelets)
Then L1_voronoi function appears in the ipelets tab.

### 2.2 (Linux)

## 3. Example
![L1_voronoi](https://user-images.githubusercontent.com/17876333/92321277-9d514900-f063-11ea-943c-154da99940a0.png)

