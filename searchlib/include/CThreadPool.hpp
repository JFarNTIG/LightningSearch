// SPDX-License-Identifier: GPL-2.0
#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <thread>
#include <vector>
#include <memory>
#include <stdexcept>
#include <queue>

/**
 * @brief Manages a thread pool and delegates work tasks to threads.
 */
class CThreadPool {
public:
    explicit CThreadPool(size_t const numWorkers)
    : m_shouldTerminate(false)
    {
        for(size_t i = 0; i < numWorkers; ++i) {
            m_workers.emplace_back(&CThreadPool::workerThreadFunc, this);
        }
    }

    /**
     * @brief Adds a new task to the thread pool's queue, given
     * any callable object and optional set of arguments.
     * 
     * @return A future object holding the task result. If the task
     * threw an exception, then the future object will re-throw
     * the exception when .get() is called.
     */
    template<typename F, typename... Args>
    auto enqueue(F &&f, Args &&...args) -> std::future<decltype(f(args...))> {
        using returnType = decltype(f(args...));

        auto task = std::make_shared<std::packaged_task<returnType()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<returnType> result = task->get_future();
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);

            if(m_shouldTerminate) {
                throw std::runtime_error("Cannot queue new tasks; the thread pool is terminating");
            }

            m_taskQueue.emplace([task]() {
                (*task)();
            });
        }

        m_condition.notify_one();
        return result;
    }

    ~CThreadPool() {
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            m_shouldTerminate = true;
        }

        m_condition.notify_all();

        for(std::thread &worker : m_workers) {
            worker.join();
        }
    }

private:
    void workerThreadFunc() {
        while(true) {
            std::packaged_task<void()> task;
            {
                std::unique_lock<std::mutex> lock(m_queueMutex);

                m_condition.wait(lock, [this] {
                    return m_shouldTerminate || !m_taskQueue.empty();
                });

                if(m_shouldTerminate && m_taskQueue.empty()) {
                    return;
                }

                task = std::move(m_taskQueue.front());

                m_taskQueue.pop();
            }

            task();
        }
    }

    std::vector<std::thread> m_workers;
    std::atomic_bool m_shouldTerminate;

    std::mutex m_queueMutex;
    std::condition_variable m_condition;
    std::queue<std::packaged_task<void()>> m_taskQueue;
};