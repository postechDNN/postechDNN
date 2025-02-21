# EpsGraph Demo

## 1. Introduction
Given an polygonal obstacle set $P$ in 3D Euclidean space and a point set $Q \subset \mathbb{R}^3$, compute the $k$-nearest neighbor points in $Q$ for a query point $p$ and a positive integer $k$.

## 2. UI

## Operations
It supports GUI using openGL and UI using MFC.
|Rendering object||
|:---|:---|
|Vertex| If selected, the GUI renders the vertices of the polygonal obstacle.|
|Edge| If selected, the GUI renders the edges of the polygonal obstacle.|
|Query| If selected, the GUI renders the query in the space. |
|Points set| If selected, the GUI renders the points set. |

|Input object||
|:---|:---|
|Polygon|If selected, the user can generate vertices of polygon as input.  |
|Point| If selected, the user can generate points inside a polygon as input.|
|Query| If selected, the user can generate query points inside a polygon.|

|Input mode||
|:---|:---|
|Keyboard| If checked, the user can generate a point as input by setting its x- and y-coordinates using the keyboard.|

## 3. How to Use DNN Demo

### Step 1: Open the saved obstacles
Use the function `3D nearest neighbor search`. In the `Function` object, check `Polytope` and `More (File I/O)` then click `Add` to add the file saves obstacles.

### Step 2: Create the point Set Q not intersects with obstacles
In the `Function` object, check `Free Point` and `More (File I/O)`press the `Add` button to create the point set $Q$. 

### Step 3: KNN Setting and Query
Enter the value of $k$ in the `KNN` window and click `Query`. Then enter the coordinates about the query point $p$. The system will return the $k$-nearest neighbor points and their corresponding geodesic distances. You can also change the query point $p$ and make additional queries.

### Step 4: Editing Points
You can delete points using the `Delete` button or add new points by pressing the `Add` button again.

### Step 5: Modify KNN and Re-query
Input a new query point $p'$ by pressing the `Query` button. Then, in the `KNN` window, enter a new $k'$ value and click change to update the query.


## 4. Before compile and use it
This field is for users who want to compile.
This program is developed using Microsoft Visual Studio 2022.
To compile the source code, you may need `C++ MFC for the latest v143 build tool`.
You can download it by `Visual Studio Installer`.