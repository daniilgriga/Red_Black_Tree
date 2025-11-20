#include "rbtree.hpp"
#include "processor.hpp"
#include <iostream>

int main ()
{
    std::string input_line;
    std::getline (std::cin, input_line);

    auto results = rb_app::process_input (input_line);

    rb_app::print_results (results);

    return 0;
}
