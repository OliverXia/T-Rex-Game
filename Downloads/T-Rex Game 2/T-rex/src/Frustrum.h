#ifndef FRUSTRUM_H__

#include "glm/glm.hpp"
#include <vector>
#include <array>

class FPlane {
private:
  glm::vec3 normal, point;
  float d;

public:
  FPlane() {} 
  FPlane(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
    glm::vec3 a1 = p1 - p2;
    glm::vec3 a2 = p3 - p2;

    normal = glm::cross(a2, a1);
    if (!normal.x && !normal.y && !normal.z) {
      normal = glm::vec3(1.f, 0.f, 0.f);
      printf("!!!!!!\n%lf %lf %lf\n", a1.x, a1.y, a1.z);
      printf("%lf %lf %lf\n", a2.x, a2.y, a2.z);
    }

    normal = glm::normalize(normal);
    point  = p2;
    d      = -1 * glm::dot(normal, point);
  }

  float distanceToSphere(glm::vec3 pt) {
    return d + glm::dot(normal, pt);
  }
};

class Frustrum {
private: 
  enum { TOP_P, BOT_P, LEFT_P, RIGHT_P, NEAR_P, FAR_P };
  std::array<FPlane, FAR_P+1> planes;

public:
  enum { NOVIS, VISIBLE };

  void resetCam(float angle, float ratio, float nearDist, float farDist, 
    glm::vec3 camPos, glm::vec3 lookAt, glm::vec3 upVec);
  int checkEnt(glm::vec3 center, glm::vec3 scale);
};


#endif