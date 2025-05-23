/// Implementation of Queue class.

#pragma once

template<typename T, size_t N>
Queue<T, N>::Queue() {
    // Requirement was for dynamic memory allocation. This is accom
    data_ = new T[N];
}

template<typename T, size_t N>
Queue<T, N>::~Queue() {
    delete[] data_;
}

template<typename T, size_t N>
void Queue<T, N>::push(T item, std::optional<std::chrono::milliseconds> timeout) {
    std::unique_lock lock(dataMutex_);

    if (timeout.has_value()) {
        if (dataFullCond_.wait_for(lock, timeout.value(), [this]() {
            return queueSize_ >= N;
        })) {
            throw std::runtime_error("Timeout waiting for push to queue");
        }
    } else {
        dataFullCond_.wait(lock, [this]() {
            return queueSize_ < N;
        });
    }

    data_[queueSize_++] = item;
    dataEmptyCond_.notify_one();
}


template<typename T, size_t N>
T Queue<T, N>::pop(std::optional<std::chrono::milliseconds> timeout) {
    std::unique_lock lock(dataMutex_);

    if (timeout.has_value()) {
        if (dataEmptyCond_.wait_for(lock, timeout.value(), [this]() {
            return queueSize_ > 0;
        })) {
            throw std::runtime_error("Timeout waiting for pop from queue");
        }
    } else {
        dataEmptyCond_.wait(lock, [this]() {
            return queueSize_ > 0;
        });
    }

    // FIFO - pop from front of queue
    T returnValue = data_[0];
    --queueSize_;
    dataFullCond_.notify_one();
    return returnValue;
}
