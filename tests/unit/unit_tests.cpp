#include "rbtree.hpp"

#include <gtest/gtest.h>
#include <vector>
#include <algorithm>

TEST (RBTreeTest, BasicInsertAndSize)
{
    rb::Tree<int> tree;

    ASSERT_TRUE (tree.empty());
    ASSERT_EQ (tree.size(), 0);

    tree.insert (13);
    ASSERT_FALSE (tree.empty());
    ASSERT_EQ (tree.size(), 1);

    tree.insert (27);
    ASSERT_EQ (tree.size(), 2);

    tree.insert (13);               // duplicate
    ASSERT_EQ (tree.size(), 2);
}

TEST (RBTreeTest, IteratorsWork)
{
    rb::Tree<int> tree;
    tree.insert (35);
    tree.insert (17);
    tree.insert (23);

    std::vector<int> values;
    for (auto it = tree.begin(); it != tree.end(); ++it)
        values.push_back (*it);

    std::vector<int> expected = {17, 23, 35};
    ASSERT_EQ (values, expected);
}

TEST (RBTreeTest, BoundFuncs)
{
    rb::Tree<int> tree;
    tree.insert (13);
    tree.insert (27);
    tree.insert (35);
    tree.insert (48);
    tree.insert (59);

    auto low_1 = tree.lower_bound (19);
    ASSERT_NE (low_1, tree.end());
    ASSERT_EQ (*low_1, 27);

    auto low_2 = tree.lower_bound (27);
    ASSERT_EQ (*low_2, 27);

    auto low_3 = tree.lower_bound (67);
    ASSERT_EQ (low_3, tree.end());

    auto upp_1 = tree.upper_bound (19);
    ASSERT_EQ (*upp_1, 27);

    auto upp_2 = tree.upper_bound (27);
    ASSERT_EQ (*upp_2, 35);

    auto upp_3 = tree.upper_bound (59);
    ASSERT_EQ (upp_3, tree.end());
}

TEST (RBTreeTest, LowerBoundEdgeCases)
{
    rb::Tree<int> tree;
    tree.insert (10);
    tree.insert (20);
    tree.insert (30);

    ASSERT_EQ (*tree.lower_bound (20), 20);

    ASSERT_EQ (*tree.lower_bound (15), 20);

    ASSERT_EQ (tree.lower_bound (40), tree.end());

    rb::Tree<int> empty_tree;
    ASSERT_EQ (empty_tree.lower_bound (5), empty_tree.end());
}

TEST (RBTreeTest, UpperBoundEdgeCases)
{
    rb::Tree<int> tree;
    tree.insert (10);
    tree.insert (20);
    tree.insert (30);

    ASSERT_EQ (*tree.upper_bound (20), 30);

    ASSERT_EQ (*tree.upper_bound (15), 20);

    ASSERT_EQ (tree.upper_bound (30), tree.end());

    rb::Tree<int> empty_tree;
    ASSERT_EQ (empty_tree.upper_bound (5), empty_tree.end());
}

TEST (RBTreeTest, RangeQueries)
{
    rb::Tree<int> tree;
    tree.insert (13);
    tree.insert (27);
    tree.insert (35);
    tree.insert (48);
    tree.insert (59);

    ASSERT_EQ (tree.range_queries_solve (7, 63), 5);

    ASSERT_EQ (tree.range_queries_solve (19, 51), 3);

    ASSERT_EQ (tree.range_queries_solve (13, 59), 5);

    ASSERT_EQ (tree.range_queries_solve (14, 58), 3);

    ASSERT_EQ (tree.range_queries_solve (7, 12), 0);

    ASSERT_EQ (tree.range_queries_solve (61, 67), 0);

    ASSERT_EQ (tree.range_queries_solve (27, 13), 0);
}

TEST (RBTreeTest, RangeQueriesEdgeCases)
{
    rb::Tree<int> tree;

    ASSERT_EQ (tree.range_queries_solve (3, 107), 0);

    tree.insert (53);

    ASSERT_EQ (tree.range_queries_solve (7, 109), 1);

    ASSERT_EQ (tree.range_queries_solve (53, 107), 1);

    ASSERT_EQ (tree.range_queries_solve (3, 53), 1);

    ASSERT_EQ (tree.range_queries_solve (54, 107), 0);

    ASSERT_EQ (tree.range_queries_solve (3, 52), 0);
}

