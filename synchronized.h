#pragma once
#include <mutex>

template <typename T>
class Synchronized {
public:
    explicit Synchronized(T initial = T()) : value(std::move(initial)){}

    template <typename U>
    struct Access{
        U& ref_to_value;
        std::lock_guard<std::mutex> lock;
    };

    Access<T> GetAccess(){
        return Access<T>{value, std::lock_guard<std::mutex>(mtx)};
    }

    Access<const T> GetAccess() const{
        return Access<const T>{value, std::lock_guard<std::mutex>(mtx)};
    }

private:
    T value;
    mutable std::mutex mtx;
};