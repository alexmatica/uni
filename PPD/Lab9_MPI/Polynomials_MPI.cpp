#include <iostream>
#include <mpi.h>
#include <time.h>
#include <stdint.h>
#include <vector>

using namespace std;

void productWorker(){
    MPI_Status status;

    int n, begin, end;

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    vector<int> a(n), b(n);

    MPI_Bcast(a.data(), n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(b.data(), n, MPI_INT, 0, MPI_COMM_WORLD);
    
    MPI_Recv(&begin, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&end, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);

    vector<int> s(2*n, 0);

    // for (int i=0; i<n; i++)
    //     cout << a[i] << " ";
    // cout << "\n";
    for (int i=begin; i<end; i++)
        for (int j=0; j<n; j++)
            s[i+j] += a[i]*b[j];

    MPI_Send(s.data(), 2*n, MPI_INT, 0, 3, MPI_COMM_WORLD);

}

vector<int> productMaster(vector <int> &a, vector<int> &b, int nrProcs){
    int n = a.size();

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(a.data(), n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(b.data(), n, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i=1; i<nrProcs; i++){
        int begin = (i*n) / nrProcs;
        int end = ((i+1)*n) / nrProcs;
        
        MPI_Send(&begin, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
        MPI_Send(&end, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
    }

    vector<int> res (2*n, 0);
    for (int i=0; i < n/nrProcs; i++)
        for (int j=0; j<n; j++)
            res[i+j] += a[i]*b[j];

    MPI_Status status;
    for (int i=1; i<nrProcs; i++){
        vector<int> slaveVector (2*n);
        MPI_Recv(slaveVector.data(), 2*n, MPI_INT, i, 3, MPI_COMM_WORLD, &status);
        for (int k=0; k<2*n; k++)
            res[k] += slaveVector[k];
    }

    return res;
}

void generate(vector<int>& v, size_t n)
{
    v.reserve(n);
    for(size_t i=0 ; i<n ; ++i) {
        // v.push_back(rand());
        v.push_back(i+1);
    }
}

int main(int argc, char** argv)
{
    MPI_Init(0, 0);
    int me;
    int nrProcs;
    MPI_Comm_size(MPI_COMM_WORLD, &nrProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);

    if (me == 0){

        unsigned n;
        if(argc != 2 || 1!=sscanf(argv[1], "%u", &n) ){
            fprintf(stderr, "usage: <prog_name> <n> (number of elements in a polynomial - i.e. polynomial of degree n-1)\n");
            return 1;
        }
        
        cout << "Nr. of processes: " << nrProcs << "\n";

        vector<int> a, b;
        vector<int> res(2*n);
        generate(a, n);
        generate(b, n);
        for (int i=a.size()-1; i >= 0; i--)
            cout<<a[i]<<" ";
        cout << "\n";

        for (int i=b.size()-1; i >=0 ; i--)
            cout<<b[i]<<" ";
        cout << "\n";
        
        struct timespec beginTime;
        clock_gettime(CLOCK_REALTIME, &beginTime);
        res = productMaster(a, b, nrProcs);
        struct timespec endTime;
        clock_gettime(CLOCK_REALTIME, &endTime);

        int64_t const elapsedNs = (int64_t(endTime.tv_sec) - int64_t(beginTime.tv_sec))*1000000000LL
            + int64_t(endTime.tv_nsec) - int64_t(beginTime.tv_nsec);

        cout << "Elapsed time=" <<  (elapsedNs/1000000) <<"ms\n";
        for (int i = 2*a.size() - 2; i>=0; i--)
            cout<<res[i]<< " ";
        cout << "\n";
    } 
    else{
        productWorker();
    }
    
    MPI_Finalize();
}