#include "rbtree.hpp"
#include "processor.hpp"
#include <gtest/gtest.h>

TEST (RQTest, SampleInputFromAssignment)
{
    std::string input ("k 10 k 20 q 8 31 q 6 9 k 30 k 40 q 15 40\n");
    auto output = rb_app::process_input (input);

    std::vector<size_t> expected = {2, 0, 3};
    EXPECT_EQ (output, expected);
}

TEST (RQTest, ComplexInputWithInvalidRanges)
{
    std::string input = "k 5 k 8 k 3 k 1 k 9 q 2 7 q 10 5 q 4 4 q 0 10 q 6 6";
    auto output = rb_app::process_input (input);

    std::vector<size_t> expected = {2, 0, 0, 5, 0};
    EXPECT_EQ (output, expected);
}

TEST (RQTest, EmptyInput)
{
    std::string input = "";
    auto output = rb_app::process_input (input);

    std::vector<size_t> expected = {};
    EXPECT_EQ (output, expected);
}

TEST (RQTest, OnlyInsertions)
{
    std::string input = "k 1 k 2 k 3 k 4 k 5";
    auto output = rb_app::process_input (input);

    std::vector<size_t> expected = {};
    EXPECT_EQ (output, expected);
}

TEST (RQTest, OnlyQueriesOnEmptyTree)
{
    std::string input = "q 1 10 q 5 15 q 0 100";
    auto output = rb_app::process_input (input);

    std::vector<size_t> expected = {0, 0, 0};
    EXPECT_EQ (output, expected);
}

TEST (RQTest, InvalidCommands)
{
    std::string input = "k 10 x 5 q 1 10 k 20 q 1 20";
    auto output = rb_app::process_input (input);

    std::vector<size_t> expected = {1, 2};
    EXPECT_EQ (output, expected);
}

TEST (RQTest, LargeInput)
{
    std::string input;
    for (int i = 1; i <= 100; ++i)
        input += "k " + std::to_string(i) + " ";

    input += "q 1 100";

    auto output = rb_app::process_input (input);

    std::vector<size_t> expected = {100};
    EXPECT_EQ (output, expected);
}

TEST (RQTest, BoundaryValues)
{
    std::string input = "k 0 k 100 k 50 q 0 100 q 1 99 q 0 0";
    auto output = rb_app::process_input (input);

    std::vector<size_t> expected = {3, 1, 0};
    EXPECT_EQ (output, expected);
}
