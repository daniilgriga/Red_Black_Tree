#include "benchmark.hpp"

#include <set>
#include <iostream>
#include <string>

int main ()
{
    std::string input_line;
    std::getline (std::cin, input_line);

    auto commands = benchmark::parse_commands (input_line);

    benchmark::StdSetAdapter<std::set<int>> adapter;

    long long time_mcs = benchmark::run_benchmark (commands, adapter);

    std::cout << time_mcs << std::endl;

    return 0;
}
