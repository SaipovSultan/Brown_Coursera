#pragma once
#include <forward_list>
#include <iterator>
#include <algorithm>

using namespace std;

template <typename Type, typename Hasher>
class HashSet {
public:
    using BucketList = std::forward_list<Type>;

public:
    explicit HashSet(
            size_t num_buckets,
            const Hasher& hasher = {}
    ) : hasher(hasher), buckets(num_buckets) {}

    void Add(const Type& value){
        if(Has(value)) return;
        BucketList& bucket = buckets[hash(value)];
        bucket.push_front(value);
    }

    bool Has(const Type& value) const{
        const BucketList& bucket = GetBucket(value);
        return std::find(bucket.begin(), bucket.end(), value) != bucket.end();
    }

    void Erase(const Type& value){
        BucketList& bucket = buckets[hash(value)];
        bucket.remove(value);
    }
    const BucketList& GetBucket(const Type& value) const{
        return buckets[hash(value)];
    }

private:
    size_t hash(const Type& value) const{
        return hasher(value) % buckets.size();
    }
    Hasher hasher;
    std::vector<BucketList> buckets;
};