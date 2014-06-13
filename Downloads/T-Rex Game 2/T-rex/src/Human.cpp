#include "Human.h"

#define HUMAN_SPEED 20.0f

void Human::update(double nmillis, glm::vec3 playerPos) {
  _update(nmillis, playerPos);

  if (glm::distance(playerPos, getPosition()) < 500) {
	glm::vec3 dirToEnt = glm::normalize(playerPos - getPosition());
	dirToEnt.y = 0;
  	setDirection(-1.f * dirToEnt);
  	glm::vec3 v = getPosition() - dirToEnt * (float) nmillis * HUMAN_SPEED;
  	v.y = getPosition().y;
  	setPosition(v);
  }
}
