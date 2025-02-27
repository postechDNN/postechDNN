# Convex Metric Dynamic Nearest Neighbor Demo

# 1. Introduction
Given a set of convex metric point $P$ and points set $Q$, we compute the $k$-nearest neighbors of a query point $q$ in $Q$.
Convex metric is a distance metric.

  
## 2. UI

## Operations
It supports GUI using openGL and UI using MFC.
|Rendering object||
|:---|:---|
|CVM input| If selected, the GUI renders the input of convex metric. |
|CVM result| If selected, the GUI renders the result of convex metric. |
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

|QParamters||
|:---|:---|
|KNN| If selected, the user can compute the KNN of query point. |

## 3. How to Use CMDNN Demo

### Step 1: Creat a Input Set 
Use the function `1. Make input file`. The input be a set of convex metric point $P$ and input points set $Q$. 
The bounding box of convex points must include origin point $(0,0)$.
In the Input object, press the CVM input button to input $P$.
In the Input object, press the Points button to input $Q$.

### Step 2: Save the Input Set 
After creating the input set $P$, press the save button to store the $P$.

### Step 3: Open the Saved Input Set
Use the function `2. Test the input file`. Click the File open button to open the input set file saved in Step 2.

### Step 4: KNN Setting and Query
Enter the value of $k$ in the Result KNN button and click change. In the Input object, press the Query button to input the query point $p$. The system will return the $k$-nearest neighbor points and their corresponding convex metric. 



## 4. Before compile and use it
This field is for users who want to compile.
This program is developed using Microsoft Visual Studio 2022.
To compile the source code, you may need `C++ MFC for the latest v143 build tool`.
You can download it by `Visual Studio Installer`.