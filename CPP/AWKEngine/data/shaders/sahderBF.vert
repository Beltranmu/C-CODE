#version 460 core

const vec2 position[3] = vec2[3](
                   vec2( -1.0f,  1.0f),
                   vec2( -1.0f, -3.0f),
                   vec2(  1.0f,  3.0f));
                   
 const vec2 uv[3] = vec2[3](
                   vec2( 0.0f, 1.0f),
                   vec2( 0.0f, 0.0f),
                   vec2( 1.0f, 1.0f));                  
                   
out vec2 uv_;
void main() {
    gl_Position =  vec4(position[gl_VertexID], 1.0f, 1.0f);
    uv_ = uv[gl_VertexID];
}