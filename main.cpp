#include"vector.h"
#include<iostream>
#include<cstdio>
#include<vector>
#include<chrono>

#define N 100000

template<typename T>
void test(T& vint){
    for(int i=0; i< N; ++i){
        vint.push_back(3*i);
    }
    for(int i=0; i< N; ++i){
        if(vint[i] != 3*i){
            printf("Wrong answer at index %i", i);
        }
    }
    for(int i=0; i< N; ++i){
        vint[i] = i*2;
    }
    for(int i=0; i< N; ++i){
        if(vint[i] != 2*i){
            printf("Wrong answer at index %i", i);
        }
    }
}

int main(){
    std::chrono::steady_clock::time_point begin, end;

    printf("Testing std vector\n");
    begin = std::chrono::steady_clock::now();
    std::vector<int> stdvec;
    test(stdvec);
    end = std::chrono::steady_clock::now();
    /*
    for(auto i: stdvec){
        std::cout<<i<<"\n";
    }
    */
    std::cout<<"Time difference: "<< std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count() << " ms\n" ;

    printf("Testing my vector\n");
    begin = std::chrono::steady_clock::now();
    Vector<int> myvec;
    test(myvec);
    end = std::chrono::steady_clock::now();
    /*
    for(auto i: myvec){
        std::cout<<i<<"\n";
    }
    */
    std::cout<<"Time difference: "<< std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count() << " ms\n" ;
}