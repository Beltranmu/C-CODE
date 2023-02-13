/// @author Ricardo Beltrán Muriel
/// Math Library definition

#include "rbmmath.h"
#include "board.h"
#include <stdio.h>

// Vec2
/*
RBM::Vec2 RBM::AddVec2(const RBM::Vec2* v1, 
                       const RBM::Vec2* v2){
  return {v1->x + v2->x, v1->y + v2->y};
}

//Vec3

RBM::Vec3 RBM::M3ProductVec3(RBM::Mat3 m, RBM::Vec3 v){
	
	RBM::Vec3 vr={
       m.m[0]*v.x + m.m[3]*v.y + m.m[6]*v.z,
			 m.m[1]*v.x + m.m[4]*v.y + m.m[7]*v.z,
			 m.m[2]*v.x + m.m[5]*v.y + m.m[8]*v.z};
	return vr;                      
}

void RBM::PrintV3(const RBM::Vec3* v){
  printf("(%10.03f,%10.03f,%10.03f) \n", v->x, v->y, v->z);
}

//Mat3

RBM::Mat3 RBM::M3Identity(){
	
	RBM::Mat3 mr={1,0,0,
                0,1,0,
                0,0,1};
	return mr;
}

RBM::Mat3 RBM::M3Translate(const float x, 
                      const float y){
	
	RBM::Mat3 mr = RBM::M3Identity();
	
	mr.m[6]=x;
	mr.m[7]=y;
	
	return mr;
}

RBM::Mat3 RBM::M3Rotation(float alfa){
	
	RBM::Mat3 mr={(float)cos(alfa), (float)sin(alfa),0,
                (float)-sin(alfa),(float)cos(alfa),0,
                      0         ,0        ,1};
	return mr;
}

RBM::Mat3 RBM::M3Scale(float sc_x, float sc_y){
	
	RBM::Mat3 mr = RBM::M3Identity();
	
	mr.m[0]= sc_x;
	mr.m[4]= sc_y;
	
	return mr;
}


RBM::Mat3 RBM::M3Product(RBM::Mat3 m1, RBM::Mat3 m2){
  
	RBM::Mat3 mr={
       m1.m[0]*m2.m[0] + m1.m[3]*m2.m[1] + m1.m[6]*m2.m[2], 
			 m1.m[1]*m2.m[0] + m1.m[4]*m2.m[1] + m1.m[7]*m2.m[2], 
			 m1.m[2]*m2.m[0] + m1.m[5]*m2.m[1] + m1.m[8]*m2.m[2], 
																  
			 m1.m[0]*m2.m[3] + m1.m[3]*m2.m[4] + m1.m[6]*m2.m[5], 
			 m1.m[1]*m2.m[3] + m1.m[4]*m2.m[4] + m1.m[7]*m2.m[5], 
			 m1.m[2]*m2.m[3] + m1.m[5]*m2.m[4] + m1.m[8]*m2.m[5], 
																  
																  
			 m1.m[0]*m2.m[6] + m1.m[3]*m2.m[7] + m1.m[6]*m2.m[8], 
			 m1.m[1]*m2.m[6] + m1.m[4]*m2.m[7] + m1.m[7]*m2.m[8], 
			 m1.m[2]*m2.m[6] + m1.m[5]*m2.m[7] + m1.m[8]*m2.m[8]};
 
	return mr;
}
*/
//Ohters

unsigned char RBM::GetMatrixPosition(unsigned char pos,
                                     const int desp){
                                       
  unsigned char resul = pos + (unsigned char)desp;
    
  resul %= Board::kBoardSize;
  
  return resul;

}
/*
int RBM::Abs(int v){
  
  if(v< 0){
    v *=-1;
  }
  return v;
}*/

