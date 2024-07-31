#include "CanInterface.h"

#include "CanDriver.h"
#include "CanMessage.h"
#include "CanMessageObserverManager.h"

CanInterface::CanInterface(
  std::unique_ptr<CanDriver> can_driver, 
  const std::shared_ptr<CanMessageObserverManager>& observer_manager
)
  : can_driver(std::move(can_driver))
  , observer_manager(observer_manager)
{
}

void CanInterface::Start()
{
  if (!running)
  {
    running = true;
    can_driver->Open();
    observer_manager->Start();

    read_thread = std::thread(&CanInterface::ReadLoop, this);
  }
}

void CanInterface::Stop()
{
  running = false;
  can_driver->Close();
  observer_manager->Stop();

  if (read_thread.joinable()) 
  {
    read_thread.join();
  }
}

bool CanInterface::SendMessage(const CanMessage& msg) const 
{
  return can_driver->SendMessage(msg);
}

void CanInterface::ReadLoop() const
{
  while (running)
  {
    const std::optional<CanMessage> msg = can_driver->ReadMessage();
    if (msg.has_value())
    {
      observer_manager->EnqueueMessage(*msg);
    }
  }
}
