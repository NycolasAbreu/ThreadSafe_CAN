#pragma once

#include "CanMessageObserver.h"

class SimpleMessageObserver : public CanMessageObserver
{
  public:
    SimpleMessageObserver() = default;
    ~SimpleMessageObserver() override = default;

    void OnMessageReceived(const CanMessage& msg) override;
};
