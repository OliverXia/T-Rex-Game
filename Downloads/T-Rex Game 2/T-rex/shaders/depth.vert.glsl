#version 120

attribute vec3 aPosition;

attribute vec2 aBoneIndices;
attribute vec2 aBoneWeights;
uniform mat4 uBoneM[50];
uniform int uHasAnims;

uniform mat4 uDepthPV;
uniform mat4 uModelM;

varying vec4 vPos;

void main(void) {
   mat4 animBones, animBone1, animBone2, compModel;

   // Won't work, but replace this with aBoneWeights.x...
   animBone1 = 1.0 * uBoneM[int(aBoneIndices.x)];
   animBone2 = 0.0 * uBoneM[int(aBoneIndices.y)];
   if (aBoneWeights.x + aBoneWeights.y == 0.1)
     animBones = mat4(1.0);
   else
   	animBones = animBone1; // add + animBone2
   compModel = uHasAnims != 0 ? uModelM * animBones : uModelM;

   vPos = uDepthPV * compModel * vec4(aPosition, 1);

   gl_Position = vPos;
}
