#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <iterator>
#include <cstddef>
#include <cassert>

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

            void upd_subtree_size()
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
                subtree_size_ (1) { upd_subtree_size(); }

            explicit Node (T&& data, Color c = Color::RED,
                                          Node* left = nullptr,
                                          Node* right = nullptr,
                                          Node* parent = nullptr) :
                data_ (std::move(data)),
                color_ (c),
                left_ (left),
                right_ (right),
                parent_ (parent),
                subtree_size_ (1) { upd_subtree_size(); }

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

            Node* grandparent() const
            {
                return (parent_) ? parent_->parent_ : nullptr;
            }

            Node* uncle() const
            {
                Node* gp = grandparent();
                if (gp == nullptr) return nullptr;

                return (parent_ == gp->left_) ? gp->right_ : gp->left_;
            }

            Node* sibling() const
            {
                if (parent_ == nullptr)
                    return nullptr;

                return (this == parent_->left_) ? parent_->right_ : parent_->left_;
            }

            bool is_left_child() const
            {
                return (parent_ && this == parent_->left_);
            }

            bool is_right_child() const
            {
                return (parent_ && this == parent_->right_);
            }

            friend class Tree;
        }; // class Node

        enum class Dir { LEFT, RIGHT };

        Node* root_;
        size_t size_;

    public:
        class Iterator
        {
        private:
            Iterator(const Tree* owner, Node* current)
                : owner_(owner), curr_(current) {}

            const Tree* owner_ = nullptr;
            Node* curr_ = nullptr;

        public:
            // ==== Type Traits ==== //
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using reference = const T&;
            using pointer = const T*;
            using iterator_category = std::bidirectional_iterator_tag;
            // ===================== //

            Iterator() = default;

            reference operator*() const
            {
                assert (curr_ != nullptr);
                return curr_->data();
            }

            pointer operator->() const
            {
                return &(*(*this));
            }

            Iterator& operator++()
            {
                assert (curr_ != nullptr);
                curr_ = owner_->next_node (curr_);

                return *this;
            }

            Iterator operator++ (int)
            {
                Iterator dumb = *this;
                ++(*this);

                return dumb;
            }

            Iterator& operator--()
            {
                curr_ = (curr_ != nullptr) ? owner_->prev_node (curr_)
                                           : owner_->max_node (owner_->root_);
                assert (curr_ != nullptr);

                return *this;
            }

            Iterator operator-- (int)
            {
                Iterator dumb = *this;
                --(*this);

                return dumb;
            }

            bool operator== (const Iterator& rht_sd) const
            {
                assert (owner_ == rht_sd.owner_);
                return curr_ == rht_sd.curr_;
            }

            bool operator!= (const Iterator& rht_sd) const
            {
                return !(*this == rht_sd);
            }

            friend class Tree;
        }; // class Iterator

        // for STL
        using iterator = Iterator;
        using const_iterator = Iterator;
        using value_type = T;

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

        Iterator begin() const
        {
            return Iterator (this, min_node());
        }

        Iterator end() const
        {
            return Iterator (this, nullptr);
        }

        Iterator lower_bound (const T& key) const
        {
            return Iterator (this, find_lower_bound (key));
        }

        Iterator upper_bound (const T& key) const
        {
            return Iterator (this, find_upper_bound (key));
        }

        size_t range_queries_solve (const T& low, const T& high) const
        {
            if (low > high)
                return 0;

            Iterator it_low = lower_bound (low);
            Iterator it_high = upper_bound (high);

            size_t count = 0;

            while (it_low != it_high)
            {
                count++;
                it_low++;
            }

            return count;
        }

        bool  empty() const noexcept { return size_ == 0; }
        size_t size() const noexcept { return size_; }

        void clear() const
        {
            clear_tree (root_);
        }

        void insert (const T& data)
        {
            Node* new_node = insert_data (data);
            if (new_node != nullptr)
            {
                fix_insert (new_node);
                update_sizes (new_node);
            }
        }

        void save_dot_to_file (const std::string& filename) const
        {
            std::ofstream file (filename);
            file << make_dot();
            file.close();
            std::cout << "[.dot file saved]: " << filename << std::endl;
        }

    private:
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

            new_node->upd_subtree_size();

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

            std::vector<Node*> vec;
            vec.push_back (node);

            while (!vec.empty())
            {
                Node* current = vec.back();
                vec.pop_back();

                Node* left = current->left();
                if (left)
                    vec.push_back (left);

                Node* right = current->right();
                if (right)
                    vec.push_back (right);

                delete current;
            }
        }

        Node* insert_data (const T& data)
        {
            Node* new_node = new Node (data);

            if (root_ == nullptr)
            {
                root_ = new_node;
                root_->set_color (Node::Color::BLACK);
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

        void rotate (Node* node, Dir dir)
        {
            if (node == nullptr)
                return;

            Node* pivot = (dir == Dir::LEFT) ? node->right() : node->left();
            if (pivot == nullptr)
                return;

            pivot->set_parent (node->parent());
            if (node->parent() != nullptr)
            {
                if (node->parent()->left() == node)
                    node->parent()->set_left (pivot);
                else
                    node->parent()->set_right (pivot);
            }
            else
            {
                root_ = pivot;
            }

            if (dir == Dir::LEFT)
            {
                node->set_right (pivot->left());
                if (pivot->left() != nullptr)
                    pivot->left()->set_parent (node);

                pivot->set_left (node);
            }
            else
            {
                node->set_left (pivot->right());
                if (pivot->right() != nullptr)
                    pivot->right()->set_parent (node);

                pivot->set_right (node);
            }

            node->set_parent (pivot);

            node->upd_subtree_size();
            pivot->upd_subtree_size();
        }

        void update_sizes (Node* node)
        {
            while (node != nullptr)
            {
                node->upd_subtree_size();
                node = node->parent();
            }
        }

        void rotate_left  (Node* node) { rotate (node, Dir::LEFT); }
        void rotate_right (Node* node) { rotate (node, Dir::RIGHT); }

        void fix_insert (Node* node)
        {
            fix_insert_root (node);
        }

        void fix_insert_root (Node* node)
        {
            if (node->parent() == nullptr)
            {
                node->set_color (Node::Color::BLACK);
                root_ = node;
                return;
            }

            fix_insert_black_parent (node);
        }

        void fix_insert_black_parent (Node* node)
        {
            Node* parent = node->parent();
            if (parent == nullptr || parent->is_black())
                return;

            fix_insert_red_uncle (node);
        }

        void fix_insert_red_uncle (Node* node)
        {
            Node* uncle = node->uncle();

            if (uncle != nullptr && uncle->is_red())
            {
                node->parent()->set_color (Node::Color::BLACK);
                uncle->set_color (Node::Color::BLACK);

                Node* gp = node->grandparent();
                if (gp != nullptr)
                {
                    gp->set_color (Node::Color::RED);
                    fix_insert_root (gp);
                }
            }
            else
            {
                fix_insert_zigzag_case (node);
            }
        }

        void fix_insert_zigzag_case (Node* node)
        {
            Node* parent = node->parent();
            Node* gp = node->grandparent();
            if (parent == nullptr || gp == nullptr)
                return;

            if (node->is_right_child() && parent->is_left_child())
            {
                rotate_left (parent);
                node = node->left();
            }
            else if (node->is_left_child() && parent->is_right_child())
            {
                rotate_right (parent);
                node = node->right();
            }

            fix_insert_straight_case (node);
        }

        void fix_insert_straight_case (Node* node)
        {
            Node* parent = node->parent();
            Node* gp = node->grandparent();
            if (parent == nullptr || gp == nullptr)
                return;

            parent->set_color (Node::Color::BLACK);
            gp->set_color (Node::Color::RED);

            if (node->is_left_child() && parent->is_left_child())
                rotate_right (gp);
            else if (node->is_right_child() && parent->is_right_child())
                rotate_left (gp);
        }

        void generate_dot_node (std::stringstream& sstr, const Node* node) const
        {
            if (node == nullptr)
                return;

            std::string node_color = node->is_red() ? "red" : "black";
            std::string fill_color = node->is_red() ? "#FF4444" : "#333333";
            std::string font_color = "white";

            sstr << "    \"" << node << "\" [label=\"" << node->data()
                 << "\\n(size=" << node->subtree_size() << ")\", style=filled, fillcolor=\""
                 << fill_color << "\", fontcolor=" << font_color << "];\n";

            if (node->left())
            {
                sstr << "    \"" << node << "\" -> \"" << node->left() << "\" [label=\"L\"];\n";
                generate_dot_node (sstr, node->left());
            }
            else
            {
                sstr << "    \"null_" << node << "_left\" [shape=point, width=0.2];\n";
                sstr << "    \"" << node << "\" -> \"null_" << node << "_left\" [style=dashed];\n";
            }

            if (node->right())
            {
                sstr << "    \"" << node << "\" -> \"" << node->right() << "\" [label=\"R\"];\n";
                generate_dot_node (sstr, node->right());
            }
            else
            {
                sstr << "    \"null_" << node << "_right\" [shape=point, width=0.2];\n";
                sstr << "    \"" << node << "\" -> \"null_" << node << "_right\" [style=dashed];\n";
            }
        }

        std::string make_dot() const
        {
            std::stringstream sstr;

            sstr << "digraph RBTree {\n";
            sstr << "    node [fontname=\"Arial\", shape=circle, width=1.5, height=1.5];\n";
            sstr << "    edge [arrowhead=normal];\n\n";

            if (root_ != nullptr)
                generate_dot_node (sstr, root_);
            else
                sstr << "    \"empty\" [label=\"Empty Tree\"];\n";

            sstr << "}\n";

            return sstr.str();
        }

        Node* min_node (Node* node) const
        {
            while (node != nullptr && node->left() != nullptr)
                node = node->left();

            return node;
        }

        Node* max_node (Node* node) const
        {
            while (node != nullptr && node->right() != nullptr)
                node = node->right();

            return node;
        }

        Node* min_node() const { return min_node (root_); }
        Node* max_node() const { return max_node (root_); }

        Node* next_node (Node* node) const
        {
            if (node == nullptr)
                return nullptr;

            if (node->right() != nullptr)
                return min_node (node->right());

            Node* parent = node->parent();
            while (parent != nullptr && node == parent->right())
            {
                node = parent;
                parent = parent->parent();
            }

            return parent;
        }

        Node* prev_node (Node* node) const
        {
            if (node == nullptr)
                return nullptr;

            if (node->left() != nullptr)
                return max_node (node->left());

            Node* parent = node->parent();
            while (parent != nullptr && node == parent->left())
            {
                node = parent;
                parent = parent->parent();
            }

            return parent;
        }

        enum class BoundType { LOWER, UPPER };

        Node* find_bound (const T& key, BoundType type) const
        {
            Node* curr = root_;
            Node* target = nullptr;

            while (curr != nullptr)
            {
                bool cond = (type == BoundType::LOWER)
                          ? (key <= curr->data_)
                          : (key <  curr->data_);
                if (cond)
                {
                    target = curr;
                    curr = curr->left();
                }
                else
                {
                    curr = curr->right();
                }
            }

            return target;
        }

        Node* find_lower_bound (const T& key) const
        {
            return find_bound (key, BoundType::LOWER);
        }

        Node* find_upper_bound (const T& key) const
        {
            return find_bound (key, BoundType::UPPER);
        }
    }; // class Tree

} // namespace rb
