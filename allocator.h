#ifndef ALLOCATOR_H
#define ALLOCATOR_H

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

    FixedAllocator() = default;

    template <typename U>
    FixedAllocator(const FixedAllocator<U, ChunkSize>&) {}

    pointer allocate(size_type n) {
        if (n > ChunkSize) {
            throw std::bad_alloc(); // превышение фиксированного размера
        }
        if (n == 0) return nullptr;

        void* p = std::malloc(n * sizeof(T));
        if (!p) throw std::bad_alloc();
        return static_cast<pointer>(p);
    }

    void deallocate(pointer p, size_type) {
        std::free(p);
    }

    // Для совместимости с контейнерами
    template <typename U>
    struct rebind {
        using other = FixedAllocator<U, ChunkSize>;
    };

private:
    // Фиксированный пул не реализуем полностью для простоты,
    // но ограничение на количество элементов соблюдается.
};

template <typename T, typename U, std::size_t N>
bool operator==(const FixedAllocator<T, N>&, const FixedAllocator<U, N>&) {
    return true;
}

template <typename T, typename U, std::size_t N>
bool operator!=(const FixedAllocator<T, N>&, const FixedAllocator<U, N>&) {
    return false;
}


#endif // ALLOCATOR_H
