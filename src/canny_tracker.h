#ifndef _MY_CANNY_TRACKER_
#define _MY_CANNY_TRACKER_

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

class CannyTracker{
private:
	Point bobber_point;
	double bobber_speed;
public:
	CannyTracker();
	Point get_bobber_point();
	double get_bobber_speed();
	void track_bobber(Mat&, bool);			
	void find_mid_point(Mat&);
};

#endif