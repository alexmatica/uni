#include <iostream>
#include <vector>
#include <mpi.h>

using namespace std;


int procId, nrProcs;


struct ChildData{
	int size;
	int procsLeft;

	ChildData(int s, int p){
		size = s;
		procsLeft = p;
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

int partition(vector<int> &v, int left, int right){
	int pivot = v[right];
	int i = left;

	for (int j=left; j<right; j++)
		if (v[j] <= pivot){
			if (i != j){
				int aux = v[i];
				v[i] = v[j];
				v[j] = aux;
			}
			i++;
		}

	if (v[right] < v[i]){
		v[right] = v[i];
		v[i] = pivot;
	}

	return i;
}

void quick_sort(vector<int> &v, int left, int right, int procsLeft){
	if (left == right)
		return;

	int piv = partition(v, left, right);
	if (piv == left){
		quick_sort(v, left + 1, right, procsLeft);
		return;
	}
	if (piv == right){
		quick_sort(v, left, right - 1, procsLeft);
		return;
	}

	if (procsLeft > 1){
		int child = procId + procsLeft/2 + procsLeft%2;
		int size = right - piv;

		ChildData cd(size, procsLeft/2);
		MPI_Send(&cd, 2, MPI_INT, child, 1, MPI_COMM_WORLD);
		MPI_Send(v.data() + piv + 1, size, MPI_INT, child, 2, MPI_COMM_WORLD);

		quick_sort(v, left, piv - 1, procsLeft/2 + procsLeft%2);

		MPI_Status status;
		// cout << "Waiting for recv...\n";
		MPI_Recv(v.data() + piv + 1, size, MPI_INT, child, 3, MPI_COMM_WORLD, &status);
		// cout << "Recv done...\n";
	} else{
		quick_sort(v, left, piv - 1, 1);
		quick_sort(v, piv + 1, right - 1, 1);
	}

}

void quick_sort_worker(){
	MPI_Status status;

	ChildData cd(0,0);

	//	Each child is targetet exactly once, so we can find the parent from "status"
	MPI_Recv(&cd, 2, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);

	//	Notified by the main process that there is no more work to be done
	if (cd.size == -1)
		return;

	int parent = status.MPI_SOURCE;
	vector<int> v(cd.size);
	MPI_Recv(v.data(), cd.size, MPI_INT, parent, 2, MPI_COMM_WORLD, &status);

	quick_sort(v, 0, v.size() - 1, cd.procsLeft);

	MPI_Send(v.data(), cd.size, MPI_INT, parent, 3, MPI_COMM_WORLD);
}

int main(int argc, char **argv){
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &procId);
	MPI_Comm_size(MPI_COMM_WORLD, &nrProcs);

	if (!procId){
		if (argc != 2){
			cout<<"Usage: mpirun [-n PROCESSES] <program.out> <vector_size>\n";
			return 1;
		}

		int n = atoi(argv[1]);
		vector<int> v = generate(n);
		cout << "Generated "; printVector(v);

		quick_sort(v, 0, v.size() - 1, nrProcs);

		// if process 0 continuously follows the first two conditions in the function,
		// no data will get to the children and they will wait, so we notify them in the end 
		ChildData cd(-1, -1);
		for (int i=1; i < nrProcs; i++)
			MPI_Send(&cd, 2, MPI_INT, i, 1, MPI_COMM_WORLD);

		printVector(v);
	} else {
		quick_sort_worker();
	}

	MPI_Finalize();
	return 0;
}