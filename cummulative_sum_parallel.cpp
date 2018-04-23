#include <iostream>
#include <thread>
#include <vector>
#include <sys/time.h>
#include<mutex>

static  std::mutex barrier;

const int array_size = 100000;
int nr_threads = 16;

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
void sum(int i, int ar[], int &result) {
	int intial_b;
	int final_b;
	int local_sum = 0;
	chunk(i, intial_b,final_b);
	for (int pos = intial_b; pos<= final_b; pos++){
		local_sum = local_sum + ar[pos];		
	}
	//std::cout<<intial_b<<" "<<final_b<<std::endl;
	result = local_sum + result;
	std::lock_guard<std::mutex> block_threads_unti_finish_this_job(barrier);
	}


int main() {
	int ar[array_size];
	int intial_b;
	int final_b;
	int result = 0;
	struct timeval start, end;
	std::vector<std::thread> th;


	//starting the clock
	gettimeofday(&start, NULL);
	
	//filling array with some constant value
	std::fill_n(ar, array_size, 2);

	//Launch a group of threads
	for (int i=0; i< nr_threads; ++i) {
		
		th.push_back(std::thread(sum,i,ar,std::ref(result)));
	}

	//Join the threads with the main thread
	for (auto &t : th) {
		t.join();
	}

	//to print the array
	//print(ar);
	std::cout<<result<<std::endl;
	//ending the clock
	gettimeofday(&end, NULL);

	float delta = ((end.tv_sec  - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
	std::cout<<delta;	

	return 0;

}
