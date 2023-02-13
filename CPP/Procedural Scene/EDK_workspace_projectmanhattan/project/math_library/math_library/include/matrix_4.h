#ifndef __MATRIX4_H__
#define __MATRIX4_H__ 1

#include "vector_3.h"
#include "vector_4.h"
#include "matrix_3.h"

class Matrix4x4{
 public:

  Matrix4x4();
  Matrix4x4(float a[16]);
  Matrix4x4(const float a[16]);
  Matrix4x4(float value);
  Matrix4x4(const Matrix4x4& copy);
  ~Matrix4x4();

  Matrix4x4 Identity() const;
  Matrix4x4 Multiply(const Matrix4x4& other) const;

  float Determinant() const;
  Matrix4x4 Adjoint() const;
  bool GetInverse(Matrix4x4* out) const;
  bool Inverse();

  Matrix4x4 Transpose() const;

  static Matrix4x4 LookAt(Vector3 eye, Vector3 target, Vector3 up);

  static Matrix4x4 Translate(const Vector3& distance);
  static Matrix4x4 Translate(float x, float y, float z);

  static Matrix4x4 Scale(const Vector3& scale);
  static Matrix4x4 Scale(float x, float y, float z);

  static Matrix4x4 RotateX(float radians);
  static Matrix4x4 RotateY(float radians);
  static Matrix4x4 RotateZ(float radians);

  static Matrix4x4 GetTransform(const Vector3& translate, const Vector3& scale,
                      float rotateX, float rotateY, float rotateZ);

  static Matrix4x4 GetTransform(float trans_x, float trans_y, float trans_z,
                      float scale_x, float scale_y, float scale_Z,
                      float rotateX, float rotateY, float rotateZ);

  Matrix4x4 PerspectiveMatrix(float fov, float aspect,
	  float near, float far) const;

  Matrix4x4 OrthoMatrix(float right, float left, float top, float valueottom,
	  float near, float far) const;

  Vector4 GetColum(int colum) const;
  Vector4 GetLine(int line) const;

  Matrix4x4 operator+(const Matrix4x4& other) const;
  Matrix4x4& operator+=(const Matrix4x4& other);
  Matrix4x4 operator+(float value) const;
  Matrix4x4& operator+=(float value);
  Matrix4x4 operator-(const Matrix4x4& other) const;
  Matrix4x4& operator-=(const Matrix4x4& other);
  Matrix4x4 operator-(float value) const;
  Matrix4x4& operator-=(float value);
  Matrix4x4& operator*=(float value);
  Matrix4x4 operator*(float value) const;
  Matrix4x4& operator/=(float value);
  Matrix4x4 operator/(float value) const;
  bool operator==(const Matrix4x4& other);
  bool operator!=(const Matrix4x4& other);
  void operator=(const Matrix4x4& other);

  void printM4();

  float m[16];
};


inline Matrix4x4::Matrix4x4() {

}

inline Matrix4x4::Matrix4x4(float array[16]) {
  m[0] = array[0];
  m[1] = array[1];
  m[2] = array[2];
  m[3] = array[3];
  m[4] = array[4];
  m[5] = array[5];
  m[6] = array[6];
  m[7] = array[7];
  m[8] = array[8];
  m[9] = array[9];
  m[10] = array[10];
  m[11] = array[11];
  m[12] = array[12];
  m[13] = array[13];
  m[14] = array[14];
  m[15] = array[15];
}

inline Matrix4x4::Matrix4x4(const float array[16]) {
  m[0] = array[0];
  m[1] = array[1];
  m[2] = array[2];
  m[3] = array[3];
  m[4] = array[4];
  m[5] = array[5];
  m[6] = array[6];
  m[7] = array[7];
  m[8] = array[8];
  m[9] = array[9];
  m[10] = array[10];
  m[11] = array[11];
  m[12] = array[12];
  m[13] = array[13];
  m[14] = array[14];
  m[15] = array[15];
}

inline Matrix4x4::Matrix4x4(float value) {
  m[0] = value;
  m[1] = value;
  m[2] = value;
  m[3] = value;
  m[4] = value;
  m[5] = value;
  m[6] = value;
  m[7] = value;
  m[8] = value;
  m[9] = value;
  m[10] = value;
  m[11] = value;
  m[12] = value;
  m[13] = value;
  m[14] = value;
  m[15] = value;
}

inline Matrix4x4::Matrix4x4(const Matrix4x4& copy) {
  m[0] = copy.m[0];
  m[1] = copy.m[1];
  m[2] = copy.m[2];
  m[3] = copy.m[3];
  m[4] = copy.m[4];
  m[5] = copy.m[5];
  m[6] = copy.m[6];
  m[7] = copy.m[7];
  m[8] = copy.m[8];
  m[9] = copy.m[9];
  m[10] = copy.m[10];
  m[11] = copy.m[11];
  m[12] = copy.m[12];
  m[13] = copy.m[13];
  m[14] = copy.m[14];
  m[15] = copy.m[15];
}

inline Matrix4x4::~Matrix4x4() {

}

