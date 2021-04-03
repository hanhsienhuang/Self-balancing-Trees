#include "gtest/gtest.h"
#include "vector.h"

class VectorTest : public ::testing::Test{
protected:
    void SetUp() override {
        N = 10000;
    }

    void TearDown() override {
    }

    Vector<int> v;
    int N;
};

TEST_F(VectorTest, TestPush){
    EXPECT_EQ(v.size(), 0);
    for(int i=0; i<N; ++i){
        v.push_back(i);
        EXPECT_EQ(v.size(), i+1);
        EXPECT_GE(v.capacity(), v.size());
    }
}

TEST_F(VectorTest, TestIterator){
    int i = 0;
    for(int i=0; i<N; ++i){
        v.push_back(i);
    }
    for(auto it=v.begin(); it!=v.end(); ++it){
        EXPECT_EQ(*it, i);
        ++i;
    }
}

TEST_F(VectorTest, TestIndexOperator){
    for(int i=0; i<N; ++i){
        v.push_back(i);
    }
    for(int i=0; i<N; ++i){
        EXPECT_EQ(v[i], i);
    }
}

TEST_F(VectorTest, TestAssignIndexOperator){
    for(int i=0; i<N; ++i){
        v.push_back(i);
    }
    for(int i=0; i<N; ++i){
        v[i] = N-i;
        EXPECT_EQ(v[i], N-i);
    }
}

TEST_F(VectorTest, TestCopy){
    for(int i=0; i<N; ++i){
        v.push_back(i);
    }
    Vector<int> v2{v};
    ASSERT_EQ(v2.size(), v.size());
    for(int i=0; i<N; ++i){
        EXPECT_EQ(v2[i], i);
    }

}