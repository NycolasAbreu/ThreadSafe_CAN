#include "SimpleMessageObserver.h"

#include <iostream>
#include <thread>

#include "CanMessage.h"

void SimpleMessageObserver::OnMessageReceived(const CanMessage& msg)
{
  std::cout << std::this_thread::get_id() << "\n";
  std::cout << "Received CAN frame with ID: " << std::hex << msg.GetCanID() << "\n";
  std::cout << "Data: ";
  for (const auto& byte : msg.GetData())
  {
    std::cout << std::hex << static_cast<int>(byte) << " ";
  }
  std::cout << "\n";
}
