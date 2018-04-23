#include <iostream>
#include <thread>
#include <vector>
#include <sys/time.h>

const int array_size = 100000;
int nr_threads = 20;

//Approximate size of array to be worked on by each thread
int chunk_size = int(array_size/nr_threads);

// to print the array
void print(int ar[]) {
for (int i = 0; i< array_size ;i++) {
std::cout<<ar[i]<<'\t';
}
std::cout<<'\n';
}

//finding boundaries for the thread
void chunk(int i,int &intial_b,int &final_b) {
	intial_b = i*chunk_size;
	final_b = intial_b + chunk_size -1;
	if (i == nr_threads-1){
		final_b = array_size-1;
	}
	
}

//incrementing array value at ith position.
void increment(int i, int ar[]) {
	int intial_b;
	int final_b;
	chunk(i, intial_b,final_b);
	for (int pos = intial_b; pos<= final_b; pos++){
		//incrementing 100 times
		for (int j=0; j<1000000 ; j++){
		ar[pos]++;		
		}
	}
	std::cout<<i<<std::endl;
	}


int main() {
	int ar[array_size];
	int intial_b;
	int final_b;
	struct timeval start, end;
	std::vector<std::thread> th;


	//starting the clock
	gettimeofday(&start, NULL);
	
	//filling array with some constant value
	std::fill_n(ar, array_size, 0);

	//Launch a group of threads
	for (int i=0; i< nr_threads; ++i) {
		
		th.push_back(std::thread(increment,i,ar));
	}

	//Join the threads with the main thread
	for (auto &t : th) {
		t.join();
	}

	//to print the array
	//print(ar);

	//ending the clock
	gettimeofday(&end, NULL);

	float delta = ((end.tv_sec  - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
	std::cout<<delta;	

	return 0;

}
