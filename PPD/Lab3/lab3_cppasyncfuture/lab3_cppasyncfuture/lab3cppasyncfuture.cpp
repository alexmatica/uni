#include "stdafx.h"

#define M 3000
#define N 3000
#define P 3000
#define Q 3000
#define T 4
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <vector>
#include <future>
typedef std::chrono::high_resolution_clock Clock;

int matrix1[M][N], matrix2[P][Q];

//result matrices
int additionSerial[M][N];
int additionThreads[M][N];
int multiplySerial[M][Q];
int multiplyThreads[M][Q];

void initMatrices() {
	srand(time(NULL));
	for (int i = 0; i< M; i++) {
		for (int j = 0; j< N; j++) {
			matrix1[i][j] = rand() % 10 + 1;
		}
	}
	for (int i = 0; i< P; i++) {
		for (int j = 0; j< Q; j++) {
			matrix2[i][j] = rand() % 10 + 1;
		}
	}
}

void printMatrix1(int **m) {
	for (int i = 0; i<M; i++) {
		for (int j = 0; j< N; j++) {
			std::cout << m[i][j] << " ";
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

void addSerial() {
	for (int i = 0; i<M; i++)
		for (int j = 0; j< N; j++)
			additionSerial[i][j] = matrix1[i][j] + matrix2[i][j];
}

void addWithThreads(int id){
	for (int i = id; i < M; i += T)	{
		for (int j = 0; j < N; j++)		{
			additionThreads[i][j] = matrix1[i][j] + matrix2[i][j];
		}
	}
}

void mulSerial() {
	for (int i = 0; i < M; i++)	{
		for (int j = 0; j < Q; j++)	{
			int resInt = 0;
			for (int idx = 0; idx < N; idx++){
				resInt += matrix1[i][idx] * matrix2[idx][j];
			}
			multiplySerial[i][j] = resInt;
		}
	}
}

void mulThreads(int i){
	int r;
	for (int k = i; k<M; k += T)
		for (int j = 0; j<Q; j++){
			r = 0;
			for (int id = 0; id < N; id++){
				r += matrix1[i][id] * matrix2[id][j];
			}
			multiplyThreads[k][j] = r;
		}
}

void Add() {
	//one thread
	auto addSerialBegin = Clock::now();
	addSerial();
	auto addSerialEnd = Clock::now();
	std::cout << "Adding two " << M << " by " << N << " matrices on one thread took ";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(addSerialEnd - addSerialBegin).count() << " milliseconds" << '\n';

	//    std::cout<<"-->Result:\n";
	//    for(int i=0;i<M;i++){
	//        for(int j = 0; j< N;j++){
	//            std::cout<<additionSerial[i][j]<<" ";
	//        }
	//        std::cout<<'\n';
	//    }
	//    std::cout<<'\n';

	//T threads
	auto addThreadsBegin = Clock::now();
	std::vector< std::future<void>> addThreadFutures;
	for (int i = 0; i < T; ++i) {
		addThreadFutures.emplace_back(std::async(addWithThreads, i));
	}
	for (auto &&res : addThreadFutures) {
		res.get();
	}

	auto addThreadsEnd = Clock::now();
	std::cout << "Adding two " << M << " by " << N <<" matrices on " << T << " threads took ";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(addThreadsEnd - addThreadsBegin).count() << " milliseconds" << '\n';

	//    std::cout<<"-->Result:\n";
	//    for(int i=0;i<M;i++){
	//        for(int j = 0; j< N;j++){
	//            std::cout<<additionThreads[i][j]<<" ";
	//        }
	//        std::cout<<'\n';
	//    }
	//    std::cout<<'\n';
}

void Multiply() {

	//one thread
	auto multiplySerialBegin = Clock::now();
	mulSerial();
	auto multiplySerialEnd = Clock::now();
	std::cout << "Multiplying a " << M << " by " << N << " with a " << P << " by " << Q << " matrices on one thread took ";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(multiplySerialEnd - multiplySerialBegin).count() << " millseconds" << '\n';

	//    std::cout<<"-->Result:\n";
	//    for(int i=0;i<M;i++){
	//        for(int j = 0; j< Q;j++){
	//            std::cout<<multiplySerial[i][j]<<" ";
	//        }
	//        std::cout<<'\n';
	//    }
	//    std::cout<<'\n';

	//T threads
	std::vector< std::future<void>> multiplyThreadFutures;
	auto multiplyThreadsBegin = Clock::now();
	for (int i = 0; i < T; i++) {
		multiplyThreadFutures.emplace_back(std::async(mulThreads, i));
	}

	for (auto &&res : multiplyThreadFutures) {
		res.get();
	}

	auto multiplyThreadsEnd = Clock::now();
	std::cout << "Multiplying a " << M << " by " << N << " with a " << P << " by " << Q << " matrices on " << T << " threads took ";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(multiplyThreadsEnd - multiplyThreadsBegin).count() << " milliseconds" << '\n';

	//    std::cout<<"-->Result:\n";
	//    for(int i=0;i<M;i++){
	//        for(int j = 0; j< Q;j++){
	//            std::cout<<multiplyThreads[i][j]<<" ";
	//        }
	//        std::cout<<'\n';
	//    }
	//    std::cout<<'\n';
}



int main(int argc, const char * argv[]) {
	initMatrices();
	//printMatrix1();
	//printMatrix2();
	if (M == P && N == Q) {
		std::cout << "------Addition:------" << '\n';
		Add();
	}
	else {
		std::cout << "Invalid matrix for addition \n";
	}
	if (N == P) {
		std::cout << '\n' << "------Multiplication:------" << '\n';
		Multiply();
	}
	else {
		std::cout << "Invalid matrix for multiplication \n";
	}
	return 0;
}


