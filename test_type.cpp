#include <iostream>
#include <memory>
#include <vector>
#include <list>

template<class T>
class test_has_propagate_on_container_copy_assignment {
private:
    struct __two {char l; char h;};
    // 利用SFINAE(Substitution Failure Is Not An Error)
    template<class U> static __two __test(...); // 可变参数接收所有参数
    template<class U> static char __test(typename U::propagate_on_container_copy_assignment* = 0); // 仅接受T中包含类型propagate_on_container_copy_assignment的指针
public:
    static const bool value = sizeof(__test<T>(0)) == 1; // 利用sizeof不会真正调用函数
};

// bool模版默认类型不能少，否则has_propagate_on_container_copy_assignment<T>会报缺少参数的错误
// 该方式利用偏特化，用于检测T中的类型propagate_on_container_copy_assignment
template<class T, bool = test_has_propagate_on_container_copy_assignment<T>::value>
class has_propagate_on_container_copy_assignment {
public:
    using type = std::false_type;
};

template<class T>
class has_propagate_on_container_copy_assignment<T, true> {
public:
    using type = typename T::propagate_on_container_copy_assignment;
};

template<class T>
struct A {
    using value_type = T;
    using propagate_on_container_copy_assignment = std::true_type;
};

template<class T>
struct B {
    using value_type = T;
};


template<class T>
struct Type;


struct Base {
    ~Base() {
        std::cout << "Destructor Base\n";
    }
};

struct Derived : public Base {
    ~Derived() {
        std::cout << "Destructor Derived\n";
    }

};


template<class T>
class __test_iterator_category {
private:
    struct __two {char l; char h;};
    template <class U> static __two __test(...);
    template <class U> static char __test(typename U::iterator_category* = 0);
public:
    const static bool value = sizeof(__test<T>(0)) == 1;
};


template<class T, bool = __test_iterator_category<T>::value>
class test_iterator_category {
public:
    const static bool value = false;
};

template<class T>
class test_iterator_category<T, true> {
public:
    using type = typename std::iterator_traits<T>::iterator_category;
    const static bool value = true;
};


template<class Iterator, bool = test_iterator_category<Iterator>::value>
class is_forward_iterator {
public:
    const static bool value = false;
};

template<class Iterator>
class is_forward_iterator<Iterator, true> {
public:
    const static bool value = std::is_convertible<typename test_iterator_category<Iterator>::type, std::forward_iterator_tag>::value;
};

template<class Iterator, bool = test_iterator_category<Iterator>::value>
class is_random_iterator {
public:
    const static bool value = false;
};

template<class Iterator>
class is_random_iterator<Iterator, true> {
public:
    const static bool value = std::is_convertible<typename test_iterator_category<Iterator>::type, std::random_access_iterator_tag>::value;
};


int main() {
    static_assert(std::is_same<has_propagate_on_container_copy_assignment<A<char>>::type, std::true_type>::value, "");
    static_assert(std::is_same<has_propagate_on_container_copy_assignment<B<char>>::type, std::false_type>::value, "");

    static_assert(is_forward_iterator<int>::value, "");
    static_assert(is_forward_iterator<std::vector<int>::iterator>::value, "");
    static_assert(is_forward_iterator<std::list<int>::iterator>::value, "");
    static_assert(!is_forward_iterator<std::istream_iterator<int>>::value, "");
    static_assert(is_random_iterator<std::istream_iterator<int>>::value, "");
    static_assert(is_random_iterator<std::vector<int>::iterator>::value, "");
    static_assert(is_random_iterator<std::list<int>::iterator>::value, "");
    std::cout << "Hello, World!" << std::endl;

    return 0;
}