/* FILE NAME   : a.frag
 * PURPOSE     : Simple fragment shader.
 * PROGRAMMER  : VG4.
 * LAST UPDATE : 13.06.2014
 */

#version 420

/* Выходное значение цвета рисования */
out vec4 OutColor;

/* Глобальные данные */
uniform float Time;
uniform vec4 LightPosition;
uniform vec4 LightColor;
uniform vec3 ViewDir;

/* Материал */
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Phong;
uniform float Trans;

uniform vec4 UseColor;
uniform mat4 MatrWVP;
uniform mat4 MatrWorldInverseTranspose;
uniform mat4 MatrWorld;

uniform vec3 Eye;
uniform vec3 Lantern;

/* Текстура */
uniform sampler2D DrawTexture;

/* Входные данные */
in vec4 DrawColor;
in vec3 DrawPos;
in vec2 DrawTexCoord;
in vec3 DrawNormal;
in vec4 CameraPos;

in vec4 TstPos;

vec3 Illum( vec3 N )
{
  vec4 texc = texture2D(DrawTexture, DrawTexCoord.xy);

  vec3 color = Ka;
  vec3 Dir = normalize(ViewDir); 

  vec3 l = normalize(Lantern - DrawPos);

  N = faceforward(N, ViewDir, N);
  float nl = dot(N, l);
  if (nl > 0)
    color += (texc.xyz * Kd) * nl;
    //color += vec3(1, 1, 1) * nl;

  vec3 R = reflect(Dir, N);
  R = normalize( Dir - N * (2 * dot(Dir, N)) );
  float rl = dot(R, l);
  if (rl > 0)
    color += Ks * pow(rl, 30);

  return color;
}


/* Main function */
void main( void )
{
  OutColor = vec4(Illum(DrawNormal).xyz, Trans);
} /* End of 'main' function */

/* End of 'a.frag' file */
