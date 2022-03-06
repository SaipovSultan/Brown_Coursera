#pragma once
#include <functional>
#include <optional>
#include <mutex>

template <typename T>
class LazyValue {
public:
    explicit LazyValue(std::function<T()> init) : init(std::move(init)) {
    }

    bool HasValue() const {
        return value.has_value();
    }

    const T& Get() const {
        if (std::lock_guard lock(mtx);!HasValue()) {
            value = init();
        }
        return *value;
    }

private:
    std::function<T()> init;
    mutable std::optional<T> value;
    mutable std::mutex mtx;
};