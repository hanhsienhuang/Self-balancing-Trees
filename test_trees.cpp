#include"trees/binary_search_tree.h"
#include"trees/red_black_tree.h"
#include"trees/avl_tree.h"
#include<iostream>
#include<cstdio>
#include<chrono>


struct Less{
    bool operator()(int a, int b){
        return a<b;
    }
};


#define TEST(name) std::cout<< "Testing " #name <<std::endl;\
test_tree< name >();

#define N 100000

template<template<typename, typename>typename Tree>
void test_tree(){
    std::chrono::steady_clock::time_point begin, end;
    begin = std::chrono::steady_clock::now();

    Tree<int, Less> tree;
    for(int i=-N; i<0; ++i){
        tree.insert(i);
    }
    std::cout << "size(should be " << N << "):" << tree.size() << "\n";
    std::cout << "depth:" << tree.depth() << "\n";
    for(int i=0; i<N; ++i){
        tree.insert(i);
    }
    std::cout << "size(should be " << 2*N << "):" << tree.size() << "\n";
    std::cout << "depth:" << tree.depth() << "\n";
    for(int i=0; i<N; ++i){
        tree.insert(i);
    }
    std::cout << "size(should be " << 3*N << "):" << tree.size() << "\n";
    std::cout << "depth:" << tree.depth() << "\n";
    for(int i=-N; i<N; ++i){
        auto it = tree.find(i);
        tree.erase(it);
    }
    std::cout << "size(should be " << N << "):" << tree.size() << "\n";
    std::cout << "depth:" << tree.depth() << "\n";
    int i = 0;
    int count = 0;
    for(auto it: tree){
        if(it != i){
            std::cout << it << "!=" << i << "\n";
        }
        ++count;
        ++i;
    }
    std::cout << "count(should be " << N << "):" << count << "\n";
    end = std::chrono::steady_clock::now();
    std::cout<<"Time spent: "<< std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << " ms\n" ;

    std::cout << "\n";
}


int main(){
    //TEST(BinarySearchTree)
    TEST(RedBlackTree)
    TEST(AVLTree)
}