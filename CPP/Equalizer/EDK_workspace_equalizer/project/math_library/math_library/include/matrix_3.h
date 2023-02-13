#ifndef __MATRIX3_H__
#define __MATRIX3_H__ 1

#include "vector_2.h"
#include "vector_3.h"

class Matrix3x3 {
public:

	Matrix3x3();
	Matrix3x3(float *values_array);
	Matrix3x3(const float value);
	Matrix3x3(Vector3 a, Vector3 b, Vector3 c);

	Matrix3x3(const Matrix3x3& copy);
	~Matrix3x3();

	static Matrix3x3 Identity();

	Matrix3x3 Multiply(const Matrix3x3& other) const;

	float Determinant() const;

	Matrix3x3 Adjoint() const;
	bool GetInverse(Matrix3x3& out) const;
	bool Inverse();

	Matrix3x3 Transpose() const;

	static Matrix3x3 Translate(const Vector2& position);
	static Matrix3x3 Translate(float x, float y);

	Vector3 GetColum(int colum) const;
	Vector3 GetLine(int line) const;

	inline Matrix3x3 operator+(const Matrix3x3& other) const;
	inline Matrix3x3& operator+=(const Matrix3x3& other);
	inline Matrix3x3 operator+(float value) const;
	inline Matrix3x3& operator+=(float value);
	inline Matrix3x3 operator-(const Matrix3x3& other) const;
	inline Matrix3x3& operator-=(const Matrix3x3& other);
	inline Matrix3x3 operator-(float value) const;
	inline Matrix3x3& operator-=(float value);
	inline Matrix3x3 operator*(float value) const;
	inline Matrix3x3& operator*=(float value);
	inline Matrix3x3 operator/(float value) const;
	inline Matrix3x3& operator/=(float value);
	bool operator==(const Matrix3x3& other) const;
	bool operator!=(const Matrix3x3& other) const;
	inline void operator=(const Matrix3x3& other);

	float m[9];
};


inline Matrix3x3::Matrix3x3() {

}

inline Matrix3x3::Matrix3x3(float value) {
  m[0] = value;
  m[1] = value;
  m[2] = value;
  m[3] = value;
  m[4] = value;
  m[5] = value;
  m[6] = value;
  m[7] = value;
  m[8] = value;
}

inline Matrix3x3::Matrix3x3(float *values_array) {
  m[0] = values_array[0];
  m[1] = values_array[1];
  m[2] = values_array[2];
  m[3] = values_array[3];
  m[4] = values_array[4];
  m[5] = values_array[5];
  m[6] = values_array[6];
  m[7] = values_array[7];
  m[8] = values_array[8];
}

inline Matrix3x3::Matrix3x3(Vector3 a, Vector3 b, Vector3 c) {
  m[0] = a.x;
  m[1] = a.y;
  m[2] = a.z;

  m[3] = b.x;
  m[4] = b.y;
  m[5] = b.z;

  m[6] = c.x;
  m[7] = c.y;
  m[8] = c.z;
}

inline Matrix3x3::Matrix3x3(const Matrix3x3& other) {
  m[0] = other.m[0];
  m[1] = other.m[1];
  m[2] = other.m[2];
  m[3] = other.m[3];
  m[4] = other.m[4];
  m[5] = other.m[5];
  m[6] = other.m[6];
  m[7] = other.m[7];
  m[8] = other.m[8];
}

inline Matrix3x3::~Matrix3x3() {
}

inline Matrix3x3 Matrix3x3::operator+(const Matrix3x3& other) const {
  Matrix3x3 result_matrix = *this;
  result_matrix += other;
  return result_matrix;
}

inline Matrix3x3& Matrix3x3::operator+=(const Matrix3x3& other) {
  m[0] += other.m[0];
  m[1] += other.m[1];
  m[2] += other.m[2];
  m[3] += other.m[3];
  m[4] += other.m[4];
  m[5] += other.m[5];
  m[6] += other.m[6];
  m[7] += other.m[7];
  m[8] += other.m[8];
  
  return *this;
}

inline Matrix3x3 Matrix3x3::operator+(float value) const {
  Matrix3x3 result_matrix = *this;
  result_matrix += value;
  return result_matrix;
}

inline Matrix3x3& Matrix3x3::operator+=(float value) {
  m[0] += value;
  m[1] += value;
  m[2] += value;
  m[3] += value;
  m[4] += value;
  m[5] += value;
  m[6] += value;
  m[7] += value;
  m[8] += value;

  return *this;
}

