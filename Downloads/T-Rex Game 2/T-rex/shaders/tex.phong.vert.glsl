#version 120

attribute vec3 aPosition;
attribute vec2 aTexCoord;
attribute vec3 aNormal;
attribute vec3 aTangent;
attribute vec3 aBitangent;

attribute vec2 aBoneIndices;
attribute vec2 aBoneWeights;
uniform mat4 uBoneM[50];
uniform int uHasAnims;

uniform mat4 uProjM;
uniform mat4 uViewM;
uniform mat4 uModelM;
uniform mat4 uDepthPV;

uniform vec3 uCamPos;
uniform vec3 uSunDir;

varying vec3 vColor;
varying vec3 vNormal;

varying vec3 view;
varying vec3 reflection;

varying vec2 vTexCoord;
varying vec3 shadowCoord;

varying vec3 lightDirTang;
varying vec3 eyeDirTang;

varying vec3 sunDirCamspace;

void main() {
   mat4 bias = mat4(
      0.5, 0.0, 0.0, 0.0,
      0.0, 0.5, 0.0, 0.0,
      0.0, 0.0, 0.5, 0.0,
      0.5, 0.5, 0.5, 1.0
   );
   mat4 animBones, animBone1, animBone2, compModel;
   vec3 diffuse, specular, ambient, camDiff, pos;
   vec3 Bitang, Norm, Tang;
   float camDist, primWeight;

   if (uHasAnims == 0)
      animBones = mat4(1.0);
   else {
      primWeight = aBoneWeights.x < 1.0 ? aBoneWeights.x : 1.0;
      animBone1 = primWeight * uBoneM[int(aBoneIndices.x)];
      animBone2 = (1.0 - primWeight) * uBoneM[int(aBoneIndices.y)];
      animBones = animBone1 + animBone2;
   }

   compModel = uModelM * animBones;

   vNormal = vec3(compModel * normalize(vec4(aNormal, 0.0)));
   pos = vec3(compModel * vec4(aPosition, 1.0));

   view = normalize(uCamPos - pos);
   reflection = uSunDir - 2.0 * dot(uSunDir, vNormal) * vNormal;

   shadowCoord = (bias * uDepthPV * compModel * vec4(aPosition, 1.0)).xyz;
   gl_Position = uProjM * uViewM * vec4(pos, 1.0);

   mat3 ViewModelMat = mat3(uViewM * compModel);
   Tang = (normalize(aTangent - aNormal * dot(aNormal, aTangent)));

   if (dot(cross(aNormal, Tang), aBitangent) < 0.0f){
      Tang = Tang * -1.0f;
   }

   Norm = ViewModelMat * normalize(aNormal);
   Tang = ViewModelMat * normalize(Tang);
   Bitang = ViewModelMat * normalize(aBitangent);

   mat3 tangMat = transpose(mat3(
      Tang,
      Bitang,
      Norm
   ));

   /*lightDirTang = tangMat * (ViewModelMat * normalize(uSunDir));
   eyeDirTang = tangMat * (ViewModelMat * normalize(view));*/
   lightDirTang =  normalize(uSunDir) * tangMat * ViewModelMat;
   eyeDirTang = normalize(view) * tangMat * ViewModelMat;

   vTexCoord = aTexCoord;
}
