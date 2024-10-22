export module linked_list;

import std;

// NOTE: This is not as efficient as std::forward_list, which uses
// functions like before_begin() and insert_after() to help insert
// elements without having to traverse nodes to find the insertion
// point.
export class linked_list
{
public:
    linked_list() = default;

    linked_list(std::initializer_list<int> values)
    {
        insert(end(), values.begin(), values.end());
    }

    linked_list(const linked_list& other)
    {
        insert(end(), other.begin(), other.end());
    }

    ~linked_list()
    {
        clear();
    }
   
    linked_list& operator=(const linked_list& other)
    {
        if (this != &other)
        {
            clear();
            insert(end(), other.begin(), other.end());
        }

        return *this;
    }

    linked_list& operator=(std::initializer_list<int> values)
    {
        clear();
        insert(end(), values.begin(), values.end());
        return *this;
    }

    class list_node;

    class iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = int;
        using difference_type = std::ptrdiff_t;
        using pointer = int*;
        using reference = int&;

        using node_pointer = list_node*;

        iterator() = default;

        iterator(node_pointer n) : node_{ n }
        {
        }

        bool operator==(const iterator& other) const = default;

        reference operator*() const
        {
            return node_->value;
        }

        pointer operator->() const
        {
            return &(node_->value);
        }

        iterator& operator++()
        {
            if (node_ != nullptr)
            {
                node_ = node_->next;
            }
            return *this;
        }

        iterator operator++(int)
        {
            iterator temp{ *this };
            ++(*this);
            return temp;
        }

        node_pointer node() const
        {
            return node_;
        }

    private:
        node_pointer node_{ nullptr };
    };

    class const_iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = const int;
        using difference_type = std::ptrdiff_t;
        using pointer = const int*;
        using reference = const int&;

        using node_pointer = const list_node*;

        const_iterator() = default;

        const_iterator(node_pointer n) : node_{ n }
        {
        }

        bool operator==(const const_iterator& other) const = default;

        reference operator*() const
        {
            return node_->value;
        }

        pointer operator->() const
        {
            return &(node_->value);
        }

        const_iterator& operator++()
        {
            if (node_ != nullptr)
            {
                node_ = node_->next;
            }
            return *this;
        }

        const_iterator operator++(int)
        {
            const_iterator temp{ *this };
            ++(*this);
            return temp;
        }

        node_pointer node() const
        {
            return node_;
        }

    private:
        node_pointer node_{ nullptr };
    };

    iterator begin()
    {
        return iterator{ head_ };
    }

    iterator end()
    {
        return iterator{ nullptr };
    }

    const_iterator begin() const
    {
        return const_iterator{ head_ };
    }

    const_iterator end() const
    {
        return const_iterator{ nullptr };
    }

    bool empty() const
    {
        return head_ == nullptr;
    }

    std::size_t size() const
    {
        return size_;
    }

    void clear()
    {
        auto* node = head_;
        head_ = nullptr;
        size_ = 0;

        while (node != nullptr)
        {
            auto* temp = node;
            node = node->next;
            delete temp;
        }
    }

    iterator insert(iterator pos, int value)
    {
        auto* new_node = new list_node
        {
            .value = value,
            .next = nullptr
        };

        // the new node is the new head_
        if (pos == begin())
        {
            new_node->next = head_;
            head_ = new_node;
            ++size_;
            return begin();
        }
        else
        {
            for (auto current_pos = begin(); current_pos != end();
                ++current_pos)
            {
                // next is the insert position; insert the new node in between
                // the current and the next
                if (current_pos.node()->next == pos.node())
                {
                    new_node->next = current_pos.node()->next;
                    current_pos.node()->next = new_node;
                    ++size_;
                    return current_pos++;
                }
            }
        }

        // should never happen
        return end();
    }

    template <std::input_iterator InputIt>
    requires (std::same_as<std::iter_value_t<InputIt>, int>)
    iterator insert(iterator pos, InputIt first, InputIt last)
    {
        if (first == last)
        {
            return pos;
        }

        // construct the new nodes, keeping track of the first, last and count
        auto* first_node = new list_node{ .value = *first, .next = nullptr };
        auto* last_node = first_node;
        std::size_t new_node_count{ 1 };
        
        for (auto pos = std::next(first); pos != last; ++pos)
        {
            auto* new_node = new list_node{ .value = *pos, .next = nullptr };
            last_node->next = new_node;
            last_node = new_node;
            ++new_node_count;
        }

        // the new nodes are inserted at the front
        if (pos == begin())
        {
            last_node->next = head_;
            head_ = first_node;
            size_ += new_node_count;
            return begin();
        }
        else
        {
            for (auto current_pos = begin(); current_pos != end();
                ++current_pos)
            {
                // next is the insert position; insert the new nodes in between
                // the current and the next
                if (current_pos.node()->next == pos.node())
                {
                    last_node->next = current_pos.node()->next;
                    current_pos.node()->next = first_node;
                    size_ += new_node_count;
                    return current_pos++;
                }
            }
        }

        // should never happen
        return end();
    }

    iterator erase(iterator pos)
    {
        // erase the head
        if (pos == begin())
        {
            auto* temp = head_;
            head_ = temp->next;
            delete temp;
            --size_;
            return begin();
        }
        else
        {
            for (auto current_pos = begin(); current_pos != end();
                ++current_pos)
            {
                // next is the erase position; delete the next, then set the
                // current next to the next of the next
                if (current_pos.node()->next == pos.node())
                {
                    auto* temp = current_pos.node()->next;
                    current_pos.node()->next = temp->next;
                    delete temp;
                    --size_;
                    return current_pos++;
                }
            }
        }

        return end();
    }

    void push_back(int value)
    {
        insert(end(), value);
    }

    void push_front(int value)
    {
        insert(begin(), value);
    }

private:
    struct list_node
    {
        int value{ 0 };
        list_node* next{ nullptr };
    };

    list_node* head_{ nullptr };
    std::size_t size_{ 0 };
};

export bool operator==(const linked_list& lhs, const linked_list& rhs)
{
    return std::ranges::equal(lhs, rhs);
}

export auto operator<=>(const linked_list& lhs, const linked_list& rhs)
{
    return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(),
        rhs.begin(), rhs.end());
}

export template <>
struct std::formatter<linked_list>
{
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const linked_list& list, FormatContext& ctx) const
    {
        // TODO: Use range formatter?
        auto out = ctx.out();
        out = std::format_to(out, "[");

        auto pos = list.begin();

        if (pos != list.end())
        {
            out = std::format_to(out, "{}", *pos);
            ++pos;
        }

        for (; pos != list.end(); ++pos)
        {
            out = std::format_to(out, ", {}", *pos);
        }

        return std::format_to(out, "]");
    }
};
