#include "Game.h"
#include "MeshLoader.h"

#define GL_GLEXT_PROTOTYPES
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include <vector>
#include <stdio.h>

#include "Entity.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "GLSL_helper.h"
#include "Utils.h"
#include "Defines.h"
#include "Player.h"
#include "Keyboard.h"
#include "Human.h"
#include "ParticleSpawner.h"

#define PITCH_MAX -0.1 // 80 DEGREES
#define PITCH_MIN 1.3962634 // 80 DEGREES
#define TILE_SIZE 100.0f // keep as this for the moment
#define TILE_Y -5.0f
#define VIEW_DIST 30000.0f
#define VIEW_NEAR 0.1f


#define TILE_SCALE 1.0f

static Model *trexModel;
static Model *benchModel;
static Model *lampModel;
static Model *hydrantModel;
static Model *guyModel;
static Model *girlModel;
static Model *trashModel;
static Model *birdModel;
static Model *fountainModel;
static Model *bikeModel;
static Model *skyBox;
static Model *plane;
static Model *meteorSky;

static Model *gridModel;

static Model *sBuildModel;
static Model *sBuildModel_TWO_ONE; // (x, z)
static Model *sBuildModel_ONE_TWO;
static Model *mBuildModel;
static Model *mBuildModel_TWO_ONE; // (x, z)
static Model *mBuildModel_ONE_TWO;
static Model *mBuildModel_TWO_TWO;
static Model *lBuildModel_TWO_TWO;
static Model *lBuildModel_TWO_TRE;
static Model *lBuildModel_TRE_TWO;
static Model *xlBuildModel_TWO_TWO;
static Model *xlBuildModel_TRE_TRE;

static World gameWorld;
static float worldYScale = 0.0f;

static bool BUILDINGS = true;

Game * game;

Game::Game(int argc, char ** argv) {
   gameWorld = World(); 
   camPos = glm::vec3(2.0f, 3.0f, -1.0f);
	camUp = glm::vec3(0.0f, 1.0f, 0.0f);
	lightPos = glm::vec3(0.0f, 5.0f, 0.0f);
	sunColor = glm::vec3(1.0f, 1.0f, 1.0f);
	sunDir = glm::vec3(-1.0f, -1.0f, -1.0f);

   zoom = 20;

   unsigned int maxVerts = 5000;
   unsigned int numMeshes = 20;

   unsigned int rmk = 0;

   if (argc == 2) {
      if (!strcmp(argv[1], "-remake")) {
         std::cerr << "Creating all mod files...\n";
         rmk = REMAKE;
      }
      else
         std::cerr << "Unknown argument: " << argv[1];
   }


   // Flags: 0 | REMAKE | CENTER | RESIZE | REDUCE
   unsigned int flags = rmk;
   unsigned int groundFlags = rmk;
   unsigned int staticFlags = rmk | CENTER | REDUCE;

   sHandler = new ShaderHandler();

   gridModel = new Model("resources/models/1x1_tile.dae", numMeshes, maxVerts, groundFlags);
   gridModel->rotAdjust = glm::rotate(glm::mat4(1.0f), 90.0f, Y_AXIS);

   sBuildModel = new Model("resources/models/1x1_sbuilding.dae", numMeshes, maxVerts, groundFlags);
   sBuildModel_ONE_TWO = new Model("resources/models/1x2_sbuilding.dae", numMeshes, maxVerts, groundFlags);
   sBuildModel_TWO_ONE = new Model("resources/models/2x1_sbuilding.dae", numMeshes, maxVerts, groundFlags);
   mBuildModel = new Model("resources/models/1x1_mbuilding.dae", numMeshes, maxVerts, groundFlags);
   mBuildModel_ONE_TWO = new Model("resources/models/1x2_mbuilding.dae", numMeshes, maxVerts, groundFlags);
   mBuildModel_TWO_ONE = new Model("resources/models/2x1_mbuilding.dae", numMeshes, maxVerts, groundFlags);
   mBuildModel_TWO_TWO = new Model("resources/models/2x2_mbuilding.dae", numMeshes, maxVerts, groundFlags);
   lBuildModel_TWO_TWO = new Model("resources/models/2x2_Lbuilding.dae", numMeshes, maxVerts, groundFlags);
   lBuildModel_TWO_TRE = new Model("resources/models/2x3_Lbuilding.dae", numMeshes, maxVerts, groundFlags);
   lBuildModel_TRE_TWO = new Model("resources/models/3x2_Lbuilding.dae", numMeshes, maxVerts, groundFlags);
   xlBuildModel_TWO_TWO = new Model("resources/models/2x2_xLbuilding.dae", numMeshes, maxVerts, groundFlags);
   xlBuildModel_TRE_TRE = new Model("resources/models/3x3_xLbuilding.dae", numMeshes, maxVerts, groundFlags);

   fountainModel = new Model("resources/models/fountain.dae", numMeshes, maxVerts, staticFlags);
   fountainModel->rotAdjust = glm::rotate(MAT4_ID, 0.0f, X_AXIS);

   trashModel = new Model("resources/models/trash.dae", numMeshes, maxVerts, staticFlags);
   trashModel->rotAdjust = glm::rotate(MAT4_ID, 0.0f, X_AXIS);

   benchModel = new Model("resources/models/bench.dae", numMeshes, maxVerts, staticFlags);
   benchModel->rotAdjust = glm::rotate(MAT4_ID, 90.0f, X_AXIS);

   hydrantModel = new Model("resources/models/fire_hydrant.dae", numMeshes, maxVerts, staticFlags);
   hydrantModel->rotAdjust = glm::rotate(MAT4_ID, 0.0f, X_AXIS);

   birdModel = new Model("resources/models/bird.dae", numMeshes, maxVerts, staticFlags);
   birdModel->loadAnimation("resources/animations/bird_flying.dae");   
   birdModel->rotAdjust = glm::rotate(MAT4_ID, -90.0f, Y_AXIS);

   lampModel = new Model("resources/models/lamp_post.dae", numMeshes, maxVerts, staticFlags);
   lampModel->rotAdjust = glm::rotate(MAT4_ID, 0.0f, X_AXIS);

   guyModel = new Model("resources/models/guy.dae", numMeshes, maxVerts, flags);
   guyModel->loadAnimation("resources/animations/guy_running.dae");
   guyModel->rotAdjust = glm::rotate(MAT4_ID, 90.0f, Y_AXIS);

   bikeModel = new Model("resources/models/bike.dae", numMeshes, maxVerts, flags);
   
   // girlModel = new Model("resources/models/girl.dae", numMeshes, maxVerts, flags);
   // girlModel->loadAnimation("resources/animations/girl_running.dae");
   // girlModel->rotAdjust = glm::rotate(MAT4_ID, 90.0f, Y_AXIS);

   trexModel = new Model("resources/models/trex.dae", numMeshes, maxVerts, flags | CENTER);
   trexModel->loadAnimation("resources/animations/trex_idol.dae");
   trexModel->loadAnimation("resources/animations/trex_walk.dae");
   trexModel->loadAnimation("resources/animations/trex_roar.dae");
   trexModel->rotAdjust = glm::rotate(MAT4_ID, 90.0f, Y_AXIS);

   skyBox = new Model("resources/models/sky.dae", numMeshes, 10000, flags);
   skyBox->rotAdjust = glm::rotate(MAT4_ID, -90.0f, Z_AXIS);

   plane = new Model("resources/models/plane.dae", 0, 10000, flags);
   plane->rotAdjust = glm::rotate(MAT4_ID, 0.0f, Z_AXIS);
   DebugBufferData(GL_ARRAY_BUFFER, plane->base->vbo, 3, 12);
   
   //meteor
   meteorSky = new Model("resources/models/meteor.dae", numMeshes, maxVerts, flags);
   meteorSky->rotAdjust = glm::rotate(MAT4_ID, 0.0f, X_AXIS);

   glEnable(GL_CULL_FACE);
   glEnable(GL_NORMALIZE);

   sHandler->installShader("texonly",
      "shaders/tex.vert.glsl", "shaders/tex.frag.glsl", GL_FRONT);
   sHandler->installShader("hud",
      "shaders/HUD.vert.glsl", "shaders/HUD.frag.glsl", GL_BACK);
   sHandler->installShader("EndScreen",
      "shaders/EndScreen.vert.glsl", "shaders/EndScreen.frag.glsl", GL_BACK);
   sHandler->installShader("texphong", 
       "shaders/silhouette.vert.glsl", "shaders/silhouette.frag.glsl", GL_FRONT); 
   sHandler->installShader("texphong",
      "shaders/tex.phong.vert.glsl", "shaders/tex.phong.frag.glsl", GL_BACK);
   sHandler->installShader("fxaa",
      "shaders/fxaa.vert.glsl", "shaders/fxaa.frag.glsl", GL_BACK);
   sHandler->installShader("depth",
      "shaders/depth.vert.glsl", "shaders/depth.frag.glsl", GL_FRONT);
   sHandler->installShader("particles",
      "shaders/particle.vert.glsl", "shaders/particle.frag.glsl", GL_FRONT);

   InitPlayer();
   InitSkybox();
   InitMeteor(); 
   InitHUD();
   InitWorld();
   InitFXAA(WIN_WIDTH, WIN_HEIGHT);
   InitShadow();
   //InitFountain();
   InitEndScreen();
   
   bunnyTimer = 0;

   ekdTree = KDNode::kdTree(entities);
   lastTime = 0;
}

