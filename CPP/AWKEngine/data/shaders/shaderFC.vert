#version 460 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 1) in vec3 UV;

layout (std140, binding = 0) uniform PerFrameData{
    uniform mat4 viewprojectionMatrix;
  };
  
layout (std140, binding = 1) uniform PerModelData{
    uniform mat4 modelmatrix;
    uniform vec4 color1;
    uniform vec4 color2;
    uniform vec4 color3;
    uniform vec4 color4;
  };
  
  
/*layout (std140, binding = 2) uniform MaterialSolidColor{
    uniform vec4 color1;
    uniform vec4 color2;
    uniform vec4 color3;
    uniform vec4 color4;
  };*/


out vec4 _color;

void main() {
    gl_Position = viewprojectionMatrix * modelmatrix * vec4(Position, 1.f);
    _color = mix(color1, color2, (gl_Position.x+1) * 0.5f);
    vec4 color_to_mix =  mix(color3, color4, (gl_Position.y+1) * 0.5f);
    _color = mix (_color, color_to_mix, 0.5f);
   
    
  }