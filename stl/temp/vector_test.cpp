//
// Created by 吴凡 on 2017/3/29.
//

#include "stl_vector.h"
#include <iostream>

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

int main() {
    test_simple();
    test_constructor();
    test_assignment();
    test_size();

    return 0;
}