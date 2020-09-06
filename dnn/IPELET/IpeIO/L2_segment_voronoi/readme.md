# L2 Segment Voronoi Diagram for IPELET

## Classes and Library
[Point](Point.md)  
[Edge](Edge.md)  
[SimplePolygon](SimplePolygon.md)  
[PolygonClipping](PolygonClipping.md)  
[Segment](Segment.md)  
[SVD_Function](SVD_Function.md)  

## Summary
You can draw L2 segment voronoi diagram with IPELET

## How to use
[Download IPELET](https://www.softpedia.com/get/Office-tools/Other-Office-Tools/Ipe.shtml)  
Draw segments more than 2 with IPELET  
Select segments and run Ipelets->L2SegVoronoi

## How to compile
First, move the folder inside the ipelet folder.  
Then, compile in that folder.
```console
>> g++ -c -O2 -DWIN32 -fno-exceptions -fno-rtti -Iinclude *.cpp
>> g++ -shared -o L2_seg_voronoi.dll *.o -L../bin -lipe
```
You can also using ```make``` command.  
Move the L2_seg_voronoi.dll file to '../ipelets' and create an appropriate lua file(or move the L2_seg_voronoi.lua file to '/ipelets').
