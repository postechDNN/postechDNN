# Shortest Visibility Path Demo

## 1. Introduction
Given two points $p_1$ and $p_2$ in a simple polygon $P$, compute the minimum distance that $p_1$ and $p_2$ must travel to see each other in $P$ and the total minimum distance that $p_1$ and $p_2$ must travel to see each other in $P$.

## 2. UI

## Operations
It supports GUI using openGL and UI using MFC.
|Rendering object||
|:---|:---|
|Vertex| If selected, the GUI renders the vertices of the simple polygon.|
|Edge| If selected, the GUI renders the edges of the simple polygon. |
|Path| If selected, the GUI renders the path of the points. |

|Input object||
|:---|:---|
|Polygon|If selected, the user can generate vertices of polygon as input.  |
|Point| If selected, the user can generate points inside a polygon as input.|
|Query| If selected, the user can generate query points inside a polygon.|

|Input mode||
|:---|:---|
|Mouse| If selected, the user can generate a point as input by clicking the mouse at the desired location |
|Keyboard| If checked, the user can generate a point as input by setting its x- and y-coordinates using the keyboard.|


|Query mode||
|:---|:---|
|min-max| If selected, the user can compute the min-max distance path. |
|min-sum| If checked, the user can compute the min-sum distance path.|

## 3. How to Use SPSP Demo

### Step 1: Creat a Simple Polygon
Use the function `1. Make input file`. The input must be a simple polygon, meaning the polygon must satisfy the condition of being simple. That is, no intersections exist between the edges of the polygon except at the vertices of $P$.

### Step 2: Save the Simple Polygon
After creating the simple polygon $P$, press the save button to store the polygon.

### Step 3: Open the Saved Simple Polygon
Use the function `2. Test the input file`. Click the File open button to open the simple polygon file saved in Step 2.

### Step 4: Compute the min-max and min-sum distance path
In the Query mode, press the min-max button to compute the minimum distance path. Similar way, you can compute the min-sum distance path by pressing the min-sum button in the Query mode. 

### Step 5: Add the guide line
In the rendering object, press the Guide line button to add the guide line. The moment when two traveling points can see each other is represented by a guideline.

## 4. Before compile and use it
This field is for users who want to compile.
This program is developed using Microsoft Visual Studio 2022.
To compile the source code, you may need `C++ MFC for the latest v143 build tool`.
You can download it by `Visual Studio Installer`.