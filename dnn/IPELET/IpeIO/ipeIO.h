#ifndef IPE_IO
#define IPE_IO

#include "ipelet.h"
#include "ipepath.h"
#include "ipepage.h"
#include "ipeshape.h"
#include "ipereference.h"

#include <vector>
#include <string>

using namespace ipe;

/** 0x01:color  0x02:arrow  0x04:dash  0x08:value  
0x10:point_size 0x11:point_style  0x20:pen_width  0x40:fill*/
enum ATTR_FLAG
{
	FLAG_EMPTY=0x00,
	FLAG_COLOR=0x01,
	FLAG_ARROW=0x02,
	FLAG_DASH=0x04,
	FLAG_VALUE=0x08,
	FLAG_POINT_SIZE=0x10,
	FLAG_POINT_STYLE=0x11,
	FLAG_PEN_WIDTH=0x20,
	FLAG_FILL=0x40
};

/**	Get the selected marks on the primary selected page (IPE) */
bool Get_points(IpeletData *data, IpeletHelper *helper, 
	std::vector<Vector> &ret, bool applyTrans = true);

/**	Get the selected segments on the primary selected page (IPE) \n
	If only_single_subpath is true, it returns path contain sigle straight line segment. \n
	Otherwise, it returns all path object including polygon. */
bool Get_segments(IpeletData *data, IpeletHelper *helper, bool only_single_subpath, 
	std::vector<CurveSegment> &ret, bool applyTrans = true);

/** Get selected splines on the primary selected page (IPE) */
bool Get_splines(IpeletData *data, IpeletHelper *helper, std::vector<CurveSegment> &ret);

/**	Get selected polygons on the primary selected page (IPE) */
bool Get_polygons(IpeletData *data, IpeletHelper *helper, 
	std::vector<const SubPath*> &ret, bool applyTrans = true);

/** Get selected polylines on the primary selected page (IPE) */
bool Get_polylines(IpeletData *data, IpeletHelper *helper, 
	std::vector<const SubPath*> &ret, bool applyTrans = true);

/** Convert subpath object to set of point(Vector) (IPE) \n
	subpath must be Curve, and Ellipse and ClosedSpline may cause error. \n
	If add_first is true, return vector ret satisfies ret[0]==ret[ret.size()-1] */
std::vector<Vector> SubPath2Vec(const SubPath *subpath, bool add_first);

/** return the first end point of cs */
Vector getFirst(CurveSegment cs);
/** return the second end point of cs */
Vector getSecond(CurveSegment cs);


class IPEIO{
private:
	/** 0x01:color  0x02:arrow  0x04:dash  0x08:value  0x10:point_size  0x20:pen_width  0x40:fill*/
	int Attr_flag;

	//common attrs
	std::vector<Property> props;
	std::vector<Attribute> vals;

	Attribute construct;
	Attribute color_attr;

	//for points
	Attribute pts_size;
	Attribute pts_type;
	
	//for segments
	bool farrow;

	//for segments and polygons
	Attribute dash;
	Attribute pen_width;

	//for polygons
	Attribute fill;
	bool mode;

	void set_Attrs(Reference *obj);
	void set_Attrs(Path *obj);
public:
	IPEIO(): Attr_flag(FLAG_EMPTY),pts_type(true,String("mark/disk(sx)")),mode(false){};
	/* Drawing tools */
	bool Draw_point(IpeletData *data, IpeletHelper *helper, Vector p);
	bool Draw_segment(IpeletData *data, IpeletHelper *helper, Vector first, Vector second);
	bool Draw_segment(IpeletData *data, IpeletHelper *helper, const CurveSegment &cs);
	bool Draw_poly(IpeletData *data, IpeletHelper *helper, std::vector<Vector> pts, bool closed);
	bool Draw_poly(IpeletData *data, IpeletHelper *helper, SubPath *sp, bool closed);
	bool Draw_spline(IpeletData *data, IpeletHelper *helper, std::vector<Vector> spline);

	/* Attribute setting */
	void reset_attr(int flag);
	void set_color(std::string color); //color must be defined in IPE
	void set_color(int r,int g,int b); //each values are in range [0,255]
	void set_pts_size(int size); //normal:0  large:1  small:-1  tiny:-2
	void set_pts_style(int type); //disk:0 circle:1  square:2 box:3  cross:4
	void set_arrow(bool forward);
	void set_dash(int type); // No:0  Dot:1  Dash:2 Dash&Dot:3  Dash&Dot&Dot:4
	void set_pen(int type); //normal:0  heavier:1  fat:2  ultrafat:3
	void set_fill(std::string color,bool fillonly);
	void set_fill(int r,int g,int b,bool fillonly);
	void add_Attribute(Property prop, Attribute val); //It does not check object type and properties.
};

#endif


