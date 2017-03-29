//
// Created by 吴凡 on 2017/3/29.
//

#ifndef SAMPLES_STL_ALLOCATOR_H
#define SAMPLES_STL_ALLOCATOR_H

#include <stdlib.h>

namespace stl {

// Simplest Allocator
template<class T>
class Allocator {
public:
    using value_type = T;
    T* allocate(size_t n) {
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }
    void deallocate(T* p, size_t n) {
        ::operator delete(p);
    }
};


}

#endif //SAMPLES_STL_ALLOCATOR_H
