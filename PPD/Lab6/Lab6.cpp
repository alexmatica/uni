#define N 10
#define T 4
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <chrono>
#include <vector>
#include <future>
#include <thread>
#include <mutex>

typedef std::chrono::high_resolution_clock Clock;

int P1[N+1], P2[N+1], R[2*N + 1];
int D[N+1][N+1];
std::mutex mtx_protect[2*N + 1];

unsigned values_computed = 0;

void initPolynoms() {
	srand(time(NULL));
	for (int i=0; i<=N; i++){
		P1[i] = rand() % 10;
		P2[i] = rand() % 10;
	}
}

void resetResult(){
	for (int i=0; i <= 2*N; i++)
		R[i] = 0;
}

void printPolynom(int *P, int len){
	for (int i=0; i<=len; i++)
		std::cout << P[i]<<" ";
	std::cout<<'\n';
}

void multiplyLinear(){

	for (int i=0; i<=N; i++)
		for (int j=0; j<=N; j++)
			R[i+j] += (P1[i] * P2[j]);
}

void multiplyWorker(int tid){

	//for each degree in the resulting polynomial
	for (int i=tid; i <= 2*N; i += T)
		//take the pairs of indices that summed give that degree (no need for mutexes)
		for (int x = 0; x <= i && x <= N; x++)
			if (i - x <= N)
				R[i] += (P1[x] * P2[i-x]); 

	// for (int i=tid; i<N; i+=T)
	// 	for (int j=0; j<N; j++){
	// 		mtx_protect[i+j].lock();
	// 		R[i+j] += P1[i] * P2[j];
	// 		mtx_protect[i+j].unlock();
	// 	}
}

void karatsubaLinear(){

	// std::cout<<"CE???" << std::flush;

	for (int i=0; i<=N; i++)
		D[i][i] = P1[i] * P2[i];
	// std::cout<<"G" << std::flush;
	for(int s=0; s <= N-1; s++)
		for(int t=s+1; t<=N; t++)
			D[s][t] = ((P1[s] + P1[t]) * (P2[s] + P2[t]));
	// std::cout<<"O" << std::flush;
	for (int s=0; s<=N-1; s++)
		for (int t=s+1; t<=N; t++)
			R[s+t] += (D[s][t] - D[s][s] - D[t][t]);
	// std::cout<<"GOOD TIL NOW\n" << std::flush;

	for (int i=2; i <= 2*N; i+=2)
		R[i] += (D[i/2][i/2]);
	// std::cout<<"D\n" << std::flush;
	R[0] = D[0][0];
	R[2*N] = D[N][N];
}

int main(int argc, char **argv)
{
	initPolynoms();
	// printPolynom(P1, N);
	// printPolynom(P2, N);
	// std::vector<std::future<void>> multiplyThreads;
	// auto mt_start = Clock::now();
	// for (int i = 0; i < T; i++) {
	// 	multiplyThreads.emplace_back(std::async(multiplyWorker, i));
	// }
	// for (auto &&res: multiplyThreads){
	// 	res.wait();
	// }
	
	// auto mt_end = Clock::now();
	// std::cout<< N <<" degree polynomials on " << T << " threads: ";
	// std::cout << (std::chrono::duration_cast<std::chrono::microseconds>(mt_end - mt_start)).count() << " micros" << '\n';
	// // printPolynom(R, 2*N);
	// resetResult();
	auto mt_start1 = Clock::now();
	multiplyLinear();
	auto mt_end1 = Clock::now();
	std::cout<< N <<" degree polynomials on one thread: ";
	std::cout << (std::chrono::duration_cast<std::chrono::microseconds>(mt_end1 - mt_start1)).count() << " micros" << '\n';
	printPolynom(R, 2*N);
	resetResult();
	auto mt_start2 = Clock::now();
	karatsubaLinear();
	auto mt_end2 = Clock::now();
	std::cout<< N <<" degree polynomials on one thread (Karatsuba): ";
	std::cout << (std::chrono::duration_cast<std::chrono::microseconds>(mt_end2 - mt_start2)).count() << " micros" << '\n';
	printPolynom(R, 2*N);

	return 0;
}

