//
//  main.cpp
//  Lab3_threadpool
//
//  Created by Alexandra Neamtu on 29/10/2017.
//  Copyright © 2017 Alexandra Neamtu. All rights reserved.
//

#define M 1000
#define N 1000
#define P 1000
#define Q 1000
#define T 100
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <vector>
#include "ThreadPool.hpp"
typedef std::chrono::high_resolution_clock Clock;
using namespace std;


int matrix1[M][N],matrix2[P][Q];

//result matrices
int additionSerial[M][N];
int additionThreads[M][N];
int multiplySerial[M][Q];
int multiplyThreads[M][Q];

void initMatrix1(){
    srand(time(NULL));
    for(int i = 0; i< M;i++){
        for(int j = 0; j< N; j++){
            matrix1[i][j]=rand()%10+1;
        }
    }
    for(int i = 0; i< P;i++){
        for(int j = 0; j< Q; j++){
            matrix2[i][j]=rand()%10+1;
        }
    }
}

void printMatrix1(){
    cout<<"Matrix 1:\n";
    for(int i=0;i<M;i++){
        for(int j = 0; j< N;j++){
            cout<<matrix1[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

void printMatrix2(){
    cout<<"Matrix 2:\n";
    for(int i=0;i<P;i++){
        for(int j = 0; j< Q;j++){
            cout<<matrix2[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

void addSerial(){
    for(int i = 0;i<M;i++)
        for(int j = 0; j< N;j++)
            additionSerial[i][j]=matrix1[i][j]+matrix2[i][j];
}


void addWithThreads(int id)
{
    
    for (int i = id; i < M; i += T)
    {
        for (int j = 0; j < N; j++)
        {
            additionThreads[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }
}

void mulSerial(){
    for (int i = 0; i < M; i++)
    {
        
        for (int j = 0; j < Q; j++)
        {
            int resInt = 0;
            for (int idx = 0; idx < N; idx++)
            {
                
                resInt += matrix1[i][idx] * matrix2[idx][j];
            }
            
            multiplySerial[i][j] = resInt;
        }
    }
}

void mulThreads(int i)
{
    int r;
    for (int k=i;k<M;k+=T)
        for(int j=0;j<Q;j++)
        {
            r = 0;
            
            for (int id = 0; id < N; id++)
            {
                r += matrix1[i][id] * matrix2[id][j];
            }
            multiplyThreads[k][j] = r;
        }
}

void Add(){
    
    
    //one thread
    
    auto addSerialBegin = Clock::now();
    addSerial();
    auto addSerialEnd = Clock::now();
    cout<<"Adding a "<<M<<" by "<<N<<" with a "<<P<<" by "<<Q<<" matrices on one thread took ";
    cout<<std::chrono::duration_cast<std::chrono::nanoseconds>(addSerialEnd - addSerialBegin).count()<<" nanoseconds"<<endl;
    
    //    cout<<"-->Result:\n";
    //    for(int i=0;i<M;i++){
    //        for(int j = 0; j< N;j++){
    //            cout<<additionSerial[i][j]<<" ";
    //        }
    //        cout<<endl;
    //    }
    //    cout<<endl;
    
    
    
    //T threads
    auto addThreadsBegin = Clock::now();
    ThreadPool poolT(T);
    std::vector< std::future<void>> addThreadFutures;
    for(int i = 0; i < T; ++i) {
        addThreadFutures.emplace_back(
                                      poolT.enqueue([i] {addWithThreads(i);}));
    }
    for(auto &&  res:addThreadFutures){
        res.get();
    }
    
    auto addThreadsEnd = Clock::now();
    cout<<"Adding a "<<M<<" by "<<N<<" with a "<<P<<" by "<<Q<<" matrices on "<<T<<" threads took ";
    cout<<std::chrono::duration_cast<std::chrono::nanoseconds>(addThreadsEnd - addThreadsBegin).count()<<" nanoseconds"<<endl;
    
    //    cout<<"-->Result:\n";
    //    for(int i=0;i<M;i++){
    //        for(int j = 0; j< N;j++){
    //            cout<<additionThreads[i][j]<<" ";
    //        }
    //        cout<<endl;
    //    }
    //    cout<<endl;
}

void Multiply(){
    
    //one thread
    auto multiplySerialBegin = Clock::now();
    mulSerial();
    auto multiplySerialEnd = Clock::now();
    cout<<"Multiplying a "<<M<<" by "<<N<<" with a "<<P<<" by "<<Q<<" matrices on one thread took ";
    cout<<std::chrono::duration_cast<std::chrono::nanoseconds>(multiplySerialEnd - multiplySerialBegin).count()<<" nanoseconds"<<endl;
    
    //    cout<<"-->Result:\n";
    //    for(int i=0;i<M;i++){
    //        for(int j = 0; j< Q;j++){
    //            cout<<multiplySerial[i][j]<<" ";
    //        }
    //        cout<<endl;
    //    }
    //    cout<<endl;
    
    
    //T threads
    auto multiplyThreadsBegin = Clock::now();
    std::vector<std::future<void>> multiplyThreadFutures;
    ThreadPool poolT(T);
    for(int i = 0; i< T;i++){
        multiplyThreadFutures.emplace_back(
                                           poolT.enqueue([i] {mulThreads(i);}));
    }
    
    for(auto &&  res:multiplyThreadFutures){
        res.get();
    }
    
    auto multiplyThreadsEnd = Clock::now();
    cout<<"Multiplying a "<<M<<" by "<<N<<" with a "<<P<<" by "<<Q<<" matrices on "<<T<<" threads took ";
    cout<<std::chrono::duration_cast<std::chrono::nanoseconds>(multiplyThreadsEnd - multiplyThreadsBegin).count()<<" nanoseconds"<<endl;
    
    //    cout<<"-->Result:\n";
    //    for(int i=0;i<M;i++){
    //        for(int j = 0; j< Q;j++){
    //            cout<<multiplyThreads[i][j]<<" ";
    //        }
    //        cout<<endl;
    //    }
    //    cout<<endl;
}



int main(int argc, const char * argv[]) {
    initMatrix1();
    //printMatrix1();
    //printMatrix2();
    if(M==P and N==Q){
        cout<<"------Addition:------"<<endl;
        Add();
    }else{
        cout<<"Invalid matrix for addition \n";
    }
    if(N==P){
        cout<<endl<<"------Multiplication:------"<<endl;
        Multiply();
    }
    else{
        cout<<"Invalid matrix for multiplication \n";
    }
    return 0;
}

