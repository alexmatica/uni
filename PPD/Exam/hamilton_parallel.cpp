#include <iostream>
#include <vector>
#include <set>
#include <thread>
#include <time.h>
#include <future>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>

using namespace std;

int N;

class Graph
{
public:
    int V;    			
    set<int> *adj;  
    Graph(int V);
    ~Graph();
    Graph(const Graph& g);
 
    void addEdgeUndirected(int v, int w);
    void addEdgeDirected(int v, int w); 
    void greedyColoring();
};
 
Graph::Graph(int V){
	this->V = V;
	this->adj = new set<int>[V];
}

Graph::Graph(const Graph& g){
	this->V = g.V;
	this->adj = new set<int>[V];

	for(int i=0; i<g.V; i++)
		for (set<int>::iterator it = g.adj[i].begin(); it != g.adj[i].end(); ++it)
			this->adj[i].insert(*it);
}

Graph::~Graph(){
	delete [] adj;
}

void Graph::addEdgeUndirected(int v, int w){
    adj[v].insert(w);
    adj[w].insert(v); 
}

void Graph::addEdgeDirected(int v, int w){
	adj[v].insert(w);
}

Graph generate(int V, int fillPercent, bool directed){
	srand(time(NULL));

	Graph g{V};
	for (int i=0; i < V; i++)
		for (int j=0; j < V; j++)
			if (i != j && (rand() % 100) < fillPercent)
				if (directed)
					g.addEdgeDirected(i, j);
				else
					g.addEdgeUndirected(i, j);

	return g;
}

void printVector(vector<int> a){
	for (int i=0; i<a.size(); i++)
		cout << a[i] << " ";
	cout << "\n";
}

bool isSafe(int vertex, vector<int> path, const Graph &g){
	if (g.adj[path[path.size()-1]].find(vertex) == g.adj[path[path.size()-1]].end())
		return false;

	for (int i=0; i < N; i++)
		if (path[i] == vertex)
			return false;
	return true;
}	


bool hamilton_worker_naive(const Graph &g, vector<int> &path){

	if (path.size() == N){
		if (g.adj[path[N-1]].find(path[0]) != g.adj[path[N-1]].end()){
			// for (int i=0; i<N; i++)
			// 	cout << path[i] << " ";
			// cout << "\n";
			return true;
		}
	}

	for (int v=0; v<N; v++){
		if (v != path[0] && isSafe(v, path, g)){

			path.push_back(v);

			if (hamilton_worker_naive(g, path))
				return true;

			path.pop_back();
		}
	}

	return false;

}


vector<int> hamilton_worker_prepare(const Graph &g, int vertex){
	vector<int> path;
	path.push_back(vertex);
	// cout << "Path before worker: "; printVector(path);

	bool result = hamilton_worker_naive(g, path);
	if (result)
		return path;

	return vector<int>();
}


/*
	Assuming the graph is directed;
	If the graph is undirected, we set the first value to the first vertex and then we 
	start out workers from the next positions
*/
void hamilton_main_naive(const Graph &g){

	vector<future<vector<int>>> futures;
	for (int i=0; i<N; i++){
		futures.push_back(async(hamilton_worker_prepare, g, i));
	}

	int i = 0;
	for (; i<N; i++){
		vector<int> tmp = futures[i].get();
		if (tmp.size() != 0){
			cout << "Found a solution: "; printVector(tmp);
		}
	}
}

#define QUEUE_MAX 10000

template <typename T>
class Queue{
public:
	void enqueue(T value){
		unique_lock<mutex> lck(mtx);
		while (q.size() >= QUEUE_MAX){
			condFull.wait(lck);
		}
		q.push(value);
		condEmpty.notify_one();
	}

	void close(){
		unique_lock<mutex> lck(mtx);
		isClosed = true;
		condEmpty.notify_all();
	}

	bool dequeue(T &value){
		unique_lock<mutex> lck(mtx);
		while(true){
			if(!q.empty()){
				value = q.front();
				q.pop();
				condFull.notify_one();
				return true;
			}
			if (isClosed)
				return false;
			condEmpty.wait(lck);
		}
	}

	bool getClosed(){
		unique_lock<mutex> lck(mtx);
		return this->isClosed;
	}

private:
	bool isClosed;
	queue<T> q;
	condition_variable condFull;
	condition_variable condEmpty;
	mutex mtx;
};

Queue<vector<int>> paths_queue;
atomic_bool job_done;
void main_producer(){

	job_done = false;
	for(int i=0; i<N; i++){
		vector<int> v;
		v.push_back(i);
		paths_queue.enqueue(v);
	}

	paths_queue.close();
	cout << "Main producer done queueing...\n";
}


void consumer_producer(const Graph& g){

	vector<int> path;
	while(paths_queue.dequeue(path)){

		// cout << "Dequeued "; printVector(path);
		if (path.size() == N){
			if(g.adj[path[N-1]].find(path[0]) != g.adj[path[N-1]].end()){
				//	Atomic check for the closing state; there might be another thread which reached 
				//	the condition above, but we only want one solution
				if(!job_done){
					job_done = true;
					cout << "Reached a solution...\n";
					printVector(path);
				}
				return;
			}
		}

		for (int v=0; v<N; v++){
			if (isSafe(v, path, g)){
				path.push_back(v);
				paths_queue.enqueue(path);
				// cout << "Added "; printVector(path); cout <<" to the queue\n";
				path.pop_back();
			}
		}
	}
}

#define THREADS 4

int main(int argc, char **argv){
	if (argc != 2){
		cout << "Usage: program.out <nr_of_vertices>\n";
		return 1;
	}

	N = atoi(argv[1]);
	Graph g = generate(N, 100, true);

	for (int i=0; i<N; i++){
		cout << "Vertex " << i << ": \n\t";
		for (set<int>::iterator it = g.adj[i].begin(); it != g.adj[i].end(); ++it)
			cout << *it << " ";
		cout << "\n\n";
	} 

	// hamilton_main_naive(g);

	vector<thread> threads;
	main_producer();

	for(int i=0; i < THREADS; i++)
		threads.push_back(thread(consumer_producer, g));
	for(int i=0; i < THREADS; i++)
		threads[i].join();

	return 0;
}