void Game::InitFXAA(int width, int height) {
   glGenFramebuffers(1, &fxaaFb);
   glGenRenderbuffers(1, &fxaaRb);
   Texture tex;

   glBindFramebuffer(GL_FRAMEBUFFER, fxaaFb);
   
   texHelper.createTexture("screen", width, height);
   tex = texHelper.getTexture("screen");

   /* glActiveTexture(GL_TEXTURE0 + tex.activeTex); */
   glBindTexture(GL_TEXTURE_2D, texHelper.getTexture("screen").texId);

   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
         GL_TEXTURE_2D, texHelper.getTexture("screen").texId, 0);

   glBindRenderbuffer(GL_RENDERBUFFER, fxaaRb);
   glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
   glFramebufferRenderbuffer(GL_FRAMEBUFFER, 
         GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fxaaRb);
   
   // back to normal mode
   glBindFramebuffer(GL_FRAMEBUFFER, 0);

   fxaaScreen = new Entity (
      plane,
      sHandler->getShader("fxaa"),
      player->getPosition(),
      glm::vec3(1),
      glm::vec3(0.f, 0.f, -1.f), // direction
      0.0f,
      NULL // mat
   );

   fxaaScreen->setTexture(texHelper.getTexture("screen"));
}

/* void Game::InitOutlines(int width, int height) { */
/*    glGenFramebuffers(1, &outlineFb); */
/*    glGenRenderbuffers(1, &outlineRb); */
/*    Texture tex; */

/*    glBindFramebuffer(GL_FRAMEBUFFER, outlineFb); */
   
/*    texHelper.createTexture("outlines", width, height, OUTLINE_TEXTURE_UNIT); */
/*    tex = texHelper.getTexture("outlines"); */

/*    /1* glActiveTexture(GL_TEXTURE0 + tex.activeTex); *1/ */
/*    glBindTexture(GL_TEXTURE_2D, texHelper.getTexture("outlines").texId); */

/*    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, */
/*          GL_TEXTURE_2D, texHelper.getTexture("outlines").texId, 0); */

/*    glBindRenderbuffer(GL_RENDERBUFFER, outlineRb); */
/*    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height); */
/*    glFramebufferRenderbuffer(GL_FRAMEBUFFER, */ 
/*          GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, outlineRb); */
   
/*    // back to normal mode */
/*    glBindFramebuffer(GL_FRAMEBUFFER, 0); */
/* } */

