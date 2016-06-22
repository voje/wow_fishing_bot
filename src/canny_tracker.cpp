#include "canny_tracker.h"
#include <limits>
#include <iostream>

using namespace std;

CannyTracker::CannyTracker(){
	this->bobber_point = Point(-1, -1);
	this->bobber_speed = -1;
}

Point CannyTracker::get_bobber_point(){
	return this->bobber_point;
}

double CannyTracker::get_bobber_speed(){
	return this->bobber_speed;
}

void CannyTracker::find_mid_point(Mat& frame){
	int qx = floor(frame.cols/4);
	int qy = floor(frame.rows/4);
	Point center(floor(frame.cols/2), floor(frame.rows/2));
	double min_dist = numeric_limits<double>::max();
	Point best_point = Point(-1, -1);
	for(int i=qy; i<frame.rows-qy; i++){
		for(int j=qx; j<frame.cols-qx; j++){
			Point p(j, i);
			if(frame.at<unsigned char>(p) != 0){
				double dist = norm(p - center);
				if(dist < min_dist){
					min_dist = dist;
					best_point = p;
				}
			}
		}
	}

	bool up = true;
	while(up && best_point.x >= 0){
		if(frame.at<unsigned char>(best_point + Point(-1, -1)) != 0){
			best_point = best_point + Point(-1, -1);
		}else if(frame.at<unsigned char>(best_point + Point(0, -1)) != 0){
			best_point = best_point + Point(0, -1);
		}else if(frame.at<unsigned char>(best_point + Point(1, -1)) != 0){
			best_point = best_point + Point(1, -1);
		}else if(frame.at<unsigned char>(best_point + Point(1, 0)) != 0){
			best_point = best_point + Point(1, 0);
		}else{
			up = false;
		}
	}

	if(this->bobber_point.x >= 0){
		this->bobber_speed = norm(best_point - this->bobber_point);
	}
	this->bobber_point = best_point;
}

void CannyTracker::track_bobber(Mat& frame, bool visualize){
	cvtColor(frame, frame, CV_BGR2GRAY);
	//frame should be gray
	Mat detected_edges;
	blur(frame, detected_edges, Size(3,3));
	int low_threshold = 90;
	int ratio = 4;
	int kernel_size = 3;
	Canny(detected_edges, detected_edges, low_threshold, low_threshold*ratio, kernel_size);
	//do some morph
	int morph_size = 10;
	Mat element = getStructuringElement(0, Size(2*morph_size+1, 2*morph_size+1), Point(morph_size,morph_size));
	morphologyEx(detected_edges, detected_edges, 1, element);

	this->find_mid_point(detected_edges);

	//visualize
	if(visualize){
		Mat detected_edges3 = Mat::zeros(detected_edges.size(), CV_8UC3);
		cvtColor(detected_edges, detected_edges3, CV_GRAY2BGR);
		circle(detected_edges3, this->bobber_point, 4, Scalar(0,255,0));
		imshow("canny", detected_edges3);	
		waitKey(0);
	}
}
