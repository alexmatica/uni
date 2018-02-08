#include <iostream>
#include <vector>
#include <future>

using namespace std;

int N;

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

int worker(vector<int> a, vector<int> b, int procsLeft){

	if (a.size() == 1)
		return a[0]*b[0];

	if (procsLeft >= 2){
		vector<int>::iterator mid_a = a.begin() + a.size()/2;
		vector<int>::iterator mid_b = b.begin() + b.size()/2;
		vector<int> a_left(a.begin(), mid_a);
		vector<int> a_right(mid_a, a.end());
		vector<int> b_left(b.begin(), mid_b);
		vector<int> b_right(mid_b, b.end());

		future<int> f_left = async(worker, a_left, b_left, procsLeft/2);
		future<int> f_right = async(worker, a_right, b_right, procsLeft - procsLeft/2);

		return f_left.get() + f_right.get();
	} else{
		int s = 0;
		for (int i=0; i<a.size(); i++)
			s += a[i]*b[i];

		return s;
	}

}

int main(int argc, char **argv){

	if(argc != 3){
		cout << "Usage: <program.out> <vector_size> <threads>\n";
		return 1;
	}

	N = atoi(argv[1]);
	int threads = atoi(argv[2]);

	vector<int> a = generate(N);
	vector<int> b = generate(N);
	printVector(a);
	printVector(b);

	cout << "The product is " << worker(a, b, threads) << "\n";
	return 0;
}