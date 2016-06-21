#include "blob_tracker.h"
#include <iostream>

using namespace cv;
using namespace std;

BlobTracker::BlobTracker(){
	// Setup SimpleBlobDetector parameters.
	SimpleBlobDetector::Params params;
	 
	// Change thresholds
	params.minThreshold = 100;
	params.maxThreshold = 255;

	// Filter by Area.
	params.filterByArea = false;
	params.minArea = 1500;
	 
	// Filter by Circularity
	params.filterByCircularity = true;
	params.minCircularity = 0.7;
	 
	// Filter by Convexity
	params.filterByConvexity = true;
	params.minConvexity = 0.87;
	 
	// Filter by Inertia
	params.filterByInertia = true;
	params.minInertiaRatio = 0.9;

	this->sbd = SimpleBlobDetector::create(params);
	this->bobber_point = Point(-1, -1);
	this->bobber_speed = 0;
}

void BlobTracker::find_center_keypoint(Mat& frame, vector<KeyPoint>& kp){
	//go through all keypoints and select the centermost one
	Point2f center(frame.cols/2, frame.rows/2);
	cout << center << endl;
	for(int i=0; i<kp.size(); i++){
		//todo
	}
}

void BlobTracker::find_bobber(Mat& frame){
	vector<KeyPoint> kp;
	sbd->detect(frame, kp);
	Mat frame_with_kp;
	this->find_center_keypoint(frame, kp);
	drawKeypoints(frame, kp, frame_with_kp, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
	imshow("test_show", frame_with_kp);	
	waitKey(0);
}