void Game::InitShadow() {
   // TODO: 1) make shadows cast coherently / from the right direction
   //       2) figure out how to do higer-res shadow maps
   //       3) join the dance party
   glGenFramebuffers(1, &shadowFb);
   glBindFramebuffer(GL_FRAMEBUFFER, shadowFb);
   texHelper.createDepthTexture("shadowmap", SHADOW_RES);
   glFramebufferTexture2D(
      GL_FRAMEBUFFER,
      GL_DEPTH_ATTACHMENT,
      GL_TEXTURE_2D,
      texHelper.getTexture("shadowmap").texId,
      0
   );

   glDrawBuffer(GL_NONE); // No color buffer is drawn to.

   // Always check that our framebuffer is ok
   if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      printf("OH SHEEZY\n");
      return;
   } else {
      printf("WE'RE GOOD\n");
   }
}

// TODO: delegate player controls and player-specific logic to a Player class
void Game::onMouseMove(GLFWwindow * window, int x, int y) {
   /* int endx = x; */
   /* int endy = w_height - y - 1; */

   /* if (!g_startx || !endx) { */
   /*    g_startx = endx; */
   /*    g_starty = endy; */
   /*    return; */
   /* } */
   
   /* float startWX = linScale(0, w_width, g_startx, -1, 1); */
   /* float startWY = linScale(0, w_height, g_starty, -1, 1); */
   /* float endWX = linScale(0, w_width, endx, -1, 1); */
   /* float endWY = linScale(0, w_height, endy, -1, 1); */
   
   /* if (g_startx != endx || g_starty != endy) { */
   /*    g_pitch = g_pitch + (endWY - startWY) * M_PI / 2.0; */
   /*    g_yaw = g_yaw + (endWX - startWX) * M_PI / 2.0; */
   /*    g_pitch = qmin(g_pitch, PITCH_MAX); */
   /*    g_pitch = qmax(g_pitch, -PITCH_MIN); */
   /*    if (g_yaw >= 2.0 * M_PI) */
   /*       g_yaw -= 2.0 * M_PI; */
   /*    if (g_yaw < 0) */
   /*       g_yaw += 2.0 * M_PI; */

   /*    float tx = cos(g_pitch)*cos(g_yaw); */
   /*    float ty = sin(g_pitch); */
   /*    float tz = cos(g_pitch)*cos(-g_yaw + M_PI/2); */

   /*    player->setDirection(glm::normalize(glm::vec3(tx, ty, tz))); */
   /* } */

   /* g_startx = endx; */
   /* g_starty = endy; */
}

bool Game::onKey(int key, int scancode, int action, int mods) {
   if ((!engine->isCurrentlyPlaying("./audio/explosion.wav")) && (!engine->isCurrentlyPlaying("./audio/footstep.wav"))) {
      if (player->getScale().x > 2.f) 
         engine->play2D("./audio/explosion.wav", false);
      else 
         engine->play2D("./audio/footstep.wav", false);
   }

   if (action == GLFW_PRESS && key == GLFW_KEY_SPACE)
      engine->play2D("./audio/dinoroar.wav", false);

   return Keyboard::onKey(player, key, scancode, action, mods);
}

#define CAM_SPEED 0.10f
#define DIST_DELTA 0.2f

void Game::moveCamera(double d) {
   glm::vec3 pDir = player->getDirection();
   glm::vec3 pPos;
   if (player->getScale().x > 1) {
      pPos = player->getPosition();
      pPos.y += 10;
   }
   else
      pPos = player->getPosition();
   glm::vec3 delta = (pDir - camDir) * CAM_SPEED;
   camDir += delta;
   
   glm::vec3 restPos = pPos - 2 * zoom * pDir;
   glm::vec3 distToRest = restPos - camPos;

   camPos += DIST_DELTA * distToRest * (float) (d*10);
}

#define MAX_UI_TIME 150.0

void Game::detectCollisions() {
   KDNode *cursor = ekdTree;

   double pz = player->getPosition().z;
   double px = player->getPosition().x;

   while (cursor) {
      for (auto e : entities) { 
         float dist = glm::distance(e->getPosition(), player->getPosition());      
         if ((dist < 0.33*(player->getEdibleScale().x + e->getEdibleScale().x) 
            || dist < 0.33*(player->getEdibleScale().z + e->getEdibleScale().z))
            && e->isEdible() && !e->isDeleted()) {
            // if bigger, remove the object from entities
            if (player->getEdibleScale().x > e->getEdibleScale().x 
                  || player->getEdibleScale().z > e->getEdibleScale().z
                  || player->getEdibleScale().y > e->getEdibleScale().y) {
               e->markDeleted();
               engine->play2D("./audio/dinoroar.wav", false);
               player->setScale(player->getScale().x + 0.15);
               player->setSpeed(player->getSpeed() + 3);
               glm::vec3 posAdj = player->getPosition();
               posAdj.y = player->getEdibleScale().x / 2.0f;
               player->setPosition(posAdj);
               zoom = 2 * player->getEdibleScale().x;
               camShake = 0.05f;
            } 
            // if smaller than the object's scale, then push away from its center.
            else {
               player->resetPosition();
            }
         }
      }

      cursor = cursor->getAxis() == KD_Z_AXIS ? 
         pz < (*cursor->getEnts())[0]->getPosition().z ? cursor->getLeft() : cursor->getRight()
         : px < (*cursor->getEnts())[0]->getPosition().x ? cursor->getLeft() : cursor->getRight();
   }
}

