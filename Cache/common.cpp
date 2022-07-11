#include "common.h"

#include <list>
#include <algorithm>
#include <iterator>
#include <mutex>
using namespace std;

class LruCache : public ICache {
public:
    LruCache(shared_ptr<IBooksUnpacker> books_unpacker, const Settings& settings) : books_unpacker(std::move(books_unpacker)),
                                                                                    settings(settings), memory_used_by_books(0u) {
    }

    BookPtr GetBook(const string& book_name) override {
        lock_guard<mutex> lock(mtx);

        auto book_it = find_if(begin(linked_list), end(linked_list), [&book_name](const BookPtr& book_ptr) { return book_ptr->GetName() == book_name;});
        if(book_it != end(linked_list)) {
            iter_swap(book_it, begin(linked_list));
            return *begin(linked_list);
        }

        BookPtr book_ptr = books_unpacker->UnpackBook(book_name);
        const size_t memory_used_by_book = book_ptr->GetContent().size();

        while(!linked_list.empty() && memory_used_by_books + memory_used_by_book > settings.max_memory) {
            memory_used_by_books -= linked_list.back()->GetContent().size();
            linked_list.pop_back();
        }
        if(memory_used_by_book < settings.max_memory) {
            linked_list.push_front(book_ptr);
            memory_used_by_books += memory_used_by_book;
        }
        return book_ptr;
    }

private:
    list<BookPtr> linked_list;
    shared_ptr<IBooksUnpacker> books_unpacker;
    Settings settings;
    size_t memory_used_by_books;
    mutex mtx;
};


unique_ptr<ICache> MakeCache(shared_ptr<IBooksUnpacker> books_unpacker, const ICache::Settings& settings) {
    return make_unique<LruCache>(std::move(books_unpacker), settings);
}
