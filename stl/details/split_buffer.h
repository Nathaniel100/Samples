//
// Created by 吴凡 on 2017/4/7.
//

#ifndef SAMPLES_SPLIT_BUFFER_H
#define SAMPLES_SPLIT_BUFFER_H

#include <memory>
#include <algorithm>

namespace my_stl {
namespace details {

template <class T, class Allocator = std::allocator<T> >
class split_buffer {
public:
    using allocator_type = Allocator;
    using allocator_type_rr = typename std::remove_reference<Allocator>::type;
    using allocator_traits = std::allocator_traits<allocator_type_rr>;
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = typename allocator_traits::pointer;
    using const_pointer = typename allocator_traits::const_pointer;
    using difference_type = typename allocator_traits::difference_type;
    using size_type = typename allocator_traits::size_type;
    using iterator = pointer;
    using const_iterator = const_pointer;

    split_buffer() noexcept(std::is_nothrow_default_constructible<allocator_type>::value)
        :first_{nullptr}, begin_{nullptr}, end_{nullptr}, cap_end_{nullptr}, alloc_{allocator_type()} {
    }
    explicit split_buffer(allocator_type_rr& alloc)
        :first_{nullptr}, begin_{nullptr}, end_{nullptr}, cap_end_{nullptr}, alloc_{alloc} {
    }
    explicit split_buffer(const allocator_type_rr& alloc)
        :first_{nullptr}, begin_{nullptr}, end_{nullptr}, cap_end_{nullptr}, alloc_{alloc} {
    }
    explicit split_buffer(size_type cap, size_type start, allocator_type_rr& alloc)
        :alloc_{alloc} {
        first_ = cap != 0 ? allocator_traits::allocate(alloc_, cap) : nullptr;
        end_ = begin_ = first_ + start;
        cap_end_ = first_ + cap;
    }
    ~split_buffer() {
        clear();
        if(first_) {
            allocator_traits::deallocate(alloc_, first_, capacity());
        }
    }

    split_buffer(split_buffer&& s): first_{std::move(s.first_)}, begin_{std::move(s.begin_)},
                                    end_{std::move(s.end_)}, cap_end_{std::move(s.cap_end_)},
                                    alloc_{std::move(s.alloc_)} {
        s.first_ = s.begin_ = s.end_ = s.cap_end_ = nullptr;
    }
    split_buffer& operator=(split_buffer&& s) {
        first_ = std::move(s.first_);
        begin_ = std::move(s.begin_);
        end_ = std::move(s.end_);
        cap_end_ = std::move(s.cap_end_);
        alloc_ = std::move(s.alloc_);
        s.first_ = s.begin_ = s.end_ = s.cap_end_ = nullptr;
        return *this;
    }

    iterator begin() {
        return begin_;
    }
    const_iterator begin() const {
        return begin_;
    }
    iterator end() {
        return end_;
    }
    const_iterator end() const {
        return end_;
    }

    void clear() {
        destruct_at_end(begin_);
    }
    size_type size() const {
        return end_ - begin_;
    }
    bool empty() const {
        return begin_ == end_;
    }
    size_type capacity() const {
        return cap_end_ - first_;
    }
    size_type front_sqare() const {
        return begin_ - first_;
    }
    size_type back_sqare() const {
        return cap_end_ - end_;
    }

    reference front() {
        return *begin_;
    }
    const_reference front() const {
        return *begin_;
    }
    reference back() {
        return *(end_-1);
    }
    const_reference back() const {
        return *(end_-1);
    }

    void reserve(size_type cap) {
        if(capacity() < cap) {
            split_buffer<value_type, allocator_type_rr&> t(cap, 0, alloc_);
            t.construct_at_end(std::move_iterator<iterator>(begin()), std::move_iterator<iterator>(end()));
            swap(t);
        }
    }
    void shrink_to_fit() {
        if(capacity() != size()) {
            split_buffer<value_type , allocator_type_rr&> t(size(), 0, alloc_);
            t.construct_at_end(std::move_iterator<iterator>(begin()), std::move_iterator<iterator>(end()));
            swap(t);
        }
    }

