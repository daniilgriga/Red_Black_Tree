#pragma once

#include <iostream>

namespace rb
{
    template<typename T>
    class Tree
    {
        enum class Dir { LEFT, RIGHT };

        class Node
        {
        private:
            enum class Color { RED, BLACK };

            T data_;
            Color color_;
            Node* left_;
            Node* right_;
            Node* parent_;
            size_t subtree_size_;

        public:
            template<typename K>
            explicit Node (const K& data, Color c = Color::RED) :
                data_ (data),
                color_ (c),
                left_ (nullptr),
                right_ (nullptr),
                parent_ (nullptr),
                subtree_size_ (1) {}

            template<typename K>
            explicit Node (K&& data, Color c = Color::RED) :
                data_ (std::move(data)),
                color_ (c),
                left_ (nullptr),
                right_ (nullptr),
                parent_ (nullptr),
                subtree_size_ (1) {}

            Node (const Node& oth) :
                data_ (oth.data_),
                color_ (oth.color_),
                left_ (nullptr),
                right_ (nullptr),
                parent_ (nullptr),
                subtree_size_ (oth.subtree_size_) {}

            const T& data() const { return data_; }
            T& data() { return data_; }

            Color color() const { return color_; }
            void set_color (Color color) { color_ = color; }

            const Node* left() const { return left_; }
            Node* left() { return left_;  }
            void set_left (Node* left) { left_ = left; }

            const Node* right() const { return right_; }
            Node* right() { return right_; }
            void set_right (Node* right) { right_ = right; }

            const Node* parent() const { return parent_; }
            Node* parent() { return parent_; }
            void set_parent (Node* parent) { parent_ = parent; }

            size_t subtree_size() const { return subtree_size_; }
            void set_subtree_size (size_t s_size) { subtree_size_ = s_size; }

        }; // class Node

    }; // class Tree

} // namespace rb
