#ifndef __MATERIAL_H_
#define __MATERIAL_H_

#define GL_GLEXT_PROTOTYPES
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include "GLSL_helper.h"

class Material {
private:
  float _amb[3], _dif[3], _spec[3], _shine;

public:
  Material() {
  }
  Material(float ambR, float ambG, float ambB, 
           float difR, float difG, float difB,
           float specR, float specG, float specB,
           float shiny) {
           
    _amb[0] = ambR;
    _amb[1] = ambG;
    _amb[2] = ambB;

    _dif[0] = difR;
    _dif[1] = difG;
    _dif[2] = difB; 

    _spec[0] = specR;
    _spec[1] = specG;
    _spec[2] = specB;

    _shine = shiny;
  }

  void attach(GLuint & hAmb, GLuint & hDif, GLuint & hSpec, GLuint & hShine) {
    safe_glUniform3f(hAmb, _amb[0], _amb[1], _amb[2]);
    safe_glUniform3f(hDif, _dif[0], _dif[1], _dif[2]);
    safe_glUniform3f(hSpec, _spec[0], _spec[1], _spec[2]);
    safe_glUniform1f(hShine, _shine);
  }
};

#endif
