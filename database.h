#pragma once
#include <string>
#include <unordered_map>
#include <map>

struct Record {
    std::string id;
    std::string title;
    std::string user;
    int timestamp;
    int karma;
};

class Database {
public:

    bool Put(const Record& record){
        auto [it, inserted] = storage.insert(
                {record.id, Data{record,{},{},{}}}
        );
        if(!inserted){
            return false;
        }
        auto& data = it->second;
        const Record* ptr = &data.record;
        data.timestamp_iter = timestamp_index.insert({record.timestamp, ptr});
        data.karma_iter = karma_index.insert({record.karma, ptr});
        data.user_iter = user_index.insert({record.user, ptr});
        return true;
    }
    const Record* GetById(const std::string& id) const{
        if(auto it = storage.find(id);it != storage.end()){
            return &it->second.record;
        }else{
            return nullptr;
        }
    }
    bool Erase(const std::string& id){
        if(auto it = storage.find(id);it != storage.end()){
            const auto& data = it->second;
            timestamp_index.erase(data.timestamp_iter);
            karma_index.erase(data.karma_iter);
            user_index.erase(data.user_iter);
            storage.erase(it);
            return true;
        }else{
            return false;
        }
    }

    template <typename Callback>
    void RangeByTimestamp(int low, int high, Callback callback) const{
        auto it_begin = timestamp_index.lower_bound(low);
        auto it_end = timestamp_index.upper_bound(high);
        for(auto it = it_begin;it != it_end;it = next(it)){
            if(!callback(*it->second)) break;
        }
    }

    template <typename Callback>
    void RangeByKarma(int low, int high, Callback callback) const{
        auto it_begin = karma_index.lower_bound(low);
        auto it_end = karma_index.upper_bound(high);
        for(auto it = it_begin;it != it_end;it = next(it)){
            if(!callback(*it->second)) break;
        }
    }

    template <typename Callback>
    void AllByUser(const std::string& user, Callback callback) const{
        auto it_begin = user_index.lower_bound(user);
        auto it_end = user_index.upper_bound(user);
        for(auto it = it_begin;it != it_end;it = next(it)){
            if(!callback(*it->second)) break;
        }
    }

private:
    template <typename Type>
    using index = std::multimap<Type, const Record*>;
    struct Data{
        Record record;
        index<int>::iterator timestamp_iter;
        index<int>::iterator karma_iter;
        index<std::string>::iterator user_iter;
    };
    std::unordered_map<std::string, Data> storage;
    index<int> timestamp_index;
    index<int> karma_index;
    index<std::string> user_index;
};