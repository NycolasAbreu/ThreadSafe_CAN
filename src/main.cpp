#include <iostream>
#include <memory>
#include <unistd.h>

#include "CanDriver.h"
#include "CanInterface.h"
#include "CanMessage.h"
#include "CanMessageObserverManager.h"
#include "SimpleMessageObserver.h"

void PrintCanMessage(const CanMessage& msg) {
  std::cout << "Sent CAN frame with ID: " << std::hex << msg.GetCanID() << "\n";
  std::cout << "Data: ";
  for (const auto& byte : msg.GetData())
  {
    std::cout << std::hex << static_cast<int>(byte) << " ";
  }
  std::cout << "\n";
}

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "Usage: <CAN interface> <CAN mode S send R read>" << "\n";
    return 1;
  }
  const std::string interface = argv[1];
  const std::string mode = argv[2];

  auto observer_manager = std::make_shared<CanMessageObserverManager>();

  auto observer1 = std::make_shared<SimpleMessageObserver>();
  observer_manager->RegisterObserver(observer1);

  auto observer2 = std::make_shared<SimpleMessageObserver>();
  observer_manager->RegisterObserver(observer2);

  CanInterface can(std::make_unique<CanDriver>(interface), observer_manager);
  can.Start();

  if (mode == "S" || mode == "s") {
    while (true)
    {
      const auto msg = CanMessage(0x111, {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08});
      if (can.SendMessage(msg))
      {
        PrintCanMessage(msg);
      }

      sleep(2);
    }
  }
  if (mode == "R" || mode == "r")
  {
    while (true)
    {
      // infinite loop to keep the program running
    }
  }
}
