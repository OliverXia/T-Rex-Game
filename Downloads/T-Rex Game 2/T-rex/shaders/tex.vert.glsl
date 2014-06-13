#version 120

attribute vec3 aPosition;
attribute vec2 aTexCoord;

uniform mat4 uProjM;
uniform mat4 uViewM;
uniform mat4 uModelM;

varying vec2 vTexCoord;

void main() {
  vec4 vPosition;

  /* First model transforms */
  vPosition = uViewM * uModelM * vec4(aPosition, 1);
  gl_Position = uProjM * vPosition;

  vTexCoord = aTexCoord;
  /* vTexCoord = vec2(0.5, 0.5); */
}