inline Matrix4x4 Matrix4x4::Identity() const {
  Matrix4x4 result_matrix;
 
  result_matrix.m[0] = 1.0f;
  result_matrix.m[1] = 0.0f;
  result_matrix.m[2] = 0.0f;
  result_matrix.m[3] = 0.0f;

  result_matrix.m[4] = 0.0f;
  result_matrix.m[5] = 1.0f;
  result_matrix.m[6] = 0.0f;
  result_matrix.m[7] = 0.0f;

  result_matrix.m[8] = 0.0f;
  result_matrix.m[9] = 0.0f;
  result_matrix.m[10] = 1.0f;
  result_matrix.m[11] = 0.0f;

  result_matrix.m[12] = 0.0f;
  result_matrix.m[13] = 0.0f;
  result_matrix.m[14] = 0.0f;
  result_matrix.m[15] = 1.0f;

	return result_matrix;
}

inline Matrix4x4 Matrix4x4::Multiply(const Matrix4x4& other)const  {
  
  Matrix4x4 result_matrix;

  result_matrix.m[0] =  ((m[0] * other.m[0])  + (m[4] * other.m[1])  + (m[8] * other.m[2])   + (m[12] * other.m[3]));
  result_matrix.m[1] =  ((m[1] * other.m[0])  + (m[5] * other.m[1])  + (m[9] * other.m[2])   + (m[13] * other.m[3]));
  result_matrix.m[2] =  ((m[2] * other.m[0])  + (m[6] * other.m[1])  + (m[10] * other.m[2])  + (m[14] * other.m[3]));
  result_matrix.m[3] =  ((m[3] * other.m[0])  + (m[7] * other.m[1])  + (m[11] * other.m[2])  + (m[15] * other.m[3]));
       
  result_matrix.m[4] =  ((m[0] * other.m[4])  + (m[4] * other.m[5])  + (m[8] * other.m[6])   + (m[12] * other.m[7]));
  result_matrix.m[5] =  ((m[1] * other.m[4])  + (m[5] * other.m[5])  + (m[9] * other.m[6])   + (m[13] * other.m[7]));
  result_matrix.m[6] =  ((m[2] * other.m[4])  + (m[6] * other.m[5])  + (m[10] * other.m[6])  + (m[14] * other.m[7]));
  result_matrix.m[7] =  ((m[3] * other.m[4])  + (m[7] * other.m[5])  + (m[11] * other.m[6])  + (m[15] * other.m[7]));
       
  result_matrix.m[8] =  ((m[0] * other.m[8])  + (m[4] * other.m[9])  + (m[8] * other.m[10])  + (m[12] * other.m[11]));
  result_matrix.m[9] =  ((m[1] * other.m[8])  + (m[5] * other.m[9])  + (m[9] * other.m[10])  + (m[13] * other.m[11]));
  result_matrix.m[10] = ((m[2] * other.m[8])  + (m[6] * other.m[9])  + (m[10] * other.m[10]) + (m[14] * other.m[11]));
  result_matrix.m[11] = ((m[3] * other.m[8])  + (m[7] * other.m[9])  + (m[11] * other.m[10]) + (m[15] * other.m[11]));
       
  result_matrix.m[12] = ((m[0] * other.m[12]) + (m[4] * other.m[13]) + (m[8] * other.m[14])  + (m[12] * other.m[15]));
  result_matrix.m[13] = ((m[1] * other.m[12]) + (m[5] * other.m[13]) + (m[9] * other.m[14])  + (m[13] * other.m[15]));
  result_matrix.m[14] = ((m[2] * other.m[12]) + (m[6] * other.m[13]) + (m[10] * other.m[14]) + (m[14] * other.m[15]));
  result_matrix.m[15] = ((m[3] * other.m[12]) + (m[7] * other.m[13]) + (m[11] * other.m[14]) + (m[15] * other.m[15]));
                                                                               
  return result_matrix;
  
 
}

inline float Matrix4x4::Determinant() const {

  Matrix3x3 sub_matrix[4];

  sub_matrix[0].m[0] = m[5];
  sub_matrix[0].m[1] = m[6];
  sub_matrix[0].m[2] = m[7];
                       
  sub_matrix[0].m[3] = m[9];
  sub_matrix[0].m[4] = m[10];
  sub_matrix[0].m[5] = m[11];
                      
  sub_matrix[0].m[6] = m[13];
  sub_matrix[0].m[7] = m[14];
  sub_matrix[0].m[8] = m[15];

  /*-----------------------*/
  sub_matrix[1].m[0] = m[4];
  sub_matrix[1].m[1] = m[6];
  sub_matrix[1].m[2] = m[7];

  sub_matrix[1].m[3] = m[8];
  sub_matrix[1].m[4] = m[10];
  sub_matrix[1].m[5] = m[11];

  sub_matrix[1].m[6] = m[12];
  sub_matrix[1].m[7] = m[14];
  sub_matrix[1].m[8] = m[15];

  /*------------------------*/
  sub_matrix[2].m[0] = m[4];
  sub_matrix[2].m[1] = m[5];
  sub_matrix[2].m[2] = m[7];

  sub_matrix[2].m[3] = m[8];
  sub_matrix[2].m[4] = m[9];
  sub_matrix[2].m[5] = m[11];

  sub_matrix[2].m[6] = m[12];
  sub_matrix[2].m[7] = m[13];
  sub_matrix[2].m[8] = m[15];

  /*------------------------*/
  sub_matrix[3].m[0] = m[4];
  sub_matrix[3].m[1] = m[5];
  sub_matrix[3].m[2] = m[6];
             
  sub_matrix[3].m[3] = m[8];
  sub_matrix[3].m[4] = m[9];
  sub_matrix[3].m[5] = m[10];
             
  sub_matrix[3].m[6] = m[12];
  sub_matrix[3].m[7] = m[13];
  sub_matrix[3].m[8] = m[14];

  float det[4];
  det[0] = sub_matrix[0].Determinant();
  det[1] = sub_matrix[1].Determinant();
  det[2] = sub_matrix[2].Determinant();
  det[3] = sub_matrix[3].Determinant();
 


  return m[0] * sub_matrix[0].Determinant() - m[1] * sub_matrix[1].Determinant() +
         m[2] * sub_matrix[2].Determinant() - m[3] * sub_matrix[3].Determinant();
           
}