bool Game::doFrame(double timePassed, int width, int height) {
   w_width = width;
   w_height = height;
   static int winOrLose = 0;

   // first run game logic
   double delta = timePassed - lastTime;
   bunnyTimer += delta;

   if (bunnyTimer > 1.0) {
      for (std::vector<Entity *>::iterator ent = entities.begin(); ent != entities.end(); ent++)
         if ((*ent)->isDeleted())
            entities.erase(ent);
      ekdTree = KDNode::kdTree(entities);
      bunnyTimer = 0;
   }

   if (timePassed > MAX_UI_TIME)
      return 1;

   if (lastTime >= MAX_UI_TIME) {
      glm::vec3 metPos = skyMeteor->getPosition();
      metPos.y -= 43 * delta;
      skyMeteor->setPosition(metPos);

      float dist = glm::distance(skyMeteor->getPosition(), player->getPosition());      
      if (dist < 3*player->getEdibleScale().y && 2.0*player->getEdibleScale().y > skyMeteor->getEdibleScale().y) {
         winOrLose = 1;
         engine->stopAllSounds();
      }
   }

   moveCamera(delta);
   
   detectCollisions();

   glm::vec3 shake = glm::vec3(rand() / (float)RAND_MAX,
         rand() / (float)RAND_MAX,
         rand() / (float)RAND_MAX) * camShake;
   camShake -= delta / 50.0;
   if (camShake < 0)
      camShake = 0;

   player->update(delta, player->getPosition());
   skybox->setPosition(player->getPosition());
   for (auto ent : entities) {
      ent->update(delta, player->getPosition());
   }
   UpdateHUD();

   // then start rendering
   glViewport(0, 0, SHADOW_RES, SHADOW_RES);
      
   #define PROJ_ARGS 90.0f, (float)w_width/w_height, VIEW_NEAR, VIEW_DIST
   #define VIEW_ARGS camPos, camPos + camDir + shake, glm::vec3(0.0f, 1.0f, 0.0f)

   /* Set projection and view matrices */
   glm::mat4 projection = glm::perspective(PROJ_ARGS);
   glm::mat4 view = glm::lookAt(VIEW_ARGS);

   // HEY-LISTEN: copy all the args from projection and view above to the below fn. call
   frust.resetCam(PROJ_ARGS, VIEW_ARGS);

   // send all the global shader handles that don't depend on a particular entity
   sHandler->sendGenerals(projection, view, camPos, sunColor, sunDir);
   
   // SHADOW MAP RENDER PASS

   Shader *depthShader = sHandler->getShader("depth");
   glBindFramebuffer(GL_FRAMEBUFFER, shadowFb);
   /* glBindFramebuffer(GL_FRAMEBUFFER, 0); */
   glBindTexture(GL_TEXTURE_2D, 0);

   glClearDepth(1.0f);
   glDepthFunc(GL_LEQUAL);
   glEnable(GL_DEPTH_TEST);
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // this block of code makes the t-rex's shadow 4x as big as it is
   glm::vec3 pscale = player->getScale();
   glm::vec3 ppos = player->getPosition();
   glm::vec3 posAdj = ppos;
   player->setScale(pscale * 4.0f);
   posAdj.y = player->getEdibleScale().x / 2.0f;
   player->setPosition(posAdj);
   player->draw(delta, depthShader);
   player->setScale(pscale);
   player->setPosition(ppos);

   for (auto ent : entities) {
      if (ent->castsShadow() && frust.checkEnt(ent->getPosition(), ent->getEdibleScale())) {
         ent->draw(delta, depthShader);
      }
   }

   // NORMAL DRAW PASS
   glBindFramebuffer(GL_FRAMEBUFFER, fxaaFb);
   glViewport(0, 0, w_width, w_height);
   glBindTexture(GL_TEXTURE_2D, 0);

   glClearDepth(1.0f);
   glDepthFunc(GL_LEQUAL);
   glEnable(GL_DEPTH_TEST);
   glClearColor(0.4,0.2,0.2,1.0);
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   player->draw(delta);

   for (auto ent : nonUpNonEds) {
      if (ent->visible() && frust.checkEnt(ent->getPosition(), ent->getEdibleScale()))
         ent->draw(delta);
   }

   for (auto ent : entities) {
      if (ent->visible() && frust.checkEnt(ent->getPosition(), ent->getEdibleScale()))
         ent->draw(delta);
   }
   
   //particles
   //float fmm = meteorP->position.y + 0.1;
   //fountain->updateParticles(1);
	//fountain->drawParticles(sHandler->getShader("particles"));
	//fountain->setPosition(glm::vec3(0.0, fmm, 0.0));
	
	
	//float hmm = meteorP->position.y ;//+ 0.01;	
	meteorP->updateParticles(1);
	meteorP->drawParticles(sHandler->getShader("particles"));
	meteorP->setPosition(skyMeteor->getPosition());

   // draw the skybox in this pass
   skybox->draw(delta);
   
   // FXAA PASS
   glEnable(GL_TEXTURE_2D);
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // fxaaScreen->setTexture(texHelper.getTexture("shadowmap")); 
   fxaaScreen->draw(delta);

   DrawHUD();

   //If time is greater than 125, lose screen displays
   if (timePassed > 125.0) {
      DrawEndScreen(0);
      engine->stopAllSounds();
      engine->play2D("./audio/dinoroar.wav", true);
   }
   if (winOrLose == 1) {
      DrawEndScreen(winOrLose);
      engine->play2D("./audio/sunleth.wav", true);

   }

   lastTime = timePassed;
   return 0;
}

void Game::InitPlayer() {
   texHelper.createTexture("rex", "textures/rextex.bmp"); 
   texHelper.createTexture("normalRex", "resources/bumpmaps/trexNormals.bmp", 0, 12);
   float scale = 1.0f;
   player = new Player (
      trexModel, // model
      sHandler->getShader("texphong"), // shader
      glm::vec3(0.0f, scale / 2.0f, 0.0f), // position
      glm::vec3(scale), // scale
      glm::vec3(0.f, 0.f, -1.f), // direction
      66.4f, // speed
      /*NULL*/new Material(0.2, 0.0, 0.2, 0.6, 0.9, 0.6, 0.8, 0.1, 0.9, 100.f) // mat
   );
   player->setTexture(texHelper.getTexture("rex"));
   player->setDirection(glm::normalize(glm::vec3(0.5, -0.3, 0.5)));
   player->setNormalMap(texHelper.getTexture("matteNormals")); 
}

