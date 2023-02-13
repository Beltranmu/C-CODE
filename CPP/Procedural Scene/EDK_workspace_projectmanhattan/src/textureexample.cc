#include <ESAT/window.h>
#include <ESAT/input.h>
#include <ESAT/draw.h>
#include <ESAT/time.h>
#include <ESAT_extra/imgui.h>

#include <EDK3/geometry.h>
#include <EDK3/camera.h>
#include <EDK3/drawable.h>
#include <EDK3/texture.h>
#include <EDK3/matdiffusetexture.h>
#include <Material/buildingmaterial.h>
#include <EDK3/dev/gpumanager.h>
#include <light/directionallight.h>
#include <light/pointligh.h>

#include <Mollete/gpumanager.h>
#include <Mollete/camera.h>
#include <Mollete/texture.h>

#include <Framework/world.h>
#include <Geometry/point.h>
#include <Material/rainmaterial.h>
#include <Material/transparentM.h>


#include <Material/skybox.h>
#include <Material/material_custom_cubemap.h>



using namespace ESAT;
using namespace EDK3;
using namespace EDK3::dev;

double previus_time;
//Build Material

ref_ptr<DirectionalLight> dirlights[1];
ref_ptr<PointLight> polights[1];
World city;

// openGL names 
GLuint points_vertices_ID;
GLuint points_indices_ID;
GLuint vertex_shader_ID;
GLuint fragment_shader_ID;
GLuint geometry_shader_ID;
GLuint program_ID;

float verticesArray[100000];
float speed[50000];
float direction[3] = { 0.005f, -0.012f, 0.f };

const char* vertexShader = {
  "#version 330\n"
  "uniform vec3 hola;\n"
  "layout(location = 0) in vec2 aPos;\n"
  "//out vec4 test;\n"
  "void main(){\n"
  "gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);\n"
  "//test = vec4(0.0f, -0.05f, 0.0f, 0.0f) ;\n"
  "}"
};

const char* fragmentShader = {
  "#version 330\n"
  "out vec4 fragColor;\n"
  "void main(){\n"
  "fragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
  "}"
};

const char* geometryShader = {
  "#version 330\n"
  "layout(points) in;\n"
  "layout(line_strip, max_vertices = 2) out;\n"
  "uniform vec3 direction;\n"
  "void main(){\n"
  "gl_Position = gl_in[0].gl_Position + vec4(-direction*100, 0.0f);\n"
  "EmitVertex();\n"
  "gl_Position = gl_in[0].gl_Position + vec4(direction*100, 0.0f);\n"
  "EmitVertex();\n"
  "EndPrimitive();\n"
  "}"
};

struct GameState {
	ref_ptr<Mollete::Camera> camera;
	ref_ptr<Node> root;
  bool wirframe = false;
  int fog_activate = 0;
  float fog_density = 2.0f;
};

