#include <iostream>
#include <thread>
#include <vector>
#include <future>
#include <mutex>

using namespace std;

int n;

struct Node{
	int value;
	int inherit;

	Node(int v, int i){
		value = v;
		inherit = i;
	}
};


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

int bottomsUpWorker(Node *level, int start, int size){
	if (start + 1 < size)
		return level[start].value + level[start+1].value;
	return level[start].value;
}

void topDownWorker(int id, int level, vector<Node> *tree){
	tree[level-1][2*id].inherit = tree[level][id].inherit;

	if (2*id + 1 < tree[level-1].size())
		tree[level-1][2*id + 1].inherit = tree[level-1][2*id].inherit + tree[level-1][2*id].value;

}

void prefixSum(vector<int> &input){
	vector<vector<Node>> tree;
	int level = 1;

	vector<Node> leaves;
	for (int i=0; i<input.size(); i++){
		Node n(input[i], 0);
		leaves.push_back(n);
	}
	tree.push_back(leaves);

	int size = input.size();
	while (size > 1){
		vector<future<int>> results;
		vector<Node> nextLev;
		int nextSize = (size + 1) / 2;	//add an extra node if there is a single value remaining at the end
		for (int i=0; i < nextSize; i++){
			Node n(0, 0);
			nextLev.push_back(n);
		}
		tree.push_back(nextLev);

		for (int i=0; i<nextSize; i++)
			results.push_back(async(bottomsUpWorker, tree[level-1].data(), i*2, tree[level-1].size()));
		for (int i=0; i<nextSize; i++)
			tree[level][i].value = results[i].get();

		size = nextSize;
		level ++;
	}

	
	// for (int i=0; i < tree.size(); i++){
	// 	cout << "Level " << i << "\n";
	// 	for (int j=0; j<tree[i].size(); j++)
	// 		cout << "Value = " << tree[i][j].value << ", Inherit = " << tree[i][j].inherit << "\n";
	// }

	level--;
	while (size < input.size()){
		vector<thread> workers;

		for (int i=0; i<tree[level].size(); i++)
			workers.push_back(thread(topDownWorker, i, level, tree.data()));

		for (int i=0; i<tree[level].size(); i++)
			workers[i].join();

		level--;
		size *= 2;
	}

	for (int i=1; i<tree[0].size(); i++)
		cout << tree[0][i].inherit << " ";
	cout << tree[0][tree[0].size() - 1].inherit + tree[0][tree[0].size() - 1].value << "\n";
}

int main(int argc, char **argv){

	if(argc != 2){
		cout<<"Usage: <program.out> <vector_size>\n";
		return 1;
	}
	n = atoi(argv[1]);

	vector<int> v = generate(n);
	cout << "Generated vector: "; printVector(v);

	prefixSum(v);
	// printVector(v);
	return 0;
}