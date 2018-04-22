#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
static  std::mutex barrier;

std::vector<int> bounds(int parts, int mem) {
    std::vector<int>bnd;
    int delta = mem / parts;
    int reminder = mem % parts;
    int N1 = 0, N2 = 0;
    bnd.push_back(N1);
    for (int i = 0; i < parts; ++i) {
        N2 = N1 + delta;
        if (i == parts - 1)
            N2 += reminder;
        bnd.push_back(N2);
        N1 = N2;
    }
    return bnd;
}

//avoiding race conditions using atomic operations
void dot_product_using_atomic_ops(const std::vector<int> &v1, const std::vector<int> &v2, std::atomic<int> &result, int L, int R){
	int partial_sum = 0;
	for(int i = L; i < R; ++i){
		partial_sum += v1[i] * v2[i];
	}
	result += partial_sum;
}


//avoiding race conditions using mutex
void dot_product_using_mutex (const std::vector<int> &v1, const std::vector<int> &v2, int &result, int L, int R){
	int partial_sum = 0;
	for(int i = L; i<R; ++i) {
		partial_sum += v1[i] * v2[i];
	}
	std::lock_guard<std::mutex> block_threads_unti_finish_this_job(barrier);
	result += partial_sum;
}


int main() {
	int nr_elements = 100000;
	int nr_threads = 2;
	int result_mutex = 0;
	std::atomic<int> result_atomic(0);
	std::vector<std::thread> threads;

	//Fill two vectors with some constant values for a quick verification
	std::vector<int> v1(nr_elements, 1), v2(nr_elements, 2);
	
	//Split nr_elements into nr_threads part
	std::vector<int> limits = bounds(nr_threads, nr_elements);

	//Launch nr_threads threads:
	for (int i = 0; i< nr_threads; i++) {
		//call using mutex
		//threads.push_back(std::thread(dot_product_using_mutex, std::ref(v1), std::ref(v2), std::ref(result_mutex), limits[i], limits[i+1]));

		//call using atomic operations
		threads.push_back(std::thread(dot_product_using_atomic_ops, std::ref(v1), std::ref(v2), std::ref(result_atomic), limits[i], limits[i+1]));		
	}
	//Join the threads with the main thread
	for (auto &t : threads) {
		t.join();
	}

	//Print the result using mutex
	//std::cout<<result_atomic<<std::endl;


	//Print the result using atomic operations
	std::cout<<result_atomic<<std::endl;

	return 0;
}
