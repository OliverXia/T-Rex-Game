#version 120

uniform sampler2D uTexUnit;

varying vec2 vTexCoord;

void main(void) {
   vec4 texColor = texture2D(uTexUnit, vTexCoord);

   gl_FragColor = vec4(texColor.x, texColor.y, texColor.z, 1);
}

