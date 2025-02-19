# HNN Demo

## 1. Introduction
Given a point set $P$, the $k$-nearest neighbor of a query point $q$ is the set of the $k$ closest points in $P$ to $q$. 
Additionally, the $\epsilon$-nearest neighbor of $q$ is the set of points in $P$ whose distance to $q$ is less than or equal to $\epsilon$.
  

## 2. UI

## Operations
It supports GUI using openGL and UI using MFC.
|Rendering object||
|:---|:---|
|Points| If selected, the GUI renders the points.|
|Query| If selected, the GUI renders the query points. |
|Path| If selected, the GUI renders the path of the points. |

|Input object||
|:---|:---|
|Point| If selected, the user can generate points as input.|
|Query| If selected, the user can generate query points.|

|Input mode||
|:---|:---|
|Mouse| If selected, the user can generate a point as input by clicking the mouse at the desired location |
|Keyboard| If checked, the user can generate a point as input by setting its x- and y-coordinates using the keyboard.|


|Query mode||
|:---|:---|
|KNN| If selected, the user can compute the KNN of query point. |
|Epsilon| If checked, the user can compute the $\epsilon$-NN of query point.|

## 3. How to Use HNN Demo

### Step 1: Creat a Point Set
Use the function `1. Make input file`. The input be point set $P$.

### Step 2: Save the Point Set
After creating the point set $P$, press the save button to store the polygon.

### Step 3: Open the Saved Point Set
Use the function `2. Test the input file`. Click the File open button to open the point set file saved in Step 2.

### Step 4: KNN Setting and Query
Enter the value of $k$ in the Result KNN button and click change. In the Input object, press the Query button to input the query point $p$. The system will return the $k$-nearest neighbor points and their corresponding geodesic distances. You can also change the query point $p$ and make additional queries.

### Step 4:$\epsilon$-nearest neighbor Setting and Query
Enter the value of $\epsilon$ in the Result Epsilon button and click change. In the Input object, press the Query button to input the query point $p$. The system will return the $\epsilon$-nearest neighbor points and their corresponding geodesic distances. You can also change the query point $p$ and make additional queries.

## 4. Before compile and use it
This field is for users who want to compile.
This program is developed using Microsoft Visual Studio 2022.
To compile the source code, you may need `C++ MFC for the latest v143 build tool`.
You can download it by `Visual Studio Installer`.