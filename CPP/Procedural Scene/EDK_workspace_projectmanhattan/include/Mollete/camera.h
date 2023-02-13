#ifndef INCLUDE_MOLLETE_CAMERA_H_
#define INCLUDE_MOLLETE_CAMERA_H_ 1

// ----------------------------------------------------------------------------
// Copyright (C) 2014 Jose L. Hidalgo 
// Camera Class.
// ----------------------------------------------------------------------------
//
#include <EDK3/camera.h>
#include <EDK3/drawable.h>
#include <vector_3.h>
#include <matrix_4.h>

struct CameraSettings {
  float rot_speed;
  float mov_speed;
  float max_pitch;
  float min_pitch;

  float fov;
  float fov_cos;
  float znear;
  float zfar;
  float aspect;
};

namespace Mollete {

  class Camera : public virtual EDK3::Camera {
  public:
    Camera();
   
    /** determine which objects are visible */
    virtual void doCull(const EDK3::Node *root_node) override;
    virtual void doCullTR(const EDK3::Node *root_node, Matrix4x4 m) ;

    /** renders all visible (not culled) objects */
    virtual void doRender() const override;

    void setClearColor(float r, float g, float b, float a);

    void setPerspective(float fovy, float aspect, float znear, float zfar);

    void UpdatePerspective();

    //movement
    void MoveRotateCamera(float delta_time);
    void imguiPanel();


    //Movement
    bool rotate_cam_with_mouse;
    Vector3 position_c;
    Vector3 rotation;
    Vector3 forward;
    Vector3 up;
    Vector3 right;
    Vector3 world_up;
    CameraSettings settings;
    float previous_mouse_x;
    float previous_mouse_y;
    float yaw;
    float pitch;
    bool first_mouse_loc;

    //Information
    unsigned int ndrawcalls;

    //Structure to store the transformation of drawables with heritage
    struct Transformation {
      Transformation(float m[16]) {
        memcpy(transform, m, 16 * sizeof(float));
      };
      float transform[16];
    };

    std::vector<const EDK3::Drawable*> drawable_elements;
    std::vector<Transformation> drawables_tr;
    float clear_color[4];

    struct Data;
  protected:
    virtual ~Camera();

  private:

  };


} /* end of EDK3 Namespace */

#endif