inline Matrix4x4 Matrix4x4::Adjoint() const {
  Matrix3x3 sub_matrix[16];

  sub_matrix[0].m[0] = m[5];
  sub_matrix[0].m[1] = m[6];
  sub_matrix[0].m[2] = m[7];

  sub_matrix[0].m[3] = m[9];
  sub_matrix[0].m[4] = m[10];
  sub_matrix[0].m[5] = m[11];

  sub_matrix[0].m[6] = m[13];
  sub_matrix[0].m[7] = m[14];
  sub_matrix[0].m[8] = m[15];

  /*------------------------*/
  sub_matrix[1].m[0] = m[4];
  sub_matrix[1].m[1] = m[6];
  sub_matrix[1].m[2] = m[7];
             
  sub_matrix[1].m[3] = m[8];
  sub_matrix[1].m[4] = m[10];
  sub_matrix[1].m[5] = m[11];
             
  sub_matrix[1].m[6] = m[12];
  sub_matrix[1].m[7] = m[14];
  sub_matrix[1].m[8] = m[15];

  /*------------------------*/
  sub_matrix[2].m[0] = m[4];
  sub_matrix[2].m[1] = m[5];
  sub_matrix[2].m[2] = m[7];
             
  sub_matrix[2].m[3] = m[8];
  sub_matrix[2].m[4] = m[9];
  sub_matrix[2].m[5] = m[11];
             
  sub_matrix[2].m[6] = m[12];
  sub_matrix[2].m[7] = m[13];
  sub_matrix[2].m[8] = m[15];

  /*------------------------*/
  sub_matrix[3].m[0] = m[4];
  sub_matrix[3].m[1] = m[5];
  sub_matrix[3].m[2] = m[6];
             
  sub_matrix[3].m[3] = m[8];
  sub_matrix[3].m[4] = m[9];
  sub_matrix[3].m[5] = m[10];
             
  sub_matrix[3].m[6] = m[12];
  sub_matrix[3].m[7] = m[13];
  sub_matrix[3].m[8] = m[14];

/*---------------------------*/
  sub_matrix[4].m[0] = m[1];
  sub_matrix[4].m[1] = m[2];
  sub_matrix[4].m[2] = m[3];
             
  sub_matrix[4].m[3] = m[9];
  sub_matrix[4].m[4] = m[10];
  sub_matrix[4].m[5] = m[11];
             
  sub_matrix[4].m[6] = m[13];
  sub_matrix[4].m[7] = m[14];
  sub_matrix[4].m[8] = m[15];

  /*------------------------*/
  sub_matrix[5].m[0] = m[0];
  sub_matrix[5].m[1] = m[2];
  sub_matrix[5].m[2] = m[3];
             
  sub_matrix[5].m[3] = m[8];
  sub_matrix[5].m[4] = m[10];
  sub_matrix[5].m[5] = m[11];
             
  sub_matrix[5].m[6] = m[12];
  sub_matrix[5].m[7] = m[14];
  sub_matrix[5].m[8] = m[15];

  /*------------------------*/
  sub_matrix[6].m[0] = m[0];
  sub_matrix[6].m[1] = m[1];
  sub_matrix[6].m[2] = m[3];
             
  sub_matrix[6].m[3] = m[8];
  sub_matrix[6].m[4] = m[9];
  sub_matrix[6].m[5] = m[11];
             
  sub_matrix[6].m[6] = m[12];
  sub_matrix[6].m[7] = m[13];
  sub_matrix[6].m[8] = m[15];

  /*------------------------*/
  sub_matrix[7].m[0] = m[0];
  sub_matrix[7].m[1] = m[1];
  sub_matrix[7].m[2] = m[2];
             
  sub_matrix[7].m[3] = m[8];
  sub_matrix[7].m[4] = m[9];
  sub_matrix[7].m[5] = m[10];
             
  sub_matrix[7].m[6] = m[12];
  sub_matrix[7].m[7] = m[13];
  sub_matrix[7].m[8] = m[14];

  /*-------------------------*/
  sub_matrix[8].m[0] = m[1];
  sub_matrix[8].m[1] = m[2];
  sub_matrix[8].m[2] = m[3];
             
  sub_matrix[8].m[3] = m[5];
  sub_matrix[8].m[4] = m[6];
  sub_matrix[8].m[5] = m[7];
             
  sub_matrix[8].m[6] = m[13];
  sub_matrix[8].m[7] = m[14];
  sub_matrix[8].m[8] = m[15];

  /*-------------------------*/
  sub_matrix[9].m[0] = m[0];
  sub_matrix[9].m[1] = m[2];
  sub_matrix[9].m[2] = m[3];
             
  sub_matrix[9].m[3] = m[4];
  sub_matrix[9].m[4] = m[6];
  sub_matrix[9].m[5] = m[7];
             
  sub_matrix[9].m[6] = m[12];
  sub_matrix[9].m[7] = m[14];
  sub_matrix[9].m[8] = m[15];

  /*-------------------------*/
  sub_matrix[10].m[0] = m[0];
  sub_matrix[10].m[1] = m[1];
  sub_matrix[10].m[2] = m[3];
             
  sub_matrix[10].m[3] = m[4];
  sub_matrix[10].m[4] = m[5];
  sub_matrix[10].m[5] = m[7];
             
  sub_matrix[10].m[6] = m[12];
  sub_matrix[10].m[7] = m[13];
  sub_matrix[10].m[8] = m[15];

  /*------------------------*/
  sub_matrix[11].m[0] = m[0];
  sub_matrix[11].m[1] = m[1];
  sub_matrix[11].m[2] = m[2];
              
  sub_matrix[11].m[3] = m[4];
  sub_matrix[11].m[4] = m[5];
  sub_matrix[11].m[5] = m[6];
              
  sub_matrix[11].m[6] = m[12];
  sub_matrix[11].m[7] = m[13];
  sub_matrix[11].m[8] = m[14];

  /*------------------------*/
  sub_matrix[12].m[0] = m[1];
  sub_matrix[12].m[1] = m[2];
  sub_matrix[12].m[2] = m[3];
              
  sub_matrix[12].m[3] = m[5];
  sub_matrix[12].m[4] = m[6];
  sub_matrix[12].m[5] = m[7];
              
  sub_matrix[12].m[6] = m[9];
  sub_matrix[12].m[7] = m[10];
  sub_matrix[12].m[8] = m[11];

  /*------------------------*/
  sub_matrix[13].m[0] = m[0];
  sub_matrix[13].m[1] = m[2];
  sub_matrix[13].m[2] = m[3];
              
  sub_matrix[13].m[3] = m[4];
  sub_matrix[13].m[4] = m[6];
  sub_matrix[13].m[5] = m[7];
              
  sub_matrix[13].m[6] = m[8];
  sub_matrix[13].m[7] = m[10];
  sub_matrix[13].m[8] = m[11];

  /*------------------------*/
  sub_matrix[14].m[0] = m[0];
  sub_matrix[14].m[1] = m[1];
  sub_matrix[14].m[2] = m[3];
              
  sub_matrix[14].m[3] = m[4];
  sub_matrix[14].m[4] = m[5];
  sub_matrix[14].m[5] = m[7];
              
  sub_matrix[14].m[6] = m[8];
  sub_matrix[14].m[7] = m[9];
  sub_matrix[14].m[8] = m[11];

  /*------------------------*/
  sub_matrix[15].m[0] = m[0];
  sub_matrix[15].m[1] = m[1];
  sub_matrix[15].m[2] = m[2];
              
  sub_matrix[15].m[3] = m[4];
  sub_matrix[15].m[4] = m[5];
  sub_matrix[15].m[5] = m[6];
              
  sub_matrix[15].m[6] = m[8];
  sub_matrix[15].m[7] = m[9];
  sub_matrix[15].m[8] = m[10];

  Matrix4x4 adjoint_matrix;

  adjoint_matrix.m[0]  = sub_matrix[0].Determinant();
  adjoint_matrix.m[1]  = -sub_matrix[1].Determinant();
  adjoint_matrix.m[2]  = sub_matrix[2].Determinant();
  adjoint_matrix.m[3]  = -sub_matrix[3].Determinant();
 
  adjoint_matrix.m[4]  = -sub_matrix[4].Determinant();
  adjoint_matrix.m[5]  = sub_matrix[5].Determinant();
  adjoint_matrix.m[6]  = -sub_matrix[6].Determinant();
  adjoint_matrix.m[7]  = sub_matrix[7].Determinant();

  adjoint_matrix.m[8]  = sub_matrix[8].Determinant();
  adjoint_matrix.m[9]  = -sub_matrix[9].Determinant();
  adjoint_matrix.m[10] = sub_matrix[10].Determinant();
  adjoint_matrix.m[11] = -sub_matrix[11].Determinant();

  adjoint_matrix.m[12] = -sub_matrix[12].Determinant();
  adjoint_matrix.m[13] = sub_matrix[13].Determinant();
  adjoint_matrix.m[14] = -sub_matrix[14].Determinant();
  adjoint_matrix.m[15] = sub_matrix[15].Determinant();

	return adjoint_matrix;
}

