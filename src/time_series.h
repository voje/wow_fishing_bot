#ifndef _MY_TIME_SERIES_
#define _MY_TIME_SERIES_

#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

class TimeSeries{
private:
	vector<double> samples;
	double mean;
	double std_dev;
	bool collecting;
public:
	TimeSeries();
	double new_sample(double s);
	void analyze();
	double get_std_dev();
	void set_collecting(bool);
	bool is_collecting();
};

#endif