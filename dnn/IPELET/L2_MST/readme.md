# Minimum Spanning Tree

## 1. Introduction 
 This is an Ipelet source code for drawing a MST(minimum spanning tree) of a graph using Ipe. 
It uses Kruskal's algorithm to find a MST. 

## 2. Inputs and outputs
There are some restrictions to use this extension. 
First, all the vertices and edges contained in a graph of which you want to find a MST should be selected. 
Here, 'vertices' mean objects with a type of "ERefernce" which are drawn by 'Mark' in Ipe. 
And 'edges' mean objects with type of "EPath" and a straight line which are drawn by 'Lines and polylines' in Ipe.  (See ipe::Object)
Also, the two endpoints of a selected edge should have a vertice on the same place. 

If all the above conditions are satisfied, edges of an computed MST are drawn by red dashed lines. 

## 3. How to compile and edit
The main algorithm is implemented in GraphIpelet::run in mst.cpp. 
It uses some functions defined in ipeIO.h and ipeIO.cpp. 

After you edit parts of the code, you can use Makefile to compile and generate .dll and .lua files. 
Move( or copy and paste) the generated .dll and .lua files to ipelet directory of your laptop. 
