// Copyright Vibe Coding. All Rights Reserved.

#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <queue>

class EThreadPool
{
public:
    explicit EThreadPool(const size_t ThreadCount = std::thread::hardware_concurrency())
    {
        for (size_t ThreadIndex = 0; ThreadIndex < ThreadCount; ++ThreadIndex)
        {
            Workers.emplace_back([this]
                {
                    while (true)
                    {
                        std::function<void()> Task;

                        {   // блокируем очередь
                            std::unique_lock Lock(QueueMutex);

                            Condition.wait(Lock, [this]
                            {
                                return Stop || !Tasks.empty();
                            });

                            if (Stop && Tasks.empty())
                            {
                                return;
                            }

                            Task = std::move(Tasks.front());
                            Tasks.pop();
                        }

                        Task(); // выполняем задачу
                    }
                });
        }
    }

    template<class F, class... Args>
    auto enqueue(F&& InFunc, Args&&... InArgs) -> std::future<decltype(InFunc(InArgs...))>
    {
        using ReturnType = decltype(InFunc(InArgs...));
        auto TaskPtr = std::make_shared<std::packaged_task<ReturnType()>>(
            std::bind(std::forward<F>(InFunc), std::forward<Args>(InArgs)...)
        );

        std::future<ReturnType> Future = TaskPtr->get_future();
        {
            std::unique_lock Lock(QueueMutex);
            if (Stop) throw std::runtime_error("enqueue on stopped ThreadPool");
            Tasks.emplace([TaskPtr]() { (*TaskPtr)(); });
        }
        Condition.notify_one();
        return Future;
    }

    ~EThreadPool()
    {
        {
            std::unique_lock Lock(QueueMutex);
            Stop = true;
        }
        Condition.notify_all();
        for (std::thread& Worker : Workers) Worker.join();
    }

private:
    std::vector<std::thread> Workers;
    std::queue<std::function<void()>> Tasks;

    std::mutex QueueMutex;
    std::condition_variable Condition;
    bool Stop = false;
};