void prepare(GameState *state) {

	GPUManager::CheckGLError("Prepare Start");
	
	// Nodes
	Node* root = state->root.alloc();	
  //Skybox
  EDK3::ref_ptr<EDK3::Drawable> cubeMap;
  cubeMap.alloc();

  EDK3::ref_ptr<EDK3::Geometry> cube;
  EDK3::CreateCube(&cube, 1.0f, true, false);

  EDK3::ref_ptr<Skybox::SettingsSkyboxMat> cubeMatSettings;
  cubeMatSettings.alloc();

  EDK3::ref_ptr<Skybox> cubeMat;
  cubeMat.alloc();
  cubeMat->loadCubeMap(&cubeMatSettings->texture_);
  cubeMatSettings->set_texture(cubeMatSettings->texture_.get());
  cubeMat->load_shaders_from_filepaths("shaders/skybox.vert", "shaders/skybox.frag");

  EDK3::ref_ptr<Material_custom_cubemap> matCustom;
  EDK3::ref_ptr<Material_custom_cubemap::Settings> matCustomSettings;

  matCustom.alloc();
  matCustomSettings.alloc();
  matCustomSettings->set_texture(cubeMatSettings->texture_.get());
  matCustom->load_shaders_source();

  cubeMap->set_geometry(cube.get());
  cubeMap->set_material(matCustom.get());
  cubeMap->set_material_settings(matCustomSettings.get());

  root->addChild(cubeMap.get());


  







	/*ref_ptr<Drawable> drawable;
	drawable.alloc();

  ref_ptr<Point> rain;
  rain.alloc();
  rain->intensity = 50;
  rain->range = 2;
  rain->minRainForce = 20;
  rain->maxRainForce = 100;
  rain->Generate();

  ref_ptr<Drawable> rainDrawable;
  rainDrawable.alloc();
  rainDrawable->set_geometry(rain.get());

  ref_ptr<RainMaterial> rainMaterial;
  rainMaterial.alloc();
  rainMaterial->load_shaders_from_filepaths("shaders/rain.vert", "shaders/rain.frag");
  rainDrawable->set_material(rainMaterial.get());

  ref_ptr<RainMaterial::SettingsBuildingMat> rainMatSettings;
  rainMatSettings.alloc();
  rainDrawable->set_material_settings(rainMatSettings.get());
  rainDrawable->set_position(0.f, 0.f, 0.f);
  rainDrawable->set_scale(100.f, 100.f, 100.f);
  root->addChild(rainDrawable.get());*/
  
  // Create a Camera
  state->camera.alloc();
  float pos[] = { 0.0f, 1000.0f, 2200.0f };
  float view[] = { 0.0f, 0.0f, -1.0f };
  state->camera->set_position(pos);
  state->camera->set_view_direction(view);
  //state->camera->setupPerspective(90, 1280.0f / 720.0f, 0.01f, 16000.0f);
  state->camera->setPerspective(90, 1280.0f / 720.0f, 0.01f, 16000.0f);
  //state->camera->set_clear_color(0.5f, 0.5f, 0.5f, 1.0f);
  state->camera->setClearColor(0.5f, 0.5f, 0.5f, 1.0f);


  // Test World.h  
  city.initGeometries(20);
  city.loadAllGeometries();

  // Lights
  city.initLights();
  city.initDirectionalLight(city.getDirectionalLight(), 10.f, 0.f, 0.f);
		
  // City Material
  city.initAllMaterials(4);
  city.initMaterial("shaders/test.vert", "shaders/test.frag");  

  //Textures
  city.loadTexture("../assets/textures/T_building_01_Dif_b.png", &city.worldTextures[0]);
  city.loadTexture("../assets/textures/T_building_01_Dif.png", &city.worldTextures[1]);
  city.loadTexture("../assets/textures/T_Street_Diffuse.png", &city.worldTextures[2]);
  city.loadTexture("../assets/textures/T_LightsSigns_Dif.png", &city.worldTextures[3]);
  
  // City Material Settings
  city.initMatSettings(city.getMatSettings(0), 1.f, 1.f, 1.f, 1.f,
                       city.worldTextures[0], 2, 3,
                       &state->camera->position_c.x, 20, &state->fog_activate,
    &state->fog_density);

  city.initMatSettings(city.getMatSettings(1), 1.f, 1.f, 1.f, 1.f,
                       city.worldTextures[1], 2, 3,
                       &state->camera->position_c.x, 20, &state->fog_activate, 
    &state->fog_density);
  
  city.initMatSettings(city.getMatSettings(2), 1.f, 1.f, 1.f, 1.f,
                       city.worldTextures[2], 2, 3,
                       &state->camera->position_c.x, 20, &state->fog_activate, 
    &state->fog_density);

  city.initMatSettings(city.getMatSettings(3), 1.f, 1.f, 1.f, 1.f,
    city.worldTextures[3], 2, 1,
    &state->camera->position_c.x, 20, &state->fog_activate, &state->fog_density);


  //Cube 
  EDK3::ref_ptr<Drawable> cube_t;
  cube_t.alloc();
  EDK3::ref_ptr<transparentM> cube_t_m;
  EDK3::ref_ptr<transparentM::SettingstransparentM> cube_t_ms;

  cube_t_m.alloc();
  cube_t_ms.alloc();
  cube_t->set_geometry(cube.get());
  cube_t->set_position(-100.0f, 5000.0f, -100.00f);
  cube_t->set_scale(1000.f, 1000.0f, 1000.f);
  cube_t->set_material(cube_t_m.get());
  cube_t->set_material_settings(cube_t_ms.get());
  cube_t_m->load_shaders_from_filepaths("shaders/transparent_b.vert", "shaders/transparent_b.frag");

  float color_cube[] = {1.0f, 0.0f, 0.0f, 0.5f};
  cube_t_ms->set_color(color_cube);
    
  root->addChild(cube_t.get());
  city.generateCity(state->root, 10, 10);







	GPUManager::CheckGLError("Prepare END");
}

