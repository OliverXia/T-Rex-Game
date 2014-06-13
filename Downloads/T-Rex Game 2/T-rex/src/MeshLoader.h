#ifndef MESH_LOADER_H_
#define MESH_LOADER_H_

#define GL_GLEXT_PROTOTYPES
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

/*
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "freeglut.lib")*/

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "GLSL_helper.h"
#include "Defines.h"
#include "Utils.h"

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <queue>
#include <map>
#include <fstream>
#include <sstream>
#include <assert.h>

#define NONE   0 // just a place holder (no remaking, reducint, ... etc.)
#define REMAKE 1 // if you want to remake the mod file that's loaded
#define REDUCE 2 // if you want to reduce the mesh into 'meshCount' meshes
#define RESIZE 4 // if you want the model to have a max size of 2
#define CENTER 8 // if you want to center the model

#define MAX_BONES 50
#define MAX_WEIGHTS 2

struct aiScene;

class Model;
class Mesh;
class MrMesh;
class Face;
class Vert;
class Bone;
class Animation;
class KeyFrame;

typedef std::map<uint, Mesh> MeshMap;  // from mesh number to mesh
typedef std::map<std::string, uint> BoneMap;
typedef std::map<std::string, Animation> AnimMap; // from name of anim to animation

class Model {
friend Mesh;

public:
   glm::mat4 rotAdjust;
   Mesh * base; // highest detailed mesh that is loaded in
   uint maxVerts;
   uint numMeshes;
   glm::vec3 center;
   glm::vec3 dimensions;
   float edibleScale = 1.0f;
   uint numAnimations;

   Model(const std::string & path, uint meshCount, uint maxVerts, uint flags);
   ~Model();

   Mesh * getMesh(uint numVerts);
   void loadAnimation(const std::string & path);
   // Gets the animation specified by animNum at the specified point in time (seconds)
   glm::mat4 * getAnimationTransforms(uint animNum, double timePassed, double animTime, glm::mat4 * animMats); 
   Animation * getAnimation(uint animNum);

private:
   MeshMap meshTable;
   BoneMap boneTable; // look up bone index from name of bone
   std::vector<glm::mat4> boneMats; // array of bone offset transforms
   std::vector<Animation> animations;

   Bone * makeBoneTree(Bone * pBone, aiNode * ainode, const aiScene * scene);
   aiNode * findRoot(aiNode * ainode);
   aiNodeAnim * findNodeAnim(aiAnimation * anim, std::string name);

   void computeBoneTransform(Bone * bone, glm::mat4 * allMats, float tickTime,
    glm::mat4 parentTransform);
   glm::mat4 getAnimTransform(Bone * bone, float tickTime);
   glm::quat calcInterpolatedRotation(Bone * bone, float tickTime);
   glm::vec3 calcInterpolatedPosition(Bone * bone, float tickTime);
   glm::vec3 calcInterpolatedScaling(Bone * bone, float tickTime);
   uint findEarlyRotationKeyIndex(Bone * bone, float tickTime);
   uint findEarlyPositionKeyIndex(Bone * bone, float tickTime);
   uint findEarlyScalingKeyIndex(Bone * bone, float tickTime);
};

class Animation {
public:
   uint ticksPerSecond;
   float duration;
   Bone * root;
};

class Bone {
public:
   uint index;
   std::string name;
   glm::mat4 offset;
   std::vector<aiQuatKey> rotationKeys;
   std::vector<aiVectorKey> positionKeys;
   std::vector<aiVectorKey> scaleKeys;
   std::vector<Bone *> children;

   Bone();
};

class Mesh {
friend Model;

public:
   GLuint vbo;  // Vertex Buffer Object
   GLuint nbo;  // Normal Buffer Object
   GLuint uvbo; // TexCoords Buffer Object
   GLuint tabo; // Tangent Buffer Object
   GLuint btbo; // Bitangent Buffer Object
   GLuint ibo;  // Index Buffer Object
   GLuint bmbo; // Gives boneMatrix, indexed by boneId 
   GLuint bibo; // Gives boneId, indexed by vertex id
   GLuint bwbo; // Gives boneWeight, indexed by vertex id
   uint numFaces;
};


// ====================================================== //
// ================= Mesh Reducer Stuff ================= //
// ====================================================== //

class MrModel {
public:
   static void writeModFile(const std::string & path, 
    uint meshCount, uint maxVerts, uint flags);
};

class MrMesh {
public:
   std::vector<Vert *> verts;
   std::vector<Face *> faces;

   Vert * findLowestCostVertex();
   void collapseEdge(Vert * u, Vert * v);
   void connectVerts(Vert * u, Vert * v);
   void removeVertex(Vert * vert);
   void removeFace(Face * face);
   void writeToFile(std::ofstream & file, uint meshNum,
    uint vertCount, uint faceCount);
};

struct BoneIndex {
   uint id; // bone index of 
   float weight;   // weight associated with bone
};

class Vert {
public:
   uint id; // place of vertex in original list 

   glm::vec3 position; // location of this point
   glm::vec3 normal;   // normal for the shader
   glm::vec2 uv;       // texture coordinate
   glm::vec3 tangent;
   glm::vec3 bitangent;
   std::vector<BoneIndex> bIndices; // array of affecting bones and their weights

   std::vector<Vert *> neighbors;   // adjacent vertices
   std::vector<Face *> faces;       // adjacent triangles
   float cost;                      // cached cost of collapsing edge
   Vert * collapse;                 // candidate vertex for collapse

   Vert(uint ndx, glm::vec3 p, glm::vec3 n, glm::vec2 uvCoord, glm::vec3 tang, glm::vec3 bitang);

   ~Vert();

   void computeEdgeCost();
   bool hasNeighbor(Vert * v);   
   void removeIfNonNeighbor(Vert *n);
};

class Face {
public:
   Vert* vertices[3]; // the 3 points that make this triangle
   glm::vec3 normal;  // orthogonal unit vector

   Face(Vert * v0, Vert * v1, Vert * v2);
   ~Face();

   void computeNormal();
   bool hasVertex(Vert * v);
};

#endif // MESH_LOADER_H_

