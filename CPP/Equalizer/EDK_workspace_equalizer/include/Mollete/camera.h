#ifndef INCLUDE_MOLLETE_CAMERA_H_
#define INCLUDE_MOLLETE_CAMERA_H_ 1

// ----------------------------------------------------------------------------
// Copyright (C) 2014 Jose L. Hidalgo 
// Camera Class.
// ----------------------------------------------------------------------------
//
#include <EDK3/camera.h>
#include <matrix_4.h>

namespace Mollete {

  class Camera : public virtual EDK3::Camera {
  public:
    Camera();
   
    /** determine which objects are visible */
    virtual void doCull(const EDK3::Node *root_node) override;
    void doCullTR(const EDK3::Node *root_node, Matrix4x4 m) ;


    /** renders all visible (not culled) objects */
    virtual void doRender() const override;

    void setClearColor(float r, float g, float b, float a);

  

    //Structure to store the transformation of drawables with heritage
    struct Transformation {
      Transformation(float m[16]) {
        memcpy(transform, m, 16 * sizeof(float));
      };
      float transform[16];
    };
    unsigned int ndrawcalls;

    // hidden camera structure data
    std::vector<const EDK3::Drawable*> drawable_elements;
    std::vector<Transformation> drawables_tr;
    float clear_color[4];

  protected:
    virtual ~Camera();

  private:

  };


} /* end of EDK3 Namespace */

#endif
