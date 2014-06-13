#version 120

struct Material {
   vec3 aColor;
   vec3 dColor;
   vec3 sColor;
   float shine;
};

uniform Material uMat;

attribute vec3 aPosition;

uniform mat4 uProjM;
uniform mat4 uViewM;

varying vec4 particlecolor;

void main() {
   
   gl_Position = uProjM * uViewM * vec4(aPosition, 1.0);
   
   particlecolor = vec4(uMat.aColor, 1.0);
}
