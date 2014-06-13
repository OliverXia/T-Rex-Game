#version 120

uniform sampler2D uTexUnit;

varying vec2 vTexCoord;

void main(void) {
   vec4 tex = texture2D(uTexUnit, vTexCoord);

   gl_FragColor = vec4(tex.y, tex.x, tex.z, 1.0);
}

