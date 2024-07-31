#include "CanDriver.h"

#include <algorithm>
#include <iostream>
#include <linux/can.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <cstring>

#include "CanMessage.h"

CanDriver::CanDriver(const std::string& interface_name)
  : interface_name(interface_name)
{
}

CanDriver::~CanDriver()
{
  Close();
}

bool CanDriver::Open()
{
  sockaddr_can addr{};
  ifreq ifr{};

  socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
  if (socket_fd == -1) 
  {
    std::cerr << "Error opening socket" << std::endl;
    return false;
  }

  std::strcpy(ifr.ifr_name, interface_name.c_str());
  if (ioctl(socket_fd, SIOCGIFINDEX, &ifr) == -1) 
  {
    std::cerr << "Error getting interface index" << std::endl;
    close(socket_fd);
    return false;
  }

  addr.can_family = AF_CAN;
  addr.can_ifindex = ifr.ifr_ifindex;

  if (bind(socket_fd, (sockaddr *)&addr, sizeof(addr)) == -1) 
  {
    std::cerr << "Error binding socket" << std::endl;
    close(socket_fd);
    socket_fd = -1;
    return false;
  }

  return true;
}

void CanDriver::Close()
{
  if (socket_fd != -1) 
  {
    close(socket_fd);
    socket_fd = -1;
  }
}

bool CanDriver::SendMessage(const CanMessage& msg) const
{
  if (socket_fd == -1) 
  {
    return false;
  }

  can_frame frame{};
  frame.can_id = msg.GetCanID();
  constexpr size_t MAX_DATA_LENGTH = CAN_MAX_DLC;
  frame.can_dlc = static_cast<unsigned char>(std::min(msg.GetData().size(), MAX_DATA_LENGTH));
  std::copy_n(msg.GetData().begin(), frame.can_dlc, frame.data);

  const ssize_t bytes_sent = write(socket_fd, &frame, sizeof(frame));
  return bytes_sent == sizeof(frame);
}

// TODO: Insert polling to check for new messages
std::optional<CanMessage> CanDriver::ReadMessage() const
{
  if (socket_fd == -1) 
  {
    return std::nullopt;
  }

  can_frame frame{};
  const ssize_t bytes_read = read(socket_fd, &frame, sizeof(frame));
  if (bytes_read == sizeof(frame))
  {
    CanMessage::Data data{};
    data.reserve(frame.can_dlc);
    std::copy_n(std::begin(frame.data), frame.can_dlc, std::back_inserter(data));

    return CanMessage(frame.can_id, data);
  }

  return std::nullopt;
}
