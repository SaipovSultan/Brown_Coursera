#include <cstddef>

template <typename T>
class UniquePtr {
private:
    T* _ptr;

    void Free() {
        if(_ptr) delete _ptr;
    }

    void Set(T* ptr) {
        _ptr = ptr;
    }
public:
    UniquePtr() : _ptr(nullptr) {}

    UniquePtr(T * ptr) : _ptr(ptr) {}

    UniquePtr(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) {
        Reset(other.Release());
    }

    UniquePtr& operator = (const UniquePtr&) = delete;

    UniquePtr& operator = (std::nullptr_t null) {
        Reset(null);
        return *this;
    }

    UniquePtr& operator = (UniquePtr&& other) {
        if(this != &other) {
            Reset(other.Release());
            return *this;
        }
    }

    ~UniquePtr() {
        Free();
    }

    T& operator * () const {
        return *_ptr;
    }

    T* operator -> () const {
        return _ptr;
    }

    T* Release() {
        auto ptr = Get();
        Set(nullptr);
        return ptr;
    }

    void Reset(T* ptr) {
        Free();
        Set(ptr);
    }

    void Swap(UniquePtr& other) {
        auto ptr = Release(), other_ptr = other.Release();
        Set(other_ptr);
        other.Set(ptr);
    }

    T* Get() const {
        return _ptr;
    }
};


