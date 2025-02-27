# L1 Nearest Neighbor Search Demo

## 1. Introduction
Given a polygonal domain $P$ and points set $Q$, we compute the $k$-nearest neighbors of a query point $q$ with respect to a polygonal domain as an obstacle.
$L_1$ norm is a distance metric.

  
## 2. UI

## Operations
It supports GUI using openGL and UI using MFC.
|Rendering object||
|:---|:---|
|Vertex| If selected, the GUI renders the points.|
|Edge| If selected, the GUI renders the edges.|
|Query| If selected, the GUI renders the querys of polygonal domain. |
|Points set| If selected, the GUI renders the points set. |

|Input object||
|:---|:---|
|Polygonal domain| If selected, the user can generate rectangles as input.|
|Point| If selected, the user can generate points as input.|


|Input mode||
|:---|:---|
|Mouse| If selected, the user can generate a point as input by clicking the mouse at the desired location |
|Keyboard| If checked, the user can generate a point as input by setting its x- and y-coordinates using the keyboard.|


|Query mode||
|:---|:---|
|KNN| If selected, the user can compute the KNN of query point. |

## 3. How to Use L1NNS Demo

### Step 1: Creat a Input Set 
Use the function `1. Make input file`. The input be rectangle polygonal domain and a set of points. 
In the Input object, press the Polygonal domain button to input the polygonal domain $P$.
Polygonal domain is consisted only of rectangles. A rectangle is created by setting two vertices that form its diagonal.
In the Input object, press the Point button to input the point set $Q$.

### Step 2: Save the Input Set 
After creating the input set $P$, press the save button to store the input set.

### Step 3: Open the Saved Input Set
Use the function `2. Test the input file`. Click the File open button to open the input set file saved in Step 2.

### Step 4: KNN Setting and Query
Enter the value of $k$ in the Result KNN button and click change. In the Input object, press the Query button to input the query point $p$. The system will return the $k$-nearest neighbor points and their corresponding $L_1$ distances. You can also change the query point $p$ and make additional queries.



## 4. Before compile and use it
This field is for users who want to compile.
This program is developed using Microsoft Visual Studio 2022.
To compile the source code, you may need `C++ MFC for the latest v143 build tool`.
You can download it by `Visual Studio Installer`.