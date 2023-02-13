/// @author Javier Benito Abolafio <benitoab@esat-alumni.com>
/// Math Library definition
#ifndef __JBAMATH_H__
#define __JBAMATH_H__ 1

#include <stdio.h>
#include <math.h>

#include "SDL.h"

namespace JBA{

	const float PI = 3.14159265359;

  // Vector 2D
	struct Vector2{

		float x, y;
	
	};

  // Vector 3D
  struct Vector3{

		float x, y, z;
	
	};

  // Vector 4D
  struct Vector4{

		float x, y, z, w;
	
	};

  // Matrix 3x3
  struct Matrix3{

    float mat[9];

  };

  // Matrix 3
  Matrix3 M3Multiply(JBA::Matrix3 m1, JBA::Matrix3 m2);
  Vector3 M3MultiplyVector3(JBA::Matrix3 m, SDL_Point v);
  SDL_Point M3MultiplyPoint(JBA::Matrix3 m, SDL_Point v);

  // Transform Matrix 3x3
  Matrix3 M3Identity();
  Matrix3 M3Translate(const float x, const float y);
  Matrix3 M3Rotate(float alpha);
  Matrix3 M3Scale(float x, float y);

}

#endif // __JBAMATH_H__