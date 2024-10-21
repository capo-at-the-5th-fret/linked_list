import std;
import linked_list;

int main()
{
    linked_list list;
    list.print();
    //assert(list.empty());

    list.push_back(1);
    list.print();
    //assert(list.empty() == false);

    list.push_back(2);
    list.print();

    list.insert(list.begin(), 0);
    list.print();

    list.insert(list.end(), 3);
    list.print();

    auto pos = list.begin();
    pos++;
    pos++;

    list.insert(pos, 4);
    list.print();

    linked_list list2{ list };
    list2.print();

    list.erase(list.begin());
    list.print();

    pos = list.begin();
    pos++;
    list.erase(pos);
    list.print();

    pos = list.begin();
    pos++;
    pos++;
    list.erase(pos);
    list.print();

    list.clear();
    list.print();

    linked_list list3 = { 2, 4, 6 };
    linked_list list4{ list3 };
    list4.print();

    linked_list list5;
    list5 = { 1, 3, 5 };
    list5.print();

    {
        const linked_list a = { 1, 3, 5 };

        std::vector<std::tuple<linked_list, std::strong_ordering>> tests =
        {
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
