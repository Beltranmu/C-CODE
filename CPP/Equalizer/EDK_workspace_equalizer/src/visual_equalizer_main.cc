#include <ESAT/window.h>
#include <ESAT/input.h>
#include <ESAT/draw.h>
#include <ESAT/time.h>

#include <EDK3/geometry.h>
#include <EDK3/camera.h>
#include <EDK3/drawable.h>
#include <EDK3/matdiffuse.h>
#include <EDK3/dev/gpumanager.h>

#include <Mollete/gpumanager.h>
#include <Mollete/camera.h>

#include <ESAT_extra/soloud/soloud.h>
#include <ESAT_extra/soloud/soloud_wav.h>

#include <geometry/sphere.h>
#include <geometry/spherev2.h>
#include <geometry/triangler.h>
#include <geometry/cube.h>

#include <camerafps.h>
#include <matrix_4.h>
#include <material/material2color.h>
#include <material/materialcube.h>
#include <material/materialcube1.h>


double previus_time;
float song_data;
float current_explosion_scale = 0.0f;
Vector3 camera_pos;
Vector3 camera_view;
bool rise;
float camera_distance;
float time_change_distance;
float angle_cam;
float camera_speed = 3.0f;
float camera_p_y = 0.5f; 
float cam_y = 0.5f;
float cube_deformation;

float ang_cubes = 0.0490873852f;
float  cube_rad = 60.0f;

const unsigned int kCubes = 129;

float color_cube = 0.0f;

struct GameState {
  EDK3::ref_ptr<Mollete::Camera> camera;
  //EDK3::ref_ptr<EDK3::Camera> camera;
  EDK3::ref_ptr<EDK3::Node> root;
  EDK3::ref_ptr<Cube> Cube_deformded;
  SoLoud::Soloud soloud;

};

float lerp(float min, float max, float t) {

  if (t < 0) { t = 0; }
  if (t > 1) { t = 1; }

  return min * (1 - t) + max * t;
}

