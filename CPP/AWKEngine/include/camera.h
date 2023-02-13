
/************************************************
 * Camera struct
 * Developed by Ricardo BM & Javier BA
 * Graphic Engine Development
 * ESAT - 2021/2022
 ***********************************************/

#ifndef __CAMERA_H__
#define __CAMERA_H__ 1

#include <stdint.h>

namespace AWK {

  struct Camera {

    int32_t width, height;
    float x, y, z;
    float rx, ry, rz;
    float zNear;
    float zFar;
    float FOV;
    float aspectRatio;

  };
 
}

#endif // !__CAMERA_H__