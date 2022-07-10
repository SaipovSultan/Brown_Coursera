#pragma once
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

struct Email {
    explicit Email(std::string from, std::string to, std::string body) : from(std::move(from)), to(std::move(to)), body(std::move(body)) {

    }

    std::string from;
    std::string to;
    std::string body;
};


class Worker {
public:
    virtual ~Worker() = default;
    virtual void Process(std::unique_ptr<Email> email) = 0;
    virtual void Run() {
        throw std::logic_error("Unimplemented");
    }

protected:
    std::unique_ptr<Worker> next;

    void PassOn(std::unique_ptr<Email> email) const {
        if (next) {
            next->Process(std::move(email));
        }
    }

public:
    void SetNext(std::unique_ptr<Worker> next) {
        this->next = std::move(next);
    }
};


class Reader : public Worker {
public:
    explicit Reader(std::istream& input) : input(input) {}

    void Process(std::unique_ptr<Email> email) override {
        PassOn(std::move(email));
    }

    void Run() override{
        std::string from, to, body;
        while(std::getline(input, from) && std::getline(input, to) && std::getline(input, body)) {
            Process(std::make_unique<Email>(std::move(from), std::move(to), std::move(body)));
        }
    }
private:
    std::istream& input;
};


class Filter : public Worker {
public:
    using Function = std::function<bool(const Email&)>;

    explicit Filter(Function pred) : pred(std::move(pred)) {}

    void Process(std::unique_ptr<Email> email) override{
        if (pred(*email.get())) {
            PassOn(std::move(email));
        }
    }
private:
    Function pred;
};


class Copier : public Worker {
public:
    explicit Copier(std::string recipient) : recipient(std::move(recipient)) {}

    void Process(std::unique_ptr<Email> email) {
        Email email_copy = *email.get();
        PassOn(std::move(email));
        if (recipient != email_copy.to) {
            PassOn(std::make_unique<Email>(std::move(email_copy.from), recipient, std::move(email_copy.body)));
        }
    }
private:
    std::string recipient;
};


class Sender : public Worker {
public:
    explicit Sender(std::ostream& output) : output(output) {}

    void Process(std::unique_ptr<Email> email) {
        const Email& email_ref = *email.get();
        output << email_ref.from << "\n" << email_ref.to << "\n" << email_ref.body << "\n";
    }
private:
    std::ostream& output;
};

class PipelineBuilder {
public:
    explicit PipelineBuilder(std::istream& in) {
        workers.push_back(std::make_unique<Reader>(in));
    }

    PipelineBuilder& FilterBy(Filter::Function filter) {
        workers.push_back(std::make_unique<Filter>(std::move(filter)));
        return *this;
    }

    PipelineBuilder& CopyTo(std::string recipient) {
        workers.push_back(std::make_unique<Copier>(std::move(recipient)));
        return *this;
    }

    PipelineBuilder& Send(std::ostream& out) {
        workers.push_back(std::make_unique<Sender>(out));
        return *this;
    }

    std::unique_ptr<Worker> Build() {
        auto it = std::rbegin(workers), prev = std::next(it);
        while(prev != std::rend(workers)) {
            (*prev)->SetNext(std::move(*it));
            it = prev;
            prev = std::next(prev);
        }
        std::unique_ptr<Worker> worker = std::move(*it);
        workers.clear();
        return worker;
    }

private:
    std::vector<std::unique_ptr<Worker>> workers;
};
