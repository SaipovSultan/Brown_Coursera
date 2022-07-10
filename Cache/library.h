#include "common.h"

#include <atomic>
#include <future>
#include <numeric>
#include <random>
#include <unordered_map>

using namespace std;

class Book : public IBook {
public:
    Book(
            std::string name,
            std::string content,
            atomic<size_t>& memory_used_by_books
    )
            : name_(std::move(name))
            , content_(std::move(content))
            , memory_used_by_books_(memory_used_by_books)
    {
        memory_used_by_books_ += content_.size();
    }

    ~Book() {
        memory_used_by_books_ -= content_.size();
    }

    const std::string& GetName() const override {
        return name_;
    }

    const std::string& GetContent() const override {
        return content_;
    }

private:
    std::string name_;
    std::string content_;
    atomic<size_t>& memory_used_by_books_;
};

class BooksUnpacker : public IBooksUnpacker {
public:
    unique_ptr<IBook> UnpackBook(const std::string& book_name) override {
        ++unpacked_books_count_;
        return std::make_unique<Book>(
                book_name,
                "Dummy content of the book " + book_name,
                memory_used_by_books_
        );
    }

    size_t GetMemoryUsedByBooks() const {
        return memory_used_by_books_;
    }

    int GetUnpackedBooksCount() const {
        return unpacked_books_count_;
    }

private:
    atomic<size_t> memory_used_by_books_ = 0;
    atomic<int> unpacked_books_count_ = 0;
};

struct Library {
    std::vector<std::string> book_names;
    std::unordered_map<std::string, std::unique_ptr<IBook>> content;
    size_t size_in_bytes = 0;

    explicit Library(std::vector<std::string> a_book_names, IBooksUnpacker& unpacker)
            : book_names(std::move(a_book_names))
    {
        for (const auto& book_name : book_names) {
            auto& book_content = content[book_name];
            book_content = unpacker.UnpackBook(book_name);
            size_in_bytes += book_content->GetContent().size();
        }
    }
};

