#pragma once
//this class defines a point in 2D space

//can use int, double

//using namespace CY;

namespace CY {
	class Point {
	protected:
		double x, y;

	public:

		Point();
		Point(double, double);
		Point(CY::Point&);
		~Point();
		bool operator==(CY::Point);
		CY::Point operator- (CY::Point);
		double getx(void);
		void setx(double);
		double gety(void);
		void sety(double);

		double distance(CY::Point);
	};
}