#include "blob_tracker.h"
#include <iostream>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>

using namespace std;

const string samples_folder = "../samples/";

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
	//get test input
	string input_img;
	if(argc < 2){
		input_img = "fishing1.png";
	}else{
		input_img = argv[1];
	}
	input_img = samples_folder + input_img;

	BlobTracker bt;
	Mat frame;

	frame = imread(input_img);
	//bt.find_bobber(frame);	

	//screenshot
	Mat screenshot;
	ImageFromDisplay(screenshot);

	imshow("ss", screenshot);
	waitKey(0);

	return 0;
}