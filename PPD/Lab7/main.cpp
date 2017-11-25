#include <iostream>
#include <thread>
#include <future>
#include <fstream>
#include <string.h>
#include <vector>
#include <chrono>

typedef std::chrono::high_resolution_clock Clock;
#define T 4

int readN(const char *fName){
    std::ifstream fin;
    fin.open(fName);
    int n;
    fin >> n;
    fin.close();
    return n;
}

void generateArray(int *a, int n){
    for (int i = 0; i < n; i++)
        a[i] = rand() % 10 + 1;
}

void printArray(int *a, int n){
    for (int i=0; i < n; i++){
        std::cout << a[i] << " ";
    }
    std::cout << '\n';
}

void sumLinear(int *a, int n, int *r){
    r[0] = a[0];
    for (int i=1; i < n; i++){
        r[i] = r[i-1] + a[i];
    }
}

void sumWorker(int *a, int n, int *r, bool *done, int tid){
    for (int i=tid; i < n; i+=T){
        int j = i-1;
        while (!done[j]){
            if (j == 0)
                break;
            j--;
        }
        r[i] = r[j];
        for (int k = j + 1; k <= i; k++)
            r[i] += a[k];
        done[i] = true;
    }
}

int main() {
    srand(time(NULL));
    int n = readN("/home/fmatica/Uni/uni/PPD/Lab7/n.in");

    int *a = new int[n];
    int *r = new int[n];
    bool *done = new bool[n];
    memset(done, 0, n*sizeof(bool));

    generateArray(a, n);
//    printArray(a, n);

    auto start = Clock::now();
    sumLinear(a, n, r);
    auto end = Clock::now();
    std::cout << "Linear sum took: ";
    std::cout << (std::chrono::duration_cast<std::chrono::microseconds>(end - start)).count() << " micros" << '\n';
//    printArray(r, n);

    start = Clock::now();
    done[0] = true;
    std::vector<std::future<void>> workerThreads;
    for (int i=0;  i < T; i++){
        workerThreads.emplace_back(std::async(sumWorker, a, n, r, done, i+1));
    }
    for(auto &res: workerThreads){
        res.get();
    }
    end = Clock::now();
    std::cout << "Parallel sum took: ";
    std::cout << (std::chrono::duration_cast<std::chrono::microseconds>(end - start)).count() << " micros" << '\n';
//    printArray(r, n);

    delete(a);
    delete(r);
    delete(done);
    return 0;
}