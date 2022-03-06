#pragma once
#include <mutex>
#include <unordered_map>
#include <vector>

template <typename K, typename V, typename Hash = std::hash<K>>
class ConcurrentMap {
public:
    using MapType = std::unordered_map<K, V, Hash>;
private:
    struct Bucket{
        MapType data;
        mutable std::mutex mtx;
    };
    std::vector<Bucket> buckets;

public:
    struct WriteAccess : std::lock_guard<std::mutex> {
        V& ref_to_value;
        WriteAccess(const K& key, Bucket& bucket) : std::lock_guard<std::mutex>(bucket.mtx),
                                                    ref_to_value(bucket.data[key]){}
    };

    struct ReadAccess : std::lock_guard<std::mutex> {
        const V& ref_to_value;
        ReadAccess(const K& key, const Bucket& bucket) : std::lock_guard<std::mutex>(bucket.mtx),
                                                         ref_to_value(bucket.data.at(key)){}
    };

    explicit ConcurrentMap(size_t bucket_count) : buckets(bucket_count){}

    WriteAccess operator[](const K& key){
        return {key, buckets[getindex(key)]};
    }

    ReadAccess At(const K& key) const{
        return {key, buckets[getindex(key)]};
    }

    bool Has(const K& key) const{
        const Bucket& bucket = buckets[getindex(key)];
        std::lock_guard<std::mutex> lock(bucket.mtx);
        return bucket.data.count(key) > 0;
    }

    MapType BuildOrdinaryMap() const{
        MapType result;
        for(auto& [data, mtx] : buckets){
            std::lock_guard<std::mutex> lock(mtx);
            result.insert(std::begin(data), std::end(data));
        }
        return result;
    }

private:
    Hash hasher;
    size_t getindex(const K& key) const{
        return hasher(key) % buckets.size();
    }
};