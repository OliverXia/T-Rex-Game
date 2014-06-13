#ifndef PARTICLESPAWNER_H__
#define PARTICLESPAWNER_H__

#define GL_GLEXT_PROTOTYPES
#define GLFW_INCLUDE_GLU


#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "GLSL_helper.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include <string>
#include "Utils.h"
#include <iostream>
#include "TextureHelper.h"
#include "Shader.h"
#include <iterator> 

class ParticleSpawner{

public:
   glm::vec3 position;			      //spawner pos
   std::vector<glm::vec3> points;	//the particles in CPU
   GLuint posBuff;                  //initail spawn position
   GLuint movBuff;
   std::vector<glm::vec3> velocity;
   float pntSize;
   
   ParticleSpawner();

   void startParticles(glm::vec3 pos, int amount, float size, Material* material, glm::vec3 direction, Shader *shader);
	
	void drawParticles(Shader *shaderover);
	
	int checkDistance(glm::vec3 spawn, glm::vec3 curr);
	
	void updateParticles(int num);
	
	void setPosition(glm::vec3 pos);
	
};


#endif
