



#include "..\..\include\Mollete\camera.h"
#include <EDK3/node.h>
#include <EDK3/drawable.h>
#include <ESAT/input.h>
#include <EDK3/dev/gpumanager.h>
#include <vector>
#include <math.h>
#include "matrix_4.h"
#include <ESAT_extra/imgui.h>
#include <Material/skybox.h>


Mollete::Camera::Camera(){
  clear_color[0] = 0.0f;
  clear_color[1] = 0.0f;
  clear_color[2] = 0.0f;
  clear_color[3] = 0.0f;

  position_c = 0.0f;

  rotation = 0.0f;

  forward.x = 0.0f;
  forward.y = 0.0f;
  forward.z = -1.0f;

  right.x = 0.0f;
  right.y = 0.0f;
  right.z = 0.0f;

  up = 0.0f;

  world_up.x = 0.0f;
  world_up.y = 1.0f;
  world_up.z = 0.0f;

  previous_mouse_x = 0.0f;
  previous_mouse_y = 0.0f;

  yaw = 0.0f;
  pitch = 0.0f;

  settings.mov_speed = 500.0f;
  settings.rot_speed = 50.f;//0.174533f*800.0f;

  settings.max_pitch = 89.0f;
  settings.min_pitch = -89.0f;

  first_mouse_loc = true;
  ndrawcalls = 0;
  rotate_cam_with_mouse = false;

}

void Mollete::Camera::doCullTR(const EDK3::Node * root_node, Matrix4x4 matrix) {

  for (int i = 0; i < root_node->num_children(); i++) {
    const EDK3::Drawable* is_drawable = dynamic_cast<const EDK3::Drawable*>(root_node->child(i));
    Matrix4x4 child = Matrix4x4(root_node->child(i)->local_transform());
    Matrix4x4 parent_tr = matrix.Multiply(child);

    // This node has children
    if (nullptr != is_drawable) {
      Vector3 cam_to_obj = Vector3(is_drawable->position()) - position_c;
      float dis = cam_to_obj.Magnitude();
      float dot = forward.DotProduct(forward, cam_to_obj.Normalized());

 
      if (dot > settings.fov_cos && (dis > settings.znear) && dis < settings.zfar) {
        drawable_elements.push_back(is_drawable);
        drawables_tr.push_back(Transformation(parent_tr.m));
      }
    }
    if (root_node->child(i)->num_children() > 0) {
      doCullTR(root_node->child(i), parent_tr);
 
    }
  }

}

void Mollete::Camera::doCull(const EDK3::Node * root_node) {

  drawable_elements.clear();
  drawables_tr.clear();

  for (int i = 0; i < root_node->num_children(); i++) {
    const EDK3::Drawable* is_drawable = dynamic_cast<const EDK3::Drawable*>(root_node->child(i));
    Matrix4x4 parent_tr = Matrix4x4(root_node->child(i)->local_transform());


    
    // This node has children
    if (nullptr != is_drawable) {
      Vector3 cam_to_obj = Vector3(is_drawable->position()) - position_c;
      float dis = cam_to_obj.Magnitude();
      float dot = forward.DotProduct(forward, cam_to_obj.Normalized());

     

      if (i == 0 ||
         (dot > settings.fov_cos && (dis > settings.znear) && dis < settings.zfar)) {
        drawable_elements.push_back(is_drawable);
        drawables_tr.push_back(Transformation(parent_tr.m));
      }
    }
    if (root_node->child(i)->num_children() > 0) {
      // drawables_tr.push_back(Transformation(parent_tr.m));
      doCullTR(root_node->child(i), parent_tr);
    }
  }
  ndrawcalls = drawable_elements.size();
}





void Mollete::Camera::doRender() const{

  EDK3::dev::GPUManager::Instance()->clearFrameBuffer(clear_color);
  
  for (int i = 0; i < drawable_elements.size(); ++i) {
    drawable_elements[i]->material()->enable(drawable_elements[i]->material_settings());
    drawable_elements[i]->material()->setupModel(drawable_elements[i]->local_transform());
    drawable_elements[i]->material()->setupCamera(projection_matrix(), view_matrix());
    drawable_elements[i]->geometry()->bindAttributesAndRenderWithCurrentMaterial(drawable_elements[i]->material());
  }
}

void Mollete::Camera::setClearColor(float r, float g, float b, float a) {
  clear_color[0] = r;
  clear_color[1] = g;
  clear_color[2] = b;
  clear_color[3] = a;
}

void Mollete::Camera::setPerspective(float fovy, float aspect, float znear, float zfar)
{
  setupPerspective(fovy, aspect, znear, zfar);

  settings.fov = fovy;
  settings.fov_cos = cosf(fovy * 3.1415f / 180.0f);
  settings.znear = znear;
  settings.zfar = zfar;
  settings.aspect = aspect;
}