Texture Game::getShadowMap() {
   return texHelper.getTexture("shadowmap");
}

void Game::InitSkybox() {
   texHelper.createTexturePNG("skybox", "textures/sky.png");
   skybox = new Entity (
      skyBox,
      sHandler->getShader("texonly"),
      player->getPosition(),
      glm::vec3(0.8 * VIEW_DIST),
      glm::vec3(0.f, 0.f, -1.f), // direction
      0.0f,
      new Material(0.2, 0.0, 0.2, 0.5, 0.1, 0.6, 0.8, 0.1, 0.9, 200.f) // mat
   );

   skybox->setTexture(texHelper.getTexture("skybox"));
}

void Game::InitEndScreen() {
   endScreen = new Entity (
      plane,
      sHandler->getShader("hud"),
      glm::vec3(0, 0, 0),
      glm::vec3(1.1f, 1.0, 1.0),
      glm::vec3(0.0f, 0.f, 1.0f), // direction
      0.0f,
      NULL // mat
   );
   endScreen->setTexture(texHelper.getTexture("grass"));
}

void Game::DrawEndScreen(int winOrLose) {
   Shader *shader = sHandler->getShader("EndScreen");

   glm::mat4 Transl = glm::translate(MAT4_ID, endScreen->getPosition());
   glm::mat4 Scale = glm::scale(MAT4_ID, endScreen->getScale());
   glm::mat4 ModelM = Transl * Scale;
   Mesh * mesh = plane->base;
   if (winOrLose == 0)
      shader->render(ModelM, NULL, mesh, NULL, texHelper.getTexture("loseScreen"), true);
   else 
      shader->render(ModelM, NULL, mesh, NULL, texHelper.getTexture("winScreen"), true);
}

void Game::InitHUD() {
   UpdateHUD();

   hudLeft = new Entity (
      plane,
      sHandler->getShader("hud"),
      glm::vec3(-0.89, 0, 0),
      glm::vec3(0.12, 1, 1),
      glm::vec3(0.f, 0.f, 1.0f), // direction
      0.0f,
      NULL // mat
   );
   hudLeft->setTexture(texHelper.getTexture("lhud"));

   hudRight = new Entity (
      plane,
      sHandler->getShader("hud"),
      glm::vec3(0.88, -0.15, 0),
      glm::vec3(0.12, 1, 1),
      glm::vec3(0.f, 0.f, 1.0f), // direction
      0.0f,
      NULL // mat
   );
   hudRight->setTexture(texHelper.getTexture("rhud"));
}

void Game::DrawHUD() {
   Shader *shader = sHandler->getShader("hud");

   glm::mat4 Transl = glm::translate(MAT4_ID, hudLeft->getPosition());
   glm::mat4 Scale = glm::scale(MAT4_ID, hudLeft->getScale());
   glm::mat4 ModelM = Transl * Scale;
   Mesh * mesh = plane->base;
   shader->render(ModelM, NULL, mesh, NULL, texHelper.getTexture("lhud"), true);
   
   Transl = glm::translate(MAT4_ID, hudRight->getPosition());
   Scale = glm::scale(MAT4_ID, hudRight->getScale());
   ModelM = Transl * Scale;
   mesh = plane->base;
   shader->render(ModelM, NULL, mesh, NULL, texHelper.getTexture("rhud"), true);
}

#define UI_PSCALE 5.5

void Game::UpdateHUD() {
   static int l_growth = -1;
   static int l_meteor = -1;

   char filename[50];

   int growth = 0;
   int meteor = 5;

   if (player->getScale().x >= UI_PSCALE)
      growth = 5;
   else if (player->getScale().x >= UI_PSCALE - 1.0)
      growth = 4;
   else if (player->getScale().x >= UI_PSCALE - 2.0)
      growth = 3;
   else if (player->getScale().x >= UI_PSCALE - 3.0)
      growth = 2;
   else if (player->getScale().x >= UI_PSCALE - 4.0)
      growth = 1;

   glm::vec3 mScale;

   if (lastTime >= MAX_UI_TIME) {
      meteor = 0;
      mScale = glm::vec3(180.f, 180.f, 180.f);
   }
   else if (lastTime >= MAX_UI_TIME * 4/5) {
      meteor = 1;
      mScale = glm::vec3(140.f, 140.f, 140.f);
   }
   else if (lastTime >= MAX_UI_TIME * 3/5) {
      meteor = 2;
      mScale = glm::vec3(120.f, 120.f, 120.f);
   }
   else if (lastTime >= MAX_UI_TIME * 2/5) {
      meteor = 3;
      mScale = glm::vec3(100.f, 100.f, 100.f);
   }
   else if (lastTime >= MAX_UI_TIME / 5) {
      meteor = 4;
      mScale = glm::vec3(80.f, 80.f, 80.f);
   }

   if (skyMeteor)
      skyMeteor->setScale(mScale);

   if (growth != l_growth && growth != -1) {
      sprintf(filename, "resources/gauges_bmp/growth%d.bmp", growth*20);
      texHelper.createTexture("lhud", filename);
      l_growth = growth;
   }
   if (meteor != l_meteor && meteor != -1) {
      sprintf(filename, "resources/gauges_bmp/meteor%d.bmp", meteor*20);
                                                // i screwed up the rgb ordering for these files. gonna fix this later.
      texHelper.createTexture("rhud", filename, meteor != 0 && meteor != 1 && meteor != 5, 2);
      l_meteor = meteor;
   }
}

