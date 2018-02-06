#include <iostream>
#include <vector>
#include <mpi.h>

using namespace std;

int procId, nrProcs;
int count_sol;

#define START_VALUE_TAG 1
#define SLAVE_SOL_COUNT 2

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

void permute(vector<int> &a, vector<int> &used, int count, int N){
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
			permute(a, used, count + 1, N);
			used[i] = 0;
		}
	}
}

void master(int N){

	MPI_Status status;
	MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
	for (int i = 2; i <= nrProcs; i++){
		MPI_Send(&i, 1, MPI_INT, i-1, START_VALUE_TAG, MPI_COMM_WORLD);
	}

	for (int i=1; i<=N; i += nrProcs){
		vector<int> a(N);
		a[0] = i;
		vector<int> used(N+1, 0);
		used[i] = 1;
		permute(a, used, 1, N);
	}

	int slave_count;
	for (int i=1; i < nrProcs; i++){
		MPI_Recv(&slave_count, 1, MPI_INT, i, SLAVE_SOL_COUNT, MPI_COMM_WORLD, &status);
		count_sol += slave_count;
	}
}

void slave(){
	MPI_Status status;
	int N, start;
	MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Recv(&start, 1, MPI_INT, 0, START_VALUE_TAG, MPI_COMM_WORLD, &status);

	cout << "Process " << procId << " received N = " << N << " and start = " << start << "\n" << flush;

	for (int i=start; i<=N; i+=nrProcs){
		vector<int> a(N);
		a[0] = i;
		vector<int> used(N+1, 0);
		used[i] = 1;
		permute(a, used, 1, N);
	}

	MPI_Send(&count_sol, 1, MPI_INT, 0, SLAVE_SOL_COUNT, MPI_COMM_WORLD);
}

int main(int argc, char **argv){
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &procId);
	MPI_Comm_size(MPI_COMM_WORLD, &nrProcs);

	if (!procId){
		if (argc != 2){
			cout << "Usage: mpirun [-n PROCESSES] <program.out> <N>\n";
			return 0;
		}
		master(atoi(argv[1]));
		cout << "Permutations which satisfy pred: " << count_sol << "\n";
	} else{
		slave();
	}

	MPI_Finalize();
	return 0;
}