inline bool Matrix4x4::Inverse() {

  float det = Determinant();
  if (det == 0) {
    return false;
  }
  Matrix4x4 inv = Adjoint();
  inv = inv.Transpose();
  inv /= det;
  *this = inv;
  
  return true;

}

inline bool Matrix4x4::GetInverse(Matrix4x4* out) const {

  float det = Determinant();

  if (det == 0) {
    return false;
  }
  Matrix4x4 inv =Adjoint();
  inv = inv.Transpose();
  inv /= det;
  *out = inv;
 
  return true;
 
}

inline Matrix4x4 Matrix4x4::Transpose() const {

  Matrix4x4 transpose_matrix;
  
  transpose_matrix.m[0] = m[0];
  transpose_matrix.m[1] = m[4];
  transpose_matrix.m[2] = m[8];
  transpose_matrix.m[3] = m[12];

  transpose_matrix.m[4] = m[1];
  transpose_matrix.m[5] = m[5];
  transpose_matrix.m[6] = m[9];
  transpose_matrix.m[7] = m[13];
  
  transpose_matrix.m[8] = m[2];
  transpose_matrix.m[9] = m[6];
  transpose_matrix.m[10] = m[10];
  transpose_matrix.m[11] = m[14];
  
  transpose_matrix.m[12] = m[3];
  transpose_matrix.m[13] = m[7];
  transpose_matrix.m[14] = m[11];
  transpose_matrix.m[15] = m[15];

  return transpose_matrix;
	
}

