#version 120

varying vec4 vPos;

void main(void) {
   gl_FragDepth = vPos.z;
   /* gl_FragDepth = 1.0; */
   gl_FragColor.xyz = vec3(vPos.z);
   gl_FragColor.w = 1.0;
}
