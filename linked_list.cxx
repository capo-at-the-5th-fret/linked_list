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
        for (auto value : values)
        {
            push_back(value);
        }
    }

    linked_list(const linked_list& other)
    {
        for (auto value : other)
        {
            push_back(value);
        }
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
            for (auto value : other)
            {
                push_back(value);
            }
        }

        return *this;
    }

    linked_list& operator=(std::initializer_list<int> values)
    {
        clear();
        for (auto value : values)
        {
            push_back(value);
        }

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
    // TODO: Use ranges version of equal
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

export std::strong_ordering operator<=>(const linked_list& lhs, const linked_list& rhs)
{
    auto first1 = lhs.begin();
    auto last1 = lhs.end();
    auto first2 = rhs.begin();
    auto last2 = rhs.end();

    bool exhaust1 = (first1 == last1);
    bool exhaust2 = (first2 == last2);
    for (; !exhaust1 && !exhaust2; exhaust1 = (++first1 == last1),
        exhaust2 = (++first2 == last2))
    {
        if (auto c = *first1 <=> *first2; c != 0)
        {
            return c;
        }
    }
 
    return !exhaust1 ? std::strong_ordering::greater:
           !exhaust2 ? std::strong_ordering::less:
                       std::strong_ordering::equal;
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
        // TODO: support for ranges can handle this instead?
        auto out = ctx.out();
        out = std::format_to(out, "[");

        if (auto pos = list.begin(); pos != list.end())
        {
            out = std::format_to(out, "{}", *pos);
            ++pos;

            for (; pos != list.end(); ++pos)
            {
                out = std::format_to(out, ", {}", *pos);
            }
        }

        return std::format_to(out, "]");
    }
};
