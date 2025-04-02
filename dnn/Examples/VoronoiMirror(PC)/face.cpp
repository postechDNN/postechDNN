#include "voronoi_camera.h"


#define FACE_MAT_WIDTH 640
#define FACE_MAT_HEIGHT 360
#define OUTER_WIDTH_PADDING 20
#define OUTER_HEIGHT_PADDING 10
//lbr OUTER_WIDTH_PADDING 15
//lbr OUTER_HEIGHT_PADDING 5
//haar OUTER_WIDTH_PADDING 20
//haar OUTER_HEIGHT_PADDING 10
vector<Rect> detectFace(Mat &frame) {
	vector<Rect> faceBoundary;
	Mat frame_gray;



	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	resize(frame_gray, frame_gray, Size(FACE_MAT_WIDTH, FACE_MAT_HEIGHT));
	equalizeHist(frame_gray, frame_gray);
	face_cascade.detectMultiScale(frame_gray, faceBoundary, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(20, 20));

	for (int i = 0; i < faceBoundary.size(); i++) {
		faceBoundary[i].x *= (wdth / FACE_MAT_WIDTH);
		faceBoundary[i].y *= (ht / FACE_MAT_HEIGHT);
		faceBoundary[i].width *= (wdth / FACE_MAT_WIDTH);
		faceBoundary[i].height *= (ht / FACE_MAT_HEIGHT);
		//faceBoundary[i].x -= faceBoundary[i].width / OUTER_WIDTH_PADDING;
		//faceBoundary[i].width += faceBoundary[i].width / OUTER_WIDTH_PADDING;
		//faceBoundary[i].y -= faceBoundary[i].height / OUTER_HEIGHT_PADDING;
		//faceBoundary[i].height += faceBoundary[i].height / OUTER_HEIGHT_PADDING;
	}

	return faceBoundary;
}