void Mollete::Camera::UpdatePerspective()
{
  setPerspective(settings.fov, settings.aspect, settings.znear, settings.zfar);
}


void Mollete::Camera::MoveRotateCamera(float delta_time) {

  float camera_lineal_speed = settings.mov_speed * delta_time;
  float camera_rot_speed = settings.rot_speed * delta_time;


  // Mouse input
  
  float mouse_x = (float)ESAT::MousePositionX();
  float mouse_y = (float)ESAT::MousePositionY();

  if (first_mouse_loc) {
    previous_mouse_x = mouse_x;
    previous_mouse_y = mouse_y;
    first_mouse_loc = false;
  }

  float offset_x = mouse_x - previous_mouse_x;
  float offset_y = previous_mouse_y - mouse_y;

  previous_mouse_x = mouse_x;
  previous_mouse_y = mouse_y;
  if (rotate_cam_with_mouse) {
    if (ESAT::MouseButtonPressed(1)) {
      yaw += camera_rot_speed * offset_x;
      pitch += camera_rot_speed * offset_y;
    }
  }
  pitch = MathUtils::Clamp(pitch, settings.min_pitch, settings.max_pitch);

  if (ESAT::IsKeyPressed('W')) {
    position_c += (forward * camera_lineal_speed);
  }
  if (ESAT::IsKeyPressed('S')) {
    position_c -= (forward * camera_lineal_speed);
  }
  if (ESAT::IsKeyPressed('A')) {
    position_c -= (right * camera_lineal_speed);
  }
  if (ESAT::IsKeyPressed('D')) {
    position_c += (right * camera_lineal_speed);
  }

  if (ESAT::IsKeyPressed('Q')) {
    position_c -= (up * camera_lineal_speed);
  }
  if (ESAT::IsKeyPressed('E')) {
    position_c += (up * camera_lineal_speed);
  }

  Vector3 new_forward(0.0f);
  new_forward.x = cosf(MathUtils::DegreesToRadian(yaw)) * cosf(MathUtils::DegreesToRadian(pitch));
  new_forward.y = sinf(MathUtils::DegreesToRadian(pitch));
  new_forward.z = sinf(MathUtils::DegreesToRadian(yaw)) * cosf(MathUtils::DegreesToRadian(pitch));

  forward = new_forward.Normalized();

  right = Vector3::CrossProduct(forward, world_up).Normalized();
  up = Vector3::CrossProduct(right, forward).Normalized();

  Matrix4x4 new_view_matrix = Matrix4x4::LookAt(position_c, position_c + forward, world_up);
  set_view_matrix(new_view_matrix.m);


  setupPerspective(settings.fov, settings.aspect, settings.znear, settings.zfar);
}



void Mollete::Camera::imguiPanel() {

  ImGui::Begin("Camera Panel", NULL, ImGuiWindowFlags_AlwaysAutoResize);
  ImGui::Text("Pitch : %f", pitch);
  ImGui::Text("Yaw : %f", yaw);
  ImGui::Text("Position:");
  ImGui::Text("X : %f", position_c.x);
  ImGui::SameLine(); ImGui::Text("Y : %f", position_c.y);
  ImGui::SameLine(); ImGui::Text("Z : %f", position_c.z);
  ImGui::Separator();
  ImGui::SliderFloat("Cam Speed", &settings.mov_speed, 100.0f, 20000.0f);
  ImGui::SliderFloat("Cam Rotation", &settings.rot_speed, 1.0f, 100.0f);
  ImGui::SliderFloat("Fov", &settings.fov, 1.0f, 179.0f);
  ImGui::SliderFloat("ZNear", &settings.znear, 0.01f, 100.0f);
  ImGui::SliderFloat("ZFar", &settings.zfar, 100.0f, 1000000.0f);
  ImGui::SliderFloat("Aspect", &settings.aspect, 0.01f, 5.0f);
  if (ImGui::Button("UpdateCam")) {
    UpdatePerspective();
  }

  ImGui::Separator();
  ImGui::Text("Rotation with mouse", position_c.z);

  ImGui::Checkbox("Rotation with mouse", &rotate_cam_with_mouse);
  /*if (ImGui::Button("Activate")) {
    rotate_cam_with_mouse = true;
  }
  ImGui::SameLine();
  if (ImGui::Button("Desactivate")) {
    rotate_cam_with_mouse = false;
  }*/
  ImGui::Separator(); //Linea separadora
  ImGui::End();

}


Mollete::Camera::~Camera(){
}
