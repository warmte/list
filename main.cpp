#include <iostream>
#include "list.h"
#include <iterator>
#include "counted.h"

template <typename T>
T const& as_const(T& obj)
{
    return obj;
}

int main() {

    list <int> a;
    list<int>::iterator it = a.begin();
    bool fl = as_const(it) == it;
    return 0;
}