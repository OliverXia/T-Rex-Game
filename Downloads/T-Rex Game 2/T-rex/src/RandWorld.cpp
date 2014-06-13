#include "RandWorld.h"

Build::Build(int s) {
   size = s;
   coordArr = new glm::vec2*[size];
   direction = new glm::vec3*[size];
   mesh = new std::string*[size];
   mat = new Material*[size];
   for (int i = 0; i < size; i++) {
      coordArr[i] = new glm::vec2[size];
      direction[i] = new glm::vec3[size];
      mesh[i] = new std::string[size];
      mat[i] = new Material[size];
   }
}

glm::vec3 direct(char code) { 
   glm::vec3 ret = glm::vec3(0.0, 0.0, 0.1);
   
   switch (code) {
      case 'r':
         ret = glm::vec3(frandRange(-10, 10)/10.f, 0.f, frandRange(-10, 10)/10.f);      
         break;
      case '<':
         ret = glm::vec3(0.0f, 0.0f, -1.0f);
         break;
      case '>':
         ret = glm::vec3(0.0f, 0.0f, 1.0f);
         break;
      case '^':
         ret = glm::vec3(1.0f, 0.0f, 0.0f);
         break;
      case 'd':
         ret = glm::vec3(-1.0f, 0.0f, 0.0f);
         break;
      default:
         break;
   }
   return ret;
}

glm::vec3 Build::chooseDir(std::ifstream* loadFile, char code) {
   char outputChar;

   glm::vec3 ret = glm::vec3(0.0, 0.0, 0.1);
   switch (code) {
      case 'I':
         *loadFile >> outputChar;
         ret = direct(outputChar);      
         break; 
      case 'P':
         *loadFile >> outputChar;
         ret = direct(outputChar);      
         break; 
      case 'E':
         *loadFile >> outputChar;
         ret = direct(outputChar);      
         break;
      case 'L':
         *loadFile >> outputChar;
         ret = direct(outputChar);      
         break;  
      case 'H':
         *loadFile >> outputChar;
         ret = direct(outputChar);      
         break; 
      case 'T':
         *loadFile >> outputChar;
         ret = direct(outputChar);      
         break; 
      case 'F':
         *loadFile >> outputChar;
         ret = direct(outputChar);      
         break; 
      case 'Y':
         *loadFile >> outputChar;
         ret = direct(outputChar); 
         break;
      default :
         break; 
   }
   return glm::normalize(ret);
}

std::string Build::chooseBuilding(char code) { //stand-in when no textures can be used with assimp yet  
   std::string ret; 

   switch(code) {
      case 'b':
         if (randRange(0, 2)) 
            ret = "sBuild";
         else
            ret = "mBuild";
         break;
      case 'h':
         if (randRange(0, 2)) 
            ret = "sBuildModel_TWO_ONE";
         else
            ret = "mBuildModel_TWO_ONE";
         break;
      case 'v':
         if (randRange(0, 2)) 
            ret = "sBuildModel_ONE_TWO";
         else
            ret = "mBuildModel_ONE_TWO";
         break;
      case 'B':
         if (randRange(0, 2)) 
            ret = "mBuildModel_TWO_TWO";
         else //if (randRange(0, 2))
            ret = "lBuildModel_TWO_TWO";
         /*else
            ret = "xlBuildModel_TWO_TWO";*/
         break;
      case 'I':
         ret = "bird";
         break;
      case 'P':
         ret = "people";         
         break;
      case 'E':
         ret = "bench";
         break;
      case 'L':
         ret = "lamp";
         break;
      case 'H':
         ret = "hydrant";
         break;
      case 'T':
         ret = "trash";      
         break;
      case 'F':
         ret = "fountain";            
         break; 
      case 'Y':
         ret = "bike";
         break;
      case '*':
         ret = "nan";
         break; 
   }
   return ret;
}

Material Build::chooseMaterial(char code) { //stand-in when no textures can be used with assimp yet  
   Material ret; 
   switch(code) {
      case 'b':
         ret = Material(0.2, 0.2, 0.2, 0.3, 0.3, 0.3, 0.4, 0.4, 0.4, 200.f);
         break;
      case 'h':
         ret = Material(0.2, 0.2, 0.2, 0.3, 0.3, 0.3, 0.4, 0.4, 0.4, 200.f);
         break;
      case 'v':
         ret = Material(0.2, 0.2, 0.2, 0.3, 0.3, 0.3, 0.4, 0.4, 0.4, 200.f);
         break;
      case 'B':
         ret = Material(0.2, 0.2, 0.2, 0.3, 0.3, 0.3, 0.4, 0.4, 0.4, 200.f);
         break;
      case 'I':
         ret = Material(0.2, 0.0, 0.2, 0.7, 0.2, 0.6, 0.2, 0.2, 0.1, 200.f);    
         break;
      case 'P':
         ret = Material(0.2, 0.0, 0.2, 0.7, 0.2, 0.6, 0.2, 0.2, 0.1, 200.f);         
         break;
      case 'E':
         ret = Material(0.2, 0.0, 0.0, 0.7, 0.2, 0.1, 0.2, 0.2, 0.1, 200.f); 
         break;   
      case 'L':
         ret = Material(0.0, 0.0, 0.4, 0.1, 0.2, 0.8, 0.2, 0.2, 0.1, 200.f); 
         break;  
      case 'H':
         ret = Material(0.3, 0.0, 0.0, 0.9, 0.0, 0.0, 0.2, 0.2, 0.1, 200.f);
         break; 
      case 'T':
         ret = Material(0.3, 0.3, 0.0, 0.9, 0.9, 0.0, 0.2, 0.2, 0.1, 200.f);
         break;
      case 'F':
         ret = Material(0.3, 0.3, 0.0, 0.9, 0.9, 0.0, 0.2, 0.2, 0.1, 200.f);
         break;
      case 'Y':
         ret = Material(0.3, 0.3, 0.0, 0.9, 0.9, 0.0, 0.2, 0.2, 0.1, 200.f);
         break;
      case '*':
         ret = Material(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0f);
         break; 
   }
   return ret;
}

