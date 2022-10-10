#include <iostream>
#include <memory>

class A {
public:
// ...

    static void * operator new(size_t size) {
        std::cout << "operator new!" << std::endl;
        return ::operator new(size);
    }

    static void operator delete(void *p, size_t size) {
        std::cout << "operator delete!" << std::endl;
        return ::operator delete(p);
    }
};

template <class T, class Y = T>
struct my_allocator {
    typedef T value_type;

    my_allocator() noexcept {}

    template <class U> 
    my_allocator (const my_allocator<U>&) noexcept {}

    T* allocate (std::size_t size) {
        return static_cast<T*>(Y::operator new(sizeof(T) * size));
    };

    void deallocate (T* p, std::size_t n) {
        ::operator delete(p);
    };
};

int main() {
    // auto sp = std::make_shared<A>();
    std::shared_ptr<A> sp = std::allocate_shared<A>(my_allocator<A>());
}