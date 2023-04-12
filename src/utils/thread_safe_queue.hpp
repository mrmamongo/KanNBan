//
// Created by teufel on 12.04.23.
//

#ifndef KANNBAN_THREAD_SAFE_QUEUE_HPP
#define KANNBAN_THREAD_SAFE_QUEUE_HPP

#include <mutex>
#include <shared_mutex>
#include <queue>

namespace kanban_server::utils {
    template<typename T>
    class thread_safe_queue {
    public:
        using wlock = std::unique_lock<std::shared_mutex>;
        using rlock = std::shared_lock<std::shared_mutex>;

    public:
        thread_safe_queue() = default;

        ~thread_safe_queue() {
            clear();
        }

        thread_safe_queue(const thread_safe_queue &) = delete;
        thread_safe_queue(thread_safe_queue &&) = delete;
        thread_safe_queue &operator=(const thread_safe_queue &) = delete;
        thread_safe_queue &operator=(thread_safe_queue &&) = delete;

    public:
        bool empty() const {
            rlock lock(_mtx);
            return _queue.empty();
        }

        size_t size() const {
            rlock lock(_mtx);
            return _queue.size();
        }

    public:
        void clear() {
            wlock lock(_mtx);
            while (!_queue.empty()) {
                _queue.pop();
            }
        }

        void push(const T &obj) {
            wlock lock(_mtx);
            _queue.push(obj);
        }

        template<typename... Args>
        void emplace(Args &&... args) {
            wlock lock(_mtx);
            _queue.emplace(std::forward<Args>(args)...);
        }

        bool pop(T &holder) {
            wlock lock(_mtx);
            if (_queue.empty()) {
                return false;
            } else {
                holder = std::move(_queue.front());
                _queue.pop();
                return true;
            }
        }

    private:
        std::queue<T> _queue;
        mutable std::shared_mutex _mtx;
    };
}
#endif //KANNBAN_THREAD_SAFE_QUEUE_HPP
