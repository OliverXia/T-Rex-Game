#version 120

attribute vec3 aPosition;
attribute vec3 aNormal;

uniform mat4 uProjM;
uniform mat4 uViewM;
uniform mat4 uModelM;

uniform vec3 uCamPos;
uniform vec3 uSunDir;

varying vec3 vColor;
varying vec3 vNormal;

varying vec3 view;
varying vec3 reflection;

void main() {
   vec3 diffuse, specular, ambient, camDiff, pos;
   float camDist;

   vNormal = vec3(uModelM * normalize(vec4(aNormal, 0.0)));
   pos = vec3(uModelM * vec4(aPosition, 1.0));

   view = normalize(uCamPos - pos);
   reflection = uSunDir - 2.0 * dot(uSunDir, vNormal) * vNormal;

   gl_Position = uProjM * uViewM * vec4(pos, 1.0);
}
