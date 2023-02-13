#version 330


uniform mat4 u_m_matrix;
uniform mat4 u_vp_matrix;
uniform vec3 hola;

layout(location = 0) in vec3 aPos;

void main(){

  gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);

}