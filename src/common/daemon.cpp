#include "daemon.hpp"

Daemon::Daemon(double intervalInSeconds, Task task)
    : interval_(intervalInSeconds)
    , task_(std::move(task))
    , lastTaskCompleted_(TimePoint::min())
    , thread_(&Daemon::loop, this)
{
}

Daemon::~Daemon()
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        shouldRun_ = false;
    }
    cv_.notify_all();
    thread_.join();
}

void
Daemon::loop()
{
    while (shouldRun_)
    {
        task_();
        lastTaskCompleted_ = std::chrono::steady_clock::now();
        {
            std::unique_lock<std::mutex> lock(mutex_);

            // TODO: use wait_until instead of wait_for, warn about missed time
            // slots
            cv_.wait_for(lock, interval_, [this]() { return !shouldRun_; });
        }
    }
}
