//
// Created by 吴凡 on 2017/3/29.
//

#include "stl_allocator.h"
#include <vector>
#include <iostream>
#include <type_traits>
#include <string>

template<class T>
using my_stl_vector = std::vector<T, my_stl::allocator<T>>;

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
        std::cout << *it << " ";
    }
    std::cout << "\n";
}

#define STATIC_ASSERT( ... ) static_assert(__VA_ARGS__, #__VA_ARGS__)

template<class T>
class Base {
public:
    Base(T i = 0): i_{i} {}
    Base(const Base &b): i_{b.i_} {
        std::cout << "Base Copy Constructor\n";
    }
    Base& operator=(const Base &b) {
        std::cout << "Base Copy Assignment\n";
        i_ = b.i_;
        return *this;
    }
    Base(Base &&b): i_{b.i_} {
    }
    Base& operator=(Base &&b) {
        i_ = b.i_;
        return *this;
    }
    T& get() {
        return i_;
    }
    const T& get() const {
        return i_;
    }
protected:
    T i_;
};

template<class T>
class CC : public Base<T> {
public:
    CC(T i = 0): Base<T>{i} {};
    CC(const CC &b): Base<T>{b} {
        std::cout << "CC Copy Constructor\n";
    }
    CC& operator=(const CC &c) {
        std::cout << "CC Copy Assignment\n";
        CC tmp(c);
        std::swap(tmp, *this);
        return *this;
    }
    CC(CC &&c): Base<T>{c} {
    }
    CC& operator=(CC &&c) {
        Base<T>::get() = c.get();
        return *this;
    }
};

int main() {
    my_stl_vector<int> v = {1, 2, 3, 4, 5, 6, 7};
    print(v.begin(), v.end());

    my_stl_vector<Foo> fooV = {{1, "a"}, {2, "b"}, {3, "c"}};
    print(fooV.begin(), fooV.end());

    my_stl_vector<const int> cv = {1, 2, 3, 4, 5, 6, 7};
    print(cv.begin(), cv.end());

    using alloc = my_stl::allocator<int>;
    using std_alloc = std::allocator<int>;
    STATIC_ASSERT(std::is_same<alloc::value_type, std_alloc::value_type>::value);
    STATIC_ASSERT(std::is_same<alloc::pointer, std_alloc::pointer>::value);
    STATIC_ASSERT(std::is_same<alloc::const_pointer, std_alloc::const_pointer>::value);
    STATIC_ASSERT(std::is_same<alloc::difference_type, std_alloc::difference_type>::value);
    STATIC_ASSERT(std::is_same<alloc::size_type, std_alloc::size_type>::value);
    STATIC_ASSERT(std::is_same<alloc::reference, std_alloc::reference>::value);
    STATIC_ASSERT(std::is_same<alloc::const_reference, std_alloc::const_reference>::value);

    using const_alloc = my_stl::allocator<const int>;
    using std_const_alloc = std::allocator<const int>;
    STATIC_ASSERT(std::is_same<const_alloc::value_type, std_const_alloc::value_type>::value);
    STATIC_ASSERT(std::is_same<const_alloc::pointer, std_const_alloc::pointer>::value);
    STATIC_ASSERT(std::is_same<const_alloc::const_pointer, std_const_alloc::const_pointer>::value);
    STATIC_ASSERT(std::is_same<const_alloc::difference_type, std_const_alloc::difference_type>::value);
    STATIC_ASSERT(std::is_same<const_alloc::size_type, std_const_alloc::size_type>::value);

    CC<int> cc(1);
    std::cout << cc.get() << "\n";
    CC<int> cc1;
    cc1 = cc;
    std::cout << cc.get() << "\n";
    return 0;
}

