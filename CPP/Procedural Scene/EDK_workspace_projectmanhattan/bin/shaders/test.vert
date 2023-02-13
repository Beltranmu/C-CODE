#version 330

uniform mat4 u_m_matrix;
uniform mat4 u_vp_matrix;

layout (location=0) in vec3 a_position;
layout (location=1) in vec2 a_uv;
layout (location=2) in vec3 a_normal;

out vec3 normal_r;
out vec3 normal;
out vec3 FragPos;
out vec2 uv;



void main()
{
  //float a = step(0,a_position.y);
  //normal_r = u_m_matrix * vec4(a_normal, 1);
  //normal = vec4(a_normal, 1);
  
 
  gl_Position = (u_vp_matrix*u_m_matrix ) * vec4(a_position, 1) ;
  uv = a_uv;
  FragPos = vec3(u_m_matrix * vec4(a_position, 1.0));
  normal = a_normal;
}