TEST (RBTreeTest, InsertStressTest)
{
    rb::Tree<int> tree;
    const int N = 1007;

    for (int i = 0; i < N; ++i)
        tree.insert (i * 3 + 1);

    ASSERT_EQ (tree.size(), N);

    int count = 0;
    int expected = 1;
    for (auto it = tree.begin(); it != tree.end(); ++it)
    {
        ASSERT_EQ (*it, expected);
        expected += 3;
        count++;
    }

    ASSERT_EQ (count, N);
}

TEST (RBTreeTest, SubtreeSize)
{
    rb::Tree<int> tree;

    std::vector<int> values = {53, 31, 73, 23, 47, 61, 83};
    for (int val : values)
        tree.insert(val);

    ASSERT_EQ (tree.size(), values.size());

    ASSERT_EQ (tree.range_queries_solve (29, 67), 4);
    ASSERT_EQ (tree.range_queries_solve (24, 62), 4);
    ASSERT_EQ (tree.range_queries_solve (75, 90), 1);
}

TEST (RBTreeBigFiveTest, CopyCtor)
{
    rb::Tree<int> orig;

    orig.insert (17);
    orig.insert (29);
    orig.insert (41);
    orig.insert (53);
    orig.insert (67);

    rb::Tree<int> copy(orig);

    ASSERT_EQ (orig.size(), copy.size());
    ASSERT_EQ (orig.size(), 5);

    auto it_orig = orig.begin();
    auto it_copy = copy.begin();
    while (it_orig != orig.end() && it_copy != copy.end())
    {
        ASSERT_EQ (*it_orig, *it_copy);
        ++it_orig;
        ++it_copy;
    }

    ASSERT_EQ (orig.range_queries_solve (20, 60), 3);
    ASSERT_EQ (copy.range_queries_solve (20, 60), 3);

    copy.insert (79);
    ASSERT_EQ (orig.size(), 5);
    ASSERT_EQ (copy.size(), 6);
}

TEST (RBTreeBigFiveTest, MoveCtor)
{
    rb::Tree<int> orig;

    orig.insert(13);
    orig.insert(37);
    orig.insert(59);

    size_t orig_size = orig.size();

    rb::Tree<int> moved (std::move (orig));

    ASSERT_EQ (moved.size(), orig_size);
    ASSERT_EQ (moved.range_queries_solve (10, 70), 3);

    ASSERT_TRUE (orig.empty());
    ASSERT_EQ (orig.size(), 0);

    orig.insert (71);
    ASSERT_EQ (orig.size(), 1);
    ASSERT_EQ (orig.range_queries_solve (70, 80), 1);
}

TEST (RBTreeBigFiveTest, CopyAssignmentOp)
{
    rb::Tree<int> src;
    rb::Tree<int> target;

    src.insert (11);
    src.insert (23);
    src.insert (37);

    target.insert (101);
    target.insert (103);

    target = src;

    ASSERT_EQ (target.size(), src.size());
    ASSERT_EQ (target.size(), 3);

    ASSERT_EQ (target.range_queries_solve (10, 40), 3);
    ASSERT_EQ (target.range_queries_solve (20, 30), 1);

    ASSERT_EQ (src.size(), 3);

    target.insert (47);
    ASSERT_EQ (target.size(), 4);
    ASSERT_EQ (src.size(), 3);
}

TEST (RBTreeBigFiveTest, MoveAssignmentOp)
{
    rb::Tree<int> src;
    rb::Tree<int> target;

    src.insert (19);
    src.insert (31);
    src.insert (43);

    target.insert (201);
    target.insert (203);

    size_t src_size = src.size();

    target = std::move (src);

    ASSERT_EQ (target.size(), src_size);
    ASSERT_EQ (target.range_queries_solve (15, 50), 3);

    ASSERT_TRUE (src.empty());
}

TEST (RBTreeTest, BalanceAfterInserts)
{
    rb::Tree<int> tree;

    for (int i = 1; i <= 31; ++i)
        tree.insert (i);

    ASSERT_EQ (tree.size(), 31);

    int expected = 1;
    for (auto val : tree)
    {
        ASSERT_EQ (val, expected);
        expected++;
    }
}
