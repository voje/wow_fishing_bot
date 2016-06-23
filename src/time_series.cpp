#include "time_series.h"
#include <cmath>

TimeSeries::TimeSeries(){
	this->mean = -1;
	this->std_dev = -1;
	this->collecting = true;
}

void TimeSeries::analyze(){
	mean = accumulate(samples.begin(), samples.end(), 0.0) / samples.size();
	double variance = 0;
	for(int i=0; i<samples.size(); i++){
		variance += pow((samples[i]-mean), 2);
	}
	variance /= samples.size();
	std_dev = sqrt(variance);
	cout << "mean: " << mean << ", std_dev: " << std_dev << endl;
}

double TimeSeries::new_sample(double s){
	//if collecting, adds sample, of not, returns deviation of sample
	if(collecting){
		this->samples.push_back(s);
		return -1;
	}else{
		return abs((s-mean)/std_dev);
	}
}

void TimeSeries::set_collecting(bool b){
	if(collecting && !b){
		this->analyze();
	}
	this->collecting = b;
}

int main(int argc, char** argv){
	TimeSeries ts;
	int my_ints[] = {5,6,4,7,5,3};
	vector<int> test(my_ints, my_ints + sizeof(my_ints)/sizeof(int));
	for(int i=0; i<test.size(); i++){
		ts.new_sample(test[i]);
	}
	ts.set_collecting(false);
	cout << ts.new_sample(6) << endl;
	return 0;
}