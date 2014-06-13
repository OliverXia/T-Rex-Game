#ifndef __GAME_H__
#define __GAME_H__

#ifdef __APPLE__
#include <OPENGL/gl.h>
#include <ApplicationServices/ApplicationServices.h>
#endif

#include <string>
#include <vector>
#include <irrklang/irrKlang.h>
#include <iostream>  

class Entity;
class Game;
class KDNode;
class ShaderHandler;
class Shader;
class Player;

#include "glm/glm.hpp"
#include "Entity.h"
#include "TextureHelper.h"
#include "RandWorld.h"
#include "Defines.h"
#include "Spatial.h"
#include "Shader.h"
#include "Frustrum.h"
#include "ParticleSpawner.h"


#define SHADOW_RES 1024

extern Game * game;

extern irrklang::ISoundEngine *engine;

class Game {
public:
   glm::vec3 camPos;
   glm::vec3 camDir;
   glm::vec3 camUp;
   glm::vec3 lightPos;
   glm::vec3 sunColor;
   glm::vec3 sunDir;
   float g_yaw, g_pitch;

private:
	std::vector<Entity *> entities;
   KDNode *ekdTree;

   std::vector<Entity *> nonUpNonEds;
   KDNode *nuneTree;

   Frustrum frust;

   Player *player;
   Entity *skybox;
   Entity *skyMeteor;
   
   Entity *hudLeft;
   Entity *hudRight;  
   Entity *endScreen; 

   
   ParticleSpawner* fountain; 
   ParticleSpawner* meteorP;

   // fxaa stuff
   GLuint fxaaFb;
   GLuint fxaaRb;

   Entity *fxaaScreen;
   // end fxaa stuff

   /* GLuint outlineFb; */
   /* GLuint outlineRb; */
   
   // shadow stuff
   GLuint shadowFb;
   // end shadow stuff

   int g_startx, g_starty;
   int w_width, w_height;
   float zoom;
   float camShake;
   double lastTime;
   
   void moveCamera(double delta);
   void detectCollisions();
      
   void InitPlayer();
   void InitEnvironment();
   void InitSkybox();
   void InitHUD();
   void InitMeteor();
   void DrawHUD();
   void InitEndScreen();
   void DrawEndScreen(int winOrLose);
   void UpdateHUD();
   void InitFXAA(int width, int height);
   void InitShadow();
   void InitFountain();

   void drawGrid(int x, int z, int tileNum, glm::vec2 offset, World gameWorld);
   void drawGridC(int x, int z, int tileNum, glm::vec2 offset, World gameWorld);
   void drawBuilding(int x, int z, int buildNum, glm::vec2 offset, World gameWorld);
   void drawObjects(int x, int z, int buildNum, glm::vec2 offset, World gameWorld);
   void InitPark(float xO, float zO, int num);
   void InitCity(float xO, float zO, int num);
   void InitWorld();

   int bunnyTimer, timePassed;

   ShaderHandler *sHandler;
   TextureHelper texHelper;

public:
	Game(int argc, char ** argv);
	void onMouseMove(GLFWwindow * window, int x, int y);
	bool onKey(int key, int scancode, int action, int mods);
	bool doFrame(double timePassed, int w_width, int w_height);
   Texture getShadowMap();
   TextureHelper *getTexHelper() { return &texHelper; }

   glm::vec3 getCamUp() { return camUp; }
   ShaderHandler *getSHandler() { return sHandler; }
   Player *getPlayer() { return player; }
};

#endif
