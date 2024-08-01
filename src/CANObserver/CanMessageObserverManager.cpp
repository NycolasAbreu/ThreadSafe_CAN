#include "CanMessageObserverManager.h"

#include <algorithm>
#include <utility>

#include "CanMessage.h"
#include "CanMessageObserver.h"

CanMessageObserverManager::~CanMessageObserverManager() 
{
  Stop();
}

void CanMessageObserverManager::Start()
{
  running = true;

  for (auto& context : observer_contexts)
  {
    observer_threads.emplace_back(&CanMessageObserverManager::ObserverThread, this, context.get());
  }
}

void CanMessageObserverManager::Stop() 
{
  running = false;

  for (auto& context : observer_contexts) 
  {
    context->cv.notify_all();
  }

  for (auto& thread : observer_threads) 
  {
    if (thread.joinable()) 
    {
      thread.join();
    }
  }

  observer_threads.clear();
}

// TODO: Maybe the thread will not end when the observer is unregistered,
//       also the running flag will aways be true when the manager is alive
void CanMessageObserverManager::ObserverThread(ObserverContext* context) const
{
  while (running)
  {
    std::unique_lock lock(context->mutex);
    context->cv.wait(lock, [&]{ return !context->msg_queue.empty() || !running; });

    if (!running) break;

    const CanMessage msg = context->msg_queue.front();
    context->msg_queue.pop();
    lock.unlock();

    context->observer->OnMessageReceived(msg);
  }
}

void CanMessageObserverManager::EnqueueMessage(const CanMessage& msg) const
{
  for (auto& context : observer_contexts)
  {
    std::lock_guard lock(context->mutex);
    context->msg_queue.push(msg);
    context->cv.notify_one();
  }
}

void CanMessageObserverManager::RegisterObserver(const std::shared_ptr<CanMessageObserver>& observer)
{
  auto context = std::make_unique<ObserverContext>();
  context->observer = observer;
  observer_contexts.push_back(std::move(context));
  
  if (running) 
  {
    observer_threads.emplace_back(&CanMessageObserverManager::ObserverThread, this, observer_contexts.back().get());
  }
}

void CanMessageObserverManager::UnregisterObserver(const CanMessageObserver& observer)
{
  auto it = std::find_if(observer_contexts.begin(), observer_contexts.end(),
    [&observer](const auto& context) { return context->observer.get() == &observer; });
  
  if (it != observer_contexts.end()) 
  {
    observer_contexts.erase(it);
  }
}
