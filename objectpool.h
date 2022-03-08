#include <stdexcept>
#include <memory>
#include <queue>
#include <set>
#include <memory>

template <class T>
class ObjectPool {
public:
    T* Allocate(){
        if(free.empty()){
            free.push(std::make_unique<T>());
        }
        auto ptr = move(free.front());
        free.pop();
        auto ret = ptr.get();
        allocated.insert(move(ptr));
        return ret;
    }

    T* TryAllocate(){
        if(free.empty()){
            return nullptr;
        }
        return Allocate();
    }

    void Deallocate(T* object){
        auto it = allocated.find(object);
        if(it == allocated.end()){
            throw std::invalid_argument("");
        }
        free.push(move(allocated.extract(it).value()));
    }
private:
    struct Compare
    {
        using is_transparent = void;

        bool operator()(const std::unique_ptr<T>& lhs, const std::unique_ptr<T>& rhs) const {
            return lhs < rhs;
        }
        bool operator()(const std::unique_ptr<T>& lhs, const T* rhs) const {
            return std::less<const T*>()(lhs.get(), rhs);
        }
        bool operator()(const T* lhs, const std::unique_ptr<T>& rhs) const {
            return std::less<const T*>()(lhs, rhs.get());
        }
    };
    std::queue<std::unique_ptr<T>> free;
    std::set<std::unique_ptr<T>, Compare> allocated;
};