#pragma once

#include "types.hpp"

enum class CommandType { OnFloor = 'F', InCabin = 'C' };

struct Command {
  CommandType type;
  core::FloorNum floor;
};