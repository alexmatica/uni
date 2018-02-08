#include <iostream>
#include <mpi.h>
#include <vector>

using namespace std;

int procId, nrProcs;


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
		MPI_Status status;

		MPI_Send(&size, 1, MPI_INT, child, 1, MPI_COMM_WORLD);
		MPI_Send(v.data() + piv + 1, size, MPI_INT, child, 2, MPI_COMM_WORLD);

		quick_sort(v, left, piv - 1, procsLeft/2 + procsLeft%2);

		// cout << procId << " waiting for data...\n";
		MPI_Recv(v.data() + piv + 1, size, MPI_INT, child, 3, MPI_COMM_WORLD, &status);
		// cout << procId << " received data...\n";
	} else {
		quick_sort(v, left, piv - 1, 1);
		quick_sort(v, piv + 1, right, 1);
	}
}

void quick_sort_worker(){

	int child = 0, procsLeft = nrProcs, parent = 0;

	while (child < procId){
		int step = procsLeft/2 + procsLeft%2;
		if (child + step <= procId){
			parent = child;
			procsLeft /= 2;
			child += step;
		} else{
			procsLeft = step;
		}
	}

	MPI_Status status;
	int size;
	MPI_Recv(&size, 1, MPI_INT, parent, 1, MPI_COMM_WORLD, &status);
	vector<int> v(size);
	MPI_Recv(v.data(), size, MPI_INT, parent, 2, MPI_COMM_WORLD, &status);

	quick_sort(v, 0, size - 1, procsLeft);

	MPI_Send(v.data(), size, MPI_INT, parent, 3, MPI_COMM_WORLD);
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
		printVector(v);
	} else {
		quick_sort_worker();
	}


	MPI_Finalize();
	return 0;
}