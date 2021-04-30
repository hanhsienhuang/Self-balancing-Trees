#include "gtest/gtest.h"
#include "map.h"
#include "trees/avl_tree.h"
#include "trees/b_tree.h"

class TestMap : public ::testing::Test{
protected:

    void SetUp() override {
    }

    void TearDown() override {
    }

    void insert(int num){
        for(int i=0; i<num; ++i){
            map.insert({i, i});
        }
    }

    int N = 1000;
    Map<int, int, AVLTree> map;
};

TEST_F(TestMap, TestInsertBool){
    for(int i=0; i<N; ++i){
        auto res = map.insert({i, i});
        EXPECT_EQ(res.second, true);
    }
    for(int i=0; i<N; ++i){
        auto res = map.insert({i, -i});
        EXPECT_EQ(res.second, false);
    }
}

TEST_F(TestMap, TestFor){
    insert(N);
    int n = 0;
    for(auto i: map){
        EXPECT_EQ(i.first, n);
        EXPECT_EQ(i.second, n);
        ++n;
    }
}

TEST_F(TestMap, TestSize){
    EXPECT_EQ(map.size(), 0);
    insert(3);
    EXPECT_EQ(map.size(), 3);
    map.clear();
    EXPECT_EQ(map.size(), 0);
}

TEST_F(TestMap, TestFind){
    insert(N);
    for(int i=0; i<N; ++i){
        auto it = map.find(i);
        EXPECT_EQ(it->first, i);
        EXPECT_EQ(it->second, i);
    }
}

TEST_F(TestMap, TestEqualRange){
    insert(2);
    auto p = map.equal_range(0);
    auto lower = p.first;
    auto upper = p.second;
    EXPECT_EQ(lower->first, 0);
    EXPECT_EQ(++lower, upper);
}

TEST_F(TestMap, TestAt){
    insert(2);
    EXPECT_EQ(map.at(1), 1);
}

TEST_F(TestMap, TestSquareBracket){
    insert(2);
    EXPECT_EQ(map[1], 1);
    map[1] = 100;
    EXPECT_EQ(map[1], 100);
    map[3] = 2;
    EXPECT_EQ(map.size(), 3);
    EXPECT_EQ(map[3], 2);
}

TEST_F(TestMap, TestErase){
    insert(N);
    EXPECT_EQ(map.size(), N);
    auto it = map.begin();
    while(it!=map.end()){
        it = map.erase(it);
    }
    EXPECT_EQ(map.size(), 0);
}