#include "canny_tracker.h"
#include "blob_tracker.h"
#include <iostream>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <opencv2/imgproc.hpp>
#include <string>
#include <unistd.h>

using namespace std;

const string samples_folder = "../samples/";

string my_int_to_string(int n){
	stringstream ss;
	ss << n;
	return ss.str();
}

void ImageFromDisplay(Mat& out_img){
	//this function is plagiarism
	int Width = 0;
	int Height = 0;
	int Bpp = 0;
	vector<uint8_t> Pixels;

    Display* display = XOpenDisplay(NULL);
    Window root = DefaultRootWindow(display);

    XWindowAttributes attributes = {0};
    XGetWindowAttributes(display, root, &attributes);

    Width = attributes.width;
    Height = attributes.height;

    XImage* img = XGetImage(display, root, 0, 0 , Width, Height, AllPlanes, ZPixmap);
    Bpp = img->bits_per_pixel;
    Pixels.resize(Width * Height * 4);

    memcpy(&Pixels[0], img->data, Pixels.size());

    XDestroyImage(img);
    XCloseDisplay(display);

    Mat img1;
	if (Width && Height){
		img1 = Mat(Height, Width, Bpp > 24 ? CV_8UC4 : CV_8UC3, &Pixels[0]); //Mat(Size(Height, Width), Bpp > 24 ? CV_8UC4 : CV_8UC3, &Pixels[0]); 
		out_img = img1.clone();
	}
}

int main(int argc, char** argv){
	//BlobTracker tracker;
	CannyTracker tracker;
	Mat frame;

	if(argc >= 2){
		for(int i=1; i<=atoi(argv[1]); i++){
			string num(1,  char(i+48));
			string pt1 = "fishing";
			string pt2 = ".png";
			string input_img = pt1 + num;
			input_img = input_img + pt2;
			input_img = samples_folder + input_img;
			cout << input_img << endl;
			frame = imread(input_img);
			tracker.track_bobber(frame, true);
			/*
			circle(frame, tracker.get_bobber_point(), 4, Scalar(0,255,0));
			imshow("test_show", frame);
			waitKey(0);
			*/
		}
		return 0;
	}

	cout << "you have 3 seconds" << endl;
	sleep(3);

	//if not testing
	while(True){
		//screenshot
		ImageFromDisplay(frame);
		cvtColor(frame, frame, CV_BGRA2BGR);

		tracker.track_bobber(frame, false);	
		cout << "(" << tracker.get_bobber_point() << ") [" << tracker.get_bobber_speed() << "]" << endl;
		Point p = tracker.get_bobber_point();
		if(p.x >= 0 && p.y >= 0){
			string xstring = my_int_to_string(p.x);
			string ystring = my_int_to_string(p.y);
			string system_command = "bash ../key_press.sh MOUSEMOVE ";
			system_command += xstring;
			system_command += " ";
			system_command += ystring;
			system(system_command.c_str());
		}

		waitKey(300);	//delay in ms
	}

	return 0;
}