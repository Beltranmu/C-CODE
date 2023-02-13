#version 460 core

#define albedo tex0

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 1) in vec3 UV;

layout (std140, binding = 0) uniform PerFrameData{
    uniform mat4 viewprojectionMatrix;
  };
  
layout (std140, binding = 1) uniform PerModelData{
    uniform mat4 modelmatrix;
    uniform vec4 color;
  };
  
uniform sampler2D tex0;

out vec4 _color;

void main() {
    gl_Position = viewprojectionMatrix * modelmatrix * vec4(Position, 1.f);
    _color = texture(albedo, vec2(UV.x, UV.y));
    //_color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
   
  }