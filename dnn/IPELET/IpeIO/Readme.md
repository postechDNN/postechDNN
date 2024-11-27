# IO tool for IPELET

## Summary
IPEIO is tool for implement ipelet. 
This tool helps user to get object from ipe and 
draw object to ipe.
The properties that are usually used can be applied easily.
For example, color or size can be applied by a sigle function and value to multiple objects. 

## Attribute Flag
To ease the use of attributes, we define enum ATTR_FLAG.

|ATTR_FLAG||
|:---|:---|
|FLAG_EMPTY|Represent no attributes are exist.(0x00)|
|FLAG_COLOR|Represent color attributes exist.(0x01)|
|FLAG_ARROW|Represent arrow attributes exist.0x02)|
|FLAG_DASH|Represent dash attributes exist.(0x04)|
|FLAG_VALUE|Represent custom attributes exist.(0x08)|
|FLAG_POINT_SIZE|Represent point size attributes exist.(0x10)|
|FLAG_POINT_STYLE|Represent point style attributes exist.(0x11)|
|FLAG_PEN_WIDTH|Represent pen width attributes exist.(0x20)|
|FLAG_FILL|Represent fill attributes exist.(0x40)|

## Functions
The functions for get objects(point, segment, polygon or etc) from ipe.
The parameter *data* and *helper* are common for all get functions.

The functions for data handling. (SubPath2Vec, getFirst, getSecond)

### Get_points
Get marks on the page. If no marks on the page, this function returns false.
```
bool Get_points(IpeletData *data, IpeletHelper *helper, 
	std::vector<Vector> &ret);
```
- *ret*
  - The variable for return.  
  - std::vector type and contains ipe::Vector which correnponds to marks.  

### Get_segments
Get segments on the page. If no segments on the page, this function returns false.
```
bool Get_segments(IpeletData *data, IpeletHelper *helper, 
	bool only_single_subpath, 
	std::vector<CurveSegment> &ret);
```
- *only_single_subpath*
  - If true, this function returns only subpaths consist of a single line segments.
- *ret*
  - The variable for return.  
  - std::vector type and contains ipe::CurveSegment which correnponds to segments. 

### Get_splines
Get splines on the page. If no splines on the page, this function returns false.
```
bool Get_splines(IpeletData *data, IpeletHelper *helper, 
	std::vector<CurveSegment> &ret);
```
- *ret*
  - The variable for return.  
  - std::vector type and contains ipe::CurveSegment which correnponds to splines.  

### Get_polygons
Get polygons on the page. If no polygons on the page, this function returns false.
```
bool Get_polygons(IpeletData *data, IpeletHelper *helper, 
	std::vector<const SubPath*> &ret);
```
- *ret*
  - The variable for return.  
  - std::vector type and contains ipe::SubPath which correnponds to polygon(closed SubPath).

### Get_polylines
Get polylines on the page. If no polylines on the page, this function returns false.
```
bool Get_polylines(IpeletData *data, IpeletHelper *helper, 
	std::vector<const SubPath*> &ret);
```
- *ret*
  - The variable for return.  
  - std::vector type and contains ipe::SubPath which correnponds to polyline(non-closed SubPath).

### SubPath2Vec
Convert SubPath object to std::vector\<ipe::Vector\> object.
```
std::vector<Vector> SubPath2Vec(const SubPath *subpath, bool add_first);
```
- *subpath*
  - The subpath object that we want to convert.
- *add_first*
  - If true, the first and last element of returned vector are same.

### GetFirst, GetSecond
Get the start or end point (ipe::Vector) of CurveSegment.
```
Vector getFirst(CurveSegment cs);
Vector getSecond(CurveSegment cs);
```
- *cs*
  - The input CurveSegment.

## Class IPEIO
This class is unifing class for drawing objects. 
The properties are stored in the class so that drawing methods preservind properties.

