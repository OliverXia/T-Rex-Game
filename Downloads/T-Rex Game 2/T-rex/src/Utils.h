#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>

#define GL_GLEXT_PROTOTYPES
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#define WIN_HEIGHT 600
#define WIN_WIDTH  1000

int randRange(int low, int high);
float frandRange(float low, float high);
float randDegAngle();
float randRadAngle();
void DebugBufferData(GLenum target, GLuint buffer, int elementSize, int bufferSize);
void printMatrix(glm::mat4 mat);
glm::mat4 aiToGlmMat4(aiMatrix4x4 mat);
glm::mat3 aiToGlmMat3(aiMatrix3x3 mat);
glm::quat aiToGlmQuat(aiQuaternion quat);
glm::vec3 aiToGlmVec3(aiVector3D vec);

#endif
