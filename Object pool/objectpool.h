#include <algorithm>
#include <memory>
#include <queue>
#include <stdexcept>
#include <unordered_set>

template <typename T>
struct Compare {
    using is_transparent = void;

    bool operator()(const std::unique_ptr<T>& lhs, const std::unique_ptr<T>& rhs) const{
        return lhs < rhs;
    }

    bool operator()(const std::unique_ptr<T>& lhs, const T* rhs) const {
        return std::less<const T*>()(lhs.get(), rhs);
    }

    bool operator()(const T* lhs, const std::unique_ptr<T>& rhs) const{
        return std::less<const T*>()(lhs, rhs.get());
    }
};

struct Hash {
    using is_transparent = void;
    template <class P>
    size_t operator()(const P& p) const {
        return std::hash<P>()(p);
    }
};
struct KeyEqual {
    using is_transparent = void;
    template <class P, class Q>
    bool operator()(const P& lhs, const Q& rhs) const {
        return std::to_address(lhs) == std::to_address(rhs);
    }
};

template <class T>
class ObjectPool {
public:
    T* Allocate();
    T* TryAllocate();

    void Deallocate(T* object);

private:

    std::queue<std::unique_ptr<T>> free;
    // Изменили на unordered_set с соответствующими параметрами
    std::unordered_set<std::unique_ptr<T>, Hash, KeyEqual> allocated;
};

template <typename T>
T* ObjectPool<T>::Allocate() {
    if (free.empty()) {
        free.push(std::make_unique<T>());
    }
    auto ptr = move(free.front());
    free.pop();
    T* ret = ptr.get();
    allocated.insert(std::move(ptr));
    return ret;
}

template <typename T>
T* ObjectPool<T>::TryAllocate() {
    if (free.empty()) {
        return nullptr;
    }
    return Allocate();
}


template <typename T>
void ObjectPool<T>::Deallocate(T* object) {
    auto it = allocated.find(object);
    if (it == allocated.end()) {
        throw std::invalid_argument("");
    }
    free.push(std::move(allocated.extract(it).value()));
}