inline Matrix4x4 Matrix4x4::LookAt(Vector3 eye, Vector3 target, Vector3 up) {

  Vector3 z_axis = Vector3(eye - target).Normalized();
  Vector3 x_axis = Vector3(Vector3::CrossProduct(up, z_axis)).Normalized();
  Vector3 y_axis = Vector3::CrossProduct(z_axis, x_axis);

  float view[16] = { x_axis.x, y_axis.x, z_axis.x, 0.0f,
                     x_axis.y, y_axis.y, z_axis.y, 0.0f,
                     x_axis.z, y_axis.z, z_axis.z, 0.0f,
                    -Vector3::DotProduct(x_axis, eye),
                    -Vector3::DotProduct(y_axis, eye),
                    -Vector3::DotProduct(z_axis, eye), 1.0f };

  return Matrix4x4(view);
}

inline Matrix4x4 Matrix4x4::Translate(const Vector3& distance){
	
  Matrix4x4 trasnlate_matrix;

  trasnlate_matrix.m[0] = 1.0f;
  trasnlate_matrix.m[1] = 0.0f;
  trasnlate_matrix.m[2] = 0.0f;
  trasnlate_matrix.m[3] = distance.x;

  trasnlate_matrix.m[4] = 0.0f;
  trasnlate_matrix.m[5] = 1.0f;
  trasnlate_matrix.m[6] = 0.0f;
  trasnlate_matrix.m[7] = distance.y;

  trasnlate_matrix.m[8] = 0.0f;
  trasnlate_matrix.m[9] = 0.0f;
  trasnlate_matrix.m[10] = 1.0f;
  trasnlate_matrix.m[11] = distance.z;

  trasnlate_matrix.m[12] = 0.0f;
  trasnlate_matrix.m[13] = 0.0f;
  trasnlate_matrix.m[14] = 0.0f;
  trasnlate_matrix.m[15] = 1.0f;

  return trasnlate_matrix;
}

inline Matrix4x4 Matrix4x4::Translate(float x, float y, float z){

  Matrix4x4 trasnlate_matrix;

  trasnlate_matrix.m[0] = 1.0f;
  trasnlate_matrix.m[1] = 0.0f;
  trasnlate_matrix.m[2] = 0.0f;
  trasnlate_matrix.m[3] = x;
	
  trasnlate_matrix.m[4] = 0.0f;
  trasnlate_matrix.m[5] = 1.0f;
  trasnlate_matrix.m[6] = 0.0f;
  trasnlate_matrix.m[7] = y;
 
  trasnlate_matrix.m[8] = 0.0f;
  trasnlate_matrix.m[9] = 0.0f;
  trasnlate_matrix.m[10] = 1.0f;
  trasnlate_matrix.m[11] = z;
 
  trasnlate_matrix.m[12] = 0.0f;
  trasnlate_matrix.m[13] = 0.0f;
  trasnlate_matrix.m[14] = 0.0f;
  trasnlate_matrix.m[15] = 1.0f;

	return trasnlate_matrix;
}

