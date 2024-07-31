#pragma once

#include <memory>
#include <thread>

class CanDriver;
class CanMessage;
class CanMessageObserver;
class CanMessageObserverManager;

class CanInterface 
{
  public:
    CanInterface(std::unique_ptr<CanDriver> can_driver,
                 const std::shared_ptr<CanMessageObserverManager>& observer_manager);

    virtual ~CanInterface() = default;

    void Start();
    void Stop();

    [[nodiscard]] bool SendMessage(const CanMessage& msg) const;

  private:
    void ReadLoop() const;

    std::unique_ptr<CanDriver> can_driver;
    std::shared_ptr<CanMessageObserverManager> observer_manager;

    std::thread read_thread;
    bool running = false;
};
