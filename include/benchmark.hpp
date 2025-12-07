#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <utility>

namespace benchmark
{

    using Command = std::pair<char, std::pair<int, int>>;

    inline std::vector<Command> parse_commands (const std::string& input)
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

    template <typename TreeType>
    class TreeAdapter
    {
    public:
        virtual ~TreeAdapter () = default;
        virtual void insert (int key) = 0;
        virtual size_t range_query (int low, int high) = 0;
    };

    template <typename T>
    class RBTreeAdapter : public TreeAdapter<T>
    {
    private:
        T tree_;

    public:
        void insert (int key) override
        {
            tree_.insert (key);
        }

        size_t range_query (int low, int high) override
        {
            return tree_.range_queries_solve  (low, high);
        }
    };

    template <typename T>
    class StdSetAdapter : public TreeAdapter<T>
    {
    private:
        T tree_;

    public:
        void insert (int key) override
        {
            tree_.insert (key);
        }

        size_t range_query (int low, int high) override
        {
            if (low > high)
                return 0;

            auto it_low = tree_.lower_bound (low);
            auto it_high = tree_.upper_bound (high);

            return std::distance (it_low, it_high);
        }
    };

    template <typename TreeAdapter>
    long long run_benchmark (const std::vector<Command>& commands, TreeAdapter& adapter)
    {
        auto start = std::chrono::high_resolution_clock::now();

        for (const auto& cmd : commands)
        {
            if (cmd.first == 'k')
            {
                adapter.insert (cmd.second.first);
            }
            else if (cmd.first == 'q')
            {
                volatile size_t result = adapter.range_query (cmd.second.first, cmd.second.second);
                (void) result;
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }

} // namespace benchmark
