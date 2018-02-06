#include <iostream>
#include <thread>
#include <vector>
#include <cstring>
#include <cmath>

using namespace std;

int nrThreads;
int N;
bool *marked;


void printVector(vector<int> a){
	for (int i=0; i<a.size(); i++)
		cout << a[i] << " ";
	cout << "\n";
}

void sieve_full(int n, bool *mrk){
	for (int p=2; p*p <= n; p++){
		// cout << "p = " << p << "\n";
		if (!mrk[p]){
			// cout << "Prime -> " << p << "\n";
			for (int i=p*2; i<=n; i+=p)
				mrk[i] = true;
		}
	}
}

void sieve_worker(int x){
	if (!marked[x]){
		for (int i=x*2; i<=N; i+=x)
			marked[i] = true;
	}
}

void worker(vector<int> const& primes, int id){

	for (int i = id; i<primes.size(); i += nrThreads){
		// cout << "Thread " << id << " starting sieve for " << primes[i] << "\n";
		sieve_worker(primes[i]);
	}
}


int main(int argc, char **argv){

	if (argc != 3){
		cout << "Usage: <program.out> <N> <nr_threads = {1,...,8}>\n";
		return 1;
	}

	N = atoi(argv[1]);
	nrThreads = atoi(argv[2]);

	if (nrThreads > 8 || nrThreads <= 0){
		cout << "Limiting thread number to 8\n";
		nrThreads = 8;
	}

	marked = new bool[N+1];
	memset(marked, false, (N + 1) * sizeof(bool));

	int sqr_n = sqrt(N);
	bool *init_marked = new bool[sqr_n + 1];
	memset(init_marked, false, (sqr_n + 1) * sizeof(bool));
	sieve_full(sqr_n, init_marked);
	cout << "Main full sieve done!\n";

	vector<int> primes;
	for (int i=2; i<=sqr_n; i++)
		if (!init_marked[i])
			primes.push_back(i);
	printVector(primes);

	vector<thread> threads;
	for (int i=0; i<nrThreads; i++){
		threads.push_back(thread(worker, primes, i));
	}
	for (int i=0; i<nrThreads; i++){
		threads[i].join();
	}

	for (int i=2; i<=N; i++)
		if (!marked[i])
			cout << i << " ";
	cout << "\n";

	delete(init_marked);
	delete(marked);
	return 0;
}