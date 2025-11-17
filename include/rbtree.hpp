#pragma once

#include <iostream>

namespace rb
{
    template<typename T>
    class Tree
    {
    private:

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
            bool is_red() const { return color_ == Color::RED; }
            bool is_black() const { return color_ == Color::BLACK; }

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

        enum class Dir { LEFT, RIGHT };

        Node* root_;
        size_t size_;

    public:
        Tree() : root_(nullptr), size_(0) {};

        // ~Tree() { clear_tree (root_); }

        Tree(const Tree& oth) : root_(nullptr)
        {
            // root_ = copy_subtree (oth, nullptr);
        }

        Tree(Tree&& oth) noexcept
            : root_(oth.root_) , size_(oth.size_)
        {
            oth.root_ = nullptr;
            oth.size_ = 0;
        }

        Tree& operator= (const Tree& oth)
        {
            Tree temp (oth);
            swap (temp);

            return *this;
        }

        Tree& operator= (Tree&& oth) noexcept
        {
            swap (oth);
            return *this;
        }

        void swap (Tree& oth) noexcept
        {
            std::swap (root_, oth.root_);
            std::swap (size_, oth.size_);
        }

    }; // class Tree

} // namespace rb