inline Matrix4x4 Matrix4x4::Scale(const Vector3& scale){
  
  Matrix4x4 scale_matrix;

  scale_matrix.m[0] = scale.x;
  scale_matrix.m[1] = 0.0f;
  scale_matrix.m[2] = 0.0f;
  scale_matrix.m[3] = 0.0f;
 
  scale_matrix.m[4] = 0.0f;
  scale_matrix.m[5] = scale.y;
  scale_matrix.m[6] = 0.0f;
  scale_matrix.m[7] = 0.0f;
 
  scale_matrix.m[8] = 0.0f;
  scale_matrix.m[9] = 0.0f;
  scale_matrix.m[10] = scale.z;
  scale_matrix.m[11] = 0.0f;

  scale_matrix.m[12] = 0.0f;
  scale_matrix.m[13] = 0.0f;
  scale_matrix.m[14] = 0.0f;
  scale_matrix.m[15] = 1.0f;

	return scale_matrix;
}

inline Matrix4x4 Matrix4x4::Scale(float x, float y, float z){

  Matrix4x4 scale_matrix;

  scale_matrix.m[0] = x;
  scale_matrix.m[1] = 0.0f;
  scale_matrix.m[2] = 0.0f;
  scale_matrix.m[3] = 0.0f;

  scale_matrix.m[4] = 0.0f;
  scale_matrix.m[5] = y;
  scale_matrix.m[6] = 0.0f;
  scale_matrix.m[7] = 0.0f;

  scale_matrix.m[8] = 0.0f;
  scale_matrix.m[9] = 0.0f;
  scale_matrix.m[10] = z;
  scale_matrix.m[11] = 0.0f;

  scale_matrix.m[12] = 0.0f;
  scale_matrix.m[13] = 0.0f;
  scale_matrix.m[14] = 0.0f;
  scale_matrix.m[15] = 1.0f;

  return scale_matrix;
}

inline Matrix4x4 Matrix4x4::RotateX(float radians){
  Matrix4x4 rotationX_matrix;
  float cosine = cosf(radians);
  float sine = sinf(radians);

  rotationX_matrix.m[0] = 1.0f;
  rotationX_matrix.m[1] = 0.0f;
  rotationX_matrix.m[2] = 0.0f;
  rotationX_matrix.m[3] = 0.0f;
    
  rotationX_matrix.m[4] = 0.0f;
  rotationX_matrix.m[5] = cosine;
  rotationX_matrix.m[6] = -sine;
  rotationX_matrix.m[7] = 0.0f;
    
  rotationX_matrix.m[8] = 0.0f;
  rotationX_matrix.m[9] = sine;
  rotationX_matrix.m[10] = cosine;
  rotationX_matrix.m[11] = 0.0f;
    
  rotationX_matrix.m[12] = 0.0f;
  rotationX_matrix.m[13] = 0.0f;
  rotationX_matrix.m[14] = 0.0f;
  rotationX_matrix.m[15] = 1.0f;

  return rotationX_matrix;
;
}

inline Matrix4x4 Matrix4x4::RotateY(float radians){
	
  Matrix4x4 rotationY_matrix;
  float cosine = cosf(radians);
  float sine = sinf(radians);

  rotationY_matrix.m[0] = cosine;
  rotationY_matrix.m[1] = 0.0f;
  rotationY_matrix.m[2] = sine;
  rotationY_matrix.m[3] = 0.0f;
 
  rotationY_matrix.m[4] = 0.0f;
  rotationY_matrix.m[5] = 1.0f;
  rotationY_matrix.m[6] = 0.0f;
  rotationY_matrix.m[7] = 0.0f;
 
  rotationY_matrix.m[8] = - sine;
  rotationY_matrix.m[9] = 0.0f;
  rotationY_matrix.m[10] = cosine;
  rotationY_matrix.m[11] = 0.0f;
  
  rotationY_matrix.m[12] = 0.0f;
  rotationY_matrix.m[13] = 0.0f;
  rotationY_matrix.m[14] = 0.0f;
  rotationY_matrix.m[15] = 1.0f;

  return rotationY_matrix;
}

inline Matrix4x4 Matrix4x4::RotateZ(float radians) {
	
  Matrix4x4 rotationZ_matrix;
  float cosine = cosf(radians);
  float sine = sinf(radians);

  rotationZ_matrix.m[0] = cosine;
  rotationZ_matrix.m[1] = - sine;
  rotationZ_matrix.m[2] = 0.0f;
  rotationZ_matrix.m[3] = 0.0f;
 
  rotationZ_matrix.m[4] = sine;
  rotationZ_matrix.m[5] = cosine;
  rotationZ_matrix.m[6] = 0.0f;
  rotationZ_matrix.m[7] = 0.0f;
  
  rotationZ_matrix.m[8] = 0.0f;
  rotationZ_matrix.m[9] = 0.0f;
  rotationZ_matrix.m[10] = 1.0f;
  rotationZ_matrix.m[11] = 0.0f;
 
  rotationZ_matrix.m[12] = 0.0f;
  rotationZ_matrix.m[13] = 0.0f;
  rotationZ_matrix.m[14] = 0.0f;
  rotationZ_matrix.m[15] = 1.0f;

  return rotationZ_matrix;
}

inline Matrix4x4 Matrix4x4::GetTransform(const Vector3& translate,
								const Vector3& scale,
								float rotateX, float rotateY,
								float rotateZ)   {


  return GetTransform(translate.x, translate.y, translate.z,
                      scale.x, scale.y, scale.z,
                      rotateX, rotateY, rotateZ);
  
}

