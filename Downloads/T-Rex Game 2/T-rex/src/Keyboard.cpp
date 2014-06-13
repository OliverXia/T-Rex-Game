#include "Keyboard.h"
#include "Player.h"

#define GL_GLEXT_PROTOTYPES
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

bool Keyboard::onKey(Entity *e, int key, int scancode, int action, int mods) {
   Player *player = (Player *)e;

   if (action == GLFW_PRESS) {
      switch( key ) {
         case GLFW_KEY_W:
            player->setAnimation(1);
            player->setDuration(1.2);
            player->forwarding = true;
            break;
         case GLFW_KEY_S:
            player->setAnimation(1);
            player->setDuration(1.2);
            player->backing = true;
            break;
         case GLFW_KEY_A:
            player->lefting = true;
            break;
         case GLFW_KEY_D:
            player->righting = true;
            break;
         case GLFW_KEY_SPACE:
            player->setAnimation(2);
            player->setDuration(3);
            break;
         case GLFW_KEY_ESCAPE:
            return true;
      }
   }
   else if (action == GLFW_RELEASE) {
      switch( key ) {
         case GLFW_KEY_W:
            player->setAnimation(0);
            player->setDuration(5);
            player->forwarding = false;
            break;
         case GLFW_KEY_S:
            player->setAnimation(0);
            player->setDuration(5);
            player->backing = false;
            break;
         case GLFW_KEY_A:
            player->lefting = false;
            break;
         case GLFW_KEY_D:
            player->righting = false;
            break;
      }
   }
   return false;
}