Build makeBuildingArray(std::ifstream* loadFile, int size) {
   Build temp = Build(size);
   char outputChar;

   float xF, zF;
   zF = -size/4.0f + 0.5f;
   xF = size/4.0f - 0.5f;
   
   for (int x = 0; x < size; x++) {
      for (int z = 0; z < size; z++) {
         *loadFile >> outputChar;
         temp.coordArr[x][z] = glm::vec2(xF + frandRange(-.03, .03), zF + frandRange(-.03, .03));
         temp.mesh[x][z] = temp.chooseBuilding(outputChar);
         temp.mat[x][z] = temp.chooseMaterial(outputChar);
         temp.direction[x][z] = temp.chooseDir(loadFile, outputChar);
         zF += 0.5f;
      }
      zF = -size/4.0f + 0.5f;
      xF -= 0.5f;
   }

   return temp;
}

Buildings::Buildings() {
   std::string outputString;
   int wSize, numTiles, mappedNum;

   std::ifstream* loadFile = new std::ifstream();
   loadFile->open("./resources/City_buildings.txt");

   *loadFile >> outputString;
   *loadFile >> outputString;
   *loadFile >> wSize;
   *loadFile >> outputString;
   *loadFile >> numTiles;

   for (int i = 0; i < numTiles; i++) {
      *loadFile >> mappedNum;
      buildingMap.insert(BuildMap::value_type(mappedNum, makeBuildingArray(loadFile, wSize)));
   }
   size = wSize;
   tileNums = numTiles;
}

Tile::Tile(int s) {
   size = s;
   coordArr = new glm::vec2*[size];
   text = new std::string*[size];
   mat = new Material*[size];
   for (int i = 0; i < size; i++) {
      coordArr[i] = new glm::vec2[size];
      text[i] = new std::string[size];
      mat[i] = new Material[size];
   }
}

std::string Tile::chooseColor(char code) { //stand-in when no textures can be used with assimp yet  
   std::string ret; 
   switch(code) {
      case 'G':
         ret = "grass";
         break;
      case '*':
         ret = "conc";
         break; 
      case '+':
         ret = "cross";
         break; 
      case '-':
         ret = "cross";
         break; 
      case '|':
         ret = "cross";
         break; 
   }
   return ret;
}

Material Tile::chooseMaterial(char code) { //stand-in when no textures can be used with assimp yet  
   Material ret; 
   switch(code) {
      case 'G':
         ret = Material(0.1, 0.1, 0.1, 0.1, 0.7, 0.1, 0.8, 0.9, 0.1, 200.f);
         break;
      case '*':
         ret = Material(0.1, 0.1, 0.1, 0.4, 0.4, 0.4, 0.1, 0.1, 0.1, 200.f);
         break; 
      case '+':
         ret = Material(0.1, 0.1, 0.1, 0.02, 0.05, 0.02, 0.1, 0.1, 0.1, 200.f);
         break; 
      case '-':
         ret = Material(0.1, 0.1, 0.1, 0.02, 0.05, 0.02, 0.1, 0.1, 0.1, 200.f);
         break; 
      case '|':
         ret = Material(0.1, 0.1, 0.1, 0.02, 0.05, 0.02, 0.1, 0.1, 0.1, 200.f);
         break; 
   }
   return ret;
}


Tile makeTileArray(std::ifstream* loadFile, int size) {
   Tile temp = Tile(size);
   char outputChar;

   float xF, zF;
   zF = -size/2.0f + 0.5f;
   xF = size/2.0f - 0.5f;
   
   for (int x = 0; x < size; x++) {
      for (int z = 0; z < size; z++) {
         *loadFile >> outputChar;
         temp.coordArr[x][z] = glm::vec2(xF, zF);
         temp.text[x][z] = temp.chooseColor(outputChar);
         temp.mat[x][z] = temp.chooseMaterial(outputChar);
         zF += 1.0f;
      }
      zF = -size/2.0f + 0.5f;
      xF -= 1.0f;
   }

   return temp;
}

Park::Park() {
   std::string outputString;
   int wSize, numTiles, mappedNum;

   std::ifstream* loadFile = new std::ifstream();
   loadFile->open("./resources/Park_tiles.txt");

   *loadFile >> outputString;
   *loadFile >> outputString;
   *loadFile >> wSize;
   *loadFile >> outputString;
   *loadFile >> numTiles;

   for (int i = 0; i < numTiles; i++) {
      *loadFile >> mappedNum;
      parkMap.insert(GridMap::value_type(mappedNum, makeTileArray(loadFile, wSize)));
   }
   size = wSize;
   tileNums = numTiles;
}

City::City() {
   std::string outputString;
   int wSize, numTiles, mappedNum;

   std::ifstream* loadFile = new std::ifstream();
   loadFile->open("./resources/City_tiles.txt");

   *loadFile >> outputString;
   *loadFile >> outputString;
   *loadFile >> wSize;
   *loadFile >> outputString;
   *loadFile >> numTiles;

   for (int i = 0; i < numTiles; i++) {
      *loadFile >> mappedNum;
      cityMap.insert(GridMap::value_type(mappedNum, makeTileArray(loadFile, wSize)));
   }
   size = wSize;
   tileNums = numTiles;
}

World::World() {
   pTile = Park();
   cTile = City();
   bTile = Buildings();
}
