/* Multi-pass Shader Class */

#ifndef __SHADER_H
#define __SHADER_H

#define GL_GLEXT_PROTOTYPES
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#ifdef __APPLE__
#include <OPENGL/gl.h>
#include <ApplicationServices/ApplicationServices.h>
#endif

class ParticleSpawner;

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "GLSL_helper.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include <map>
#include <string>
#include "Utils.h"
#include "Game.h"
#include "Material.h"	
#include <iostream>
#include <list>
#include "MeshLoader.h"
#include "TextureHelper.h"
#include "ParticleSpawner.h"

typedef struct {
   GLuint num;
   int cullType;
} ShadeNum;

class Shader {
public:
   std::list<ShadeNum> shadeNums; 
   
   GLuint h_uModelM, h_uViewM, h_uProjM;
   GLuint h_aPosition, h_aNormal, h_aTexCoord;
   GLuint h_aTangent, h_aBitangent;
   GLuint h_uTexUnit, h_uPixelSize, h_uUseTex;
   GLuint h_uDepthPV, h_uShadowMap, h_uShadowMapSize;
   GLuint h_uMatAmb, h_uMatDif, h_uMatSpec, h_uMatShine;
   GLuint h_uLightPos, h_uLightColor, h_uSunDir, h_uCamPos;
   GLuint h_uBoneM, h_aBoneIndex, h_aBoneWeight;
   GLuint h_uColorMod;
   GLuint h_uUseNormalMap, h_uNormalMap;
   GLuint h_uOutlineColor;
   GLuint h_uHasAnims;
   GLuint h_uWonkyShadows, h_uUseShadows;
  
   void render(glm::mat4 ModelM, Material * material, Mesh * mesh) {
      render(ModelM, material, mesh, NULL, {0, 0}, false, {0,0}, false);
   }
   void render(glm::mat4 ModelM,
      Material * material,
      Mesh * mesh,
      glm::mat4 * boneTransforms,
      Texture texture,
      bool useTexture
   ) {
      //if nothing is being passed to the boolean for useNormalMap, then just make it false and {0,0}
      render(ModelM, material, mesh, boneTransforms, texture, useTexture, {0,0}, false);
   }
   void render(
      glm::mat4 ModelM,
      Material * material,
      Mesh * mesh,
      glm::mat4 * boneTransforms,
      Texture texture,
      bool useTexture,
      Texture normalMap,
      bool useNormalMap
   ) {
      render(ModelM, material, mesh, boneTransforms, texture, useTexture, normalMap, useNormalMap, true, glm::vec3(0.0f), false);
      /* render(ModelM, material, mesh, texture, useTexture, true, glm::vec3(0.0f)); */
   }

   void render(
      glm::mat4 ModelM,
      Material * material,
      Mesh * mesh,
      glm::mat4 * boneTransforms,
      Texture texture,
      bool useTexture,
      Texture normalMap,
      bool useNormalMap,
      bool useShadowMap,
      glm::vec3 outlineColor,
      bool wonkyShadows
   );
   
   void render_particles(
      Material * material,
      GLuint posbuffer,
      //GLuint movbuffer,
      int NumPoints
   );

   void install(std::string vert, std::string frag, int cullType);
};

typedef std::map<std::string,Shader> ShaderMap;

class ShaderHandler {
public:
   ShaderHandler() {};
   void installShader(std::string tag, std::string vert, std::string frag, int cullType);
   Shader * getShader(std::string tag);
   void sendGenerals(glm::mat4 proj, glm::mat4 view, glm::vec3 camPos, glm::vec3 sunColor, glm::vec3 sunDir);
   
private:
   ShaderMap shaderTable;
};

#endif
