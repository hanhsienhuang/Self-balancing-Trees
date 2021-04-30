#include<iostream>
#include<cstdio>
#include<vector>
#include<chrono>
#include"vector.h"
#include<map>
#include"map.h"

#define N 10000

class Time{
public:
    Time(){
        begin = std::chrono::steady_clock::now();
    }

    ~Time(){
        end = std::chrono::steady_clock::now();
        std::cout<<"Execution time: "
            << std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count() 
            << " ms\n" ;
    }

private:
    std::chrono::steady_clock::time_point begin, end;
};

template<typename T>
void test(T& m){
    Time t;

    for(int i=0; i< N; ++i){
        m.insert({i, -i});
    }

    for(int i=0; i< N; ++i){
        m[i] = i;
    }

    for(int i=0; i< N; ++i){
        m.erase()
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