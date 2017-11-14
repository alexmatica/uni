/*

	Running-time analysis:

	On 1 Thread:
		10 x 10 - 291 microsec
		50 x 50 - 789 microsec
		100 x 100 - 2833 microsec
		500 x 500 - 195968 microsec
		1000 x 1000 - 10800074 microsec

	On 4 Threads (4 threads multiplying the first 2 matrices, 4 threads multiplying the result with the 3rd matrix):
		10 x 10 - 206 microsec
		50 x 50 - 886 microsec
		100 x 100 - 3016 microsec
		500 x 500 - 181575 microsec
		1000 x 1000 - 1647734 micros

*/


#include "stdafx.h"

#define N 1000
#define T 4
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <vector>
#include <future>
#include <thread>
#include <mutex>

typedef std::chrono::high_resolution_clock Clock;

int matrix1[N][N], matrix2[N][N], matrix3[N][N];
int result1[N][N], result2[N][N];

std::mutex mtx_protect[N];
std::mutex dequeue;

struct Indexes {
	int i;
	int j;
};
std::vector<Indexes> work_queue;
unsigned values_computed = 0;

void initMatrices() {
	srand(time(NULL));
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			matrix1[i][j] = rand() % 10 + 1;
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			matrix2[i][j] = rand() % 10 + 1;
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			matrix3[i][j] = rand() % 10 + 1;
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
			result2[i][j] = 0;
	}
}

void printMatrix(int m[N][N]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			std::cout << m[i][j] << " ";
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

void check(int m1[N][N], int m2[N][N], int m3[N][N]) {

	int r1[N][N];
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++) {
			int res = 0;
			for (int k = 0; k < N; k++)
				res += m1[i][k] * m2[k][j];
			r1[i][j] = res;
		}
	//printMatrix(r1);
	int r2[N][N];
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++) {
			int res = 0;
			for (int k = 0; k < N; k++)
				res += r1[i][k] * m3[k][j];
			r2[i][j] = res;
		}
	printMatrix(r2);
}

void mulProducer(int i) {
	int res;
	for (int k = i; k < N; k += T)
		for (int j = 0; j < N; j++) {
			res = 0;
			for (int id = 0; id < N; id++) {
				res += matrix1[k][id] * matrix2[id][j];
			}
			result1[k][j] = res;
			dequeue.lock();
			Indexes idx{ k, j };
			work_queue.push_back(idx);
			dequeue.unlock();
		}
}

void mulConsumer() {

	while (values_computed < N * N) {

		dequeue.lock();
		if (work_queue.size() == 0) {
			dequeue.unlock();
			continue;
		}
		Indexes idx = work_queue.back();
		work_queue.pop_back();
		values_computed += 1;
		dequeue.unlock();

		mtx_protect[idx.i].lock();
		for (int col = 0; col < N; col++) {
			result2[idx.i][col] += result1[idx.i][idx.j] * matrix3[idx.j][col];
		}
		mtx_protect[idx.i].unlock();
	}
}

int main()
{
	initMatrices();
	std::vector<std::future<void>> multiplyThreads;
	auto mt_start = Clock::now();
	for (int i = 0; i < T; i++) {
		multiplyThreads.emplace_back(std::async(mulProducer, i));
	}

	std::vector<std::future<void>> multiplyFinal;
	for (int i = 0; i < T; i++) {
		multiplyFinal.emplace_back(std::async(mulConsumer));
	}

	for (auto &&res : multiplyThreads) {
		res.get();
	}
	for (auto &&res : multiplyFinal) {
		res.get();
	}
	auto mt_end = Clock::now();
	std::cout << N << " x " << N << " matrices on " << T << " threads: ";
	std::cout << (std::chrono::duration_cast<std::chrono::microseconds>(mt_end - mt_start)).count() << " micros" << '\n';


	/*printMatrix(matrix1);
	printMatrix(matrix2);
	printMatrix(result1);
	printMatrix(matrix3);*/
	//printMatrix(result2);
	//check(matrix1, matrix2, matrix3);
	return 0;
}

