
#include <iostream>
#include <thread>
#include <vector>
#include <sys/time.h>
#include <fstream>
#include <string>
#include <sys/stat.h>


using namespace std;

string path = "/home/irfan536/programs/N-queens/refining_n_queens/";

const int n = 2;

const int m = 3;

using namespace std;

void process(string in_array[], int len_array, int target_char);

void write_file(string out_path, string out_array[], int count);

int diagonal_check( string str, int target_char);


//next step is to add threads for parallel execution
int read_file(ifstream &myfile, string in_array[]) {
	
	if (myfile.is_open()) {
		int i = 0;
		while ( getline (myfile,in_array[i]) ) {
			i++;
		}
		myfile.close();
		
		return i;	
	}


	else {
		cout << "Unable to open file"; 
		return 0;
	}

}



//processing the given chunk of array and writing it to desired output file.
void process(string in_array[], int len_array, int t_id) {

	string out_array[6][1000];
	int count = 0;
	int target_char = t_id +96 ;
	for(int i = 0 ; i < len_array ; i++) {

		int result = diagonal_check(in_array[i], target_char);

		//later check what is the maximum size is generally coming out, so that we can be efficient when the program gets large
		
		if (result == 1 ) {
			//cout << " very good" << endl;
			out_array[t_id][count] = char(target_char) + in_array[i];
			cout<< out_array[count]<<endl;
			count ++;
		}
		
	}

	string out_path = "/home/irfan536/programs/N-queens/n_queens_cpp_code/" + to_string(n)+"_file/file"+to_string(n)+"^"+to_string(m)+"^" + to_string(target_char-96) + ".txt";
	//cout<< out_path<< endl;
	write_file(out_path, out_array, count, t_id);
}

int diagonal_check( string str, int target_char) {

	for (int j = 0; j < n ; j++) {
		//cout<< int(str[j]) << " " << target_char << endl;
		int c = abs(int(str[j]) - target_char);
		if (c == 0 or c == j + 1) {
			//cout << "not good "<<endl;
			return 0;
		}
		
	}
		return 1;
	

}


void write_file(string out_path, string out_array, int count, int t_id) {
	ofstream my_out_file (out_path);
	//cout<< count;
	for (int i = 0 ; i < count ; i ++) {
		// writing to the out file
		my_out_file<< out_array[t_id][i]<<endl;
	} 
	
}


int main() {

	int target_char;
	std::vector<std::thread> th;
	ifstream myfile (path + to_string(n)+"_file/file"+to_string(n)+"^"+to_string(m)+".txt");
	string in_array[1000];
	int len_array = 10;

	//Launch a group of threads
	for (int t_id=1; t_id< 7; ++t_id) {	
			
		//th.push_back(std::thread(parallel_sum,i,ar));
		th.push_back(std::thread(process,in_array,len_array,t_id));
	}

	//Join the threads with the main thread
	for (auto &t : th) {
		t.join();
	}

	return 0;

}