inline Matrix3x3 Matrix3x3::operator-(const Matrix3x3& other) const {
  Matrix3x3 result_matrix = *this;
  result_matrix -= other;
  return result_matrix;
}

inline Matrix3x3& Matrix3x3::operator-=(const Matrix3x3& other) {
  m[0] -= other.m[0];
  m[1] -= other.m[1];
  m[2] -= other.m[2];
  m[3] -= other.m[3];
  m[4] -= other.m[4];
  m[5] -= other.m[5];
  m[6] -= other.m[6];
  m[7] -= other.m[7];
  m[8] -= other.m[8];

  return *this;
}

inline Matrix3x3 Matrix3x3::operator-(float value) const {
  Matrix3x3 result_matrix = *this;
  result_matrix -= value;
  return result_matrix;;
}

inline Matrix3x3& Matrix3x3::operator-=(float value) {
  m[0] -= value;
  m[1] -= value;
  m[2] -= value;
  m[3] -= value;
  m[4] -= value;
  m[5] -= value;
  m[6] -= value;
  m[7] -= value;
  m[8] -= value;

  return *this;
}

inline Matrix3x3 Matrix3x3::operator*(float value) const {
  Matrix3x3 result_matrix = *this;
  result_matrix *= value;
  return result_matrix;
}

inline Matrix3x3& Matrix3x3::operator*=(float value) {
  m[0] *= value;
  m[1] *= value;
  m[2] *= value;
  m[3] *= value;
  m[4] *= value;
  m[5] *= value;
  m[6] *= value;
  m[7] *= value;
  m[8] *= value;

  return *this;
}

inline Matrix3x3 Matrix3x3::operator/(float value) const {
  assert(value != 0.0f && " You can't divide by 0");
  Matrix3x3 result_matrix = *this;
  result_matrix /= value;
  return result_matrix;
}

inline Matrix3x3& Matrix3x3::operator/=(float value) {
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

  return *this;
}

inline bool Matrix3x3::operator==(const Matrix3x3& other) const {
  return (m[0] == other.m[0] && m[1] == other.m[1] && m[2] == other.m[2] &&
          m[3] == other.m[3] && m[4] == other.m[4] && m[5] == other.m[5] &&
          m[6] == other.m[6] && m[7] == other.m[7] && m[8] == other.m[8]);

}

inline bool Matrix3x3::operator!=(const Matrix3x3& other) const {
  return !(*this ==  other);
}

inline void Matrix3x3::operator=(const Matrix3x3& other) {
  m[0] = other.m[0];
  m[1] = other.m[1];
  m[2] = other.m[2];
  m[3] = other.m[3];
  m[4] = other.m[4];
  m[5] = other.m[5];
  m[6] = other.m[6];
  m[7] = other.m[7];
  m[8] = other.m[8];

}

inline Matrix3x3 Matrix3x3::Identity(){
  Matrix3x3 identity_matrix;
  
  identity_matrix.m[0] = 1.0f;
  identity_matrix.m[1] = 0.0f;
  identity_matrix.m[2] = 0.0f;

  identity_matrix.m[3] = 0.0f;
  identity_matrix.m[4] = 1.0f;
  identity_matrix.m[5] = 0.0f;

  identity_matrix.m[6] = 0.0f;
  identity_matrix.m[7] = 0.0f;
  identity_matrix.m[8] = 1.0f;

  return identity_matrix;
}

inline float Matrix3x3::Determinant() const {
  return ((m[0] * m[4] * m[8]) + (m[3] * m[7] * m[2]) + (m[1] * m[5] * m[6]))-
         ((m[2] * m[4] * m[6]) + (m[1] * m[3] * m[8]) + (m[0] * m[5] * m[7]));
}

inline bool Matrix3x3::GetInverse(Matrix3x3& out) const {
  float det = Determinant();
  if (det == 0) {
    return false;
  }

  Matrix3x3 inv = Adjoint();
  inv = inv.Transpose();
  inv /= det;
  out = inv;

  return true;
}

inline bool Matrix3x3::Inverse() {
  float det = Determinant();
  if (det == 0) {
    return false;
  }

  Matrix3x3 inv = Adjoint();
  inv = inv.Transpose();
  inv /= det;
  *this = inv;
 
  return true;
}

