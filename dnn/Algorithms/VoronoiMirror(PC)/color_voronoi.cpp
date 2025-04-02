#include "voronoi_camera.h"




Scalar obtain_color(Mat &myimage, vector <Point> &pvec, int *pctr)
{
	int minx = pvec[0].x;
	int maxx = pvec[0].x;
	int miny = pvec[0].y;
	int maxy = pvec[0].y;

	//	int height   = myimage->height;
	//	int width   = myimage->width;

	int step = myimage.step1() / sizeof(uchar);
	int channels = myimage.channels();
	uchar * data = (uchar *)myimage.data;

	for (int i = 0; i < (int)pvec.size(); i++)
	{
		if (pvec[i].x < minx) minx = pvec[i].x;
		if (pvec[i].x > maxx) maxx = pvec[i].x;
		if (pvec[i].y < miny) miny = pvec[i].y;
		if (pvec[i].y > maxy) maxy = pvec[i].y;
	}
	if (minx < 0) minx = 0;
	if (maxx > wdth) maxx = wdth;
	if (miny < 0) miny = 0;
	if (maxy > ht) maxy = ht;

	double c1 = minx + ((double)(maxx - minx) / 2);
	double c2 = miny + ((double)(maxy - miny) / 2);

	int ctr = 0;
	//int pctr=1;
	double l = 0, m = 0, n = 0;   // i rows // j columns

	//int lgt = (int)((maxx-minx)/10);
	//int bgt = (int)((maxy-miny)/10);

	for (int i = miny; i < maxy; i++) {
		for (int j = minx; j < maxx; j++)
		{
			int flag = 1;
			for (int k = 0; k < (int)pvec.size(); k++)
			{

				double a1 = (double)pvec[k].x;
				double a2 = (double)pvec[k].y;

				double b1;
				double b2;

				if (k == (int)pvec.size() - 1)

				{
					b1 = (double)pvec[0].x;
					b2 = (double)pvec[0].y;
				}
				else
				{
					b1 = (double)pvec[k + 1].x;
					b2 = (double)pvec[k + 1].y;
				}

				if (((b1 - a1)*((ht - i) - (ht - a2)) - ((ht - b2) - (ht - a2))*(j - a1)) * ((b1 - a1)*((ht - c2) - (ht - a2)) - ((ht - b2) - (ht - a2))*(c1 - a1)) < 0)  // check whether the pixel is inside the cell
				{
					flag = 0;
					break;
				}

			}


			if (flag == 1)
			{
				ctr++;
				l = l + data[i*step + j * channels + 0];
				m = m + data[i*step + j * channels + 1];
				n = n + data[i*step + j * channels + 2];
			}
		}
	}

	if (ctr == 0)
	{
		//ctr = 1;
	}
	else
	{
		l = l / ctr;
		m = m / ctr;
		n = n / ctr;
	}
	*pctr = ctr;

	return Scalar(l, m, n, 0);

}


Scalar obtain_color_boundary(Mat &myimage, vector <Point> &pvec, int *pctr)
{
	//	int height   = myimage->height;
	//	int width   = myimage->width;
	int step = myimage.step1() / sizeof(uchar);
	int channels = myimage.channels();
	uchar * data = (uchar *)myimage.data;

	int ctr = 0;
	//int pctr=1;
	double l = 0, m = 0, n = 0;


	for (int index = 0; index < (int)pvec.size(); index++) {

		int i = pvec[index].y;
		int j = pvec[index].x;

		if (i >= ht || i < 0 || j >= wdth || j < 0) continue;

		l = l + data[i*step + j * channels + 0];
		m = m + data[i*step + j * channels + 1];
		n = n + data[i*step + j * channels + 2];
		ctr++;
	}

	if (ctr == 0)
	{
		//ctr = 1;
	}
	else
	{
		l = l / ctr;
		m = m / ctr;
		n = n / ctr;
	}
	*pctr = ctr;

	return Scalar(l, m, n, 0);

}

int obtain_area(vector <Point> &pvec)
{
	double area = 0.0;

	for (int i = 0; i < (int)pvec.size(); i++)
	{

		double x1 = (double)pvec[i].x;
		double y1 = (double)pvec[i].y;
		double x2;
		double y2;

		if (i == (int)pvec.size() - 1)
		{
			x2 = (double)pvec[0].x;
			y2 = (double)pvec[0].y;
		}
		else
		{
			x2 = (double)pvec[i + 1].x;
			y2 = (double)pvec[i + 1].y;
		}

		double det = fabs(x1*y2 - x2 * y1);
		area += 0.5 * det;
	}
	//assert(area > 0);

	return (int)area;
}

void draw(Mat &img, vector <sites> &s, vector <vertices> &v, Mat &boundary_image, int total_sites, bool type)
{
	//	clock_t t1=clock(),t2=clock();

	//cvSet(img, cvScalar(0, 0, 0));
	img.setTo(Scalar(50, 50, 50));

	for (int i = 4; i < (int)s.size(); i++)
	{

		if (s[i].active == -1)  continue;

		vector <Point> pt;
		int line_type = LINE_AA;
		//		int bno=1;

		for (int j = 0; j < (int)s[i].list.size(); j++)
		{
			Point p;
			int index = s[i].list[j];

			long double xx = (v[index].x_cor);
			long double yy = (v[index].y_cor);

			p.x = (int)xx;
			p.y = (int)(ht - yy);

			pt.push_back(p);
		}

		if ((int)pt.size() == 0)
		{
			cout << endl << "ERROR: there's no point in pt.";
			display_sites_info(s);
			getchar();
			continue;
		}


		Point *ptr = { &pt[0] };

		int arr = (int)s[i].near_site.size();

		s[i].area = obtain_area(pt);
		Scalar color = obtain_color_boundary(boundary_image, pt, &s[i].area);

		s[i].old_col[0] = s[i].new_col[0];
		s[i].old_col[1] = s[i].new_col[1];
		s[i].old_col[2] = s[i].new_col[2];
		s[i].new_col[0] = (uchar)color.val[0];
		s[i].new_col[1] = (uchar)color.val[1];
		s[i].new_col[2] = (uchar)color.val[2];
		s[i].allowed++;


		double range = pow((double)s[i].area / (ht*wdth), 0.4) * (wdth / 20);
		int int_range = (int)range;

		if (s[i].create_count > 0) {
			s[i].disform_x = (int)unifRand(-range, +range);
			s[i].disform_y = (int)unifRand(-range, +range);
		}


		if (s[i].move_count == 0) {

			if (int_range <= s[i].disform_x) s[i].disform_x--;
			else if (-int_range >= s[i].disform_x) s[i].disform_x++;
			else s[i].disform_x += (int)unifRand(-1.5, 1.49);

			if (int_range <= s[i].disform_y) s[i].disform_y--;
			else if (-int_range >= s[i].disform_y) s[i].disform_y++;
			else s[i].disform_y += (int)unifRand(-1.5, 1.49);

			s[i].move_count = MOVEINTERVAL;
		}
		else {
			s[i].move_count--;
		}


		for (int ii = 0; ii < (int)pt.size(); ii++) {
			pt[ii].x += s[i].disform_x;
			pt[ii].y += s[i].disform_y;
		}


		fillConvexPoly(img, ptr, arr, color, line_type, 0);

		if (type)
			polylines(img, &ptr, &arr, 1, 1, Scalar(255, 255, 255), 1, line_type, 0);

		pt.clear();

	}
}