void SystemImgui(GameState *state , float fps) {

   
  ImGui::Begin("System Panel", NULL, ImGuiWindowFlags_AlwaysAutoResize);
  ImGui::Text("Wireframe: ");
  if(ImGui::Button("Activate")){
    state->wirframe = true;
  }
  ImGui::SameLine();
  if (ImGui::Button("Deactivate")) {
    state->wirframe = false;
  }
  ImGui::Text("N DrawCalls: %d ", state->camera->ndrawcalls);
  ImGui::Text("FPS: %02.2f ", fps);

  ImGui::Text("Fog: ");
  if (ImGui::Button("Activate1")) {
    state->fog_activate = 1;
  }
  ImGui::SameLine();
  if (ImGui::Button("Deactivate1")) {
    state->fog_activate = 0;
  }
 
  ImGui::Separator(); //Linea separadora

  ImGui::End();


  ImGui::Begin("Lights", NULL, ImGuiWindowFlags_AlwaysAutoResize);
  ImGui::Text("Direction:");  
  ImGui::SliderFloat("X", &city.getDirectionalLight()->direction[0], -1.0f, 1.0f);
  ImGui::SliderFloat("Y", &city.getDirectionalLight()->direction[1], -1.0f, 1.0f);
  ImGui::SliderFloat("Z", &city.getDirectionalLight()->direction[2], -1.0f, 1.0f);

  ImGui::Text("Ambient:");
  ImGui::SliderFloat("Ambient X", &city.getDirectionalLight()->ambient[0], 0.0f, 1.0f);
  ImGui::SliderFloat("Ambient Y", &city.getDirectionalLight()->ambient[1], 0.0f, 1.0f);
  ImGui::SliderFloat("Ambient Z", &city.getDirectionalLight()->ambient[2], 0.0f, 1.0f);
                                                       
  ImGui::Text("Diffuse:");                             
  ImGui::SliderFloat(" Diffuse X", &city.getDirectionalLight()->diffuse[0], 0.0f, 1.0f);
  ImGui::SliderFloat(" Diffuse Y", &city.getDirectionalLight()->diffuse[1], 0.0f, 1.0f);
  ImGui::SliderFloat(" Diffuse Z", &city.getDirectionalLight()->diffuse[2], 0.0f, 1.0f);
                                                       
  ImGui::Text("Specular:");                            
  ImGui::SliderFloat("Specular X", &city.getDirectionalLight()->specular[0], 0.0f, 1.0f);
  ImGui::SliderFloat("Specular Y", &city.getDirectionalLight()->specular[1], 0.0f, 1.0f);
  ImGui::SliderFloat("Specular Z", &city.getDirectionalLight()->specular[2], 0.0f, 1.0f);
     
  ImGui::Text("Position:");
  ImGui::SliderFloat("Pos X", &city.worldPointLights[0].position[0], -10000.0f, 10000.0f);
  ImGui::SliderFloat("Pos Y", &city.worldPointLights[0].position[1], -10000.0f, 10000.0f);
  ImGui::SliderFloat("Pos Z", &city.worldPointLights[0].position[2], -10000.0f, 10000.0f);

  ImGui::Text("Ambient:");
  ImGui::SliderFloat("P Ambient X", &city.worldPointLights[0].ambient[0], 0.0f, 1.0f);
  ImGui::SliderFloat("P Ambient Y", &city.worldPointLights[0].ambient[1], 0.0f, 1.0f);
  ImGui::SliderFloat("P Ambient Z", &city.worldPointLights[0].ambient[2], 0.0f, 1.0f);

  ImGui::Text("Diffuse:");
  ImGui::SliderFloat("P Diffuse X", &city.worldPointLights[0].diffuse[0], 0.0f, 1.0f);
  ImGui::SliderFloat("P Diffuse Y", &city.worldPointLights[0].diffuse[1], 0.0f, 1.0f);
  ImGui::SliderFloat("P Diffuse Z", &city.worldPointLights[0].diffuse[2], 0.0f, 1.0f);

  ImGui::Text("Specular:");
  ImGui::SliderFloat("P Specular X", &city.worldPointLights[0].specular[0], 0.0f, 1.0f);
  ImGui::SliderFloat("P Specular Y", &city.worldPointLights[0].specular[1], 0.0f, 1.0f);
  ImGui::SliderFloat("P Specular Z", &city.worldPointLights[0].specular[2], 0.0f, 1.0f);

  ImGui::SliderFloat("Constant",  &city.worldPointLights[0].constant, 0.0f, 1.0f);
  ImGui::SliderFloat("linear",    &city.worldPointLights[0].linear, 0.0f, 1.0f);
  ImGui::SliderFloat("quadratic", &city.worldPointLights[0].quadratic, 0.0f, 1.0f);
  
  ImGui::Text("Position:");
  ImGui::SliderFloat("Pos2 X", &city.worldPointLights[1].position[0], -10000.0f, 10000.0f);
  ImGui::SliderFloat("Pos2 Y", &city.worldPointLights[1].position[1], -10000.0f, 10000.0f);
  ImGui::SliderFloat("Pos2 Z", &city.worldPointLights[1].position[2], -10000.0f, 10000.0f);

  ImGui::Text("Ambient:");
  ImGui::SliderFloat("P2 Ambient X", &city.worldPointLights[1].ambient[0], 0.0f, 1.0f);
  ImGui::SliderFloat("P2 Ambient Y", &city.worldPointLights[1].ambient[1], 0.0f, 1.0f);
  ImGui::SliderFloat("P2 Ambient Z", &city.worldPointLights[1].ambient[2], 0.0f, 1.0f);

  ImGui::Text("Diffuse:");
  ImGui::SliderFloat("P2 Diffuse X", &city.worldPointLights[1].diffuse[0], 0.0f, 1.0f);
  ImGui::SliderFloat("P2 Diffuse Y", &city.worldPointLights[1].diffuse[1], 0.0f, 1.0f);
  ImGui::SliderFloat("P2 Diffuse Z", &city.worldPointLights[1].diffuse[2], 0.0f, 1.0f);

  ImGui::Text("Specular:");
  ImGui::SliderFloat("P2 Specular X", &city.worldPointLights[1].specular[0], 0.0f, 1.0f);
  ImGui::SliderFloat("P2 Specular Y", &city.worldPointLights[1].specular[1], 0.0f, 1.0f);
  ImGui::SliderFloat("P2 Specular Z", &city.worldPointLights[1].specular[2], 0.0f, 1.0f);

  ImGui::SliderFloat("Constant2", &city.worldPointLights[1].constant, 0.0f, 1.0f);
  ImGui::SliderFloat("linear2", &city.worldPointLights[1].linear, 0.0f, 1.0f);
  ImGui::SliderFloat("quadratic2", &city.worldPointLights[1].quadratic, 0.0f, 1.0f);



  ImGui::Text("Ambient:");
  ImGui::SliderFloat("S Ambient X", &city.worldSpotLights[0].ambient[0], 0.0f, 1.0f);
  ImGui::SliderFloat("S Ambient Y", &city.worldSpotLights[0].ambient[1], 0.0f, 1.0f);
  ImGui::SliderFloat("S Ambient Z", &city.worldSpotLights[0].ambient[2], 0.0f, 1.0f);

  ImGui::Text("Diffuse:");
  ImGui::SliderFloat("S Diffuse X", &city.worldSpotLights[0].diffuse[0], 0.0f, 1.0f);
  ImGui::SliderFloat("S Diffuse Y", &city.worldSpotLights[0].diffuse[1], 0.0f, 1.0f);
  ImGui::SliderFloat("S Diffuse Z", &city.worldSpotLights[0].diffuse[2], 0.0f, 1.0f);

  ImGui::Text("Specular:");
  ImGui::SliderFloat("S Specular X", &city.worldSpotLights[0].specular[0], 0.0f, 1.0f);
  ImGui::SliderFloat("S Specular Y", &city.worldSpotLights[0].specular[1], 0.0f, 1.0f);
  ImGui::SliderFloat("S Specular Z", &city.worldSpotLights[0].specular[2], 0.0f, 1.0f);

  ImGui::SliderFloat("S Constant", &city.worldSpotLights[0].constant, 0.0f, 1.0f);
  ImGui::SliderFloat("S linear", &city.worldSpotLights[0].linear, 0.0f, 1.0f);
  ImGui::SliderFloat("S quadratic", &city.worldSpotLights[0].quadratic, 0.0f, 1.0f);
  ImGui::SliderFloat("CutOFF", &city.worldSpotLights[0].cutOff, 0.0f, 1.57f);
  ImGui::SliderFloat("Cutofff el otro", &city.worldSpotLights[0].outerCutOff, 0.0f, 1.57f);



  ImGui::Text("Direction:");
  ImGui::SliderFloat("S1 dX", &city.worldSpotLights[1].direction[0], -1.0f, 1.0f);
  ImGui::SliderFloat("S1 dY", &city.worldSpotLights[1].direction[1], -1.0f, 1.0f);
  ImGui::SliderFloat("S1 dZ", &city.worldSpotLights[1].direction[2], -1.0f, 1.0f);

  ImGui::Text("Position:");
  ImGui::SliderFloat("S1 pX", &city.worldSpotLights[1].position[0], -10000.0f, 10000.0f);
  ImGui::SliderFloat("S1 pY", &city.worldSpotLights[1].position[1], -10000.0f, 10000.0f);
  ImGui::SliderFloat("S1 pZ", &city.worldSpotLights[1].position[2], -10000.0f, 10000.0f);


  ImGui::Text("Ambient:");
  ImGui::SliderFloat("S1 Ambient X", &city.worldSpotLights[1].ambient[0], 0.0f, 1.0f);
  ImGui::SliderFloat("S1 Ambient Y", &city.worldSpotLights[1].ambient[1], 0.0f, 1.0f);
  ImGui::SliderFloat("S1 Ambient Z", &city.worldSpotLights[1].ambient[2], 0.0f, 1.0f);

  ImGui::Text("Diffuse:");
  ImGui::SliderFloat("S1 Diffuse X", &city.worldSpotLights[1].diffuse[0], 0.0f, 1.0f);
  ImGui::SliderFloat("S1 Diffuse Y", &city.worldSpotLights[1].diffuse[1], 0.0f, 1.0f);
  ImGui::SliderFloat("S1 Diffuse Z", &city.worldSpotLights[1].diffuse[2], 0.0f, 1.0f);

  ImGui::Text("Specular:");
  ImGui::SliderFloat("S1 Specular X", &city.worldSpotLights[1].specular[0], 0.0f, 1.0f);
  ImGui::SliderFloat("S1 Specular Y", &city.worldSpotLights[1].specular[1], 0.0f, 1.0f);
  ImGui::SliderFloat("S1 Specular Z", &city.worldSpotLights[1].specular[2], 0.0f, 1.0f);

  ImGui::SliderFloat("S1 Constant", &city.worldSpotLights[1].constant, 0.0f, 1.0f);
  ImGui::SliderFloat("S1 linear", &city.worldSpotLights[1].linear, 0.0f, 1.0f);
  ImGui::SliderFloat("S1 quadratic", &city.worldSpotLights[1].quadratic, 0.0f, 1.0f);
  ImGui::SliderFloat("S1 CutOFF", &city.worldSpotLights[1].cutOff, 0.0f, 1.57f);
  ImGui::SliderFloat("S1 Cutofff el otro", &city.worldSpotLights[1].outerCutOff, 0.0f, 1.57f);




  ImGui::Text("Direction:");
  ImGui::SliderFloat("S2 dX", &city.worldSpotLights[2].direction[0], -1.0f, 1.0f);
  ImGui::SliderFloat("S2 dY", &city.worldSpotLights[2].direction[1], -1.0f, 1.0f);
  ImGui::SliderFloat("S2 dZ", &city.worldSpotLights[2].direction[2], -1.0f, 1.0f);

  ImGui::Text("Position:");
  ImGui::SliderFloat("S2 pX", &city.worldSpotLights[2].position[0], -10000.0f, 10000.0f);
  ImGui::SliderFloat("S2 pY", &city.worldSpotLights[2].position[1], -10000.0f, 10000.0f);
  ImGui::SliderFloat("S2 pZ", &city.worldSpotLights[2].position[2], -10000.0f, 10000.0f);


  ImGui::Text("Ambient:");
  ImGui::SliderFloat("S2 Ambient X", &city.worldSpotLights[2].ambient[0], 0.0f, 1.0f);
  ImGui::SliderFloat("S2 Ambient Y", &city.worldSpotLights[2].ambient[1], 0.0f, 1.0f);
  ImGui::SliderFloat("S2 Ambient Z", &city.worldSpotLights[2].ambient[2], 0.0f, 1.0f);

  ImGui::Text("Diffuse:");
  ImGui::SliderFloat("S2 Diffuse X", &city.worldSpotLights[2].diffuse[0], 0.0f, 1.0f);
  ImGui::SliderFloat("S2 Diffuse Y", &city.worldSpotLights[2].diffuse[1], 0.0f, 1.0f);
  ImGui::SliderFloat("S2 Diffuse Z", &city.worldSpotLights[2].diffuse[2], 0.0f, 1.0f);

  ImGui::Text("Specular:");
  ImGui::SliderFloat("S2 Specular X", &city.worldSpotLights[2].specular[0], 0.0f, 1.0f);
  ImGui::SliderFloat("S2 Specular Y", &city.worldSpotLights[2].specular[1], 0.0f, 1.0f);
  ImGui::SliderFloat("S Specular Z", &city.worldSpotLights[2].specular[2], 0.0f, 1.0f);

  ImGui::SliderFloat("S2 Constant", &city.worldSpotLights[2].constant, 0.0f, 1.0f);
  ImGui::SliderFloat("S2 linear", &city.worldSpotLights[2].linear, 0.0f, 1.0f);
  ImGui::SliderFloat("S2 quadratic", &city.worldSpotLights[2].quadratic, 0.0f, 1.0f);
  ImGui::SliderFloat("S2 CutOFF", &city.worldSpotLights[2].cutOff, 0.0f, 1.57f);
  ImGui::SliderFloat("S2 Cutofff el otro", &city.worldSpotLights[2].outerCutOff, 0.0f, 1.57f);





  ImGui::Separator(); //Linea separadora

  ImGui::End();

}

