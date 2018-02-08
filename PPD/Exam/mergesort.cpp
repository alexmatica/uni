#include <iostream>
#include <vector>
#include <thread>
#include <time.h>
#include <future>
#include <algorithm>

using namespace std;

void printVector(vector<int> a){
	for (int i=0; i<a.size(); i++)
		cout << a[i] << " ";
	cout << "\n";
}

vector<int> generate(int n){
	srand(time(0));
	vector<int> v(n);

	for (int i=0; i<n; i++)
		v[i] = rand() % 10;

	return v;
}

vector<int> merge(vector<int> a, vector<int> b){
	int i = 0, j = 0;
	vector<int> v(a.size() + b.size());
	
	a.push_back(2e9);
	b.push_back(2e9);

	while (i + j < v.size())
		if (a[i] < b[j])
			v[i+j] = a[i++];
		else
			v[i+j] = b[j++];

	cout << "Merged: ";
	printVector(v);
	return v;
}


vector<int> mergeSortWorker(vector<int> input, int threadsLeft){

	cout << "Input :";
	printVector(input);
	cout << "Threads left: " << threadsLeft << "\n";

	// Trivial case
	if (input.size() == 1)
		return input;

	int middle = input.size() / 2;
	vector<int>::const_iterator b = input.begin();
	vector<int>::const_iterator m = input.begin() + middle;
	vector<int>::const_iterator e = input.end();

	vector<int> v_left(b, m);
	vector<int> v_right(m, e);

	if (threadsLeft >= 2){
		future<vector<int>> left = async(mergeSortWorker, v_left, threadsLeft / 2);
		future<vector<int>> right = async(mergeSortWorker, v_right, threadsLeft / 2);
		v_left = left.get();
		v_right = right.get();
	} else {
		v_left = mergeSortWorker(v_left, 1);
		v_right = mergeSortWorker(v_right, 1);
	}

	return merge(v_left, v_right);
}


int main(int argc, char **argv){

	if(argc != 3){
		cout<<"Usage: <program.out> <vector_size> <number_of_threads>\n";
		return 1;
	}

	int n, nrThreads;
	n = atoi(argv[1]);
	nrThreads = atoi(argv[2]);

	vector<int> v = generate(n);
	printVector(v);

	vector<int> res = mergeSortWorker(v, nrThreads);
	printVector(res);

	return 0;
}