#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <cstring>

using namespace std;

#define MAX_THREADS 8

int N;
atomic_ulong count_sol;

bool pred(vector<int> const& v){
	// if (v[0] & 1)
	// 	return true;
	// return false;
	return true;
}

void printVector(vector<int> a){
	for (int i=0; i<a.size(); i++)
		cout << a[i] << " ";
	cout << "\n";
}

void permute(vector<int> &a, vector<int> &used, int count){
	if (count == N){
		if (pred(a)){
			count_sol++;
			// printVector(a);
		}
		return;
	}

	for (int i=1; i<=N; i++){
		if (!used[i]){
			a[count] = i;
			used[i] = 1;
			permute(a, used, count + 1);
			used[i] = 0;
		}
	}
}


void worker(int startingValue, int increment){

	// cout << "Thread [" << startingValue << "] started...\n" << flush;

	for (int i=startingValue; i<= N; i += increment){
		vector<int> a(N);
		a[0] = i;
		vector<int> used(N+1, 0);
		used[i] = 1;
		permute(a, used, 1);
	}
}



int main(int argc, char **argv){
	if (argc < 2){
		cout << "Usage: <program.out> <N>\n";
		return 1;
	}

	N = atoi(argv[1]);

	int thCount;

	if (N > MAX_THREADS)
		thCount = MAX_THREADS;
	else
		thCount = N;

	vector<thread> threads;
	for (int i=1; i<=thCount; i++){
		threads.push_back(thread(worker, i, thCount));
	}

	for (int i=0; i < thCount; i++){ 
		threads[i].join();
	}

	cout << "Permutations which satisfy pred: " << count_sol << "\n";
	return 0;
}