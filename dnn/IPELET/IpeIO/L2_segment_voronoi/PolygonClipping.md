# Class PolygonClipping v1.2

## Dependencies in postechDNN
None

## Members
|Private Functions||
|:---|:---|
|[A_func](#A_func)|Computes twice the signed area of the triange.|
|[circularNext](#circularNext)|Returns next element in order of circular.|
|[circularPrev](#circularPrev)|Returns previous element in order of circular.|
|[intersection_phase](#intersection_phase)|Using this fuction when intersection phase.|
|[intersection_case](#intersection_case)|Using this function when the simple polygon intersection other simple polygon.|
|[overlap_case](#overlap_case)|Using this function when the simple polygon overlap other simple polygon.|
|[labeling_phase](#labeling_phase)|Using this fuction when labeling phase.|
|[turn_direction](#turn_direction)|Turns direction.|
|[sublabelling](#sublabelling)|Function for sublabelling.|
|[tracing_phase](#tracing_phase)|Using this function when the edge tracing another edge.|

|Constructors & Destructors||
|:---|:---|
|[PolygonClipping()](#PolygonClipping)|Constructs a polygon clipping object.|
|[~PolygonClipping](#~PolygonClipping)|Deletes a polygon clipping object.|

|Public Functions||
|:---|:---|
|[clipping](#clipping)|Returns intersection region of two simple polygons.|



### PolygonClipping
Constructs a polygon clipping object.
```
PolygonClipping();
```

### \~PolygonClipping
Deletes a polygon clipping object.
```
~PolygonClipping();
```

### clipping
Returns intersection region of two simple polygons.
```
std::vector<SimplePolygon*> clipping(SimplePolygon& _P, SimplePolygon& _Q);
```

#### Parameters
*_P*
A simple polygon object to calculate intersection.  

*_Q*
A simple polygon object to calculate intersection.