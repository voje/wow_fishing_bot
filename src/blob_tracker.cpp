#include "blob_tracker.h"
#include <iostream>
#include <limits>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

BlobTracker::BlobTracker(){
	// Setup SimpleBlobDetector parameters.
	SimpleBlobDetector::Params params;
	 
	// Change thresholds
	params.minThreshold = 10;
	params.maxThreshold = 100;

	// Filter by Area.
	params.filterByArea = false;
	params.minArea = 1500;
	 
	// Filter by Circularity
	params.filterByCircularity = true;
	params.minCircularity = 0.5;
	 
	// Filter by Convexity
	params.filterByConvexity = true;
	params.minConvexity = 0.8;
	 
	// Filter by Inertia
	params.filterByInertia = true;
	params.minInertiaRatio = 0.9;

	this->sbd = SimpleBlobDetector::create(params);
	this->bobber_point = Point(-1, -1);
	this->bobber_speed = -1;
}

Point BlobTracker::get_bobber_point(){
	return Point(floor(bobber_point.x), floor(bobber_point.y));
}

double BlobTracker::get_bobber_speed(){
	return bobber_speed;	
}

void BlobTracker::find_center_keypoint(Mat& frame, vector<KeyPoint>& kp){
	//go through all keypoints and select the centermost one
	Point2f center(frame.cols/2, frame.rows/2);
	double min_dist = numeric_limits<double>::max();
	Point2f best_point = Point2f(-1, -1);
	for(int i=0; i<kp.size(); i++){
		double dist = norm(center - kp[i].pt);
		if(dist < min_dist){
			min_dist = dist;
			best_point = kp[i].pt;
		}
	}
	if(this->bobber_point.x != -1){
		this->bobber_speed = norm(best_point - this->bobber_point);
	}
	this->bobber_point = best_point;
}

void BlobTracker::track_bobber(Mat& frame){
	vector<KeyPoint> kp;
	sbd->detect(frame, kp);
	Mat frame_with_kp;
	this->find_center_keypoint(frame, kp);
	/*
	drawKeypoints(frame, kp, frame, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	imshow("test_show", frame);
	waitKey(0);
	*/
}




