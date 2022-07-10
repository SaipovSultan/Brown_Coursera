#include "common.h"

#include <list>
#include <algorithm>
#include <iterator>
#include <mutex>
using namespace std;

class LruCache : public ICache {
public:
    struct Book {
        string name;
        BookPtr ptr;
        Book(const string& name, BookPtr ptr) : name(name), ptr(std::move(ptr)) {}
    };

    LruCache(shared_ptr<IBooksUnpacker> books_unpacker, const Settings& settings) : books_unpacker(std::move(books_unpacker)),
                                                                                    settings(settings), memory_used_by_books(0u) {
    }

    BookPtr GetBook(const string& book_name) override {
        lock_guard<mutex> lock(mtx);

        auto book_it = find_if(begin(linked_list), end(linked_list), [&book_name](const Book& book) { return book.name == book_name;});
        if(book_it != end(linked_list)) {
            iter_swap(book_it, begin(linked_list));
            return begin(linked_list)->ptr;
        }

        BookPtr book_ptr = books_unpacker->UnpackBook(book_name);
        const size_t memory_used_by_book = book_ptr->GetContent().size();

        while(!linked_list.empty() && memory_used_by_books + memory_used_by_book > settings.max_memory) {
            Book& book = linked_list.back();
            memory_used_by_books -= book.ptr->GetContent().size();
            book.ptr.reset();
            linked_list.pop_back();
        }
        if(memory_used_by_book < settings.max_memory) {
            linked_list.emplace_front(book_name, book_ptr);
            memory_used_by_books += memory_used_by_book;
        }
        return book_ptr;
    }

private:
    list<Book> linked_list;
    shared_ptr<IBooksUnpacker> books_unpacker;
    Settings settings;
    size_t memory_used_by_books;
    mutex mtx;
};


unique_ptr<ICache> MakeCache(shared_ptr<IBooksUnpacker> books_unpacker, const ICache::Settings& settings) {
    return make_unique<LruCache>(std::move(books_unpacker), settings);
}
