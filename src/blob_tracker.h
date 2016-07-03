#ifndef _MY_BLOB_TRACKER_
#define _MY_BLOB_TRACKER_

#include "opencv2/features2d.hpp"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

class BlobTracker {
private:
    Ptr<SimpleBlobDetector> sbd;
    Point2f bobber_point;
    double bobber_speed;
public:
    BlobTracker();
    void track_bobber(Mat&);
    void find_center_keypoint(Mat&, vector<KeyPoint>&);
    Point get_bobber_point();
    double get_bobber_speed();
};

#endif