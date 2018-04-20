#include <iostream>
#include <thread>
#include <sys/time.h>
void increment(int i, int ar[]){
		ar[i]++;		
		//std::cout<<"incremented position"<< i <<"\n";
	}

int main(){
	int ar[8] = {1,1,1,1,1,1,1,1};
	struct timeval start, end;
	gettimeofday(&start, NULL);
	for (int i = 0 ; i<100000 ; i++){

	std::thread first (increment,0,ar);
	std::thread second(increment,1,ar);
	std::thread third (increment,2,ar);
	std::thread four (increment,3,ar);
	std::thread five (increment,4,ar);
	std::thread six(increment,5,ar);
	std::thread seven (increment,6,ar);
	std::thread eight (increment,7,ar);

	first.join();
	second.join();
	third.join();
	four.join();
	five.join();
	six.join();
	seven.join();
	eight.join();
	
	}
	gettimeofday(&end, NULL);
	float delta = ((end.tv_sec  - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
	std::cout<<delta;
	return 0;
}
