#include "rbtree.hpp"

#include <iostream>
#include <set>
#include <vector>
#include <chrono>
#include <sstream>
#include <string>
#include <iomanip>

using Command = std::pair<char, std::pair<int, int>>;
using ll_t = long long;

size_t std_set_range_query (const std::set<int>& s, int low, int high)
{
    if (low > high)
        return 0;

    auto it_low = s.lower_bound (low);
    auto it_high = s.upper_bound (high);

    return std::distance (it_low, it_high);
}

std::vector<Command> parse_commands (const std::string& input)
{
    std::vector<Command> commands;
    std::istringstream isstr (input);
    std::string token;

    while (isstr >> token)
    {
        if (token == "k")
        {
            int key;
            isstr >> key;
            commands.push_back ({'k', {key, 0}});
        }
        else if (token == "q")
        {
            int low, high;
            isstr >> low >> high;
            commands.push_back ({'q', {low, high}});
        }
    }

    return commands;
}

ll_t benchmark_rb_tree (const std::vector<Command>& commands)
{
    rb::Tree<int> tree;

    auto start = std::chrono::high_resolution_clock::now();

    for (const auto& cmd : commands)
    {
        if (cmd.first == 'k')
        {
            tree.insert (cmd.second.first);
        }
        else if (cmd.first == 'q')
        {
            volatile size_t result = tree.range_queries_solve (cmd.second.first, cmd.second.second);
            (void) result;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds> (end - start).count();
}

ll_t benchmark_std_set (const std::vector<Command>& commands)
{
    std::set<int> tree;

    auto start = std::chrono::high_resolution_clock::now();

    for (const auto& cmd : commands)
    {
        if (cmd.first == 'k')
        {
            tree.insert (cmd.second.first);
        }
        else if (cmd.first == 'q')
        {
            volatile size_t result = std_set_range_query (tree, cmd.second.first, cmd.second.second);
            (void) result;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds> (end - start).count();
}

void print_results (ll_t rb_time, ll_t std_time)
{
    std::cout << "rb::Tree:  " << std::setw (10) << rb_time << " mcs\n";
    std::cout << "std::set:  " << std::setw (10) << std_time << " mcs\n" << std::endl;

    double ratio = (std_time > 0) ? static_cast<double> (rb_time) / std_time : 0.0;

    std::cout << "Ratio:     " << std::fixed << std::setprecision (2) << ratio << "x" << std::endl;
}

int main ()
{
    std::string input_line;
    std::getline (std::cin, input_line);

    auto commands = parse_commands (input_line);

    ll_t rb_time = benchmark_rb_tree (commands);
    ll_t std_time = benchmark_std_set (commands);

    print_results (rb_time, std_time);

    return 0;
}
