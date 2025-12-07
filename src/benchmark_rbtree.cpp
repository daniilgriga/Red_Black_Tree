#include "rbtree.hpp"
#include "benchmark.hpp"

#include <iostream>
#include <string>

int main ()
{
    std::string input_line;
    std::getline (std::cin, input_line);

    auto commands = benchmark::parse_commands (input_line);

    benchmark::RBTreeAdapter<rb::Tree<int>> adapter;

    long long time_mcs = benchmark::run_benchmark (commands, adapter);

    std::cout << time_mcs << std::endl;

    return 0;
}
