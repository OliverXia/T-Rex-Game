#ifndef __TEXTUREHANDLER_H
#define __TEXTUREHANDLER_H

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
#include <map>
#include <string>
#include "Utils.h"
#include "Material.h"	
#include <iostream>
#include <list>

#define IMG_TEXTURE_UNIT 2
#define FRAME_TEXTURE_UNIT 3
#define DEPTH_TEXTURE_UNIT 4
#define BUMP_TEXTURE_UNIT 7

typedef struct Texture_t {
   int activeTex;
   GLuint texId;
} Texture;

typedef std::map<std::string, Texture> TextureMap;

class TextureHelper {
public:
   void createTexture(std::string tag, std::string path, int flip_colors, int slot);
   void createTexture(std::string tag, std::string path);
   void createTexturePNG(std::string tag, std::string path);
   void createTexture(std::string tag, int width, int height);
   void createDepthTexture(std::string tag, int size);
   Texture getTexture(std::string tag);

private:
   TextureMap texTable;
   int texPool = 1;
};

#endif
