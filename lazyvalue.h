#pragma once
#include <functional>
#include <optional>

template <typename T>
class LazyValue {
public:
    explicit LazyValue(std::function<T()> init) : init(std::move(init)) {
    }

    bool HasValue() const {
        return value.has_value();
    }

    const T& Get() const {
        if (!HasValue()) {
            value = init();
        }
        return *value;
    }

private:
    std::function<T()> init;
    mutable std::optional<T> value;
};