#include <iostream>
#include "list.h"
#include <iterator>

int main() {

    list <int> a, b;
    b.push_back(1);
    b.push_back(2);
    b.push_back(3);
    b.push_back(4);
    b.push_back(5);

    b.front() = 6;
    b.back() = 7;

    for (list<int>::iterator it = b.begin(); it != b.end(); ++it)
        std::cout << *it << ' ';

    std::initializer_list<int> elems = {6, 2, 3, 4, 7};
    auto i2 = b.begin(), e2 = b.end();
    auto i1 = elems.begin(), e1 = elems.end();

    for (;;)
    {
        if (i1 == e1 || i2 == e2)
        {
            std::cerr << (i1 == e1 && i2 == e2) << "\n";
            break;
        }

        std::cerr << (*i2 == *i1) << "!\n";
        ++i1;
        ++i2;
    }
    return 0;
}