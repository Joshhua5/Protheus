#pragma once
#include "Mouse.h"
#include <Vector2.h>

namespace Pro{
  namespace Input{
    struct MouseEvent{
      Math::Vector2<int> position;
      KEY_PRESSED state;
      KEY key;
    };
  }
}
