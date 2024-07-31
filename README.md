# CAN Interface Library

This project provides a robust and flexible C++ library for interfacing with CAN (Controller Area Network) buses. It's designed to be easily integrated into various applications that require CAN communication.

## Features

- Easy-to-use CAN interface with support for sending and receiving messages
- Thread-safe message handling
- Observer pattern implementation for flexible message processing
- Support for multiple message handlers
- Efficient use of modern C++ features (C++17)

## Building the Project

Install all the needed packages:

```bash
sudo apt-get update
sudo apt-get install build-essential cmake linux-headers-generic libsocketcan-dev
```

This project uses CMake as its build system. To build the project:

```bash
mkdir build
cd build
cmake ..
make
```

## Detailed Class Descriptions

### CanDriver
The `CanDriver` class provides low-level access to the CAN bus. It handles socket operations for sending and receiving CAN frames using the SocketCAN API on Linux systems.

### CanMessage
`CanMessage` represents a CAN frame, encapsulating the CAN ID and data payload.

### CanInterface
`CanInterface` serves as a high-level interface for CAN communication. It manages the `CanDriver` and coordinates message handling.

### CanMessageObserver
An abstract base class defining the interface for objects that wish to receive CAN messages.

### CanMessageObserverManager
Manages multiple `CanMessageObserver` instances, handling message distribution and observer lifecycle.

#### Multi-threaded Observer Execution

The CanMessageObserverManager implements a multi-threaded approach for message handling:

- Each registered observer runs in its own dedicated thread.
- Observer threads efficiently wait for new messages using condition variables.
- When a message arrives, it's added to each observer's queue and the respective thread is notified.
- Observers process messages concurrently, improving system performance and responsiveness.
- This design allows for excellent scalability, as new observers can be added without significantly impacting existing ones.


### SimpleMessageObserver
An example implementation of `CanMessageObserver` that prints received CAN messages to the console.

## Message Flow

1. `CanDriver` receives a CAN frame from the bus
2. `CanInterface` reads the message via its `ReadLoop()`
3. The message is passed to `CanMessageObserverManager`
4. `CanMessageObserverManager` distributes the message to all registered observers
5. Each observer's `OnMessageReceived()` method is called with the message


## Usage

Here's a simple example of how to use the CAN Interface Library:

```C++
#include "CanInterface.h"
#include "SimpleMessageObserver.h"

int main() {
    auto driver = std::make_unique<CanDriver>("can0");
    auto observer_manager = std::make_shared<CanMessageObserverManager>();
    CanInterface can(std::move(driver), observer_manager);

    auto observer = std::make_shared<SimpleMessageObserver>();
    observer_manager->RegisterObserver(observer);

    can.Start();

    // Your application logic here

    can.Stop();
    return 0;
}
```