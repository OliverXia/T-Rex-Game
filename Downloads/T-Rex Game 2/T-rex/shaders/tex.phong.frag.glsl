#version 120

struct Material {
   vec3 aColor;
   vec3 dColor;
   vec3 sColor;
   float shine;
};
uniform Material uMat;

uniform sampler2D uTexUnit;
uniform int uUseTex;

uniform vec3 uLightColor;
uniform vec3 uSunDir;

varying vec3 vNormal;
varying vec3 view;
varying vec3 reflection;

varying vec2 vTexCoord;

uniform int uUseNormalMap;
uniform sampler2D uNormalMap;
varying vec3 eyeDirTang;
varying vec3 lightDirTang;

uniform sampler2D uShadowMap;
uniform vec2 uShadowMapSize;
varying vec3 shadowCoord;
uniform int uWonkyShadows;
uniform int uUseShadows;

uniform sampler2D uZMap;
uniform vec3 uOutlineColor;

vec2 poissonDisk[4] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 )
);

float getShadow(sampler2D smap, vec3 scoord, float lightAngle) {
   if (texture2D(smap, scoord.xy).x == 1.0) {
      return 1.0;
   }
   float shadelSize = 1.0/uShadowMapSize.x;
   float bias = 0.05;

   float obstruction = 1.0;
   float depth = shadowCoord.z - bias;
   for (int i = 0; i < 4; ++i) {
      float d = texture2D(smap, scoord.xy + poissonDisk[i] * shadelSize).x;

      if (d < depth && (uWonkyShadows == 0 || lightAngle < 0.0)) {
         obstruction -= 0.13;
      }
   }

   return obstruction;
}

void main(void) {
   vec4 tex = uUseTex != 0 ? texture2D(uTexUnit, vTexCoord) : vec4(uMat.dColor, 1.0);
   vec3 surfacetoLight, surfacetoView, refl, tangentNormal;
   vec3 diffuse, specular, rColor, nNormal;
   float specDot;
   int whatToNormalMap = 0;
   nNormal = normalize(vNormal);

   float lightAngle = dot(nNormal, normalize(-uSunDir));

   if (uUseNormalMap == 0) {
      diffuse = lightAngle * tex.xyz;
      specDot = max(dot(view, reflection)/(length(view)*length(reflection)), 0.0);
      /* specular = pow(specDot, 200.0f) * vec3(0.2, 0.2, 0.2); */
      specular = vec3(0.0);
   }
   //if (((tex.x >= 0.80 && tex.x <= 0.81) && (tex.y >= 0.44 && tex.y <= 0.45) && (tex.z >= 0.31 && tex.z <= 0.32))) {
   //   whatToNormalMap = 1;
   //}
   //NormalMapping Craziness
   if (uUseNormalMap > 0) {
      tangentNormal = normalize(texture2D(uNormalMap, vTexCoord).rgb*2.0 - 1.0);
      surfacetoLight = normalize(lightDirTang);
      surfacetoView = normalize(eyeDirTang);
      refl = reflect(surfacetoLight, tangentNormal);
      diffuse = max(0.0, dot(tangentNormal, normalize(-(surfacetoLight)))) * tex.xyz;
      specDot = max(dot(surfacetoView, refl)/(length(surfacetoView)*length(refl)), 0.0);
      /* specular = pow(specDot, 200.0f) * vec3(0.2, 0.2, 0.2); */
      specular = vec3(0.0);
   }
   /* diffuse = mix(diffuse, length(diffuse) * 0.8 * uOutlineColor, 0.08 + max(0.0, 0.2 - length(diffuse))); */

   rColor = uLightColor * (diffuse + specular);
   if (uUseShadows != 0) {
      rColor = rColor * getShadow(uShadowMap, shadowCoord, lightAngle);
   }

   if (rColor.x <= 0.0 && rColor.y <= 0.0  && rColor.z <= 0.0) {
      gl_FragColor = vec4(tex.x*0.1f, tex.y*0.1f, tex.z*0.1f, 1.0f);
   }
   else {
      gl_FragColor = vec4(rColor, 1.0f);
   }

   gl_FragColor = mix(gl_FragColor, vec4(uOutlineColor * length(diffuse), 1.0), 0.1);
   /* gl_FragColor = vec4(vec3(float(uWonkyShadows)), 1.0); */
   /* gl_FragColor = vec4(texture2D(uShadowMap, shadowCoord.xy).xxx, 1.0); */
   /* gl_FragColor = vec4(shadowCoord.zzz, 1.0); */
}

