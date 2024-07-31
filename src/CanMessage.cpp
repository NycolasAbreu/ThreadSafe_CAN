#include "CanMessage.h"

CanMessage::CanMessage(uint32_t can_id, const Data& data)
  : can_id(can_id)
  , data(data)
{
}

auto CanMessage::GetCanID() const -> uint32_t
{
  return can_id;
}

auto CanMessage::SetCanID(uint32_t new_id) -> void
{
  can_id = new_id;
}

auto CanMessage::SetData(const Data& new_data) -> void
{
  data = new_data;
}

auto CanMessage::GetData() const -> Data
{
  return data;
}
