/// @author Javier Benito Abolafio <benitoab@esat-alumni.com>
/// Math Library implementation
#include "JBAmath.h"

// Matrix3 Multiply Vector3
JBA::Vector3 JBA::M3MultiplyVector3(JBA::Matrix3 m, SDL_Point v){
	
	JBA::Vector3 vec = {m.mat[0]*v.x + m.mat[3]*v.y + m.mat[6]*1,
            			    m.mat[1]*v.x + m.mat[4]*v.y + m.mat[7]*1,
            			    m.mat[2]*v.x + m.mat[5]*v.y + m.mat[8]*1};
	return vec;                      
}

SDL_Point JBA::M3MultiplyPoint(JBA::Matrix3 m, SDL_Point v){
	
	SDL_Point pt = {m.mat[0]*v.x + m.mat[3]*v.y + m.mat[6]*1,
        			    m.mat[1]*v.x + m.mat[4]*v.y + m.mat[7]*1};
	return pt;                      
}

// Matrix3 Multiply
JBA::Matrix3 JBA::M3Multiply(JBA::Matrix3 m1, JBA::Matrix3 m2){
  
	JBA::Matrix3 mat = {m1.mat[0]*m2.mat[0] + m1.mat[3]*m2.mat[1] + m1.mat[6]*m2.mat[2], 
              			  m1.mat[1]*m2.mat[0] + m1.mat[4]*m2.mat[1] + m1.mat[7]*m2.mat[2], 
              			  m1.mat[2]*m2.mat[0] + m1.mat[5]*m2.mat[1] + m1.mat[8]*m2.mat[2], 
              																  
              			  m1.mat[0]*m2.mat[3] + m1.mat[3]*m2.mat[4] + m1.mat[6]*m2.mat[5], 
              			  m1.mat[1]*m2.mat[3] + m1.mat[4]*m2.mat[4] + m1.mat[7]*m2.mat[5], 
              		    m1.mat[2]*m2.mat[3] + m1.mat[5]*m2.mat[4] + m1.mat[8]*m2.mat[5], 
              																  
              																  
              			  m1.mat[0]*m2.mat[6] + m1.mat[3]*m2.mat[7] + m1.mat[6]*m2.mat[8], 
              			  m1.mat[1]*m2.mat[6] + m1.mat[4]*m2.mat[7] + m1.mat[7]*m2.mat[8], 
              			  m1.mat[2]*m2.mat[6] + m1.mat[5]*m2.mat[7] + m1.mat[8]*m2.mat[8]};
 
	return mat;
}

// Matrix 3 Identity
JBA::Matrix3 JBA::M3Identity() {

  JBA::Matrix3 m = {1, 0, 0,
                    0, 1, 0,
                    0, 0, 1};

  return m;

}

// Translates a Matrix 3 of points to another location
JBA::Matrix3 JBA::M3Translate(const float x, const float y) {

  JBA::Matrix3 m = JBA::M3Identity();

  m.mat[6] = x;
  m.mat[7] = y;

  return m;

}

// Rotates a Matrix 3 of points around a given one
JBA::Matrix3 JBA::M3Rotate(float alpha) {

  JBA::Matrix3 m = {(float)cos(alpha), (float)sin(alpha), 0,
                    (float)-sin(alpha), (float)cos(alpha), 0,
                    0, 0, 1};

  return m;

}

// Scales a Matrix 3 of points with a size given
JBA::Matrix3 JBA::M3Scale(float x, float y) {

  JBA::Matrix3 m = JBA::M3Identity();

  m.mat[0] = x;
  m.mat[4] = y;

  return m;

}