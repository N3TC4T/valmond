#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <thread>

/// Execute task periodically as long as Daemon is alive
class Daemon
{
public:
    /// seconds
    using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
    using Duration = std::chrono::duration<double>;

    using Task = std::function<void()>;

    explicit Daemon(double intervalInSeconds, Task task);

    /// Cannot copy
    Daemon(const Daemon&) = delete;

    /// Cannot be assigned
    Daemon&
    operator=(const Daemon&) = delete;

    /// When was task completed last?
    /// Useful for checking daemon health
    TimePoint
    lastTaskCompleted() const
    {
        return lastTaskCompleted_;
    }

    ~Daemon();

private:
    /// Internal function which runs in thread_
    void
    loop();

    Duration interval_;
    Task task_;

    bool shouldRun_ = true;
    std::atomic<TimePoint> lastTaskCompleted_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::thread thread_;
};
