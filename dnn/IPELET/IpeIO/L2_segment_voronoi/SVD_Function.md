# SVD_Function v1.2

## Summary
Compute L2 segment voronoi diagram with input segments.

## Members
|Functions||
|:---|:---|
|[checkInput](#checkInput)|Check input is valid.|
|[ComputeBoundary](#ComputeBoundary)|Calculate boundary of voronoi diagram.|
|[BFAlgorithm](#BFAlgorithm)|Brute force algorithm to compute cells of vodornoi diagram.|
|[CellsIntersection](#CellsIntersection)|Computes intersection region of all cells.|

### checkInput
Check input is valid.
```
int checkInput(std::vector<_Segment>* input);
```

#### Parameters
*input*
Array of input segments

### ComputeBoundary
Check input is valid.
```
void ComputeBoundary(double *boundary, std::vector<_Segment> inputSegments);
```

#### Parameters
*boundary*
Address of array that stores result of computing boundary.

*inputSegments*
Array of input segments

### BFAlgorithm
Check input is valid.
```
void BFAlgorithm(std::vector<_Segment> *inputSegments, double boundary[], std::vector<std::vector<SimplePolygon>*>* Cells);
```

#### Parameters
*inputSegments*
Array of input segments

*boundary*
Array of boundary of voronoi diagram.

*Cells*
Address of array that stores result of brute force algorithm.

### CellsIntersection
Computes intersection region of all cells.
```
void CellsIntersection(std::vector<std::vector<SimplePolygon>*>* Cells, std::vector<SimplePolygon> *resultCells)
```

#### Parameters
*Cells*
Array of result of brute force algorithm.

*Cells*
Address of array that stores result of cells intersection.