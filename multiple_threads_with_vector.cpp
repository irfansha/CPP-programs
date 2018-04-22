#include <iostream>
#include <thread>
#include <vector>

void func(int tid) {
	std::cout << "Launched by thread" << tid << std::endl;
}

int main() {
	std::vector<std::thread> th;

	int nr_threads = 10;
	
	//Launch a group of threads
	for (int i=0; i< nr_threads; ++i) {
		th.push_back(std::thread(func,i));
	}

	//Join the threads with the main thread
	for (auto &t : th) {
		t.join();
	}

	return 0;

}
