/**
  * @file main.cc
  * @brief Main file of the game.
  * @details This is the core file. It gathers all classes of the game to make it possible.
  * @author Ricardo Beltrán Muriel <beltranmu@esat-alumni.com> 
  * @version alfa 1.0
  * @date Dec-2020
  * @copyright ESAT
  */


#ifndef __RBMMATH_H__
#define __RBMMATH_H__ 1

#include<math.h>

namespace RBM{
  /** @struct Vector of 2 componets*/
  struct Vec2{
    float x; ///@var x coord
    float y; ///@var y coord
  };
  /** @struct Vector of 3 componets*/
  struct Vec3{
    float x; ///@var x coord
    float y; ///@var y coord
    float z; ///@var z coord
  };
  /** @struct Vector of 4 componets*/
  struct Vec4{
    float x;  ///@var x coord
    float y;  ///@var y coord
    float z;  ///@var z coord
    float t;  ///@var t coord
  };
    
  /** @struct Matrix 3x3*/
  struct Mat3{
    float m[9]; ///@var array of 9 float 
  };
  
  /** @struct Transform (position, rotation, scale)*/
  struct Transform2{
    Vec2 position;    ///@var Vec2 that contain the position
    float rotation;   ///@var contains the angle of your shape in radians
    Vec2 scale;       ///@var Vec2 that contain the scale (x scale, y scale)
  };
  /*
  const Vec3 rect_points_c[4]={{0.5f,   0.5f,1.0f},
                             {-0.5f,  0.5f,1.0f},
                             {-0.5f, -0.5f,1.0f},
                             {0.5f,  -0.5f,1.0f},};
                             
  const Vec3 rect_points[4]={{0.0f, 0.0f, 1.0f},
                             {1.0f, 0.0f, 1.0f},
                             {1.0f, 1.0f, 1.0f},
                             {0.0f, 1.0f, 1.0f},};*/
  //Vec2
  /*
  Vec2 AddVec2(const Vec2* v1, const Vec2* v2);
   
  //Vec3
  
  Vec3 M3ProductVec3(Mat3 m, Vec3 v);
  void PrintV3(const Vec3* v);

  //Vec4
  
  //Mat2
  
  //Mat3
  
  Mat3  M3Identity();
  Mat3  M3Translate(const float x, const float y);
  Mat3  M3Rotation(const float alfa);
  Mat3  M3Scale(const float sc_x, const float sc_y);
  Mat3  M3Product(Mat3 m1, Mat3 m2);*/
  
  //Mat4
  
  //Others
  /** 
   *@brief Tells you the next position after a desp of the board
   *@param unsigned char pos, your current position(x or y, but not both)
   *@param  const int desp how many tiles you want to move
   *@return the next position 
  */ 
  unsigned char GetMatrixPosition(unsigned char pos,
                                  const int desp);

  //int Abs(int v);
}

#endif