void prepare(GameState *state) {
  EDK3::dev::GPUManager::CheckGLError("Prepare Start");


  // Create cube
  EDK3::ref_ptr<Sphere2> sphere_geomery;
  EDK3::ref_ptr<Cube> Cube;
  Cube.alloc();
  Cube->Generate();
  sphere_geomery.alloc();
  sphere_geomery->Generate();
  EDK3::Node *root = state->root.alloc();

  //Cubes
  EDK3::ref_ptr <EDK3::Node> Squares_father;
  //EDK3::ref_ptr <EDK3::Node> Squares_father_upside;
  Squares_father.alloc();
  Squares_father->set_position(0.0f, 0.0f, 0.0f);
  Squares_father->set_scale(1.0f, 1.0f, 1.0f);
  root->addChild(Squares_father.get());

  /*Squares_father_upside.alloc();
  Squares_father_upside->set_position(0.0f, 0.0f, 0.0f);
  Squares_father_upside->set_scale(1.0f, 1.0f, 1.0f);
  root->addChild(Squares_father_upside.get());*/

  EDK3::ref_ptr<MaterialCube> cubes_material;
  cubes_material.alloc();
  cubes_material->load_shaders_from_filepaths("shaders/cube.vert", "shaders/cube.frag");

  EDK3::ref_ptr<MaterialCube::SettingsMatCube> mat_setting_cubes[kCubes];
  EDK3::ref_ptr<EDK3::Node> pivot[kCubes];
  EDK3::ref_ptr<EDK3::Drawable> Cubes[kCubes];

  float cube_color1[] = { 255 / 255.0f, 255 / 255.0f, 0 / 255.0f, 1.0f };
  float cube_color11[] = { 235 / 255.0f, 46 / 255.0f, 0 / 255.0f, 1.0f };

  float cube_color2[] = { 118 / 255.0f, 255 / 255.0f, 13 / 255.0f, 1.0f };
  float cube_color22[] = { 0 / 255.0f, 195 / 255.0f, 255 / 255.0f, 1.0f };

  for (int i = 0; i < kCubes; ++i) {

    pivot[i].alloc();
    Cubes[i].alloc();
    mat_setting_cubes[i].alloc();

    float t = (i - 1.0f) / 64.0f;
    if ((float)i > kCubes *0.5f) {
      t = (kCubes - i) / 64.0f;
    }
    float color_normal[] = { lerp(cube_color1[0], cube_color11[0], t),
                             lerp(cube_color1[1], cube_color11[1], t),
                             lerp(cube_color1[2], cube_color11[2], t),
                             lerp(cube_color1[3], cube_color11[3], t) };


    float color_input[] = { lerp(cube_color2[0], cube_color22[0], t),
                            lerp(cube_color2[1], cube_color22[1], t),
                            lerp(cube_color2[2], cube_color22[2], t),
                            lerp(cube_color2[3], cube_color22[3], t) };
 


    mat_setting_cubes[i]->set_color(color_normal);
    mat_setting_cubes[i]->set_color2(color_input);
    mat_setting_cubes[i]->set_input_data(&color_cube);
    mat_setting_cubes[i]->set_n_data(0);

    pivot[i]->set_position(cosf(i * ang_cubes)* cube_rad, -4.0f, sinf(i * ang_cubes) * cube_rad);
    pivot[i]->set_rotation_xyz(0.0f, i * ang_cubes, 0.0f);
    Cubes[i]->set_geometry(Cube.get());
    Cubes[i]->set_material(cubes_material.get());
    Cubes[i]->set_material_settings(mat_setting_cubes[i].get());
    Cubes[i]->set_position(0.0f, 1.0f, 0.0f);

    Squares_father->addChild(pivot[i].get());
    pivot[i]->addChild(Cubes[i].get());
  }
  pivot[0]->set_position(-1000.0f, -10000.f, -10000.f);



  //ATOM

  EDK3::ref_ptr<Materia2Color> diffuse_material_atom;

  diffuse_material_atom.alloc();
  diffuse_material_atom->load_shaders_from_filepaths("shaders/atom.vert", "shaders/atom.frag");

  EDK3::ref_ptr<Materia2Color::SettingsMat2Color> diffuse_material_settings_proton;
  EDK3::ref_ptr<Materia2Color::SettingsMat2Color> diffuse_material_settings_neutron;
  EDK3::ref_ptr<Materia2Color::SettingsMat2Color> diffuse_material_settings_electron;

  diffuse_material_settings_proton.alloc();
  diffuse_material_settings_neutron.alloc();
  diffuse_material_settings_electron.alloc();

  const unsigned int kNatoms = 1;
  //Child 1 .... kNatonms
  EDK3::ref_ptr <EDK3::Node> atom[kNatoms];
  for (int i = 0; i < kNatoms; ++i) {
    atom[i].alloc();
    atom[i]->set_position(0.0f, 0.0f, 0.0f);
    atom[i]->set_scale(1.0f, 1.0f, 1.0f);
    root->addChild(atom[i].get());
  }


  const unsigned int kNproton = 3+1;
  const unsigned int kNneutron = 3;
  const unsigned int knElectron = 6;
  const unsigned int kNelectron_centers = 3;
  const unsigned int kNcore = kNproton + kNneutron;

  EDK3::ref_ptr <EDK3::Node> electron_center[kNelectron_centers];
  EDK3::ref_ptr <EDK3::Drawable> electrons[knElectron];
  EDK3::ref_ptr <EDK3::Drawable>atom_core[kNcore];

  //Core
  for (int i = 0; i < kNcore; ++i) {
    atom_core[i].alloc();
    atom_core[i]->set_scale(1.0f, 1.0f, 1.0f);
    atom_core[i]->set_geometry(sphere_geomery.get());
    atom_core[i]->set_material(diffuse_material_atom.get());
    if (i < kNproton) {
      atom_core[i]->set_material_settings(diffuse_material_settings_proton.get());
    }
    else {
      atom_core[i]->set_material_settings(diffuse_material_settings_neutron.get());
    }
    atom[0]->addChild(atom_core[i].get());
  }

  atom_core[0]->set_position(-1000.0f, -10000.f, -10000.f);
  atom_core[1]->set_position(1.0f, 0.0f, -0.0f);
  atom_core[2]->set_position(0.0f, 1.0f, 0.0f);
  atom_core[3]->set_position(0.0f, 0.0f, 1.0f);
  atom_core[4]->set_position(-1.0f, 0.0f, 0.0f);
  atom_core[5]->set_position(0.0f, -1.0f, 0.0f);
  atom_core[6]->set_position(0.0f, 0.0f, -1.0f);

  float proton_color[] = { 255.0f / 255.0f, 0 / 255.0f, 0 / 255.0f, 0.92f };
  diffuse_material_settings_proton->set_color(proton_color);
  float proton_color2[] = { 255.0f / 255.0f, 0 / 255.0f, 255 / 255.0f, 0.60f };
  diffuse_material_settings_proton->set_color2(proton_color2);
  diffuse_material_settings_proton->set_song_data(&song_data);

  float neutron_color[] = { 255 / 255.0f, 255 / 255.0f, 255 / 255.0f, 0.90f };
  diffuse_material_settings_neutron->set_color(neutron_color);
  float neutron_color2[] = { 255 / 255.0f, 255 / 255.0f, 0 / 255.0f, 0.60f };
  diffuse_material_settings_neutron->set_color2(neutron_color2);
  diffuse_material_settings_neutron->set_song_data(&song_data);

  //Electrons
  for (int i = 0; i < kNelectron_centers; ++i) {
    electron_center[i].alloc();
    electron_center[i]->set_position(0.0f, 0.0f, 0.0f);
    electron_center[i]->set_scale(1.0f, 1.0f, 1.0f);
    atom[0]->addChild(electron_center[i].get());
  }

  for (int i = 0; i < knElectron; ++i) {
    electrons[i].alloc();
    electrons[i]->set_scale(0.5f, 0.5f, 0.5f);
    electrons[i]->set_geometry(sphere_geomery.get());
    electrons[i]->set_material(diffuse_material_atom.get());
    electrons[i]->set_material_settings(diffuse_material_settings_electron.get());
    electrons[i]->set_material_settings(diffuse_material_settings_electron.get());
    electron_center[i/2]->addChild(electrons[i].get());
  }

  float electron_dist = 4.0f;
  float pi = 3.1415f;

  electrons[0]->set_position(cosf(0.0f) * electron_dist, 0.0f, sinf(0.0f) * electron_dist);
  electrons[1]->set_position(cosf(0.0f + pi) * electron_dist, 0.0f, sinf(0.0f) * electron_dist);

  electrons[2]->set_position(cosf(1.0472f) * electron_dist, 0.0f, sinf(1.0472f) * electron_dist);
  electrons[3]->set_position(cosf(1.0472f + pi) * electron_dist, 0.0f, sinf(1.0472f + pi) * electron_dist);

  electrons[4]->set_position(cosf(-1.0472f) * electron_dist, 0.0f, sinf(-1.0472f) * electron_dist);
  electrons[5]->set_position(cosf(-1.0472f + pi) * electron_dist, 0.0f, sinf(-1.0472f + pi) * electron_dist);

 
  float electron_color[] = { 255 / 255.0f, 255 / 255.0f, 0 / 255.0f, 0.94f };
  diffuse_material_settings_electron->set_color(electron_color);
  float electron_color2[] = { 66 / 255.0f, 41 / 255.0f, 153 / 255.0f, 0.60f };
  diffuse_material_settings_electron->set_color2(electron_color2);
  diffuse_material_settings_electron->set_song_data(&song_data); 



  // CUBE Deformed

  state->Cube_deformded.alloc();
  state->Cube_deformded->Generate();
  EDK3::ref_ptr<EDK3::Node> cube_def_father;
  cube_def_father.alloc();
  cube_def_father->set_position(0.0f, 0.0f, 0.0f);
  cube_def_father->set_scale(1.0f, 1.0f, 1.0f);
  root->addChild(cube_def_father.get());

  const int n_d_c = 6 + 1;
  EDK3::ref_ptr<EDK3::Drawable>c_deformed[n_d_c];
  EDK3::ref_ptr<MaterialCube::SettingsMatCube> c_deformed_ms[n_d_c];
 
  for (int i = 0; i < n_d_c; ++i) {
    c_deformed[i].alloc();
    c_deformed[i]->set_geometry(state->Cube_deformded.get());
    c_deformed[i]->set_position(cosf(1.0472f * (i-1))*10.0f, 0.0f, sinf(1.0472f * (i - 1))*10.0f);
    c_deformed[i]->set_scale(1.0f, 1.0f, 1.0f);
    c_deformed[i]->set_material(cubes_material.get());
    c_deformed_ms[i].alloc();


    //float t = (i) / 7.0f;
    float t = 1;
   
    float color_normal[] = { lerp(cube_color1[0], cube_color11[0], t),
                             lerp(cube_color1[1], cube_color11[1], t),
                             lerp(cube_color1[2], cube_color11[2], t),
                             lerp(cube_color1[3], cube_color11[3], t) };


    float color_input[] = { lerp(cube_color2[0], cube_color22[0], t),
                            lerp(cube_color2[1], cube_color22[1], t),
                            lerp(cube_color2[2], cube_color22[2], t),
                            lerp(cube_color2[3], cube_color22[3], t) };



    c_deformed_ms[i]->set_color(color_normal);
    c_deformed_ms[i]->set_color2(color_input);
    c_deformed_ms[i]->set_input_data(&color_cube);
    c_deformed_ms[i]->set_n_data(1);
   
    c_deformed[i]->set_material_settings(c_deformed_ms[i].get());

    cube_def_father->addChild(c_deformed[i].get());
  }
  //c_deformed_ms->
  c_deformed[0]->set_position(-10000.0f, -100000.0f, - 10000.0f);



  // Create a Camera
  state->camera.alloc();
  camera_distance = 10.0f;
  rise = true;
  float pos[] = { 0.0f, 0.0f, camera_distance };
  angle_cam = 1.57f;
  float view[] = { 0.0f, 0.0f, -1.0f };
  camera_pos.x = pos[0];
  camera_pos.y = pos[1];
  camera_pos.z = pos[2];

  camera_view.x = view[0];
  camera_view.y = view[1];
  camera_view.z = view[2];

  state->camera->set_position(pos);
  state->camera->set_view_direction(view);
  state->camera->setupPerspective(90, 1280.0f / 720.0f, 1.0f, 1500.0f);
  state->camera->setClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  //state->camera->set_clear_color(0.1f, 0.1f, 0.1f, 1.0f);
  EDK3::dev::GPUManager::CheckGLError("Prepare END");
}



