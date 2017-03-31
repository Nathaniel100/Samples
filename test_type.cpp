#include <iostream>
#include <memory>

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

int main() {
    static_assert(std::is_same<has_propagate_on_container_copy_assignment<A<char>>::type, std::true_type>::value, "");
    static_assert(std::is_same<has_propagate_on_container_copy_assignment<B<char>>::type, std::false_type>::value, "");
    std::cout << "Hello, World!" << std::endl;
    return 0;
}