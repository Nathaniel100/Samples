//
// Created by 吴凡 on 2017/3/29.
//

#include "stl_allocator.h"
#include <memory>
#include <initializer_list>

namespace my_stl {

template<class T, class Allocator=allocator<T>>
class vector_base {
public:
    using allocator_type = Allocator;
    using value_type = typename allocator_type::value_type;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using void_pointer = void*;
    using const_void_pointer = const void*;
    using difference_type = ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using iterator = pointer;
    using const_iterator = const_pointer ;

    explicit vector_base(allocator_type alloc, size_type n) : alloc_{alloc}, start_{nullptr}, end_{nullptr}, cap_end_{
            nullptr} {
        size_type cap = power2up(n);
        start_ = alloc_.allocate(cap);
        end_ = start_ + n;
        cap_end_ = start_ + cap;
    }

    vector_base() : vector_base(allocator_type(), 0) {}

    explicit vector_base(allocator_type alloc) : vector_base(alloc, 0) {}

    explicit vector_base(size_type n) : vector_base(allocator_type(), n) {}

    ~vector_base() {
        if (start_) {
            alloc_.deallocate(start_, cap_end_ - start_);
        }
    }

    vector_base(const vector_base &v) : vector_base(v.alloc_, v.end_ - v.start_) {
        std::uninitialized_copy(v.start_, v.end_, start_);
    }

    vector_base& operator=(const vector_base &v) {
        vector_base tmp(v);
        std::swap(tmp, *this);
        return *this;
    }

    vector_base(vector_base &&v) : alloc_{v.alloc_}, start_{std::move(v.start_)},
                                   end_{std::move(v.end_)}, cap_end_{std::move(v.cap_end_)} {
        v.start_ = v.end_ = v.cap_end_ = nullptr;
    }

    vector_base &operator=(vector_base &&v) {
        std::swap(v.alloc_, alloc_);
        std::swap(v.start_, start_);
        std::swap(v.end_, end_);
        std::swap(v.cap_end_, cap_end_);
        return *this;
    }

    static inline size_type power2up(size_type v) {
        if(v == 0) return default_size;
        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v |= v >> 32;
        v++;
        return v;
    }

protected:
    enum { default_size = 4 };

    allocator_type alloc_;
    pointer start_;
    pointer end_;
    pointer cap_end_;
};

template<class T, class Allocator=allocator<T>>
class vector : public vector_base<T, Allocator> {
public:
    using allocator_type = Allocator;
    using value_type = typename vector_base<T, Allocator>::value_type;
    using reference = typename vector_base<T, Allocator>::reference;
    using const_reference = typename vector_base<T, Allocator>::const_reference;
    using pointer = typename vector_base<T, Allocator>::pointer;
    using const_pointer = typename vector_base<T, Allocator>::const_pointer;
    using difference_type = typename vector_base<T, Allocator>::difference_type;
    using size_type = typename vector_base<T, Allocator>::size_type;
    using iterator = typename vector_base<T, Allocator>::iterator;
    using const_iterator = typename vector_base<T, Allocator>::const_iterator;


    vector() : vector_base<T, Allocator>{} {
    }

    explicit vector(const Allocator &alloc) : vector_base<T, Allocator>{alloc} {
    }

    explicit vector(size_type count, const T &value = T(), const Allocator &alloc = Allocator()) : vector_base<T, Allocator>{
            alloc, count} {
        std::uninitialized_fill(begin(), end(), value);
    }

    template<class InputIt>
    vector(InputIt first, InputIt last, const Allocator &alloc = Allocator())
            :vector_base<T, Allocator>{alloc, (size_type)std::distance(first, last)} {
        static_assert(std::is_same<value_type, typename std::iterator_traits<InputIt>::value_type>::value,
                      "value type must be same");
        std::uninitialized_copy(first, last, begin());
    }

    vector(std::initializer_list<T> init, const Allocator& alloc = Allocator()): vector(init.begin(), init.end(), alloc) {
    }

    void push_back(const T& v) {
        if(size() == capacity()) {
            reserve(vector_base<T, Allocator>::power2up(size() + 1));
        }
        *this->end_++ = v;
    }

    void push_back(T&& v) {
        if(size() == capacity()) {
            reserve(vector_base<T, Allocator>::power2up(size() + 1));
        }
        *this->end_++ = std::move(v);
    }

    void pop_back() {
        if(size() == 0) {
            return;
        }
        (--this->end_)->~T();
    }

    inline bool empty() const {
        return size() == 0;
    }

    void resize(size_type s) {
        if(s < size()) {
            for(size_type i = size(); i != s; i--) {
                pop_back();
            }
        } else if(s <= capacity()) {
            std::uninitialized_fill(end(), this->start_ + s, T());
        } else {
            size_type old_size = size();
            size_type cap = vector_base<T, Allocator>::power2up(s);
            pointer old_start = this->start_;

            this->start_ = this->alloc_.allocate(cap);
            this->end_ = this->start_ + s;
            this->cap_end_ = this->start_ + cap;
            if(old_start) {
                std::uninitialized_copy(old_start, old_start + old_size, this->start_);
                this->alloc_.deallocate(old_start, old_size);
            }
            std::uninitialized_fill(this->start_ + old_size, end(), T());
        }
    }

    void reserve(size_type cap) {
        if(cap <= capacity()) {
            return;
        }
        size_type old_size = size();
        pointer old_start = this->start_;

        this->start_ = this->alloc_.allocate(cap);
        this->end_ = this->start_ + old_size;
        this->cap_end_ = this->start_ + cap;
        if(old_start) {
            std::uninitialized_copy(old_start, old_start + old_size, this->start_);
            this->alloc_.deallocate(old_start, old_size);
        }
    }

    inline size_type size() const {
        return this->end_ - this->start_;
    }

    inline size_type capacity() const {
        return this->cap_end_ - this->start_;
    }

    void clear() {
        resize(0);
    }

    iterator begin() {
        return this->start_;
    }

    const_iterator begin() const {
        return this->start_;
    }

    const_iterator cbegin() const {
        return this->start_;
    }

    iterator end() {
        return this->end_;
    }

    const_iterator end() const {
        return this->end_;
    }
    const_iterator cend() const {
        return this->end_;
    }


private:
};


}