inline Matrix4x4 Matrix4x4::GetTransform(float trans_x, float trans_y, float trans_z,
	float scale_x, float scale_y, float scale_z,
	float rotateX, float rotateY, float rotateZ)  {

  Matrix4x4 translate_m = Translate(trans_x, trans_y, trans_z);
  Matrix4x4 scale_m = Scale(scale_x, scale_y, scale_z);
  Matrix4x4 rotX_m = RotateX(rotateX);
  Matrix4x4 rotY_m = RotateY(rotateY);
  Matrix4x4 rotZ_m = RotateZ(rotateZ);

  
  //Matrix4x4 model = translate_m.Multiply(rotZ_m);
  Matrix4x4 model = scale_m.Multiply(rotZ_m);
  model = model.Multiply(rotX_m);
  model = model.Multiply(rotY_m);
  model = model.Multiply(translate_m);

  return model;
}

inline Vector4 Matrix4x4::GetColum(int colum) const {
  assert(colum >= 0 && colum < 4 && "colum is out of the matrix");
	return Vector4(m[0+colum*4], m[1+colum*4], m[2+colum*4], m[3+colum*4]);
}

inline Vector4 Matrix4x4::GetLine(int line) const {
  assert(line >= 0 && line < 4 && "line is out of the matrix");
	return Vector4(m[0+line], m[4+line], m[8+line], m[12+line] );
}

inline Matrix4x4 Matrix4x4::PerspectiveMatrix(float fov, float aspect,
	float near, float far) const {
  Matrix4x4 perspective_matrix;

  float angle = (fov / 180.0f) * 3.141592f;
  float f = 1.0f / tan(angle * 0.5f);

  /*matrix[0][0] = f / aspect;
matrix[1][1] = f;
matrix[2][2] = (far + near) / (near - far);
matrix[2][3] = -1.0f;
matrix[3][2] = (2.0f * far * near) / (near - far);*/
  assert(near != far && "Near and far are the same");
  assert(aspect >= 0 && "Aspect can be <= 0");
  perspective_matrix.m[0] = f / aspect;
  perspective_matrix.m[1] = 0.0f;
  perspective_matrix.m[2] = 0.0f;
  perspective_matrix.m[3] = 0.0f;
                   
  perspective_matrix.m[4] = 0.0f;
  perspective_matrix.m[5] = f;
  perspective_matrix.m[6] = 0.0f;
  perspective_matrix.m[7] = 0.0f;
              
  perspective_matrix.m[8] = 0.0f;
  perspective_matrix.m[9] = 0.0f;
  perspective_matrix.m[10] = 0.0f;
  perspective_matrix.m[11] = (2.0f * far * near) / (near - far);
         
  perspective_matrix.m[12] = 0.0f;
  perspective_matrix.m[13] = 0.0f;
  perspective_matrix.m[14] = -1.0f;
  perspective_matrix.m[15] = 0.0f;

	return perspective_matrix;
}

inline Matrix4x4 Matrix4x4::OrthoMatrix(float right, float left, float top, float bottom,
	float near, float far) const {
  
  Matrix4x4 ortho_matrix;

  assert(right != left && "Right and Left are the same");
  assert(top != bottom && "Top and Bottom are the same");
  assert(near != far   && "Near and far are the same");

  ortho_matrix.m[0] = 2.0f / (right - left);
  ortho_matrix.m[1] = 0.0f;
  ortho_matrix.m[2] = 0.0f;
  ortho_matrix.m[3] = 0.0f;

  ortho_matrix.m[4] = 0.0f;
  ortho_matrix.m[5] = 2.0f / (top - bottom);
  ortho_matrix.m[6] = 0.0f;
  ortho_matrix.m[7] = 0.0f;

  ortho_matrix.m[8] = 0.0f;
  ortho_matrix.m[9] = 0.0f;
  ortho_matrix.m[10] = -2.0f / (far - near);
  ortho_matrix.m[11] = 0.0f;

  ortho_matrix.m[12] = -1.0f * ((right + left) / (right - left));
  ortho_matrix.m[13] = -1.0f * ((top + bottom) / (top - bottom));;
  ortho_matrix.m[14] = -1.0f * ((far + near) / (far - near));;
  ortho_matrix.m[15] = 1.0f;

	return ortho_matrix;
}


inline Matrix4x4 Matrix4x4::operator+(const Matrix4x4& other) const {
  Matrix4x4 result_matrix = *this;
  result_matrix += other;

  return result_matrix;
}

inline Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& other) {

  m[0] += other.m[0];
  m[1] += other.m[1];
  m[2] += other.m[2];
  m[3] += other.m[3];
  m[4] += other.m[4];
  m[5] += other.m[5];
  m[6] += other.m[6];
  m[7] += other.m[7];
  m[8] += other.m[8];
  m[9] += other.m[9];
  m[10] += other.m[10];
  m[11] += other.m[11];
  m[12] += other.m[12];
  m[13] += other.m[13];
  m[14] += other.m[14];
  m[15] += other.m[15];
  
  return *this;
}

