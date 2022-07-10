#pragma once
#include <memory>
#include <mutex>

template <typename T>
class Synchronized {
public:
    explicit Synchronized(T initial = T()) : value(std::move(initial)) {}

    template <typename U>
    struct Access{
        std::lock_guard<std::mutex> lock;
        U& ref_to_value;
    };

    Access<T> GetAccess() {
        return {std::lock_guard<std::mutex>(mtx), value};
    }
    Access<const T> GetAccess() const {
        return {std::lock_guard<std::mutex>(mtx), value};
    }

private:
    T value;
    mutable std::mutex mtx;
};
