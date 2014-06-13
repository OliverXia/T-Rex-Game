#ifndef __PLAYER_H
#define __PLAYER_H

#include "Game.h"
#include "GLSL_helper.h"
#include "Utils.h"
#include "Defines.h"

class Player : public Entity {
private:
   glm::vec3 _lastPos;
public:
   bool lefting, righting, forwarding, backing;

   Player(Model *m, Shader *shader, glm::vec3 pos, glm::vec3 scale,
   glm::vec3 direction, float speed, Material* material) : 
     Entity(m, shader, pos, scale, direction, speed, material) {
      lefting = righting = forwarding = backing = false;
      _lastPos = pos;
      _edible = false;
      _animDuration = 6.5;
    }

    void update(double nmillis, glm::vec3 playerPos);
    void resetPosition() { setPosition(_lastPos); }
};

#endif