void update_function(GameState *state, float delta_t) {

  // Sound data
  float* waveData = state->soloud.calcFFT();

  //Cubes 
  // Rotation
  //state->root->set_rotation_y(5.0f * ESAT::Time() / 300.0f);

  state->root->child(1)->child(7)->set_rotation_y(-5.0f * ESAT::Time() / 15.0f);
  state->root->child(1)->child(8)->set_rotation_y(-5.0f * ESAT::Time() / 15.0f);
  state->root->child(1)->child(9)->set_rotation_y(-5.0f * ESAT::Time() / 15.0f);

  state->root->child(1)->child(7)->set_rotation_x(cosf(0.0f) * (-5.0f * ESAT::Time() / 50.0f));
  state->root->child(1)->child(7)->set_rotation_z(sinf(0.0f) * (-5.0f * ESAT::Time() / 50.0f));
                                                                               
  state->root->child(1)->child(8)->set_rotation_x(cosf(1.0472f) * (-5.0f * ESAT::Time() / 50.0f));
  state->root->child(1)->child(8)->set_rotation_z(sinf(1.0472f) * (-5.0f * ESAT::Time() / 50.0f));
                                                                               
  state->root->child(1)->child(9)->set_rotation_x(cosf(2.0f * 1.0472f) * (-5.0f * ESAT::Time() / 50.0f));
  state->root->child(1)->child(9)->set_rotation_z(sinf(2.0f * 1.0472f) * (-5.0f * ESAT::Time() / 50.0f));


  //state->root->child(0)->set_rotation_x(5.0f * ESAT::Time() / 50.0f);

  //state->root->child(0)->child(1)->set_rotation_x(5.0f * ESAT::Time() / 10.0f);
  //state->root->child(0)->child(1)->set_rotation_y(5.0f * ESAT::Time() / 10.0f);
  /*state->root->child(0)->child(1)->set_rotation_y(5.0f * ESAT::Time() / 100.0f);*/

  //state->root->child(0)->child(2)->set_rotation_x(5.0f * ESAT::Time() / 10.0f);
  //state->root->child(0)->child(2)->set_rotation_y(5.0f * ESAT::Time() / 10.0f);
  /*state->root->child(0)->child(2)->set_rotation_y(5.0f * ESAT::Time() / 100.0f);*/

  //state->root->child(0)->set_rotation_z(5.0f * ESAT::Time());

  // Scale
  song_data = waveData[128];
  if (ESAT::IsKeyDown('C')) {
    color_cube = 1;
  }
  if (ESAT::IsSpecialKeyDown(ESAT::kSpecialKey_Space)) {
    
    current_explosion_scale += 1.0f;
    //color_cube = 1;
  }
  cube_deformation = lerp(cube_deformation, waveData[0] / 40, 0.3);
  color_cube = lerp(color_cube, 0.0f, 0.01);
  state->Cube_deformded->Update(cube_deformation, cube_deformation*0.2f);
  float newScale = 1.0f + song_data * 0.4f;
  float transitionForce = 0.4f;

  float newScale1 = 1.0f;
  float transitionForce1 = 0.4f;

  current_explosion_scale = lerp(0.0f, current_explosion_scale, 0.99f);


  float scale1 = newScale1 * transitionForce + state->root->scale()[0] * (1.0f - transitionForce1) + current_explosion_scale;
  state->root->child(1)->set_scale(scale1, scale1, scale1);


  // Cubes
  for (int i = 0; i < kCubes ; ++i) {
    int t = 1 + i;
    if ((float)i > kCubes *0.5f) {
      t= kCubes - i;
    }
    float newScaleCube = 1.0f + waveData[t] * 0.4f;
    float scaleCube = newScaleCube * transitionForce + state->root->child(0)->child(i)->scale()[1] * (1.0f - transitionForce) + current_explosion_scale *0.1f;
    state->root->child(0)->child(i)->set_scale(0.3f, scaleCube , 0.3f);
  }

  //state->root->child(0)->set_scale(scale, scale, scale);
  //state->root->child(0)->child(0)->set_scale(scale, scale, scale);
 // state->root->child(0)->child(2)->set_scale(scale1, scale1, scale1);
  //Explosion
  /*if (current_explosion_scale > 0.0f) {
    current_explosion_scale -= 0.0001f;
  }*/

  //Mov Camera


  angle_cam += 0.00174 * camera_speed;
  time_change_distance += delta_t;
  if (time_change_distance > 0.945f * 1.0f) {
    rise = !rise;
    time_change_distance = 0.0f;
  }
  if (rise) {
    camera_distance = lerp(camera_distance, 50.0f, 0.01f);
  }
  else {
    camera_distance = lerp(camera_distance,10.0f,  0.02f);
  }
  //("distance: %f\n", camera_distance);
  //printf("time: %f\n", time_change_distance);

  if (ESAT::IsKeyDown('S')) {
    camera_p_y = 5.0f;
  }
  if (ESAT::IsKeyDown('W')) {
    camera_p_y = 0.3f;
  }
  
  cam_y = lerp(cam_y, camera_p_y, 0.0225f);
  camera_pos.x = cosf(angle_cam) * camera_distance;
  camera_pos.y = cam_y           * camera_distance;
  camera_pos.z = sinf(angle_cam) * camera_distance;

  camera_view =  camera_pos.Normalized();
  camera_view *= -1.0f;



  float pos[] = { camera_pos.x,
    camera_pos.y, camera_pos.z };
  float view[] = { camera_view.x,
    camera_view.y, camera_view.z };

  state->camera->set_position(pos);
  state->camera->set_view_direction(view);

  //state->camera->MoveRotateCamera(delta_t);
}

