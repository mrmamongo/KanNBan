//
// Created by teufel on 12.04.23.
//

#ifndef KANNBAN_THREAD_POOL_HPP
#define KANNBAN_THREAD_POOL_HPP

#include <thread>
#include <future>
#include <functional>
#include "thread_safe_queue.hpp"

namespace kanban_server::utils {
    template<typename QueueType = thread_safe_queue<std::function<void()>>>
    class thread_pool {
    public:
        using wlock = std::unique_lock<std::shared_mutex>;
        using rlock = std::shared_lock<std::shared_mutex>;

    public:
        thread_pool() = default;

        ~thread_pool() {
            terminate();
        }

        thread_pool(const thread_pool &) = delete;

        thread_pool(thread_pool &&) = delete;

        thread_pool &operator=(const thread_pool &) = delete;

        thread_pool &operator=(thread_pool &&) = delete;

    public:
        void init(size_t num) {
            std::call_once(once_, [this, num]() {
                wlock lock(mtx_);
                stop_ = false;
                cancel_ = false;
                workers_.reserve(num);
                for (size_t i = 0; i < num; ++i) {
                    workers_.emplace_back(std::bind(&thread_pool<QueueType>::spawn, this));
                }
                inited_ = true;
            });
        }

        void terminate() {
            {
                wlock lock(mtx_);
                if (_is_running()) {
                    stop_ = true;
                } else {
                    return;
                }
            }
            cond_.notify_all();
            for (auto &worker: workers_) {
                worker.join();
            }
        }

        void cancel() {
            {
                wlock lock(mtx_);
                if (_is_running()) {
                    cancel_ = true;
                } else {
                    return;
                }
            }
            tasks_.clear();
            cond_.notify_all();
            for (auto &worker: workers_) {
                worker.join();
            }
        }

    public:
        bool inited() const {
            rlock lock(mtx_);
            return inited_;
        }

        bool is_running() const {
            rlock lock(mtx_);
            return _is_running();
        }

        int size() const{
            rlock lock(mtx_);
            return workers_.size();
        }

    private:
        bool _is_running() const {
            return inited_ && !stop_ && !cancel_;
        }

    private:
        void spawn(){
            for (;;) {
                bool pop = false;
                std::function<void()> task;
                {
                    wlock lock(mtx_);
                    cond_.wait(lock, [this, &pop, &task] {
                        pop = tasks_.pop(task);
                        return cancel_ || stop_ || pop;
                    });
                }
                if (cancel_ || (stop_ && !pop)) {
                    return;
                }
                task();
            }
        }

    public:
        template<class F, class... Args>
        auto async(F &&f, Args &&... args) const -> std::future<decltype(f(args...))>{
            using return_t = decltype(f(args...));
            using future_t = std::future<return_t>;
            using task_t = std::packaged_task<return_t()>;

            {
                rlock lock(mtx_);
                if (stop_ || cancel_)
                    throw std::runtime_error(
                            "Delegating task to a threadpool "
                            "that has been terminated or canceled.");
            }

            auto bind_func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
            std::shared_ptr<task_t> task = std::make_shared<task_t>(std::move(bind_func));
            future_t fut = task->get_future();
            tasks_.emplace([task]() -> void { (*task)(); });
            cond_.notify_one();
            return fut;
        }

    private:
        // TODO: Проверить, можно ли сделать эти переменные атомиками
        bool inited_{false};
        bool stop_{false};
        bool cancel_{false};
        std::vector<std::thread> workers_;
        mutable QueueType tasks_;
        mutable std::shared_mutex mtx_;
        mutable std::condition_variable_any cond_;
        mutable std::once_flag once_;
    };
}

#endif //KANNBAN_THREAD_POOL_HPP
