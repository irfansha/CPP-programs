/* This program calculates the sum of all elements in the array both parallely and serially.
   
 
   Compile with:
	g++ -std=c++11 -pthread parallel_sum.cpp -o parallel_sum

   Usage :
	Specify number of threads you want to run by changing the value of "nr_threads".
   
	Specify the array size by changing the value of "array_size".
	
	Specify the srand "seed" to change the random values in the array

   Note:
	The array values are randomly generated for sake of simplicity

   Observations:
	-> The array size has to be significantly larger to overcome the overhead by creation of multiple threads.
	-> To get maximum performace the number of threads need to be equal to logical cores on the system i.e. 8 in this PC.
	-> But it is also important to divide the array boundaries equally so that all other threads should not wait for one long running thread.
 */

#include <iostream>
#include <thread>
#include <vector>
#include <sys/time.h>
#include<mutex>

static  std::mutex barrier;

const int array_size = 1000000;
int nr_threads = 8;

//Approximate size of array chunk to be worked on by each thread
int chunk_size = int(array_size/nr_threads);

// to print the array
void print(int ar[]) {
	for (int i = 0; i< array_size ;i++) {
		std::cout<<ar[i]<<'\t';
	}
	std::cout<<'\n';
}

//finding boundaries for a specific thread, last thread is given the remaining chunk.
void chunk(int i,int &intial_b,int &final_b) {
	intial_b = i*chunk_size;
	final_b = intial_b + chunk_size -1;
	if (i == nr_threads-1){
		final_b = array_size-1;
	}	
}

void serial_sum(int ar[], int &serial_result) {
	for (int i=0; i< array_size ;i++) {
		serial_result += ar[i];	
	}
}

//adding values within the boundaries of a given thread and adding the local_sum to the global result.
void parallel_sum(int i, int ar[], int &result) {
	int intial_b, final_b;
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
	int parallel_result = 0;
	int serial_result = 0;

	struct timeval start, end;
	std::vector<std::thread> th;

	//filling array with random values with 1 as seed for srand function
	srand(1);
	for (int i=0; i< array_size; i++) {
		ar[i] = rand()%100;
	}

	//starting the clock for parallel approach
	gettimeofday(&start, NULL);
	
	//to print the array
	//print(ar);

	//Launch a group of threads
	for (int i=0; i< nr_threads; ++i) {		
		th.push_back(std::thread(parallel_sum,i,ar,std::ref(parallel_result)));
	}

	//Join the threads with the main thread
	for (auto &t : th) {
		t.join();
	}

	//ending the clock for parallel approach
	gettimeofday(&end, NULL);

	std::cout<<"the parallel sum is: "<<parallel_result<<std::endl;

	float delta = ((end.tv_sec  - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
	std::cout<<"the time taken for parallel aprroach is: "<<delta<<std::endl;	

	//starting the clock for serial approach
	gettimeofday(&start, NULL);
	
	serial_sum(ar,std::ref(serial_result));	

	//ending the clock for serial approach
	gettimeofday(&end, NULL);

	std::cout<<"the serial sum is: "<<serial_result<<std::endl;

	delta = ((end.tv_sec  - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
	std::cout<<"the time taken for serial aprroach is: "<<delta<<std::endl;	

	return 0;

}
