#include "Entity.h"	
#include "Player.h"
#include "Utils.h"

void Entity::setAnimation(uint an) {
   if (an >= _model->numAnimations) {
      std::cerr << "Entity does not contain animation " << an << std::endl;
      exit(0);
   }

   _animNum = an;
   _animTime = 0;
}

void Entity::setDuration(double dur) {
   _animDuration = dur;
}

// _draw is for pretty much every single entity.
// entities with special draw() functions can choose to call _draw()
// for the common stuff so we don't have to copy paste
void Entity::_draw(double delta, Shader *shaderOverride) {
   if (_deleted || (_drawnthis && _edible)) 
      return;

   glm::mat4 Transl = glm::translate(MAT4_ID, _position);
   glm::mat4 Scale = glm::scale(MAT4_ID, _scale);
   glm::mat4 Rot = glm::rotate(MAT4_ID, getAngle(ENTITY_Y_AXIS, false), Y_AXIS);
   Rot = Rot * _model->rotAdjust;
   glm::mat4 ModelM = Transl * Rot * Scale;

   float distToCam = glm::distance(_position, game->camPos);
   #define FAR_DIST 700
   #define SWITCH_DIST 300
   uint meshNum = (uint)(distToCam / FAR_DIST * _model->numMeshes);
   meshNum = distToCam < SWITCH_DIST ? 0 : meshNum;
   if (meshNum >= _model->numMeshes)
      meshNum = _model->numMeshes - 1;
   Mesh * mesh = _model->getMesh(meshNum);
   
   glm::vec3 outlineColor = glm::vec3(0.0f);

   glm::mat4 * animTrans = NULL;
   glm::mat4 animMats[MAX_BONES];
   if (_model->numAnimations > 0) {
      animTrans = animMats;
      for (uint i = 0; i < MAX_BONES; i++)
         animMats[i] = glm::mat4(1.0);

      _model->getAnimationTransforms(_animNum, _animTime, _animDuration, animMats);
   }

   if (_edible && glm::distance(getPosition(), game->getPlayer()->getPosition()) < 850) {
      bool shadeRed;
      glm::vec3 playerscale = game->getPlayer()->getEdibleScale();
      glm::vec3 thisscale   = getEdibleScale();

      shadeRed = thisscale.x > playerscale.x && thisscale.y > playerscale.y
         && thisscale.z > playerscale.z;

      outlineColor = shadeRed ?
                     /* glm::vec3(0.8, 0.0, 0.0) : */
                     glm::vec3(0.0, 0.0, 0.0) :
                     glm::vec3(0.0, 0.8, 0.0);
   }
   if (shaderOverride == NULL) {
      _shader->render(ModelM, _material, mesh, animTrans, _texture, _useTexture, _normalMap, _useNormalMap, _shadowable, outlineColor, _wonkyShadows);
   } else {
      shaderOverride->render(ModelM, _material, mesh, animTrans, _texture, _useTexture, _normalMap, _useNormalMap, _shadowable, outlineColor, _wonkyShadows);
   }
}

void Entity::_update(double delta, glm::vec3 playerPos) {
   if (_deleted)
      return;

   if (_model->numAnimations > 0) {
      _animTime += delta;

      if (_animTime > _animDuration) {
         _animTime -= _animDuration;
      }
   }

   // any common update stuff goes here
   // float dist = delta * _speed;
   // _position.x += _direction.x * dist;
   // _position.y += _direction.y * dist;
   // _position.z += _direction.z * dist;
}

// TODO: figure out parameters, view frustrum culling etc
bool Entity::visible() {
   return !_deleted;
}

void Entity::setScale(glm::vec3 scale) {
   _scale = scale;
}

void Entity::setScale(float scale) {
   _scale = glm::vec3(scale);
}

glm::vec3 Entity::getScale() {
   return _scale;
}

void Entity::setPosition(glm::vec3 p) {
   _position = p;
}
  
glm::vec3 Entity::getPosition() {
   return _position;
}

void Entity::setDirection(glm::vec3 d) {
   _direction = d;
}
  
glm::vec3 Entity::getDirection() {
   return _direction;
}

float Entity::getAngle(short aboutAxis) {
   return getAngle(aboutAxis, true);
}

float Entity::getAngle(short aboutAxis, bool inRadians) {
   glm::vec2 v;

   float ang;

   switch (aboutAxis) {
      case ENTITY_X_AXIS:
         v = glm::normalize(glm::vec2(_direction.y, _direction.z));
      break;

      case ENTITY_Y_AXIS:
         v = glm::normalize(glm::vec2(_direction.x, _direction.z));
      break;

      case ENTITY_Z_AXIS:
         v = glm::normalize(glm::vec2(_direction.x, _direction.y));
      break;
   }

   assert(v.x != 0 || v.y != 0);

   ang = glm::angle(v, glm::vec2(1.0f, 0.0f));
   if (v.y > 0.0f) {
      ang = 360.0f - ang;
   }

   return inRadians ? ang * M_PI / 180.0f : ang;
}

void Entity::setSpeed(float s) {
   _speed = s;
}

float Entity::getSpeed() {
   return _speed;
}

void Entity::setTexture(Texture t) {
   _texture = t;
   _useTexture = true;
}

void Entity::setNormalMap(Texture t) {
   _normalMap = t;
   _useNormalMap = true;
}

bool Entity::UseTexture() {
   return _useTexture;
}

bool Entity::useNormalMap() {
   return _useNormalMap;
}

Entity* Entity::cloneEntity() {
   return new Entity(
      this->_model, // model
      this->_shader, // shader
      this->_position, // position
      this->_scale, // scale
      this->_direction, // direction
      this->_speed, // speed
      this->_material // mat
   );
}

Entity* Entity::cloneEntity(Model *m) {
   return new Entity(
      m, // model
      this->_shader, // shader
      this->_position, // position
      this->_scale, // scale
      this->_direction, // direction
      this->_speed, // speed
      this->_material // mat
   );
}
