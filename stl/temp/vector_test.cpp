//
// Created by 吴凡 on 2017/3/29.
//

#include "stl_vector.h"
#include <iostream>
#include <iomanip>

template<typename T>
std::ostream& operator<<(std::ostream& s, const my_stl::vector<T>& v) {
    s.put('[');
    char comma[3] = {'\0', ' ', '\0'};
    for (const auto& e : v) {
        s << comma << e;
        comma[0] = ',';
    }
    return s << ']';
}


void test_constructor() {
    // c++11 initializer list syntax:
    my_stl::vector<std::string> words1 {"the", "frogurt", "is", "also", "cursed"};
    std::cout << "words1: " << words1 << '\n';

    // words2 == words1
    my_stl::vector<std::string> words2(words1.begin(), words1.end());
    std::cout << "words2: " << words2 << '\n';

    // words3 == words1
    my_stl::vector<std::string> words3(words1);
    std::cout << "words3: " << words3 << '\n';

    // words4 is {"Mo", "Mo", "Mo", "Mo", "Mo"}
    my_stl::vector<std::string> words4(5, "Mo");
    std::cout << "words4: " << words4 << '\n';
}

void display_sizes(const my_stl::vector<int>& nums1,
                   const my_stl::vector<int>& nums2,
                   const my_stl::vector<int>& nums3)
{
    std::cout << "nums1: " << nums1.size()
              << " nums2: " << nums2.size()
              << " nums3: " << nums3.size() << '\n';
}

void test_assignment() {
    my_stl::vector<int> nums1 {3, 1, 4, 6, 5, 9};
    my_stl::vector<int> nums2;
    my_stl::vector<int> nums3;

    std::cout << "Initially:\n";
    display_sizes(nums1, nums2, nums3);

    // copy assignment copies data from nums1 to nums2
    nums2 = nums1;

    std::cout << "After assigment:\n";
    display_sizes(nums1, nums2, nums3);

    // move assignment moves data from nums1 to nums3,
    // modifying both nums1 and nums3
    nums3 = std::move(nums1);

    std::cout << "After move assigment:\n";
    display_sizes(nums1, nums2, nums3);
}

void test_simple() {
    // Create a vector containing integers
    my_stl::vector<int/*, std::allocator<int> */> v = {7, 5, 16, 8};

    // Add two more integers to vector
    v.push_back(25);
    v.push_back(13);

    // Iterate and print values of vector
    for(int n : v) {
        std::cout << n << '\n';
    }

    std::cout << "vector size: " << v.size() << "\n";
    v.clear();
    std::cout << "after clear, vector size: " << v.size() << "\n";
}

void test_size() {
    my_stl::vector<int> nums {1, 3, 5, 7};

    std::cout << "nums contains " << nums.size() << " elements.\n";

    my_stl::vector<int> numbers;
    std::cout << "Initially, numbers.empty(): " << numbers.empty() << '\n';

    numbers.push_back(42);
    numbers.push_back(13317);
    std::cout << "After adding elements, numbers.empty(): " << numbers.empty() << '\n';
}

// minimal C++11 allocator with debug output
template <class Tp>
struct NAlloc {
    typedef Tp value_type;
    NAlloc() = default;
    template <class T> NAlloc(const NAlloc<T>&) {}
    Tp* allocate(std::size_t n) {
        n *= sizeof(Tp);
        std::cout << "allocating " << n << " bytes\n";
        return static_cast<Tp*>(::operator new(n));
    }
    void deallocate(Tp* p, std::size_t n) {
        std::cout << "deallocating " << n*sizeof*p << " bytes\n";
        ::operator delete(p);
    }
};
template <class T, class U>
bool operator==(const NAlloc<T>&, const NAlloc<U>&) { return true; }
template <class T, class U>
bool operator!=(const NAlloc<T>&, const NAlloc<U>&) { return false; }

void test_reserve() {
    int sz = 100;
    std::cout << "using reserve: \n";
    {
        my_stl::vector<int, NAlloc<int>> v1;
        v1.reserve(sz);
        for(int n = 0; n < sz; ++n)
            v1.push_back(n);
        std::cout << "nums contains " << v1.size() << " elements.\n";
    }
    std::cout << "not using reserve: \n";
    {
        my_stl::vector<int, NAlloc<int>> v1;
        for(int n = 0; n < sz; ++n)
            v1.push_back(n);
        std::cout << "nums contains " << v1.size() << " elements.\n";
    }
}

void test_resize() {
    my_stl::vector<int> c = {1, 2, 3};
    std::cout << "The vector holds: ";
    for(auto& el: c) std::cout << el << ' ';
    std::cout << '\n';
    c.resize(5);
    std::cout << "After resize up 5: ";
    for(auto& el: c) std::cout << el << ' ';
    std::cout << '\n';
    c.resize(2);
    std::cout << "After resize down to 2: ";
    for(auto& el: c) std::cout << el << ' ';
    std::cout << '\n';
}

void test_pushback() {
    my_stl::vector<std::string> numbers;

    numbers.push_back("abc");
    std::string s = "def";
    numbers.push_back(std::move(s));

    std::cout << "vector holds: ";
    for (auto&& i : numbers) std::cout << i << ' ';
    std::cout << "\nMoved-from string holds " << s << '\n';
}

int main() {
    test_simple();
    test_constructor();
    test_assignment();
    test_size();
    test_resize();
    test_reserve();
    test_pushback();

    return 0;
}