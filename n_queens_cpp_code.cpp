/* For now we are assuming we can get all the data into our array.
   Later we should change it to accomodate large files

*/

#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>

using namespace std;

string path = "/home/irfan536/programs/N-queens/refining_n_queens/";

const int n = 2;

const int t_id = 5;

void read_file(ifstream &myfile);

void process(string in_array[], int len_array, int target_char);

int diagonal_check(string str, int target_char);

void write_file(string out_path, string out_array[], int count);


//reading a file 1000 lines at a time and processing it using process function

//next step is to add threads for parallel execution
void read_file(ifstream &myfile) {

	string in_array[1000];
	if (myfile.is_open()) {
		int i = 0;
		while ( getline (myfile,in_array[i]) ) {
			i++;
		}
		myfile.close();
		
		//making it a small char alphabet
		int target_char = t_id +96 ;
		
		process(in_array,i,target_char);

		/* to print the array
		for (int j=0; j<i ;j++)
			cout<<in_array[j]<<endl;*/
	
	}


	else cout << "Unable to open file"; 


}


//processing the given chunk of array and writing it to desired output file.
void process(string in_array[], int len_array, int target_char) {

	string out_array[1000];
	int count = 0;

	for(int i = 0 ; i < len_array ; i++) {

		int result = diagonal_check(in_array[i], target_char);

		//later check what is the maximum size is generally coming out, so that we can be efficient when the program gets large
		
		if (result == 1 ) {
			cout << " very good" << endl;
			out_array[count] = char(target_char) + in_array[i];
			cout<< out_array[count]<<endl;
			count ++;
		}
		
	}

	string out_path = "/home/irfan536/programs/N-queens/n_queens_cpp_code/2_file/file2^3^" + to_string(t_id) + ".txt";
	//cout<< out_path<< endl;
	write_file(out_path, out_array, count);
}

int diagonal_check( string str, int target_char) {

	for (int j = 0; j < n ; j++) {
		//cout<< int(str[j]) << " " << target_char << endl;
		int c = abs(int(str[j]) - target_char);
		if (c == 0 or c == j + 1) {
			cout << "not good "<<endl;
			return 0;
		}
		
	}
		return 1;
	

}

void write_file(string out_path, string out_array[], int count) {
	ofstream my_out_file (out_path);
	cout<< count;
	for (int i = 0 ; i < count ; i ++) {
		cout<< out_array[i]<<" "<< count;
		my_out_file<< out_array[i];
	} 
	
}


int main () {

	ifstream myfile (path + "2_file/file2^3.txt");
	
	read_file(myfile);

	return 0;
}
