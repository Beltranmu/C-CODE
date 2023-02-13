#ifndef __CAMERAFPS_H__
#define __CAMERAFPS_H__ 1

#include <EDK3/camera.h>
#include "vector_3.h"

class EDK3::Node;
  class Camerafps : public EDK3::Camera {
  public:
    Camerafps();
    virtual ~Camerafps();

    Vector3 position;
    Vector3 rotation;

    float input_mouse_x;
    float input_mouse_y;

    Vector3 target_pos;

  
    //EDK3::ref_ptr<EDK3::Node> camera_location;

    void MoveRotateCamera(float delta_time);
 


  private:

  };




#endif // !__CAMERAFPS_H__
