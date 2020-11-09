# Example of $L_1$ Nearest Neighbor Search in Rectangular Domain (L1NNS)
Sample execute file L1NNS.exe is available for $L_1$ Nearest Neighbor Search in Rectangular Domain (L1NNS).  
To execute the file, you may need freeglut 3.0.0 MSVC Package in https://www.transmissionzero.co.uk/software/freeglut-devel/.   
Unzip freeglut-MSVC-3.0.0-2.mp.zip file. You may see three folders, **bin**, **include**, and **lib**.  
and put **include** and **lib** folder into the directory in which L1NNS.exe is contained.  
Also, put freeglut.dll in *bin* folder into the above directory.  

## Operations
It supports GUI using openGL and is possible with left/right mouse click and drag and drop partially.  
|Operations||
|:---|:---|
|[left mouse click]| When clicking buttons, inserting points, querying, and deleting objects |
|[right mouse click]| When displaying pop-up menu for changing NNS/2-NNS/3-NNS/FNS |
|[drag and drop]| When inserting rectangles |

### "QUERY" button (Now the first green button)
	- Set up query point  
	- If there is no data in rectangular domain, it does not work  
	- Highlight the query and its nearest data  
	- Also support 2-NNS, 3-NNS, and Farthest-NS  

### Initial state or "INSERT" button (Now the second green button)
	- Insertion of point and rectangle  
	- Overlap is impossible  

### "DELETE" button (Now the third green button)
	- Deletion of point and rectangle  
	- The nearest object from pointed location is deleted  
	- Pointed location is too far, there is no deletion  

### "QUIT" button (Now the fourth green button)
	- Exit the program

## Before use it
This field is for users who want to compile.   
This program is developed using Microsoft Visual Studio 2019.  
To compile the source code, you may need freeglut 3.0.0 MSVC Package in https://www.transmissionzero.co.uk/software/freeglut-devel/.   
Unzip freeglut-MSVC-3.0.0-2.mp.zip file. You may see three folders, **bin**, **include**, and **lib**.  
and put **include** and **lib** folder into the directory in which source codes are contained.  
Also, put freeglut.dll in *bin* folder into the above directory.   
Then do the following in Visual Studio.  
```
Project -> Properties -> C/C++ -> General -> Additional Include Directories -> add "./include;"  
Project -> Properties -> Linker -> General -> Additional Library Directories -> add "./lib;"  
Project -> Properties -> Linker -> Input -> Additional Dependencies -> add "freeglut.lib;"
```
