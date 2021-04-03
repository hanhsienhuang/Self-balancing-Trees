#include "gtest/gtest.h"
#include "trees/avl_tree.h"
#include <utility>

class TestAVLTree : public ::testing::Test{
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

    void insert(AVLTree<int, Less>& t, int n){
        for(int i=0; i<n; ++i){
            tree.insert(i);
        }
    }

    AVLTree<int, Less> tree;
};

TEST_F(TestAVLTree, TestInitSize){
    EXPECT_EQ(tree.size(), 0);
}

TEST_F(TestAVLTree, TestInsert){
    for(int i=0; i<3; ++i){
        auto it = tree.insert(i);
        EXPECT_EQ(tree.size(), i+1);
        EXPECT_EQ(*it, i);
    }
}

TEST_F(TestAVLTree, TestIterator){
    insert(tree, 3);
    auto it = tree.begin();
    int count = 0;
    for(; it!= tree.end(); ++it){
        EXPECT_EQ(*it, count);
        ++count;
    }
    EXPECT_EQ(count, 3);
}


TEST_F(TestAVLTree, TestFind){
    insert(tree, 3);
    EXPECT_EQ(tree.find(3), tree.end());
    auto it = tree.end();
    EXPECT_NO_THROW({
        it = tree.find(0);
    });
    EXPECT_EQ(*it, 0);
}

TEST_F(TestAVLTree, TestErase){
    insert(tree, 3);
    auto it = tree.begin();
    it = tree.erase(it);
    EXPECT_EQ(tree.size(), 2);
    EXPECT_EQ(*it, 1);
    EXPECT_EQ(*tree.begin(), 1);
    it = tree.find(1);
    it = tree.erase(it);
    EXPECT_EQ(tree.size(), 1);
    EXPECT_EQ(*it, 2);
    EXPECT_EQ(++it, tree.end());
    it = tree.find(2);
    it = tree.erase(it);
    EXPECT_EQ(tree.size(), 0);
    EXPECT_EQ(tree.begin(), tree.end());
}

TEST_F(TestAVLTree, TestLowerBound){
    tree.insert(0);
    for(int i=0; i<3; ++i){
        tree.insert(1);
    }
    auto it = tree.lower_bound(1);
    ASSERT_NE(it, tree.end());
    EXPECT_EQ(*it, 1);
    EXPECT_EQ(*(++it), 1);
    EXPECT_EQ(*(++it), 1);
    EXPECT_EQ(++it, tree.end());
}

TEST_F(TestAVLTree, TestUpperBound){
    tree.insert(0);
    tree.insert(2);
    for(int i=0; i<3; ++i){
        tree.insert(1);
    }
    auto it = tree.upper_bound(1);
    ASSERT_NE(it, tree.end());
    EXPECT_EQ(*it, 2);
    EXPECT_EQ(++it, tree.end());
}

TEST_F(TestAVLTree, TestCopyConstructor){
    insert(tree, 3);
    auto tree2{tree};
    EXPECT_EQ(tree.size(), 3);
    int i = -1;
    EXPECT_EQ(tree2.size(), 3);
    for(int j: tree2){
        EXPECT_EQ(j, ++i);
    }
}

TEST_F(TestAVLTree, TestMoveConstructor){
    insert(tree, 3);
    auto tree2{std::move(tree)};
    EXPECT_EQ(tree.size(), 0);
    int i = -1;
    EXPECT_EQ(tree2.size(), 3);
    for(int j: tree2){
        EXPECT_EQ(j, ++i);
    }
}

TEST_F(TestAVLTree, TestCopyAssignment){
    insert(tree, 3);
    auto tree2 = tree;
    EXPECT_EQ(tree.size(), 3);
    int i = -1;
    EXPECT_EQ(tree2.size(), 3);
    for(int j: tree2){
        EXPECT_EQ(j, ++i);
    }
}

TEST_F(TestAVLTree, TestMoveAssignment){
    insert(tree, 3);
    auto tree2 = std::move(tree);
    EXPECT_EQ(tree.size(), 0);
    int i = -1;
    EXPECT_EQ(tree2.size(), 3);
    for(int j: tree2){
        EXPECT_EQ(j, ++i);
    }
}

TEST_F(TestAVLTree, TestInsertLarge){
    constexpr int N = 10000;
    insert(tree, N);
    EXPECT_EQ(tree.size(), N);
    int i = -1;
    for(int j: tree){
        EXPECT_EQ(j, ++i);
    }
}

TEST_F(TestAVLTree, TestInsertDuplicate){
    constexpr int N = 10;
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