#include "blob_tracker.h"
#include <iostream>

using namespace std;

const string samples_folder = "../samples/";

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
	bt.find_bobber(frame);	

	return 0;
}