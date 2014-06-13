#version 120

struct Material {
   vec3 aColor;
   vec3 dColor;
   vec3 sColor;
   float shine;
};
uniform Material uMat;
//added this
varying vec4 particlecolor;

void main(void) {
   
   gl_FragColor = particlecolor;
   
   gl_FragColor = vec4(uMat.aColor, 1.0);

}

