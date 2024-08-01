#pragma once

class CanMessage;

class CanMessageObserver
{
  public:
    CanMessageObserver() = default;
    virtual ~CanMessageObserver() = default;

    virtual void OnMessageReceived(const CanMessage& msg) = 0;
};
