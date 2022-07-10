#include "test_runner.h"

#include <functional>
#include <string>
#include <optional>

template <typename T>
class LazyValue {
public:
    explicit LazyValue(std::function<T()> init) : init(init) {}

    bool HasValue() const {
        return object.has_value();
    }
    const T& Get() const {
        if (!HasValue()) {
            object = init();
        }
        return object.value();
    }

private:
    std::function<T()> init;
    mutable std::optional<T> object;
};

