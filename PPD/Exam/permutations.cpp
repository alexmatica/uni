#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <queue>
#include <atomic>

using namespace std;

#define THREADS 4

queue<vector<int>> workingQueue;
atomic_ulong count;
mutex mtx;
int N;


bool exists(int x, vector<int> const& v){
	for (int i=0; i<v.size(); i++)
		if (v[i] == x)
			return true;
	return false;
}

void printVector(vector<int> const& v){
	for (int i=0; i<N; i++)
			cout << v[i] << " ";
		cout << "\n";
}

bool pred(vector<int> const& v){
	return true;
}


void worker(int id){

	vector<int> tmp_sol;

	while (workingQueue.size() > 0){
		mtx.lock();
		if (workingQueue.size() > 0){
			tmp_sol = workingQueue.front();
			workingQueue.pop();
			mtx.unlock();

		} else{
			mtx.unlock();
			break;
		}

		if (tmp_sol.size() == N && pred(tmp_sol)){
			count++;
			continue;
		}

		for (int i=1; i<=N; i++)
			if (!exists(i, tmp_sol)){
				tmp_sol.push_back(i);
				mtx.lock();
				workingQueue.push(tmp_sol);
				mtx.unlock();
				tmp_sol.pop_back();
			}
	}
}

int main(int argc, char **argv){

	if (argc < 2){
		cout << "Usage: <program.out> <N>\n";
		return 1;
	}

	N = atoi(argv[1]);

	for (int i=1; i<=N; i++){
		vector<int> v;
		v.push_back(i);
		workingQueue.push(v);
	}

	vector<thread> threads;
	for (int i=0; i < THREADS; i++){
		threads.push_back(thread(worker, i));	
	}
	for (int i=0; i < THREADS; i++){
		threads[i].join();
	}

	cout << "Permutations which satisfy pred: " << count << "\n" << flush;
	return 0;
}