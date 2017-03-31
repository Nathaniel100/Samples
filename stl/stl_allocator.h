//
// Created by 吴凡 on 2017/3/29.
//

#ifndef SAMPLES_STL_ALLOCATOR_H
#define SAMPLES_STL_ALLOCATOR_H

#include <new>
//#include <iostream>

namespace my_stl {

// Simplest Allocator
template<class T>
class allocator {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using void_pointer = void*;
    using const_void_pointer = const void*;
    using difference_type = ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    template<class U>
    struct rebind {
        using other = allocator<U>;
    };

    pointer allocate(size_t n) {
        pointer p = static_cast<pointer>(::operator new(n * sizeof(T)));
        //std::cout << "Pointer(allocate): " << (size_t)p << "\n";
        return p;
    }
    void deallocate(pointer p, size_t n) {
        //std::cout << "Pointer(deallocate): " << (size_t)p << "\n";
        ::operator delete((void *)p);
    }
};

template<class T>
bool operator==(const allocator<T>&, const allocator<T>&) { return true; }

template<class T>
bool operator!=(const allocator<T>&, const allocator<T>&) { return false; }

template<class Allocator>
class allocator_traits {
public:
    using allocator_type = Allocator;
    using value_type = typename Allocator::value_type;
    using pointer = typename Allocator::pointer;
    using const_pointer = typename Allocator::const_pointer;
    using void_pointer = typename Allocator::void_pointer;
    using const_void_pointer = typename Allocator::const_void_pointer;
    using difference_type = typename Allocator::difference_type;
    using size_type = typename Allocator::size_type;
    template<class U>
    using rebind_alloc = typename Allocator::template rebind<U>::other;

    static pointer allocate(Allocator& alloc, size_t n) {
        return alloc.allocate(n);
    }

    static void deallocate(Allocator& alloc, pointer p, size_t n) {
        return alloc.deallocate(p, n);
    }
};


}

#endif //SAMPLES_STL_ALLOCATOR_H