//meteor appears in the sky and gradually grows larger
void Game::InitMeteor() {   
   meteorP = new ParticleSpawner();
   meteorP->startParticles(glm::vec3(350.0f, 1200.0f, 0.0f), 5000, 4.5, 
	           new Material( 0.2, 0.2, 0.8, 0.2, 0.2, 0.5, 0.2, 0.2, 0.5, 50.f), 
	           glm::vec3(0.f, 1.f, 0.f), sHandler->getShader("particles"));
   float scaleM = 1.0f;
   texHelper.createTexture("meteor", "textures/rock.bmp");
   skyMeteor = new Entity (
      meteorSky, // model
      sHandler->getShader("texphong"), // shader
      glm::vec3(350.0f, 1200.0f, 0.0f), // position
      glm::vec3(50.0), 
      glm::vec3(0.f, 0.f, -1.f), // direction
      0.0, // speed
      NULL // mat
   );
   //skyMeteor->setDirection(glm::normalize(glm::vec3(0.5, -0.3, 0.5)));
   skyMeteor->setTexture(texHelper.getTexture("meteor"));
   nonUpNonEds.push_back(skyMeteor);
}

/*
void Game::InitFountain() {
   fountain = new ParticleSpawner();

	fountain->startParticles(glm::vec3(10.0, 10.0, 15.0), 500, 3.5, 
	           new Material( 0.2, 0.2, 0.8, 0.2, 0.2, 0.5, 0.2, 0.2, 0.5, 50.f), 
	           glm::vec3(0.f, 1.f, 0.f), sHandler->getShader("particles"));
	printf("starting the draw particles\n");
}
*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

void Game::InitEnvironment() {
   srand(time(NULL));
   float scale;
   Model * model;
   
   for (int i = 0; i < 20; i++) {
      switch (randRange(0, 5)) {
         case 0:
            model = benchModel;
            scale = .20;
            break;
         case 1:
            model = lampModel;
            scale = .20;
            break;
         case 2:
            model = hydrantModel;
            scale = 1.20;
            break;
         default:
            model = guyModel;
            scale = .20;
            break;
      }

      Entity *e = new Entity(
         model, // model
         sHandler->getShader("texphong"), // shader
         glm::vec3(frandRange(-200, 200), -1, frandRange(-200, 200)), // position
         glm::vec3(10*scale), // scale
         glm::normalize(glm::vec3(frandRange(-10, 10)/10.f, 0.f, frandRange(-10, 10)/10.f)), // direction
         0.5f, // speed
         new Material(0.2, 0.2, 0.0, 0.7, 0.2, 0.1, 0.8, 0.9, 0.1, 200.f) // mat
      );
      entities.push_back(e);
   }
}


/************** EVERYTHING PAST THIS POINT IS CREATING WORLD: IGNORE UNLESS NEEDED *************************/

static glm::vec3 build_scaler;

Model* findBModel(std::string code) {
   Model* ret; 
   worldYScale = 0.0f;
   if (code == "sBuild") {
      ret = sBuildModel;
      build_scaler = glm::vec3(TILE_SCALE * 0.7f, TILE_SCALE, TILE_SCALE * 0.7f);
      ret->edibleScale = 2.0f;
   }
   else if (code == "mBuild") {
      ret = mBuildModel;
      build_scaler = glm::vec3(TILE_SCALE * 0.7f, TILE_SCALE, TILE_SCALE * 0.7f);
      ret->edibleScale = 2.0f;
   }
   else if (code == "sBuildModel_TWO_ONE") {
      ret = sBuildModel_TWO_ONE;
      build_scaler = glm::vec3(TILE_SCALE * 0.85f, TILE_SCALE, TILE_SCALE * 0.7f);
   }
   else if (code == "mBuildModel_TWO_ONE") {
      ret = mBuildModel_TWO_ONE;
      build_scaler = glm::vec3(TILE_SCALE * 0.85f, TILE_SCALE, TILE_SCALE * 0.7f);
   }
   else if (code == "sBuildModel_ONE_TWO") {
      ret = sBuildModel_ONE_TWO;
      build_scaler = glm::vec3(TILE_SCALE * 0.7f, TILE_SCALE, TILE_SCALE * 0.85f);
      ret->edibleScale = 1.75f;
   }
   else if (code == "mBuildModel_ONE_TWO") {
      ret = mBuildModel_ONE_TWO;
      build_scaler = glm::vec3(TILE_SCALE * 0.7f, TILE_SCALE, TILE_SCALE * 0.85f);
      ret->edibleScale = 1.75f;
   }
   else if (code == "mBuildModel_TWO_TWO") {
      ret = mBuildModel_TWO_TWO;
      build_scaler = glm::vec3(TILE_SCALE * 0.85f, TILE_SCALE, TILE_SCALE * 0.85f);
      ret->edibleScale = 2.29f;
   }
   else if (code == "lBuildModel_TWO_TWO") {
      ret = lBuildModel_TWO_TWO;
      build_scaler = glm::vec3(TILE_SCALE * 0.925f, TILE_SCALE, TILE_SCALE * 0.925f);
      ret->edibleScale = 2.29f;
   }
   else if (code == "xlBuildModel_TWO_TWO") {
      ret = xlBuildModel_TWO_TWO;
      build_scaler = glm::vec3(TILE_SCALE * 0.9625f, TILE_SCALE, TILE_SCALE * 0.9625f);
   }
   else if (code == "bird") {
      ret = birdModel;
      build_scaler = glm::vec3(TILE_SCALE * 3.0f, TILE_SCALE * 3.0f, TILE_SCALE * 3.0f);
      worldYScale = 10.0f * TILE_SCALE;
   }
   else if (code == "people") {
      ret = guyModel;
      build_scaler = glm::vec3(TILE_SCALE, TILE_SCALE, TILE_SCALE);
   }
   else if (code == "bench") {
      ret = benchModel;
      build_scaler = glm::vec3(0.8 * TILE_SCALE, 0.8 * TILE_SCALE, 0.8 * TILE_SCALE);
      worldYScale = 5.0f * TILE_SCALE;
      ret->edibleScale = 0.8f;
   }
   else if (code == "lamp") {
      ret = lampModel;
      build_scaler = glm::vec3(TILE_SCALE, TILE_SCALE, TILE_SCALE);
      worldYScale = 15.0f * TILE_SCALE;
      lampModel->edibleScale = 8.0f;
   }
   else if (code == "hydrant") {
      ret = hydrantModel;
      build_scaler = glm::vec3(1.5 * TILE_SCALE, 1.5 * TILE_SCALE, 1.5 * TILE_SCALE);
      worldYScale = 5.0f * TILE_SCALE;
   }
   else if (code == "trash") {
      ret = trashModel;
      build_scaler = glm::vec3(TILE_SCALE * 5.0f, TILE_SCALE * 5.0f, TILE_SCALE * 5.0f);
      worldYScale = TILE_SCALE * 5.0f;
   }
   else if (code == "fountain") {
      ret = fountainModel;
      build_scaler = glm::vec3(TILE_SCALE * 5.0f, TILE_SCALE, TILE_SCALE * 5.0f);
      worldYScale = TILE_SCALE * 25.0f;
   }
   else if (code == "bike") {
      ret = bikeModel;
      build_scaler = glm::vec3(TILE_SCALE, TILE_SCALE, TILE_SCALE);
      //worldYScale = TILE_SCALE * 25.0f;
   }
   else {
      ret = sBuildModel;
      build_scaler = glm::vec3(TILE_SCALE * 0.7f, TILE_SCALE, TILE_SCALE * 0.7f);
   }

   return ret;
}

