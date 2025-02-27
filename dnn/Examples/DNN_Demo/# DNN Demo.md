# Dynamic Nearest Neighbor Demo

## 1. Introduction
Given a simple polygon $P$ and a point set $Q$, compute the $k$-nearest neighbor points in $Q$ for a query point $p$ and a positive integer $k$.

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

## 3. How to Use DNN Demo

### Step 1: Creat a Simple Polygon
Use the function `1. Make input file`. The input must be a simple polygon, meaning the polygon must satisfy the condition of being simple. That is, no intersections exist between the edges of the polygon except at the vertices of $P$.

### Step 2: Save the Simple Polygon
After creating the simple polygon $P$, press the save button to store the polygon.

### Step 3: Open the Saved Simple Polygon
Use the function `2. Test the input file`. Click the File open button to open the simple polygon file saved in Step 2.

### Step 4: Creat the Point Set Q within the Simple Polygon
In the Input object, press the Point button to create the point set $Q$. The point set $Q$ must exist inside the $P$.

### Step 5: KNN Setting and Query
Enter the value of $k$ in the Result KNN button and click change. In the Input object, press the Query button to input the query point $p$. The system will return the $k$-nearest neighbor points and their corresponding geodesic distances. You can also change the query point $p$ and make additional queries.

### Step 6: Editing Points
In the Input object, press the Point button to edit existing points. You can delete points using the Delete button or add new points by pressing the Point button again.

### Step 7: Modify KNN and Re-query
In the Input object, input a new query point $p'$ by pressing the Query button. Then, in the Result KNN button, enter a new $k'$ value and click change to update the query.


## 4. Before compile and use it
This field is for users who want to compile.
This program is developed using Microsoft Visual Studio 2022.
To compile the source code, you may need `C++ MFC for the latest v143 build tool`.
You can download it by `Visual Studio Installer`.