void prepare2() {

  // MAKE
  glCreateBuffers(1, &points_vertices_ID);
  glBindBuffer(GL_ARRAY_BUFFER, points_vertices_ID);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticesArray), verticesArray, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(0);

  program_ID = glCreateProgram();

  vertex_shader_ID = glCreateShader(GL_VERTEX_SHADER);
  fragment_shader_ID = glCreateShader(GL_FRAGMENT_SHADER);
  geometry_shader_ID = glCreateShader(GL_GEOMETRY_SHADER);

  glShaderSource(vertex_shader_ID, 1, &vertexShader, NULL);
  glShaderSource(fragment_shader_ID, 1, &fragmentShader, NULL);
  glShaderSource(geometry_shader_ID, 1, &geometryShader, NULL);

  glCompileShader(vertex_shader_ID);
  glCompileShader(fragment_shader_ID);
  glCompileShader(geometry_shader_ID);

  char msg_error[255];


  glGetShaderInfoLog(vertex_shader_ID, 255, 0, msg_error);
  printf("msg_error: %s", msg_error);

  glGetShaderInfoLog(fragment_shader_ID, 255, 0, msg_error);
  printf("msg_error: %s", msg_error);

  glGetShaderInfoLog(geometry_shader_ID, 255, 0, msg_error);
  printf("msg_error: %s", msg_error);

  glAttachShader(program_ID, vertex_shader_ID);
  glAttachShader(program_ID, fragment_shader_ID);
  glAttachShader(program_ID, geometry_shader_ID);

  glLinkProgram(program_ID);
  glUseProgram(program_ID);

}

