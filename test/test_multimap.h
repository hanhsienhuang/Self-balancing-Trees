#include "gtest/gtest.h"
#include "multimap.h"
#include "trees/avl_tree.h"

class TestMultimap : public ::testing::Test{
protected:

    void SetUp() override {
    }

    void TearDown() override {
    }

    void insert(int num){
        for(int i=0; i<num; ++i){
            map.insert({i, i});
            map.insert({i, -i});
        }
    }

    int N = 1000;
    Multimap<int, int, AVLTree> map;
};

TEST_F(TestMultimap, TestInsertBool){
    for(int i=0; i<N; ++i){
        auto res = map.insert({i, i});
        EXPECT_EQ(res.second, true);
    }
    for(int i=0; i<N; ++i){
        auto res = map.insert({i, -i});
        EXPECT_EQ(res.second, true);
    }
}

TEST_F(TestMultimap, TestFor){
    insert(N);
    int n = 0;
    for(auto i: map){
        EXPECT_EQ(i.first, n/2);
        ++n;
    }
}

TEST_F(TestMultimap, TestSize){
    EXPECT_EQ(map.size(), 0);
    insert(3);
    EXPECT_EQ(map.size(), 6);
    map.clear();
    EXPECT_EQ(map.size(), 0);
}

TEST_F(TestMultimap, TestFind){
    insert(N);
    for(int i=0; i<N; ++i){
        auto it = map.find(i);
        EXPECT_EQ(it->first, i);
    }
}

TEST_F(TestMultimap, TestEqualRange){
    insert(2);
    auto p = map.equal_range(0);
    auto lower = p.first;
    auto upper = p.second;
    EXPECT_EQ(lower->first, 0);
    ++lower;
    ++lower;
    EXPECT_EQ(lower, upper);
}

TEST_F(TestMultimap, TestErase){
    insert(N);
    EXPECT_EQ(map.size(), 2*N);
    auto it = map.begin();
    while(it!=map.end()){
        it = map.erase(it);
    }
    EXPECT_EQ(map.size(), 0);
}