import std;
import linked_list;

int main()
{
    linked_list list;
    std::println("default constructed list -> {}", list);

    list.push_back(1);
    std::println("push 1 to back -> {}", list);

    list.push_back(2);
    std::println("push 2 to back -> {}", list);

    list.insert(list.begin(), 0);
    std::println("insert 0 at front -> {}", list);

    list.insert(list.end(), 3);
    std::println("insert 3 at end -> {}", list);

    auto pos = std::next(list.begin(), 2);
    list.insert(pos, 4);
    std::println("insert 4 at (begin + 2) -> {}", list);

    linked_list list2{ list };
    std::println("2nd list initialized from list 1 -> {} vs {}", list2, list);

    list.erase(list.begin());
    std::println("erase from front -> {}", list);

    pos = std::next(list.begin());
    list.erase(pos);
    std::println("erase from (begin + 1) -> {}", list);

    pos = std::next(list.begin(), 2);
    list.erase(pos);
    std::println("erase from (begin + 2) -> {}", list);

    list.clear();
    std::println("clear list -> {}", list);

    linked_list list3{ 0, 2, 4, 5, 7 };
    std::println("initializer_list constructed list of 5 -> {}", list3);

    list3 = { 1, 3, 5 };
    std::println("assign from initializer list of 3 -> {}", list3);

    {
        const linked_list a = { 1, 3, 5 };

        std::vector<std::tuple<linked_list, std::strong_ordering>> tests =
        {
            { { }, std::strong_ordering::greater },
            { { 0 }, std::strong_ordering::greater },
            { { 1 }, std::strong_ordering::greater },
            { { 2 }, std::strong_ordering::less },
            { { 0, 3 }, std::strong_ordering::greater },
            { { 1, 2 }, std::strong_ordering::greater },
            { { 1, 3 }, std::strong_ordering::greater },
            { { 1, 4 }, std::strong_ordering::less },
            { { 2, 3 }, std::strong_ordering::less },
            { { 0, 3, 5 }, std::strong_ordering::greater },
            { { 1, 2, 5 }, std::strong_ordering::greater },
            { { 1, 3, 4 }, std::strong_ordering::greater },
            { { 1, 3, 5 }, std::strong_ordering::equal },
            { { 1, 3, 6 }, std::strong_ordering::less },
            { { 1, 4, 5 }, std::strong_ordering::less },
            { { 2, 3, 5 }, std::strong_ordering::less },
            { { 0, 3, 5, 0 }, std::strong_ordering::greater },
            { { 1, 2, 5, 0 }, std::strong_ordering::greater },
            { { 1, 3, 4, 0 }, std::strong_ordering::greater },
            { { 1, 3, 5, 0 }, std::strong_ordering::less },
            { { 1, 3, 6, 0 }, std::strong_ordering::less },
            { { 1, 4, 5, 0 }, std::strong_ordering::less },
            { { 2, 3, 5, 0 }, std::strong_ordering::less }
        };

        for (int i = 0; const auto& [test_value, expected] : tests)
        {
            {
                const bool result = (a == test_value);
                if (result != (expected == std::strong_ordering::equal))
                {
                    std::println("== fail: {}", i);
                    //std::println("expected: {}, actual: {} ({})",
                    //  expected, result, i);
                }
                i++;
            }

            {
                const auto result = (a <=> test_value);
                if (result != expected)
                {
                    std::println("<=> fail: {}", i);
                    //std::println("expected: {}, actual: {} ({})",
                    //  expected, result, i);
                }
                i++;
            }
        }
    }
}
