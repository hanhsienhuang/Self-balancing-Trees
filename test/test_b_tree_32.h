#include "gtest/gtest.h"
#include "trees/b_tree.h"
#include <utility>

class Test32BTree : public ::testing::Test{
protected:
    class Less{
    public:
        bool operator()(int a, int b){
            return a<b;
        }
    };

    void SetUp() override {
    }

    void TearDown() override {
    }

    void insert(BTree<int, Less, 32>& t, int n){
        for(int i=0; i<n; ++i){
            tree.insert(i);
        }
    }

    int N = 1000;
    BTree<int, Less, 32> tree;
};

TEST_F(Test32BTree, TestInitSize){
    EXPECT_EQ(tree.size(), 0);
}

TEST_F(Test32BTree, TestInsert){
    for(int i=0; i<N; ++i){
        auto it = tree.insert(i);
        EXPECT_EQ(tree.size(), i+1);
        EXPECT_EQ(*it, i);
    }
}

TEST_F(Test32BTree, TestIterator){
    insert(tree, N);
    auto it = tree.begin();
    int count = 0;
    for(; it!= tree.end(); ++it){
        EXPECT_EQ(*it, count);
        ++count;
    }
    EXPECT_EQ(count, N);
}


TEST_F(Test32BTree, TestFind){
    insert(tree,N);
    EXPECT_EQ(tree.find(N+1), tree.end());
    auto it = tree.end();
    EXPECT_NO_THROW({
        it = tree.find(0);
    });
    EXPECT_EQ(*it, 0);
}

TEST_F(Test32BTree, TestErase){
    insert(tree, N);
    auto it = tree.begin();
    int i = 0;
    while(tree.size()!=0){
        EXPECT_EQ(*it, i);
        EXPECT_NO_THROW({
            it = tree.erase(it);
        });
        ++i;
        EXPECT_EQ(tree.size(), N-i);
    }
    EXPECT_EQ(i, N);
    EXPECT_EQ(it, tree.end());
}

TEST_F(Test32BTree, TestLowerBound){
    for(int i=0; i<N; ++i){
        tree.insert(0);
        tree.insert(1);
    }
    auto it = tree.lower_bound(1);
    for(int i=0; i<N; ++i){
        ASSERT_NE(it, tree.end());
        EXPECT_EQ(*it, 1);
        ++it;
    }
    EXPECT_EQ(it, tree.end());
}

TEST_F(Test32BTree, TestUpperBound){
    for(int i=0; i<N; ++i){
        tree.insert(0);
        tree.insert(1);
    }
    auto it = tree.upper_bound(0);
    ASSERT_NE(it, tree.end());
    EXPECT_EQ(it, tree.lower_bound(1));
    EXPECT_EQ(tree.upper_bound(1), tree.end());
}

TEST_F(Test32BTree, TestCopyConstructor){
    insert(tree, N);
    auto tree2{tree};
    EXPECT_EQ(tree.size(), N);
    int i = -1;
    EXPECT_EQ(tree2.size(), N);
    for(int j: tree2){
        EXPECT_EQ(j, ++i);
    }
}

TEST_F(Test32BTree, TestMoveConstructor){
    insert(tree, N);
    auto tree2{std::move(tree)};
    EXPECT_EQ(tree.size(), 0);
    int i = -1;
    EXPECT_EQ(tree2.size(), N);
    for(int j: tree2){
        EXPECT_EQ(j, ++i);
    }
}

TEST_F(Test32BTree, TestCopyAssignment){
    insert(tree, N);
    auto tree2 = tree;
    EXPECT_EQ(tree.size(), N);
    int i = -1;
    EXPECT_EQ(tree2.size(), N);
    for(int j: tree2){
        EXPECT_EQ(j, ++i);
    }
}

TEST_F(Test32BTree, TestMoveAssignment){
    insert(tree, N);
    auto tree2 = std::move(tree);
    EXPECT_EQ(tree.size(), 0);
    int i = -1;
    EXPECT_EQ(tree2.size(), N);
    for(int j: tree2){
        EXPECT_EQ(j, ++i);
    }
}

TEST_F(Test32BTree, TestInsertLarge){
    constexpr int N = 10000;
    insert(tree, N);
    EXPECT_EQ(tree.size(), N);
    int i = -1;
    for(int j: tree){
        EXPECT_EQ(j, ++i);
    }
}

TEST_F(Test32BTree, TestInsertDuplicate){
    constexpr int N = 100;
    insert(tree, N);
    insert(tree, N);
    EXPECT_EQ(tree.size(), 2*N);
    auto it = tree.begin();
    for(int j=0; j<N; ++j){
        EXPECT_EQ(j, *it);
        ++it;
        EXPECT_EQ(j, *it);
        ++it;
    }
}
