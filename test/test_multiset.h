#include "gtest/gtest.h"
#include "multiset.h"
#include "trees/avl_tree.h"

class TestMultiset : public ::testing::Test{
protected:

    void SetUp() override {
    }

    void TearDown() override {
    }

    void insert(int num){
        for(int i=0; i<num; ++i){
            set.insert(i);
            set.insert(i);
        }
    }

    int N = 1000;
    Multiset<int, AVLTree> set;
};

TEST_F(TestMultiset, TestInsertBool){
    for(int i=0; i<N; ++i){
        auto res = set.insert(i);
        EXPECT_EQ(res.second, true);
    }
    for(int i=0; i<N; ++i){
        auto res = set.insert(i);
        EXPECT_EQ(res.second, true);
    }
}

TEST_F(TestMultiset, TestFor){
    insert(N);
    int n = 0;
    for(auto i: set){
        EXPECT_EQ(i, n/2);
        ++n;
    }
}

TEST_F(TestMultiset, TestSize){
    EXPECT_EQ(set.size(), 0);
    insert(3);
    EXPECT_EQ(set.size(), 6);
    set.clear();
    EXPECT_EQ(set.size(), 0);
}

TEST_F(TestMultiset, TestFind){
    insert(N);
    for(int i=0; i<N; ++i){
        auto it = set.find(i);
        EXPECT_EQ(*it, i);
    }
}

TEST_F(TestMultiset, TestEqualRange){
    insert(2);
    auto p = set.equal_range(0);
    auto lower = p.first;
    auto upper = p.second;
    EXPECT_EQ(*lower, 0);
    ++lower;
    ++lower;
    EXPECT_EQ(lower, upper);
}

TEST_F(TestMultiset, TestErase){
    insert(N);
    EXPECT_EQ(set.size(), 2*N);
    auto it = set.begin();
    while(it!=set.end()){
        it = set.erase(it);
    }
    EXPECT_EQ(set.size(), 0);
}