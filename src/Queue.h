#pragma once

#include <mutex>
#include <optional>
#include <type_traits>

/**
 * A FIFO Queue of fixed capacity, but the data is dynamically stored
 * @tparam T Primitive type stored in the queue
 * @tparam N Maximum capacity of the queue
 */
template<typename T, size_t N>
class Queue {
    static_assert(N > 0, "Queue capacity must be greater than 0");
    static_assert(std::is_fundamental_v<T>, "Queue can only be used with fundamental/primitive types");

    /// Current size of enqueue elements
    int queueSize_{0};
    /// Dynamic memory storage or type
    T *data_;
    /// Thread safety mutex for access to the data
    std::mutex dataMutex_;
    /// Condition variables used for waiting to push data when capacity is reached
    std::condition_variable dataFullCond_;
    /// Condition variables used for waiting on data when there are no elements
    std::condition_variable dataEmptyCond_;

public:
    static_assert(std::is_fundamental_v<T>, "Queue can only be used with fundamental/primitive types");

    Queue();
    ~Queue();

    /// Push an item onto the queue. If the queue is full, the function will block until the item is successfully pushed
    /// or the timeout expires.
    /// @param item Item to push onto queue
    /// @param timeout Timeout in milliseconds to wait for the push to succeed. If timeout is 0, the function will block
    /// until the item is successfully pushed. If timeout is reached, an exception will be thrown.
    void push(T item, std::optional<std::chrono::milliseconds> timeout = std::nullopt);

    /// Pop an item from the queue. If the queue is empty, the function will block until an item is successfully popped
    /// or the timeout expires.
    /// @param timeout Timeout in milliseconds to wait for pop to succeed. If timeout is 0, the function will block
    /// until an item is successfully popped. If timeout is reached, an exception will be thrown.
    /// @return Item at the front of the queue
    T pop(std::optional<std::chrono::milliseconds> timeout = std::nullopt);

    /// Returns the current number of elements in the queue
    int size() { return queueSize_; }
};

// Implementation
#include "Queue.tpp"