void render_function2(float time) {

  GLint directionID = glGetUniformLocation(program_ID, "direction");
  glUniform3f(directionID, direction[0] * 2, direction[1] * 2, direction[2] * 2);

  for (int i = 0; i < 50000; ++i) {
    verticesArray[i * 2 + 1] += direction[1] * speed[i];
    if (verticesArray[i * 2 + 1] <= -1.f)
      verticesArray[i * 2 + 1] = 1.f;
  }

  for (int i = 0; i < 50000; ++i) {
    verticesArray[i * 2] += direction[0] * speed[i];
    if (verticesArray[i * 2] >= 1.f)
      verticesArray[i * 2] = -1.f;
  }

  glBindBuffer(GL_ARRAY_BUFFER, points_vertices_ID);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticesArray), verticesArray, GL_DYNAMIC_DRAW);

  // ME
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

  glDrawArrays(GL_POINTS, 0, 50000);

}

void render_function(GameState *state, float fps) {
  if (state->wirframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
  GPUManager::Instance()->enableDepthTest(EDK3::dev::GPUManager::kCompareFunc_LessOrEqual);
	// Update
	

  //Update 1º SpotLight position_c and direction

  city.worldSpotLights[0].position[0] = state->camera->position_c.x;
  city.worldSpotLights[0].position[1] = state->camera->position_c.y;
  city.worldSpotLights[0].position[2] = state->camera->position_c.z;

  city.worldSpotLights[0].direction[0] = state->camera->forward.x;
  city.worldSpotLights[0].direction[1] = state->camera->forward.y;
  city.worldSpotLights[0].direction[2] = state->camera->forward.z;


  state->camera->imguiPanel();
  SystemImgui(state, fps);

	// For Every frame... determine what's visible:
	state->camera->doCull(state->root.get());
  EDK3::dev::GPUManager::Instance()->enableDepthTest(EDK3::dev::GPUManager::kCompareFunc_Less);
  //glEnable(GL_DEPTH);
  float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
  EDK3::dev::GPUManager::Instance()->enableBlend(
    GPUManager::kBlendParam_SourceAlpha,
    GPUManager::kBlendParam_OneMinusSourceAlpha, GPUManager::kBlendOp_Add, color);
	// Render
	GPUManager::CheckGLError("begin Render-->");

  EDK3::dev::GPUManager *gpu = EDK3::dev::GPUManager::Instance();
  gpu->disableCullFaces();
  gpu->enableDepthTest(EDK3::dev::GPUManager::kCompareFunc_LessOrEqual);
  state->camera->doRender();
  gpu->enableDepthTest(EDK3::dev::GPUManager::kCompareFunc_Less);
  gpu->enableCullFaces(EDK3::dev::GPUManager::kFace_Back);


	GPUManager::CheckGLError("end Render-->");
 
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  ImGui::Render();

}

int ESAT::main(int argc, char **argv) {

  //ReplaceGPU Manager
  EDK3::dev::GPUManager *gpu = EDK3::dev::GPUManager::Instance();
  Mollete::GPUManager gpu_manager;
  EDK3::dev::GPUManager::Instance()->ReplaceGPUManagerImplementation(&gpu_manager);

  //glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	// State
	GameState state;
	WindowInit(1280, 720);
	DrawSetTextFont("test.ttf");
	DrawSetTextSize(18);
	DrawSetFillColor(253, 255, 255, 128);
	prepare(&state);

  /*for (int i = 0; i < 50000; ++i) {

    verticesArray[i * 2] = (-100 + rand() % 201) / 100.f;
    verticesArray[i * 2 + 1] = (-100 + rand() % 201) / 100.f;
    speed[i] = (40 + rand() % 100) / 10.f;

  }*/
	
	// Main loop
	while (WindowIsOpened() && !IsSpecialKeyDown(kSpecialKey_Escape)) {
    
		double currentTime = RawTime();
		double delta_time = (currentTime - previus_time) / 1000.0f;
		previus_time = currentTime;
    state.camera->MoveRotateCamera(delta_time);
   
		render_function(&state, 1/(delta_time));
    
		WindowFrame();
	}

	return 0;
}
