

#include "camerafps.h"
#include <ESAT/input.h>
#include <math.h>
#include "matrix_4.h"


Camerafps::Camerafps() {
  
  position.x = 0.0f;
  position.y = 0.0f;
  position.z = 1.0f;

  rotation.x = 0.0f;
  rotation.y = 0.0f;
  rotation.z = 0.0f;

  input_mouse_x = 0.0f;
  input_mouse_y = 0.0f;

}

Camerafps::~Camerafps() {
}


void Camerafps::MoveRotateCamera(float delta_time) {

  float mov_speed = 10.0f;
  float rot_speed = 0.174533f * 5.0f;

  const float* old_view_matrix = view_matrix();
  Vector3 forward;
  Vector3 right;
  Vector3 up;
 
  forward.x = *(old_view_matrix + 2);
  forward.y = *(old_view_matrix + 6);
  forward.z = *(old_view_matrix + 10);

  right.x = *(old_view_matrix + 0);
  right.y = *(old_view_matrix + 4);
  right.z = *(old_view_matrix + 8);

  up.x = *(old_view_matrix + 1);
  up.y = *(old_view_matrix + 5);
  up.z = *(old_view_matrix + 9);


 
  //if (ESAT::MouseButtonPressed(1)) {
    if (ESAT::IsKeyPressed('W')) {
      position -= (forward * mov_speed * delta_time);
    }
    if (ESAT::IsKeyPressed('S')) {
      position += (forward * mov_speed * delta_time);
    }

    if (ESAT::IsKeyPressed('A')) {
      position -= (right * mov_speed * delta_time);
    }
    if (ESAT::IsKeyPressed('D')) {
      position += (right * mov_speed * delta_time);
    }

    if (ESAT::IsKeyPressed('Q')) {
      position -= (up * mov_speed * delta_time);
    }
    if (ESAT::IsKeyPressed('E')) {
      position += (up * mov_speed * delta_time);
    }


    //Input Mouse

    //float delta_mouse_x = (float)ESAT::MousePositionX() - input_mouse_x;
    //float delta_mouse_y = (float)ESAT::MousePositionY() - input_mouse_y;

   // rotation.x += -1.0f * delta_mouse_y *  rot_speed * delta_time;
    //rotation.y += -1.0f * delta_mouse_x *  rot_speed * delta_time;
 // }
  //input_mouse_x = (float)ESAT::MousePositionX();
  //input_mouse_y = (float)ESAT::MousePositionY();

  if (ESAT::IsSpecialKeyPressed(ESAT::kSpecialKey_Up)) {
    rotation.x += rot_speed * delta_time;
  }
  if (ESAT::IsSpecialKeyPressed(ESAT::kSpecialKey_Down)) {
    rotation.x -= rot_speed * delta_time;
  }

  if (ESAT::IsSpecialKeyPressed(ESAT::kSpecialKey_Right)) {
    rotation.y -= rot_speed * delta_time;
  }
  if (ESAT::IsSpecialKeyPressed(ESAT::kSpecialKey_Left)) {
    rotation.y += rot_speed * delta_time;
  }
  
  Matrix4x4 transform_matrix;
  Vector3 scale = { 1.0f, 1.0f, 1.0f };

  transform_matrix = transform_matrix.GetTransform(position, scale,
    rotation.x, rotation.y, 0);
  //transform_matrix.printM4();
  transform_matrix.Inverse();
  transform_matrix = transform_matrix.Transpose();
  float* new_view_matrix = transform_matrix.m;
  Vector3 v_dir = position.Normalized();
  //*(new_view_matrix + 2) = v_dir.x;
  //*(new_view_matrix + 6) = v_dir.y;
  //*(new_view_matrix + 10) = v_dir.z;
  //const float target_position[] = { v_dir.x, v_dir.y, v_dir.z };
  //set_view_target(target_position);
  //set_view_direction(target_position);

  set_view_matrix(new_view_matrix);
  //const float target_position[] = { target_pos.x, target_pos.y, target_pos.z };
 

}



