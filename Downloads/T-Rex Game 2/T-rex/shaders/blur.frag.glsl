#version 120

uniform sampler2D uTexUnit;
uniform vec2 uPixelSize;

varying vec2 vTexCoord;

void main(void) {
   vec2 ps = uPixelSize;
   /* vec3 samples[3][3]; */
   /* float weights[] = float[]( */
   /*    0.11, 0.11, 0.11, */
   /*    0.11, 0.11, 0.11, */
   /*    0.11, 0.11, 0.11 */
   /* ); */

   /* vec2 offsets[] = vec2[]( */
   /*    vec2( */

   vec4 tmp;
   float sr = 0, sg = 0, sb = 0;
   for (int i = -1; i < 2; ++i) {
      for (int j = -1; j < 2; ++j) {
         tmp = texture2D(
            uTexUnit,
            vec2(
               vTexCoord.x + (ps.x * i),
               vTexCoord.y + (ps.y * j)
            )
         );
         sr += tmp.x;
         sg += tmp.y;
         sb += tmp.z;
      }
   }

   /* vec3 texColor = normalize(uTexUnit, vTexCoord); */
   vec4 texColor = vec4(sr / 9.0, sg / 9.0, sb / 9.0, 1.0);
   if (vTexCoord.x < 0.5)
      texColor = texture2D(uTexUnit, vTexCoord);

   gl_FragColor = texColor;
}