void Game::drawGrid(int x, int z, int tileNum, glm::vec2 offset, World gameWorld) {
   float scale = TILE_SCALE;
   //std::cout << "araw " << offset.x << " " << offset.y << std::endl;
   Entity *e = new Entity(
      gridModel, // model
      sHandler->getShader("texphong"), // shader
      glm::vec3((gameWorld.pTile.parkMap.find(tileNum)->second.coordArr[x][z].x * TILE_SIZE + offset.x) * scale, TILE_Y, (gameWorld.pTile.parkMap.find(tileNum)->second.coordArr[x][z].y * TILE_SIZE + offset.y) * scale), // position
      glm::vec3(scale, 0.000000001f, scale), // scale
      glm::normalize(glm::vec3(0.0f, 0.0f, 0.1f)), // direction
      0.0f, // speed
      NULL // mat
   );
   e->setEdible(false);
   e->castsShadow(true);
   e->wonkyShadows(false);
   e->setTexture(texHelper.getTexture(gameWorld.pTile.parkMap.find(tileNum)->second.text[x][z])); //attempt
   nonUpNonEds.push_back(e);
    e->setNormalMap(texHelper.getTexture("normalGrass")); 
}

void Game::drawGridC(int x, int z, int tileNum, glm::vec2 offset, World gameWorld) {
   float scale = TILE_SCALE;
   //std::cout << "araw " << offset.x << " " << offset.y << std::endl;
   Entity *e = new Entity(
      gridModel, // model
      sHandler->getShader("texphong"), // shader
      glm::vec3((gameWorld.cTile.cityMap.find(tileNum)->second.coordArr[x][z].x * TILE_SIZE + offset.x) * scale, TILE_Y, (gameWorld.cTile.cityMap.find(tileNum)->second.coordArr[x][z].y * TILE_SIZE + offset.y) * scale), // position
      glm::vec3(scale, 0.000000001f, scale), // scale
      glm::normalize(glm::vec3(0.0f, 0.0f, 0.1f)), // direction
      0.0f, // speed
      NULL// mat
   );
   e->setEdible(false);
   e->castsShadow(true);
   e->wonkyShadows(false);
   e->setTexture(texHelper.getTexture(gameWorld.cTile.cityMap.find(tileNum)->second.text[x][z])); //attempt
   nonUpNonEds.push_back(e);
    e->setNormalMap(texHelper.getTexture("normalConcrete")); 
}

void Game::drawBuilding(int x, int z, int buildNum, glm::vec2 offset, World gameWorld) {
   float scale = TILE_SCALE;
   std::string interpret = gameWorld.bTile.buildingMap.find(buildNum)->second.mesh[x][z];
   if (interpret != "nan") {
      Entity *e = new Entity(
         findBModel(interpret), // model
         sHandler->getShader("texphong"), // shader
         glm::vec3((gameWorld.bTile.buildingMap.find(buildNum)->second.coordArr[x][z].x * TILE_SIZE + offset.x) * scale, TILE_Y + worldYScale, (gameWorld.bTile.buildingMap.find(buildNum)->second.coordArr[x][z].y * TILE_SIZE + offset.y) * scale), // position
         build_scaler, // scale
         gameWorld.bTile.buildingMap.find(buildNum)->second.direction[x][z], // direction
         0.0f, // speed
         NULL // mat
      );
      //interpret = "mBuildModel_ONE_TWO"; 
      e->setTexture(texHelper.getTexture(interpret)); //attempt
      e->setNormalMap(texHelper.getTexture("normalBrick"));
      e->shadowable(false);
      entities.push_back(e);
   }
}

//&gameWorld.bTile.buildingMap.find(buildNum)->second.mat[x][z]
void Game::drawObjects(int x, int z, int buildNum, glm::vec2 offset, World gameWorld) {
   float scale = TILE_SCALE;
   std::string interpret = gameWorld.bTile.buildingMap.find(buildNum)->second.mesh[x][z];
   Entity *e = NULL;

   #define DO_E_ARGS             \
         findBModel(interpret),        \
         sHandler->getShader("texphong"), \
         glm::vec3((gameWorld.bTile.buildingMap.find(buildNum)->second.coordArr[x][z].x * TILE_SIZE + offset.x) * scale, TILE_Y + worldYScale, (gameWorld.bTile.buildingMap.find(buildNum)->second.coordArr[x][z].y * TILE_SIZE + offset.y) * scale), \
         build_scaler,                 \
         gameWorld.bTile.buildingMap.find(buildNum)->second.direction[x][z], \
         0.0f,                         \
         NULL

   if (interpret == "people" || interpret == "bird" || interpret == "bike") {
      e = new Human(DO_E_ARGS);
      e->setTexture(texHelper.getTexture(interpret));
   }
   else if (interpret != "nan") {
      e = new Entity(DO_E_ARGS);
      e->setTexture(texHelper.getTexture(interpret));
   }

   if (e)
      entities.push_back(e);
}