### Members
|Private Variables||
|:---|:---|
|Attr_flag|Stores existing attributes. See [ATTR_FLAG](#Attribute-Flag).|
|props|Stores type of user-defined properties.(Index corresponds to vals.)|
|vals|Stores value of user-defined properties.(Index corresponds to props.)|
|construct|Variable for constructing attributes.|
|color_attr|Stores color attribute.(common)|
|pts_size|Stores point size attribute.(point)|
|pts_type|Stores point type attribute.(point)|
|farrow|Stores the information abour arrow direction.(segment or polylines)|
|dash|Stores the dash attribute.(segment, polygon or polyline)|
|pen_width|Stores the pen width attribute.(segment, polygon or polyline)|
|fill|Stores the fill attribute.(polygon or polyline)|
|mode|Stores the mode of fill.(polygon or polyline)|

|Private Methods||
|:---|:---|
|[set_Attrs(Reference)](#set_Attrs)|Set color and size attributes stored in this class to Reference.|
|[set_Attrs(Path)](#set_Attrs)|Set path attributes stored in this class to Path.|

|Public Methods||
|:---|:---|
|[IPEIO()](#IPEIO)|Constructor.|
|[Draw_point(IpeletData, IpeletHelper, Vector)](#Draw_point)|Draw a point.|
|[Draw_segment(IpeletData, IpeletHelper, Vector, Vector)](#Draw_segment)|Draw a segment.|
|[Draw_segment(IpeletData, IpeletHelper, CurveSegment)](#Draw_segment)|Draw a segment.|
|[Draw_poly(IpeletData, IpeletHelper, vector<Vector>, bool)](#Draw_poly)|Draw a polygon or polyline.|
|[Draw_poly(IpeletData, IpeletHelper, SubPath\*, bool)](#Draw_poly)|Draw a polygon or polyline.|
|[reset_attr(int)](#reset_attr)|Reset [Attr_flag](#Member).|
|[set_color(string)](#set_color)|Set color attribute.|
|[set_color(int, int, int)](#set_color)|Set color attribute.|
|[set_pts_size(int)](#set_pts_size)|Set point size attribute.|
|[set_pts_type(int)](#set_pts_type)|Set point style attribute.|
|[set_arrow(bool)](#set_arrow)|Set arrow attribute.|
|[set_dash(int)](#set_dash)|Set dash attribute.|
|[set_pen(int)](#set_pen)|Set pen width attribute.|
|[set_fill(string, bool)](#set_fill)|Set fill attribute.|
|[set_fill(int, int, int, bool)](#set_fill)|Set fill attribute.|
|[add_Attribute(Property, Attribute)](#set_Attribute)|Set custom attribute.|

### set_Attrs
These methods are used in the public method.
Before drawing objects, set attributes in the IPEIO to the input parameter *obj*.
```
void set_Attrs(Reference *obj);
void set_Attrs(Path *obj);
```

### IPEIO
Constructor of IPEIO. 
Default [Attr_flag](#Members) is [FLAG_EMPTY](#Attribute-Flag),
[pts_type](#Members) is disk,
[mode](#Members) is false.

### Draw_point
Draw a point.
```
bool Draw_point(IpeletData *data, IpeletHelper *helper, Vector p);
```
- *p*
  - A point (ipe::Vector type) that you want to draw.

### Draw_segment
Draw a segment.
```
bool Draw_segment(IpeletData *data, IpeletHelper *helper, Vector first, Vector second);
bool Draw_segment(IpeletData *data, IpeletHelper *helper, const CurveSegment &cs);
```
- *first*, *second*
  - Two endpoint (ipe::Vector type) of segment.
- *cs*
  - The CurveSegment corresponds to segment that you want to draw.

### Draw_poly
Draw a polygon or polyline.
```
bool Draw_poly(IpeletData *data, IpeletHelper *helper, std::vector<Vector> pts, bool closed);
bool Draw_poly(IpeletData *data, IpeletHelper *helper, SubPath *sp, bool closed);
```
- *pts*
  - The std::vector of points (ipe::Vector type) of polygon or polyline.
- *sp*
  - The SubPath object corresponds to polygon or polyline that you want to draw.
- *closed*
  - If true, draw polygon. 
  - Otherwise, draw polyline. 


### reset_attr
Set [Attr_flag](#Member).
```
void reset_attr(int flag);
```
- *flag*
  - The value of flag that you want to set if exists.
  - Default value is [FLAG_EMPTY](#Attribute-Flag).

### set_color
Set color attribute. You can use pre-defined string type or RGB value.
```
void set_color(std::string color);
void set_color(int r,int g,int b);
```
- *string*
  - Pre-defined color name in IPE.
- *r*, *g*, *b*
  - RGB value of color. Each values are in range [0,255]

### set_pts_size
Set size of point.
```
void set_pts_size(int size);
```
- *size*
  - normal:0 
  - large:1 
  - small:-1 
  - tiny:-2

### set_pts_style
Set style of point.
```
void set_pts_style(int type);
```
- *type*
  - disk:0 
  - circle:1  
  - square:2 box:3 
  - cross:4

### set_arrow
Set arrow of segment or polyline.
```
void set_arrow(bool forward);
```
- *forward*
  - If true, forward arrow.
  - Otherwise, reverse arrow.

### set_dash
Set dash of segment, polygon or polyline.
```
void set_dash(int type);
```
- *type*
  - No:0 
  - Dot:1 
  - Dash:2 
  - Dash&Dot:3 
  - Dash&Dot&Dot:4

### set_pen
Set pen width of segment, polygon or polyline.
```
void set_pen(int type);
```
- *type*
  - normal:0  
  - heavier:1  
  - fat:2  
  - ultrafat:3

### set_fill
Set fill color and fill type of polygon or polyline.
```
void set_fill(std::string color,bool fillonly);
void set_fill(int r,int g,int b,bool fillonly);
```
- *fillonly*
  - If true, fill type is fill only.
  - Otherwise, fill type is stroke&fill.
  - Note that default fill type is stroke only. 

### set_Attribute
Set custom attribute that does not implented in this class.
```
void add_Attribute(Property prop, Attribute val);
```
- *prop*
  - The name of property.
  - See the [ipelet document](http://ipe.otfried.org/manual/group__attr.html#gac50c1b1c913f9551a97e37ef29eb462f)
- *val*
  - The value of property.
  - See the [ipelet document](http://ipe.otfried.org/manual/classipe_1_1_attribute.html)
