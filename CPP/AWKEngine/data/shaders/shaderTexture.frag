#version 460 core

in vec4 _color;

layout (location = 0) out vec4 out_FragColor;

void main(){
  out_FragColor = vec4(_color);
}