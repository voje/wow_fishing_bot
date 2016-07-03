#include "canny_tracker.h"
#include "blob_tracker.h"
#include "time_series.h"
#include <iostream>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <opencv2/imgproc.hpp>
#include <string>
#include <unistd.h>
#include <ctime>

using namespace std;

const string samples_folder = "../samples/";

string my_int_to_string(int n) {
    stringstream ss;
    ss << n;
    return ss.str();
}

void ImageFromDisplay(Mat& out_img) {
    //this function is plagiarism
    int Width = 0;
    int Height = 0;
    int Bpp = 0;
    vector<uint8_t> Pixels;

    Display* display = XOpenDisplay(":0");
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
    if (Width && Height) {
        img1 = Mat(Height, Width, Bpp > 24 ? CV_8UC4 : CV_8UC3, &Pixels[0]); //Mat(Size(Height, Width), Bpp > 24 ? CV_8UC4 : CV_8UC3, &Pixels[0]);
        out_img = img1.clone();
    }
}

int main(int argc, char** argv) {
    //BlobTracker tracker;
    CannyTracker tracker;
    Mat frame;
    bool go_samples = false;

    if (argc >= 2) {
        int index = 1;
        while (index < argc) {
            string a1 = argv[index];
            index ++;
            if (a1.compare("-c") == 0) {
                string a2 = argv[index];
                index ++;
                tracker.set_color(a2);
            } else {
                go_samples = true;
            }
        }
    }

    if (go_samples) {
        for (int i = 1; i <= 9; i++) {
            string num(1,  char(i + 48));
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
    const double SESSION_TIME = 60 * 5;
    const int FREQ = 150;	//ms
    const double DEV_THRESHOLD = 3;
    double t_diff;
    time_t t_session = time(NULL);
    bool b_session = true;
    while (b_session) {
        time_t t_throw = time(NULL);
        bool b_throw = true;
        TimeSeries ts;
        double dev = -1;
        system("bash ../key_press.sh THROW");
        sleep(5);
        cout << "search" << endl;
        while (b_throw) {
            //fishing here
            ImageFromDisplay(frame);
            cvtColor(frame, frame, CV_BGRA2BGR);
            tracker.track_bobber(frame, false);
            Point p = tracker.get_bobber_point();
            //move mouse
            if (p.y != -1) {
                string xstring = my_int_to_string(p.x - 15);
                string ystring = my_int_to_string(p.y + 25);
                string system_command = "bash ../key_press.sh MOUSEMOVE " + xstring + " " + ystring;
                system(system_command.c_str());
            }

            if (p.y != -1) {
                dev = ts.new_sample(p.y);
            }

            t_diff = difftime(time(NULL), t_throw);
            if (t_diff >= 12 && ts.is_collecting()) {
                ts.set_collecting(false);	//stop collecting data, analyze it
                if (ts.get_std_dev() <= 0)
                    b_throw = false;
            } else if (t_diff >= 30) {
                b_throw = false;
            } else if (dev > DEV_THRESHOLD) {
                system("bash ../key_press.sh LOOT");
                b_throw = false;
            }
            waitKey(FREQ);
        }
        t_diff = difftime(time(NULL), t_session);
        if (t_diff >= SESSION_TIME) {
            b_throw = false;
        }
    }

    return 0;
}
