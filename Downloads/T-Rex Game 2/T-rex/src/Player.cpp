#include "Player.h"

void Player::update(double seconds, glm::vec3 playerPos) {
   Entity::_update(seconds, playerPos);

   glm::vec3 delta = glm::vec3(0, 0, 0);

   if (forwarding)
      delta += glm::normalize(_direction);
   if (backing)
      delta -= glm::normalize(_direction);
   if (lefting) {
      /* delta += glm::normalize(glm::cross(game->getCamUp(), _direction)); */
      game->g_yaw -= 0.08;
   }

   if (righting) {
      /* delta -= glm::normalize(glm::cross(game->getCamUp(), _direction)); */
      game->g_yaw += 0.08;
   }


   if (glm::length(delta) != 0.0f) {
      delta.y = 0;
      _lastPos   = _position;
      _position += (float) seconds * glm::normalize(delta) * _speed;
      _position.y = getEdibleScale().y * .5f - 4.4f;
   }

   float tx = seconds/1000.f * cos(game->g_pitch)*cos(game->g_yaw);
   float ty = seconds/1000.f * sin(game->g_pitch);
   float tz = seconds/1000.f * cos(game->g_pitch)*cos(M_PI/2 - game->g_yaw);

   setDirection(glm::normalize(glm::vec3(tx, ty, tz)));
}
