//
// Created by 吴凡 on 2017/3/29.
//

#include "stl_allocator.h"
#include <vector>
#include <iostream>
#include <type_traits>
#include <string>

template<class T>
using StlVector = std::vector<T, stl::Allocator<T>>;

struct Foo {
    int i_;
    std::string s_;
    Foo(int i = 0, std::string s = std::string()): i_{i}, s_{std::move(s)} {
        std::cout << "Constructor Foo\n";
    }
};

std::ostream& operator<<(std::ostream& out, const Foo& f) {
    out << f.i_ << " " << f.s_;
    return out;
}

template<class T, typename = void>
class has_ostream_operator : public std::false_type {};
template<class T>
class has_ostream_operator<T, decltype(void(std::declval<std::ostream&>() << std::declval<const T&>()))> : public std::true_type {};


template<class Iterator>
using iterator_value_type = typename std::iterator_traits<Iterator>::value_type;

template<class Iterator>
typename std::enable_if<has_ostream_operator<iterator_value_type<Iterator>>::value>::type
print(Iterator begin, Iterator end) {
    for(auto it = begin; it != end; ++it) {
        std::cout << *it << "\n";
    }
}

int main() {
    StlVector<int> v = {1, 2, 3, 4, 5, 6, 7};
    print(v.begin(), v.end());

    StlVector<Foo> fooV = {{1, "a"}, {2, "b"}, {3, "c"}};
    print(fooV.begin(), fooV.end());
    return 0;
}

