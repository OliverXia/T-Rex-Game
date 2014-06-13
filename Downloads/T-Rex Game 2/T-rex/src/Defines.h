#ifndef __DEFINES_H
#define __DEFINES_H

// useful values
#define VEC3_0 (glm::vec3(0.0f))
#define MAT4_ID (glm::mat4(1.0f))
#define X_AXIS (glm::vec3(1.0f, 0.0f, 0.0f))
#define Y_AXIS (glm::vec3(0.0f, 1.0f, 0.0f))
#define Z_AXIS (glm::vec3(0.0f, 0.0f, 1.0f))

// useful pseudo-functions
#define qmin(x, y) ((x) < (y) ? (x) : (y))
#define qmax(x, y) ((x) > (y) ? (x) : (y))
#define qabs(x)    ((x) > 0 ? (x) : (-1 * (x)))
#define linScale(a0, a1, a, b0, b1) ((float)((a) - (a0)) * (float)((b1) - (b0)) / (float)((a1) - (a0)) + (float)(b0))

#endif