void render_function(GameState *state) {



  // For Every frame... determine what's visible:
  state->camera->doCull(state->root.get());

  // Render
  EDK3::dev::GPUManager::CheckGLError("begin Render-->");
  state->camera->doRender();
  EDK3::dev::GPUManager::CheckGLError("end Render-->");
}

int ESAT::main(int argc, char **argv) {

  //ReplaceGPU Manager
  EDK3::dev::GPUManager *gpu = EDK3::dev::GPUManager::Instance();
  Mollete::GPUManager gpu_manager;
  EDK3::dev::GPUManager::Instance()->ReplaceGPUManagerImplementation(&gpu_manager);
  //gpu = EDK3::dev::GPUManager::Instance();

  // State
  GameState state;
  ESAT::WindowInit(1280, 720);
  ESAT::DrawSetTextFont("test.ttf");
  ESAT::DrawSetTextSize(18);
  ESAT::DrawSetFillColor(253, 255, 255, 128);
  prepare(&state);


  // Init soloud and play sound	
  SoLoud::Wav sample;

  state.soloud.init(SoLoud::Soloud::ENABLE_VISUALIZATION);
  sample.load("song.ogg");
  SoLoud::handle handle = state.soloud.play(sample);

  // Main loop
  while (ESAT::WindowIsOpened() && !ESAT::IsSpecialKeyDown(ESAT::kSpecialKey_Escape)) {

    double currentTime = RawTime();
    double delta_time = (currentTime - previus_time) / 1000.0f;
    previus_time = currentTime;
    //MoveCamera(&state, ESAT::Time());
    update_function(&state, delta_time);
    render_function(&state);
    ESAT::WindowFrame();
  }

  return 0;
}
