// CanDriver.h
#pragma once

#include <string>
#include <optional>

class CanMessage;

class CanDriver
{
  public:
    explicit CanDriver(const std::string& interface_name);
    ~CanDriver();

    bool Open();
    void Close();

    [[nodiscard]] bool SendMessage(const CanMessage& msg) const;
    [[nodiscard]] std::optional<CanMessage> ReadMessage() const;

  private:
    std::string interface_name;
    int socket_fd = -1;
};