inline Matrix4x4 Matrix4x4::operator+(float value) const {
  Matrix4x4 result_matrix = *this;
  result_matrix += value;
 
  return result_matrix;
}

inline Matrix4x4& Matrix4x4::operator+=(float value) {	

  m[0] += value;
  m[1] += value;
  m[2] += value;
  m[3] += value;
  m[4] += value;
  m[5] += value;
  m[6] += value;
  m[7] += value;
  m[8] += value;
  m[9] += value;
  m[10] += value;
  m[11] += value;
  m[12] += value;
  m[13] += value;
  m[14] += value;
  m[15] += value;

  return *this;
}

inline Matrix4x4 Matrix4x4::operator-(const Matrix4x4& other) const  {
  Matrix4x4 result_matrix = *this;
  result_matrix -= other;

  return result_matrix;
}

inline Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& other) {
  m[0] -= other.m[0];
  m[1] -= other.m[1];
  m[2] -= other.m[2];
  m[3] -= other.m[3];
  m[4] -= other.m[4];
  m[5] -= other.m[5];
  m[6] -= other.m[6];
  m[7] -= other.m[7];
  m[8] -= other.m[8];
  m[9] -= other.m[9];
  m[10] -= other.m[10];
  m[11] -= other.m[11];
  m[12] -= other.m[12];
  m[13] -= other.m[13];
  m[14] -= other.m[14];
  m[15] -= other.m[15];

  return *this;
}

inline Matrix4x4 Matrix4x4::operator-(float value) const  {
  Matrix4x4 result_matrix = *this;
  result_matrix -= value;

  return result_matrix;
}

inline Matrix4x4& Matrix4x4::operator-=(float value) {

  m[0] -= value;
  m[1] -= value;
  m[2] -= value;
  m[3] -= value;
  m[4] -= value;
  m[5] -= value;
  m[6] -= value;
  m[7] -= value;
  m[8] -= value;
  m[9] -= value;
  m[10] -= value;
  m[11] -= value;
  m[12] -= value;
  m[13] -= value;
  m[14] -= value;
  m[15] -= value;

  return *this;
}

inline Matrix4x4& Matrix4x4::operator*=(float value) {

  m[0] *= value;
  m[1] *= value;
  m[2] *= value;
  m[3] *= value;
  m[4] *= value;
  m[5] *= value;
  m[6] *= value;
  m[7] *= value;
  m[8] *= value;
  m[9] *= value;
  m[10] *= value;
  m[11] *= value;
  m[12] *= value;
  m[13] *= value;
  m[14] *= value;
  m[15] *= value;

  return *this;
}

inline Matrix4x4 Matrix4x4::operator*(float value) const  { 
  Matrix4x4 result_matrix = *this;
  result_matrix *= value;

  return result_matrix;
}

inline Matrix4x4& Matrix4x4::operator/=(float value) {

  assert(value != 0.0f && " You can't divide by 0");
  m[0] /= value;
  m[1] /= value;
  m[2] /= value;
  m[3] /= value;
  m[4] /= value;
  m[5] /= value;
  m[6] /= value;
  m[7] /= value;
  m[8] /= value;
  m[9] /= value;
  m[10] /= value;
  m[11] /= value;
  m[12] /= value;
  m[13] /= value;
  m[14] /= value;
  m[15] /= value;

  return *this;
}

inline Matrix4x4 Matrix4x4::operator/(float value) const {

  assert(value != 0.0f && " You can't divide by 0");
  Matrix4x4 result_matrix = *this;
  result_matrix /= value;

  return result_matrix;
}

inline bool Matrix4x4::operator==(const Matrix4x4& other) {

	return (m[0] == other.m[0] && m[1] == other.m[1] &&
          m[2] == other.m[2] && m[3] == other.m[3] && 
          m[4] == other.m[4] && m[5] == other.m[5] && 
          m[6] == other.m[6] && m[7] == other.m[7] && 
          m[8] == other.m[8] && m[9] == other.m[9] && 
          m[10] == other.m[10] && m[11] == other.m[11] &&
          m[12] == other.m[12] && m[13] == other.m[13] && 
          m[14] == other.m[14] && m[15] == other.m[15]);
}

inline bool Matrix4x4::operator!=(const Matrix4x4& other) {
  return !(*this == other);
}

inline void Matrix4x4::operator=(const Matrix4x4& other) {

  m[0] = other.m[0];
  m[1] = other.m[1];
  m[2] = other.m[2];
  m[3] = other.m[3];
  m[4] = other.m[4];
  m[5] = other.m[5];
  m[6] = other.m[6];
  m[7] = other.m[7];
  m[8] = other.m[8];
  m[9] = other.m[9];
  m[10] = other.m[10];
  m[11] = other.m[11];
  m[12] = other.m[12];
  m[13] = other.m[13];
  m[14] = other.m[14];
  m[15] = other.m[15];
  
}

inline void Matrix4x4::printM4() {

  for (int i = 0; i < 4; ++i) {

    printf("%f ", m[0+i]);
    printf("%f ", m[4+i]);
    printf("%f ", m[8+i]);
    printf("%f\n", m[12+i]);
  }
  printf("\n");
}

#endif
