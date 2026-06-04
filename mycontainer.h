#ifndef MYCONTAINER_H
#define MYCONTAINER_H

#include <memory>
#include <cstdlib>

template <typename T, typename Allocator = std::allocator<T>>
class MyContainer {
private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;
    Allocator allocator_;

    using alloc_traits = std::allocator_traits<Allocator>;

public:
    // Итератор (однонаправленный, упрощённый)
    class Iterator {
    private:
        T* ptr_;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator(T* ptr) : ptr_(ptr) {}

        reference operator*() const { return *ptr_; }
        pointer operator->() { return ptr_; }

        Iterator& operator++() { ++ptr_; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++ptr_; return tmp; }

        bool operator==(const Iterator& other) const { return ptr_ == other.ptr_; }
        bool operator!=(const Iterator& other) const { return ptr_ != other.ptr_; }
    };

    using iterator = Iterator;
    using const_iterator = Iterator;

    MyContainer() : data_(nullptr), size_(0), capacity_(0), allocator_() {}

    explicit MyContainer(const Allocator& alloc)
        : data_(nullptr), size_(0), capacity_(0), allocator_(alloc) {}

    ~MyContainer() {
        clear();
        if (data_) {
            alloc_traits::deallocate(allocator_, data_, capacity_);
        }
    }

    void push_back(const T& value) {
        if (size_ >= capacity_) {
            reserve(size_ + 1);
            // reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        alloc_traits::construct(allocator_, data_ + size_, value);
        ++size_;
    }

    void push_back(T&& value) {
        if (size_ >= capacity_) {
            reserve(size_ + 1);
            // reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        alloc_traits::construct(allocator_, data_ + size_, std::move(value));
        ++size_;
    }

    // С++ 20 universal method
    // template<typename U> requires std::constructible_from<T, U&&>
    // void push_back(U&& value) {
    //     if (size_ >= capacity_) {
    //         reserve(size_ + 1);
    //     }
    //     alloc_traits::construct(allocator_, data_ + size_, std::forward<U>(value));
    //     ++size_;
    // }


    void reserve(std::size_t new_cap) {
        if (new_cap <= capacity_) return;
        T* new_data = alloc_traits::allocate(allocator_, new_cap);
        for (std::size_t i = 0; i < size_; ++i) {
            alloc_traits::construct(allocator_, new_data + i, std::move(data_[i])); // placement_new
            alloc_traits::destroy(allocator_, data_ + i);
        }
        if (data_) {
            alloc_traits::deallocate(allocator_, data_, capacity_);
        }
        data_ = new_data;
        capacity_ = new_cap;
    }

    void clear() {
        for (std::size_t i = 0; i < size_; ++i) {
            alloc_traits::destroy(allocator_, data_ + i);
        }
        size_ = 0;
    }

    std::size_t size() const { return size_; }
    std::size_t capacity() const { return capacity_; }
    bool empty() const { return size_ == 0; }

    iterator begin() { return Iterator(data_); }
    iterator end() { return Iterator(data_ + size_); }
    const_iterator begin() const { return Iterator(data_); }
    const_iterator end() const { return Iterator(data_ + size_); }

    T& operator[](std::size_t idx) { return data_[idx]; }
    const T& operator[](std::size_t idx) const { return data_[idx]; }
};

#endif // MYCONTAINER_H
