#include "Utils.h"

#include "GLSL_helper.h"
#include "Defines.h"


int randRange(int low, int high) {
   return rand() % (high - low) + low;
}

float frandRange(float low, float high) {
   return (high - low) * static_cast<float>(rand()) / (static_cast<float>(RAND_MAX)) + low;
}

float randDegAngle() {
   return rand() / static_cast<float>(RAND_MAX/(360.0f));
}

float randRadAngle() {
   return rand() / static_cast<float>(RAND_MAX/(2.0f * M_PI));
}

void DebugBufferData(GLenum target, GLuint buffer, int elementSize, int bufferSize)
{
   printf("###### BUFFER DATA from %d ######\n", buffer);
   float *data;

   bufferSize -= bufferSize % elementSize;
   int numElements = bufferSize / elementSize;

   glBindBuffer(target, buffer);
   data = (float *)glMapBuffer(target, GL_READ_ONLY);

   for (int i = 0; i < numElements; ++i) {
      printf("<");
      for (int j = 0; j < elementSize; ++j) {
         printf(" %f ", data[i * elementSize + j]);
      }
      printf(">\n");
   }

   glUnmapBuffer(GL_ARRAY_BUFFER);
}

void printMatrix(glm::mat4 mat) {
   printf("%.2f %.2f %.2f %.2f\n"
          "%.2f %.2f %.2f %.2f\n"
          "%.2f %.2f %.2f %.2f\n"
          "%.2f %.2f %.2f %.2f\n",
    mat[0][0], mat[0][1], mat[0][2], mat[0][3],
    mat[1][0], mat[1][1], mat[1][2], mat[1][3],
    mat[2][0], mat[2][1], mat[2][2], mat[2][3],
    mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
}

glm::mat4 aiToGlmMat4(aiMatrix4x4 mat) {
   return glm::mat4(
      mat.a1, mat.b1, mat.c1, mat.d1,
      mat.a2, mat.b2, mat.c2, mat.d2,
      mat.a3, mat.b3, mat.c3, mat.d3,  
      mat.a4, mat.b4, mat.c4, mat.d4 );
}

glm::mat3 aiToGlmMat3(aiMatrix3x3 mat) {
   return glm::mat3(
      mat.a1, mat.b1, mat.c1,
      mat.a2, mat.b2, mat.c2,
      mat.a3, mat.b3, mat.c3 );
}

glm::quat aiToGlmQuat(aiQuaternion quat) {
   return glm::quat(quat.w, quat.x, quat.y, quat.z);
}

glm::vec3 aiToGlmVec3(aiVector3D vec) {
   return glm::vec3(vec.x, vec.y, vec.z);
}
