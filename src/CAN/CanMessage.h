#pragma once

#include <vector>
#include <cstdint>

class CanMessage
{
  public:
    using Data = std::vector<uint8_t>;

    CanMessage() = default;
    CanMessage(uint32_t can_id, const Data& data);
    ~CanMessage() = default;

    [[nodiscard]] auto GetCanID() const -> uint32_t;
    auto SetCanID(uint32_t new_id) -> void;

    auto SetData(const Data& new_data) -> void;
    [[nodiscard]] auto GetData() const -> Data;

  private:
    uint32_t can_id = -1;
    Data data {};
};