void Game::InitPark(float xO, float zO, int num) {
   int size = gameWorld.pTile.size;
   int rNum;
   if (num != 0) {
      rNum = num;
   }
   else {
      rNum = (rand() % gameWorld.pTile.tileNums) + 1;
   }
   for (int x = 0; x < size; x++) {
      for (int z = 0; z < size; z++) {
         drawGrid(x, z, rNum, glm::vec2(xO, zO), gameWorld);
            drawObjects(x, z, 34, glm::vec2(xO, zO), gameWorld);
      }
   }
   size = gameWorld.bTile.size;
   for (int x = 0; x < size; x++) {
      for (int z = 0; z < size; z++) {
         drawObjects(x, z, 34, glm::vec2(xO, zO), gameWorld);
      }
   }
}

void Game::InitCity(float xO, float zO, int num) {
   // creating the tiles
   int size = gameWorld.cTile.size;
   int rNum;
   if (num != 0) {
      rNum = num;
   }
   else {
      rNum = (rand() % gameWorld.cTile.tileNums) + 1;
   }
   for (int x = 0; x < size; x++) {
      for (int z = 0; z < size; z++) {
         drawGridC(x, z, rNum, glm::vec2(xO, zO), gameWorld);
      }
   }
   
   if (BUILDINGS) {
      // creating the buildings
      size = gameWorld.bTile.size;
      //rNum = 17;
      if (num == 1) 
         rNum = randRange(1, 22);
      if (num == 2 || num == 3 || num == 4 || num == 5)
         rNum = 20 + num;
      if (num == 10 || num == 11 || num == 12 || num == 13)
         rNum = 16 + num;
         //rNum = (rand() % gameWorld.bTile.tileNums) + 1;
      for (int x = 0; x < size; x++) {
         for (int z = 0; z < size; z++) {
            drawBuilding(x, z, rNum, glm::vec2(xO, zO), gameWorld);
               drawObjects(x, z, randRange(30, 34), glm::vec2(xO, zO), gameWorld);
         }
      }
   }
}

/* hard coded level - will be random generated next release */
void Game::InitWorld() {
   texHelper.createTexture("grass", "textures/GrassC.bmp"); 
   texHelper.createTexture("cross", "textures/roadCross.bmp");
   texHelper.createTexture("conc", "textures/Concrete.bmp"); 
   texHelper.createTexture("sBuild", "textures/1x1sbuild.bmp"); 
   texHelper.createTexture("mBuild", "textures/medBuild.bmp"); 
   texHelper.createTexture("mBuildModel_TWO_TWO", "textures/2x2build.bmp"); 
   texHelper.createTexture("lBuildModel_TWO_TWO", "textures/2x2Lbuild.bmp"); 
   texHelper.createTexture("xlBuildModel_TWO_TWO", "textures/2x2xLbuild.bmp"); 
   texHelper.createTexture("mBuildModel_TWO_ONE", "textures/1x2buildMedium.bmp");
   texHelper.createTexture("sBuildModel_TWO_ONE", "textures/1x2buildSmall.bmp");  
   texHelper.createTexture("sBuildModel_ONE_TWO", "textures/2x1buildSmall.bmp"); 
   texHelper.createTexture("normalBrick", "resources/bumpmaps/brickNormals.bmp", 0, 7);  
   texHelper.createTexture("normalConcrete", "resources/bumpmaps/HDbrickNormals.bmp", 0, 8);
   texHelper.createTexture("normalGrass", "resources/bumpmaps/matteNormals.bmp", 0, 9);
   texHelper.createTexture("winScreen", "textures/winEnd.bmp");
   texHelper.createTexture("loseScreen", "textures/loseEnd.bmp");
   texHelper.createTexture("mBuildModel_ONE_TWO", "textures/2x1buildMedium.bmp");   
   texHelper.createTexture("hydrant", "textures/fire_hydrant_tex.bmp");   
   texHelper.createTexture("lamp", "textures/lamp_tex.bmp");   
   texHelper.createTexture("people", "textures/guy_tex.bmp");   
   texHelper.createTexture("bench", "textures/bench_tex.bmp");  
   texHelper.createTexture("bird", "textures/bird_tex.bmp");  
   texHelper.createTexture("fountain", "textures/marble.bmp");      
   texHelper.createTexture("mBuildModel_ONE_TWO", "textures/2x1buildMedium.bmp");

   texHelper.createTexture("bike", "textures/bicycle_tex.bmp");     

   float scaler = 4.0 * 2 * TILE_SIZE;
   //InitCity(0, 0, 1);
   InitPark(0, 0, 1); //park
   InitCity(scaler, scaler, 10); // will be park outer area
   InitCity(scaler, -scaler, 11);
   InitCity(-scaler, scaler, 12);
   InitCity(-scaler, -scaler, 13);
   InitCity(scaler, 0, 2);
   InitCity(0, -scaler, 4);
   InitCity(0, scaler, 5);
   InitCity(-scaler, 0, 3);

   scaler *= 2;
   InitCity(scaler, scaler, 1); // city
   InitCity(scaler, -scaler, 1);
   InitCity(-scaler, scaler, 1);
   InitCity(-scaler, -scaler, 1);
   InitCity(scaler, 0, 1);
   InitCity(0, -scaler, 1);
   InitCity(0, scaler, 1);
   InitCity(-scaler, 0, 1);

   InitCity(scaler, scaler/2, 1);
   InitCity(scaler, -scaler/2, 1);
   InitCity(scaler/2, -scaler, 1);
   InitCity(-scaler/2, -scaler, 1);
   InitCity(scaler/2, scaler, 1);
   InitCity(-scaler/2, scaler, 1);
   InitCity(-scaler, scaler/2, 1);
   InitCity(-scaler, -scaler/2, 1);
}
