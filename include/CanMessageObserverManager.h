#pragma once

#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <memory>

class CanMessage;
class CanMessageObserver;

class CanMessageObserverManager 
{
  public:
    CanMessageObserverManager() = default;
    ~CanMessageObserverManager();

    void Start();
    void Stop();

    void RegisterObserver(const std::shared_ptr<CanMessageObserver>& observer);
    void UnregisterObserver(const CanMessageObserver& observer);

    void EnqueueMessage(const CanMessage& msg) const;

  private:
    struct ObserverContext
    {
      std::shared_ptr<CanMessageObserver> observer;
      std::queue<CanMessage> msg_queue;
      std::condition_variable cv;
      std::mutex mutex;
    };

    void ObserverThread(ObserverContext* context) const;

    std::vector<std::unique_ptr<ObserverContext>> observer_contexts;
    std::vector<std::thread> observer_threads;
    bool running = false;
};
