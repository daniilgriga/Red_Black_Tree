#pragma once

#include <iostream>
#include <vector>

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

            void update_subtree_size()
            {
                subtree_size_ = 1 +
                    (left_ ? left_->subtree_size_ : 0) +
                    (right_ ? right_->subtree_size_ : 0);
            }

        public:
            explicit Node (const T& data, Color c = Color::RED,
                                          Node* left = nullptr,
                                          Node* right = nullptr,
                                          Node* parent = nullptr) :
                data_ (data),
                color_ (c),
                left_ (left),
                right_ (right),
                parent_ (parent),
                subtree_size_ (1) { update_subtree_size(); }

            explicit Node (T&& data, Color c = Color::RED,
                                          Node* left = nullptr,
                                          Node* right = nullptr,
                                          Node* parent = nullptr) :
                data_ (std::move(data)),
                color_ (c),
                left_ (left),
                right_ (right),
                parent_ (parent),
                subtree_size_ (1) { update_subtree_size(); }

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

            friend class Tree;
        }; // class Node

        enum class Dir { LEFT, RIGHT };

        Node* root_;
        size_t size_;

        Node* copy_subtree (const Node* node, Node* parent)
        {
            if (node == nullptr)
                return nullptr;

            Node* new_node = new Node (node->data(), node->color(), nullptr, nullptr, parent);

            Node* left_child = copy_subtree (node->left(), new_node);
            new_node->set_left (left_child);
            if (left_child != nullptr)
                left_child->set_parent (new_node);

            Node* right_child = copy_subtree (node->right(), new_node);
            new_node->set_right (right_child);
            if (right_child != nullptr)
                right_child->set_parent (new_node);

            new_node->update_subtree_size();

            return new_node;
        }

        void swap (Tree& oth) noexcept
        {
            std::swap (root_, oth.root_);
            std::swap (size_, oth.size_);
        }

        void clear_tree (Node* node) noexcept
        {
            if (node == nullptr)
                return;

            std::vector<Node*> stack;
            stack.push_back (node);

            while (!stack.empty())
            {
                Node* current = stack.back();
                stack.pop_back();

                Node* left = current->left();
                if (left)
                    stack.push_back (left);

                Node* right = current->right();
                if (right)
                    stack.push_back (right);

                delete current;
            }
        }

        Node* insert_data (const T& data)
        {
            Node* new_node = new Node (data);

            if (root_ == nullptr)
            {
                root_ = new_node;
                root_->set_color(Node::Color::BLACK);
                size_ = 1;
                return new_node;
            }

            Node* curr = root_;
            Node* parent = nullptr;

            while (curr != nullptr)
            {
                parent = curr;
                if (new_node->data() < curr->data())
                {
                    curr = curr->left();
                }
                else if (new_node->data() > curr->data())
                {
                    curr = curr->right();
                }
                else
                {
                    delete new_node;
                    return curr;
                }
            }

            new_node->set_parent (parent);
            if (new_node->data() < parent->data())
                parent->set_left (new_node);
            else
                parent->set_right (new_node);

            size_++;
            return new_node;
        }

    public:
        Tree() : root_(nullptr), size_(0) {};

        ~Tree() { clear_tree (root_); }

        Tree(const Tree& oth)
            : root_(copy_subtree (oth.root_, nullptr)),
              size_(oth.size_) {}

        Tree(Tree&& oth) noexcept
            : root_(oth.root_), size_(oth.size_)
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

        bool  empty() const noexcept { return size_ == 0; }
        size_t size() const noexcept { return size_; }

        void clear() const
        {
            clear_tree (root_);
        }

        void insert (const T& data)
        {
            insert_data (data);
        }

    }; // class Tree

} // namespace rb
