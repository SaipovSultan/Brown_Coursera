#pragma once
#include "synchronized.h"
#include "../test_runner.h"

#include <vector>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <future>
#include <deque>

void TestConcurrentUpdate() {
    Synchronized<std::string> common_string;

    const size_t add_count = 50000;
    auto updater = [&common_string, add_count] {
        for (size_t i = 0; i < add_count; ++i) {
            auto access = common_string.GetAccess();
            access.ref_to_value += 'a';
        }
    };

    auto f1 = std::async(updater);
    auto f2 = std::async(updater);

    f1.get();
    f2.get();

    ASSERT_EQUAL(common_string.GetAccess().ref_to_value.size(), 2 * add_count);
}

std::vector<int> Consume(Synchronized<std::deque<int>>& common_queue) {
    std::vector<int> got;

    for (;;) {
        std::deque<int> q;

        {
            auto access = common_queue.GetAccess();
            q = std::move(access.ref_to_value);
        }

        for (int item : q) {
            if (item > 0) {
                got.push_back(item);
            } else {
                return got;
            }
        }
    }
}

void Log(const Synchronized<std::deque<int>>& common_queue, std::ostream& out) {
    for (int i = 0; i < 100; ++i) {
        out << "Queue size is " << common_queue.GetAccess().ref_to_value.size() << '\n';
    }
}

void TestProducerConsumer() {
    Synchronized<std::deque<int>> common_queue;
    std::ostringstream log;

    auto consumer = std::async(Consume, ref(common_queue));
    auto logger = std::async(Log, cref(common_queue), ref(log));

    const size_t item_count = 100000;
    for (size_t i = 1; i <= item_count; ++i) {
        common_queue.GetAccess().ref_to_value.push_back(i);
    }
    common_queue.GetAccess().ref_to_value.push_back(-1);

    std::vector<int> expected(item_count);
    std::iota(begin(expected), end(expected), 1);
    ASSERT_EQUAL(consumer.get(), expected);

    logger.get();
    const std::string logs = log.str();
    ASSERT(!logs.empty());
}

void SynchronizedTest() {
    TestRunner tr;
    RUN_TEST(tr, TestConcurrentUpdate);
    RUN_TEST(tr, TestProducerConsumer);
}