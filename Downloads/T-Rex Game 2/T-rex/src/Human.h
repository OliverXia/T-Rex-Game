#ifndef HUMAN_H__
#define HUMAN_H__

#include "Entity.h"

class Human : public Entity {
public:
    Human(Model *m, Shader *shader, glm::vec3 pos, glm::vec3 scale,
      glm::vec3 direction, float speed, Material* material) : 
        Entity(m, shader, pos, scale, direction, speed, material) {}

    void update(double nmillis, glm::vec3 playerPos);
};

#endif
