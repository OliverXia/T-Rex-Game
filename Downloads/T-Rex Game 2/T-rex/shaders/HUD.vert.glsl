#version 120
attribute vec3 aPosition;
attribute vec2 aTexCoord;

varying vec2 vTexCoord;

uniform mat4 uModelM;

void main() {
   vec4 vPosition;

   vPosition = vec4(aPosition, 1.0);
   vPosition = uModelM * vPosition;

   //Busting out the orthographic projection
   gl_Position = vPosition;

   vTexCoord = aTexCoord;
}

