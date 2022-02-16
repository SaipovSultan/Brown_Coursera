#include <vector>

template <typename T>
class PriorityCollection {
public:
    using Id = size_t;

    Id Add(T object){
        const Id new_id = objects.size();
        objects.push_back({std::move(object), 0});
        sorted_objects.insert({0, new_id});
        return new_id;
    }

    template <typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end,
             IdOutputIt ids_begin){
        while(range_begin != range_end){
            *ids_begin++ = Add(std::move(*range_begin++));
        }
    }

    bool IsValid(Id id) const{
        return id < objects.size() && objects[id].priority != NONE_PRIORITY;
    }

    const T& Get(Id id) const{
        static const T empty;
        if(IsValid(id)){
            return objects[id].object;
        }else{
            return empty;
        }
    }

    void Promote(Id id){
        if(IsValid(id)){
            Item& item = objects[id];
            size_t old_priority = item.priority;
            size_t new_priority = ++item.priority;
            sorted_objects.erase({old_priority, id});
            sorted_objects.insert({new_priority, id});
        }
    }

    std::pair<const T&, int> GetMax() const{
        const Item& item = objects[sorted_objects.rbegin()->second];
        return {item.object, item.priority};
    }

    std::pair<T, int> PopMax(){
        auto sorted_objects_it = sorted_objects.rbegin();
        Item& item = objects[sorted_objects_it->second];
        sorted_objects.erase(*sorted_objects_it);
        const int priority = item.priority;
        item.priority = NONE_PRIORITY;
        return {std::move(item.object), priority};
    }
private:
    struct Item{
        T object;
        int priority;
    };
    const int NONE_PRIORITY = -1;
    std::set<std::pair<int, Id>> sorted_objects;
    std::vector<Item> objects;
};