
#include <gtest/gtest.h>
#include <thread>

#include "src/Queue.h"

TEST(QueueTest, SimplePushPop) {
    Queue<int, 5> queue;
    queue.push(1);
    queue.push(2);
    queue.push(3);
    EXPECT_EQ(queue.size(), 3);
    queue.push(4);
    queue.push(5);
    EXPECT_EQ(queue.size(), 5);

    int front = queue.pop();
    EXPECT_EQ(front, 1);
    queue.pop();
    queue.pop();
    queue.pop();
    queue.pop();
    EXPECT_EQ(queue.size(), 0);


}

TEST(QueueTest, Timeout) {
    // Runtime exception if
    Queue<int, 1> queueSmall;
    queueSmall.push(1);
    EXPECT_THROW(queueSmall.push(3, std::chrono::milliseconds(10)), std::runtime_error);

    Queue<int, 2> queueBig;
    EXPECT_NO_THROW(queueBig.pop(std::chrono::milliseconds(10)));
}

TEST(QueueTest, ThreadSafety) {
    // Testing that no timeout on a pop when the queue is empty will "wait indefinitely"
    Queue<int, 1> queue;
    std::thread t1([&queue]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        queue.push(1);
    });
    std::mutex thread2Mtx;
    std::thread t2([&queue, &thread2Mtx]() {
        std::lock_guard thread2Lock(thread2Mtx);
        queue.pop();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    // Test if thread 2 has finished by checking status of lock
    EXPECT_FALSE(thread2Mtx.try_lock());
    EXPECT_TRUE(queue.size() == 0);
    // Confirm pop resumes after thread 1 has added an element to queue
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    EXPECT_TRUE(thread2Mtx.try_lock());

    t1.join();
    t2.join();

    // Repeat but with pushing to a full queue
    queue.push(1);
    std::mutex thread3Mtx;
    std::thread t3([&queue, &thread3Mtx]() {
        std::lock_guard thread3Lock(thread3Mtx);
        queue.push(1);
    });
    std::thread t4([&queue]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        queue.pop();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    // Test if thread 2 has finished by checking status of lock
    EXPECT_FALSE(thread3Mtx.try_lock());
    EXPECT_TRUE(queue.size() == 1);
    // Confirm push resumes after thread 1 has added an element to queue
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    EXPECT_TRUE(thread3Mtx.try_lock());

    t3.join();
    t4.join();
}