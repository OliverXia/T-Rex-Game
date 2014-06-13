#ifndef __ENTITY_H
#define __ENTITY_H

#include "Utils.h"
#include "glm/glm.hpp"
#include "Material.h"
#include "MStackHelp.h"
#include "Defines.h"
#include "Game.h"
#include "Shader.h"
#include "TextureHelper.h"
#include "MeshLoader.h"

#include <math.h>

class Entity {
protected:
   Model *           _model;
   Shader *          _shader;
   glm::vec3         _position;
   glm::vec3         _scale;
   glm::vec3         _direction;
   float             _speed;
   Material *        _material;
   RenderingHelper * _hierStack;
   Texture           _texture;
   Texture           _normalMap;
   bool              _useTexture;
   bool              _useNormalMap;
   bool              _edible;
   bool              _castsShadow;
   float             _upscale;
   bool              _deleted;
   bool              _drawnthis;
   bool              _drawOutline;
   uint              _animNum;
   double            _animTime;     // current progress of animation
   double            _animDuration; // how long the animation should take
   bool              _wonkyShadows;
   bool              _shadowable;

   void _draw(double timePassed, Shader *shaderOverride);
   void _update(double delta, glm::vec3 playerPos);

public:
   Entity(Model *m, Shader *shader, glm::vec3 pos, glm::vec3 scale,
    glm::vec3 direction, float speed, Material* material) {
      _model     = m;
      _shader    = shader;
      _position  = pos;
      _scale     = scale;
      _direction = direction;
      _speed     = speed;
      _material  = material;
      _hierStack = new RenderingHelper();
      _texture   = {0, 0};
      _normalMap = {0, 0};
      _useTexture = false;
      _useNormalMap = false;
      _deleted   = false;
      _edible    = true; // FIXME
      _castsShadow = true; // FIXME
      _upscale   = 0;    // FIXME
      _drawnthis = false;
      _animNum   = 0;
      _animTime  = 1.0;
      _animDuration = 1.0;
      _wonkyShadows = true;
      _shadowable = true;
   } 
   virtual ~Entity() {} //FIXME


   virtual void draw(double timePassed) { _draw(timePassed, NULL); }
   virtual void draw(double timePassed, Shader *shaderOverride) { _draw(timePassed, shaderOverride); }
   virtual void update(double deltaTime, glm::vec3 playerPos) { _update(deltaTime, playerPos); }

   void setAnimation(uint animNum);
   void setDuration(double blah);

   void setTexture(Texture t);
   bool UseTexture();
   void setNormalMap(Texture t);
   bool useNormalMap(); 

   bool visible();

   //add accessors as needed
   void setScale(glm::vec3 scale);
   void setScale(float scale);
   glm::vec3 getScale();
   
   glm::vec3 getEdibleScale() {
      return _model->edibleScale * _model->dimensions * _scale;
   }
   
   void setPosition(glm::vec3 p);
   glm::vec3 getPosition();

   void setDirection(glm::vec3 d);
   glm::vec3 getDirection();

   static Entity* spawnBunny();

   bool isEdible() { return _edible; }
   void setEdible(bool e) { _edible = e; }

   bool castsShadow() { return !_deleted && _castsShadow; }
   void castsShadow(bool e) { _castsShadow = e; }

   bool wonkyShadows() { return _wonkyShadows; }
   void wonkyShadows(bool e) { _wonkyShadows = e; }

   bool shadowable() { return _shadowable; }
   void shadowable(bool e) { _shadowable = e; }

   bool isDeleted() { return _deleted; }
   void markDeleted()    { _deleted = true; }

#define ENTITY_X_AXIS 0
#define ENTITY_Y_AXIS 1
#define ENTITY_Z_AXIS 2
   float getAngle(short aboutAxis);
   float getAngle(short aboutAxis, bool inRadians);

   void setSpeed(float s);
   float getSpeed();

   Entity* cloneEntity();
   Entity* cloneEntity(Model *m);
};

#endif
