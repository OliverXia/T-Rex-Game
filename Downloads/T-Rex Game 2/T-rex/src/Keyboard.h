#ifndef __KEYBOARD_H_
#define __KEYBOARD_H_

#include "Player.h"

class Keyboard {
public:
  static bool onKey(Entity* player, int key, int scancode, int action, int mods);
};

#endif