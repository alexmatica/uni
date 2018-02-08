#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

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

class ThreadPool{

public:
	ThreadPool(int n){
		sum = 0;
		for (int i=0; i<n; i++){
			auto task = [this](){this->run();};
			threads.emplace_back(task);
		}
	}

	void enqueue(int x){
		unique_lock<mutex> lck(mtx);
		work_queue.push(x);
	}

	int close(){
		unique_lock<mutex> lck(mtx);
		while(!work_queue.empty()){
			stop.wait(lck);
		}

		for (int i=0; i<threads.size(); i++)
			threads[i].join();
		return sum;
	}
private:
	condition_variable stop;
	vector<thread> threads;
	queue<int> work_queue;
	mutex mtx;
	atomic_int sum;
	bool closed;

	void run(){
		while(true){
			int value;
			{
				unique_lock<mutex> lck(mtx);
				if (!work_queue.empty()){
					value = work_queue.front();
					work_queue.pop();
				} else if (!closed){
					closed = true;
					stop.notify_one();
					return;
				} else if (closed){
					return;
				}
			}

			sum += value;
		}
	}
};


int main(int argc, char **argv){

	int n = 10;
	vector<int> a = generate(n);
	printVector(a);

	ThreadPool tp(1);
	for (int i=0; i<a.size(); i++)
		tp.enqueue(a[i]);
	int res = tp.close();
	cout << "The sum is " << res << "\n";
	return 0;
}