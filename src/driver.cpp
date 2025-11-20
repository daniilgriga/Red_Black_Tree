#include "rbtree.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

namespace rb_app
{
    void process_insert (std::istringstream& isstr, rb::Tree<int>& tree)
    {
        int key;
        if (isstr >> key)
            tree.insert(key);
    }

    void process_query (std::istringstream& isstr,
                         rb::Tree<int>& tree,
                        std::vector<size_t>& results)
    {
        int low = 0;
        int high = 0;

        if (isstr >> low >> high)
        {
            size_t count = tree.range_queries_solve (low, high);
            results.push_back (count);
        }
    }

    void process_token (const std::string& token, std::istringstream& isstr,
                        rb::Tree<int>& tree, std::vector<size_t>& results)
    {
        if (token == "k")
        {
            process_insert (isstr, tree);
        }
        else if (token == "q")
        {
            process_query (isstr, tree, results);
        }
        else
            assert (0 && "Use k for insert and q for query");
    }

    std::vector<size_t> process_input (const std::string& input)
    {
        rb::Tree<int> tree;
        std::vector<size_t> results;

        std::istringstream isstr (input);
        std::string token;

        while (isstr >> token)
            process_token (token, isstr, tree, results);

        return results;
    }

    void print_results (const std::vector<size_t>& results)
    {
        for (size_t i = 0; i < results.size(); ++i)
        {
            std::cout << results[i];

            if (i != results.size() - 1)
                std::cout << " ";
        }

        std::cout << std::endl;
    }
} // namespace rb_app

int main ()
{
    std::string input_line;
    std::getline (std::cin, input_line);

    auto results = rb_app::process_input (input_line);

    rb_app::print_results (results);

    return 0;
}