inline Matrix3x3 Matrix3x3::Translate(const Vector2& mov_vector) {	
  Matrix3x3 translate_matrix;

  translate_matrix.m[0] = 1;
  translate_matrix.m[1] = 0;
  translate_matrix.m[2] = mov_vector.x;
                  
  translate_matrix.m[3] = 0;
  translate_matrix.m[4] = 1;
  translate_matrix.m[5] = mov_vector.y;
                  
  translate_matrix.m[6] = 0;
  translate_matrix.m[7] = 0;
  translate_matrix.m[8] = 1;

	return translate_matrix;
}

inline Matrix3x3 Matrix3x3::Translate(float x, float y) {
  Matrix3x3 translate_matrix;

  translate_matrix.m[0] = 1;
  translate_matrix.m[1] = 0;
  translate_matrix.m[2] = x;
  
  translate_matrix.m[3] = 0;
  translate_matrix.m[4] = 1;
  translate_matrix.m[5] = y;
  
  translate_matrix.m[6] = 0;
  translate_matrix.m[7] = 0;
  translate_matrix.m[8] = 1;

	return translate_matrix;
}

inline Matrix3x3 Matrix3x3::Multiply(const Matrix3x3& other) const {
  Matrix3x3 result_matrix;

  result_matrix.m[0] = ((m[0] * other.m[0]) + (m[3] * other.m[1]) + (m[6] * other.m[2]));
  result_matrix.m[1] = ((m[1] * other.m[0]) + (m[4] * other.m[1]) + (m[7] * other.m[2]));
  result_matrix.m[2] = ((m[2] * other.m[0]) + (m[5] * other.m[1]) + (m[8] * other.m[2]));

  result_matrix.m[3] = ((m[0] * other.m[3]) + (m[3] * other.m[4]) + (m[6] * other.m[5]));
  result_matrix.m[4] = ((m[1] * other.m[3]) + (m[4] * other.m[4]) + (m[7] * other.m[5]));
  result_matrix.m[5] = ((m[2] * other.m[3]) + (m[5] * other.m[4]) + (m[8] * other.m[5]));
 
  result_matrix.m[6] = ((m[0] * other.m[6]) + (m[3] * other.m[7]) + (m[6] * other.m[8]));
  result_matrix.m[7] = ((m[1] * other.m[6]) + (m[4] * other.m[7]) + (m[7] * other.m[8]));
  result_matrix.m[8] = ((m[2] * other.m[6]) + (m[5] * other.m[7]) + (m[8] * other.m[8]));

	return result_matrix;
}

inline Matrix3x3 Matrix3x3::Adjoint() const {
  Matrix3x3 adjoint_matrix;
  adjoint_matrix.m[0] = ( (m[4] * m[8]) - (m[5] * m[7]));
  adjoint_matrix.m[1] = (-(m[3] * m[8]) + (m[5] * m[6]));
  adjoint_matrix.m[2] = ( (m[3] * m[7]) - (m[4] * m[6]));
  
  adjoint_matrix.m[3] = (-(m[1] * m[8]) + (m[2] * m[7]));
  adjoint_matrix.m[4] = ( (m[0] * m[8]) - (m[2] * m[6]));
  adjoint_matrix.m[5] = (-(m[0] * m[7]) + (m[1] * m[6]));
  
  adjoint_matrix.m[6] = ( (m[1] * m[5]) - (m[2] * m[4]));
  adjoint_matrix.m[7] = (-(m[0] * m[5]) + (m[2] * m[3]));
  adjoint_matrix.m[8] = ( (m[0] * m[4]) - (m[1] * m[3]));

	return adjoint_matrix;
}

inline Matrix3x3 Matrix3x3::Transpose() const {
  Matrix3x3 transpose_matrix; 
  
  transpose_matrix.m[0] = m[0];
  transpose_matrix.m[1] = m[3];
  transpose_matrix.m[2] = m[6];

  transpose_matrix.m[3] = m[1];
  transpose_matrix.m[4] = m[4];
  transpose_matrix.m[5] = m[7];
  
  transpose_matrix.m[6] = m[2];
  transpose_matrix.m[7] = m[5];
  transpose_matrix.m[8] = m[8];

	return transpose_matrix;
}

inline Vector3 Matrix3x3::GetColum(int colum) const {
  assert(colum >= 0 && colum < 3 && "colum is out of the matrix");
	return Vector3(m[0 + 3*colum], m[1 + 3*colum], m[2 + 3*colum]);
}

inline Vector3 Matrix3x3::GetLine(int line) const {
  assert(line >= 0 && line < 3 && "line is out of the matrix");
  return Vector3(m[0 + line], m[3 + line], m[6 + line]);
}

#endif 