    void push_front(const_reference x) {
        if(first_ == begin_) {
            if(end_ < cap_end_) {
                size_type d = back_sqare();
                d = (d + 1) / 2;
                begin_ = std::move_backward(begin_, end_, end_ + d);
                end_ += d;
            } else {
                size_type c = std::max<size_type>(2 * capacity(), 1);
                split_buffer t(c, (c + 3) / 4, alloc_);
                t.construct_at_end(std::move_iterator<iterator>(begin()), std::move_iterator<iterator>(end()));
                swap(t);
            }
        }
        allocator_traits::construct(alloc_, begin_ - 1, x);
        --begin_;
    }
    void push_front(value_type&& x) {
        if(first_ == begin_) {
            if(end_ < cap_end_) {
                size_type d = back_sqare();
                d = (d + 1) / 2;
                begin_ = std::move_backward(begin_, end_, end_ + d);
                end_ += d;
            } else {
                size_type c = std::max<size_type>(2 * capacity(), 1);
                split_buffer<value_type, allocator_type_rr&> t(c, (c + 3) / 4, alloc_);
                t.construct_at_end(std::move_iterator<iterator>(begin()), std::move_iterator<iterator>(end()));
                swap(t);
            }
        }
        allocator_traits::construct(alloc_, begin_ - 1, std::move(x));
        --begin_;
    }
    void push_back(const_reference x) {
        if(end_ == cap_end_) {
            if(first_ < begin_) {
                size_type d = front_sqare();
                d = (d + 1) / 2;
                end_ = std::move(begin_, end_, begin_ - d);
                begin_ -= d;
            } else {
                size_type c = std::max<size_type>(2 * capacity(), 1);
                split_buffer<value_type, allocator_type_rr&> t(c, c / 4, alloc_);
                t.construct_at_end(std::move_iterator<iterator>(begin()), std::move_iterator<iterator>(end()));
                swap(t);
            }
        }
        allocator_traits::construct(alloc_, end_, x);
        ++end_;
    }
    void push_back(value_type&& x) {
        if(end_ == cap_end_) {
            if(first_ < begin_) {
                size_type d = front_sqare();
                d = (d + 1) / 2;
                end_ = std::move(begin_, end_, begin_ - d);
                begin_ -= d;
            } else {
                size_type c = std::max<size_type>(2 * capacity(), 1);
                split_buffer<value_type, allocator_type_rr&> t(c, c / 4, alloc_);
                t.construct_at_end(std::move_iterator<iterator>(begin()), std::move_iterator<iterator>(end()));
                swap(t);
            }
        }
        allocator_traits::construct(alloc_, end_, std::move(x));
        ++end_;
    }
    template<class... Args> void emplace_back(Args&&... args) {
        if(end_ == cap_end_) {
            if(first_ < begin_) {
                size_type d = front_sqare();
                d = (d + 1) / 2;
                end_ = std::move(begin_, end_, begin_ - d);
                begin_ -= d;
            } else {
                size_type c = std::max<size_type>(2 * capacity(), 1);
                split_buffer<value_type, allocator_type_rr&> t(c, c / 4, alloc_);
                t.construct_at_end(std::move_iterator<iterator>(begin()), std::move_iterator<iterator>(end()));
                swap(t);
            }
        }
        allocator_traits::construct(alloc_, end_, std::forward<Args>(args)...);
        ++end_;
    }

    void pop_front() {
        destruct_at_begin(begin_ + 1);
    }
    void pop_back() {
        destruct_at_end(end_ - 1);
    }
    void swap(split_buffer& s) {
        std::swap(first_, s.first_);
        std::swap(begin_, s.begin_);
        std::swap(end_, s.end_);
        std::swap(cap_end_, s.cap_end_);
        std::swap(alloc_, s.alloc_);
    }

    void construct_at_end(size_type n);
    void construct_at_end(size_type n, const_reference x);
    template<class Iterator>
    void construct_at_end(Iterator first, Iterator last);
    void destruct_at_begin(pointer new_begin);
    void destruct_at_end(pointer new_end);

private:
    pointer first_;
    pointer begin_;
    pointer end_;
    pointer cap_end_;
    allocator_type alloc_;
};


}
}

#endif //SAMPLES_SPLIT_BUFFER_H
