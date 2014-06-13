/* World Randomizer Class */

#ifndef __WORLD_H
#define __WORLD_H

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
#include <iostream>
#include <list>
#include "MeshLoader.h"
#include "Material.h"

// file input
#include <iostream>
#include <fstream>
#include <string>

class Build {
public:
   Build();
   Build(int s);
   std::string chooseBuilding(char code);
   glm::vec3 chooseDir(std::ifstream* loadFile, char code);
   Material chooseMaterial(char code);
   int size;
   glm::vec2** coordArr;
   glm::vec3** direction;
   std::string** mesh;
   Material** mat;
};

class Tile {
public:
   Tile();
   Tile(int s);
   std::string chooseColor(char code);
   Material chooseMaterial(char code);
   int size;
   glm::vec2** coordArr;
   std::string** text;
   Material** mat;
};

typedef std::map<int,Build> BuildMap;
typedef std::map<int,Tile> GridMap;

class Park {
public:
   Park();
   GridMap parkMap;
   int size;
   int tileNums;
};

class City {
public:
   City();
   GridMap cityMap;
   int size;
   int tileNums;
};

class Buildings {
public:
   Buildings();
   BuildMap buildingMap;
   int size;
   int tileNums;
};

class World {
public:   
   World();
   Park pTile;
   City cTile;
   Buildings bTile;
};


#endif
