#version 460 core

layout (binding = 0) uniform sampler2D tex0;
layout (location = 0) out vec4 out_FragColor;


in vec2 uv_;

void main(){
  
  out_FragColor =  texture(tex0, uv_);
  
}