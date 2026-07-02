#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <new>

// ===============================
// Аллокатор с фиксированным резервированием
// ===============================
template <typename T, std::size_t ChunkSize = 10>
class FixedAllocator {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using size_type = std::size_t;

    std::byte* m_buffer;
    std::size_t m_capacity;
    bool m_owned;

    FixedAllocator()
        : m_buffer(nullptr)
        , m_capacity(ChunkSize * sizeof(T))
        , m_owned(true)
    {
        m_buffer = static_cast<std::byte*>(std::malloc(m_capacity));
        if (!m_buffer) {
            throw std::bad_alloc();
        }
    }

    template <typename U>
    FixedAllocator(const FixedAllocator<U, ChunkSize>& other) noexcept
        : m_buffer(other.m_buffer)
        , m_capacity(other.m_capacity)
        , m_owned(false)  // Не владеем чужим буфером
    {}

    // Конструктор копирования
    FixedAllocator(const FixedAllocator& other) noexcept
        : m_buffer(other.m_buffer)
        , m_capacity(other.m_capacity)
        , m_owned(false)  // Не владеем чужим буфером
    {}

    ~FixedAllocator() {
        if (m_owned && m_buffer) {
            std::free(m_buffer);
        }
    }

    pointer allocate(size_type n) {
        if (n == 0) return nullptr;

        if (n > ChunkSize) {
            throw std::bad_alloc(); // превышение фиксированного размера
        }

        if (n * sizeof(T) > m_capacity) {
            throw std::bad_alloc();
        }

        assert(m_buffer != 0);
        void* p = m_buffer;

        return static_cast<pointer>(p);
    }

    void deallocate(pointer p, size_type) {
        (void)p;
    }

    // Для совместимости с контейнерами
    template <typename U>
    struct rebind {
        using other = FixedAllocator<U, ChunkSize>;
    };
};

// Операторы сравнения
template <typename T, typename U, std::size_t N>
bool operator==(const FixedAllocator<T, N>&, const FixedAllocator<U, N>&) {
    return true;
}

template <typename T, typename U, std::size_t N>
bool operator!=(const FixedAllocator<T, N>&, const FixedAllocator<U, N>&) {
    return false;
}

#endif // ALLOCATOR_H
