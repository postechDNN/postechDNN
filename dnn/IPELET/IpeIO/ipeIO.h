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


/**	Get marks on primary selected page (IPE) */
bool Get_points(IpeletData *data, IpeletHelper *helper, 
	std::vector<Vector> &ret);

/**	Get segments on primary selected page (IPE) \n
	If only_single_subpath is true, it returns path contain sigle straight line segment. \n
	Otherwise, it returns all path object including polygon. */
bool Get_segments(IpeletData *data, IpeletHelper *helper, bool only_single_subpath, 
	std::vector<CurveSegment> &ret);

/**	Get polygons on primary selected page (IPE) */
bool Get_polygons(IpeletData *data, IpeletHelper *helper, 
	std::vector<const SubPath*> &ret);

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
	Attribute color_attr;
public:

	/* Drawing tools */
	bool Draw_point(IpeletData *data, IpeletHelper *helper, Vector p);
	bool Draw_segment(IpeletData *data, IpeletHelper *helper, Vector first, Vector second);
	bool Draw_segment(IpeletData *data, IpeletHelper *helper, const CurveSegment &cs);
	bool Draw_polygon(IpeletData *data, IpeletHelper *helper, std::vector<Vector> pts, bool closed);
	bool Draw_polygon(IpeletData *data, IpeletHelper *helper, SubPath *sp, bool closed);

	/* Attribute setting */
	void set_color(std::string color);
	void set_color(int r,int g,int b);

};

#endif