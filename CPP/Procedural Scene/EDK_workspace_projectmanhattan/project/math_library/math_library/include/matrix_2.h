#ifndef __MATRIX2_H__
#define __MATRIX2_H__ 1

#include "vector_2.h"
#include <stdio.h>
#include <assert.h>

class Matrix2x2 {
public:

	Matrix2x2();
	Matrix2x2(float a[4]);
	Matrix2x2(float value);
	Matrix2x2(const Vector2& a, const Vector2& b); 
	Matrix2x2(const Matrix2x2& copy);
	~Matrix2x2();
	Matrix2x2 Identity() const;
	Matrix2x2 Multiply(const Matrix2x2& other) const;
	float Determinant() const;
	Matrix2x2 Adjoint() const;
	Vector2 GetLine(int line) const;
	Vector2 GetColum(int colum) const;

	Matrix2x2 Inverse() const;
	Matrix2x2 Transpose() const;

	inline Matrix2x2 operator+(const Matrix2x2& other) const;
	inline void operator+=(const Matrix2x2& other);
	inline Matrix2x2 operator+(float value) const;
	inline void operator+=(float value);
	inline Matrix2x2 operator-(const Matrix2x2& other) const;
	inline void operator-=(const Matrix2x2& other);
	inline Matrix2x2 operator-(float value) const;
	inline void operator-=(float value);

	inline Matrix2x2 operator*(float value) const;
	inline void operator*=(float value);
	inline Matrix2x2 operator/(float value) const;
	inline void operator/=(float value);

	bool operator==(const Matrix2x2& other) const;
	bool operator!=(const Matrix2x2& other) const;
	inline void operator=(const Matrix2x2& other);

	float m[4];
};


inline Matrix2x2::Matrix2x2() {

}

inline Matrix2x2::Matrix2x2(float a[4]) {
  m[0] = a[0];
  m[1] = a[1];
  m[2] = a[2];
  m[3] = a[3];
}

inline Matrix2x2::Matrix2x2(float a) {
  m[0] = a;
  m[1] = a;
  m[2] = a;
  m[3] = a;
}

inline Matrix2x2::Matrix2x2(const Vector2& a, const Vector2& b) {
  m[0] = a.x;
  m[1] = a.y;
  m[2] = b.x;
  m[3] = b.y;
}

inline Matrix2x2::Matrix2x2(const Matrix2x2& other) {
  m[0] = other.m[0];
  m[1] = other.m[1];
  m[2] = other.m[2];
  m[3] = other.m[3];
}

inline Matrix2x2::~Matrix2x2() {

}

inline Matrix2x2 Matrix2x2::operator+(const Matrix2x2& other) const {
  Matrix2x2 result_matrix = *this;
  result_matrix += other;

  return result_matrix;
}

inline void Matrix2x2::operator+=(const Matrix2x2& other) {
  m[0] += other.m[0];
  m[1] += other.m[1];
  m[2] += other.m[2];
  m[3] += other.m[3];
  
}

inline Matrix2x2 Matrix2x2::operator+(float value) const {
  Matrix2x2 result_matrix = *this;
  result_matrix += value;

  return result_matrix;
}

inline void Matrix2x2::operator+=(float value) {
  m[0] += value;
  m[1] += value;
  m[2] += value;
  m[3] += value;
}

inline Matrix2x2 Matrix2x2::operator-(const Matrix2x2& other) const {
  Matrix2x2 result_matrix = *this;
  result_matrix -= other;

  return result_matrix;
}

inline void Matrix2x2::operator-=(const Matrix2x2& other) {
  m[0] -= other.m[0];
  m[1] -= other.m[1];
  m[2] -= other.m[2];
  m[3] -= other.m[3];
}

inline Matrix2x2 Matrix2x2::operator-(float value) const {	
  Matrix2x2 result_matrix = *this;
  result_matrix -= value;

  return result_matrix;
}

inline void Matrix2x2::operator-=(float value) {
  m[0] -= value;
  m[1] -= value;
  m[2] -= value;
  m[3] -= value;
}


inline Matrix2x2 Matrix2x2::operator*(float value) const {
  Matrix2x2 result_matrix = *this;
  result_matrix *= value;

  return result_matrix;
}

inline void Matrix2x2::operator*=(float value) {
  m[0] *= value;
  m[1] *= value;
  m[2] *= value;
  m[3] *= value;
}

inline Matrix2x2 Matrix2x2::operator/(float value) const {

  assert(value != 0.0f && " You can't divide by 0");
  
  Matrix2x2 result_matrix = *this;
  result_matrix /= value;

  return result_matrix;
}

inline void Matrix2x2::operator/=(float value) {

  assert(value != 0.0f && " You can't divide by 0");
  m[0] /= value;
  m[1] /= value;
  m[2] /= value;
  m[3] /= value;
}


inline bool Matrix2x2::operator==(const Matrix2x2& other) const {
  return (m[0] == other.m[0] && m[1] == other.m[1] &&
          m[2] == other.m[2] && m[3] == other.m[3]);
}

inline bool Matrix2x2::operator!=(const Matrix2x2& other) const {
  return !(*this == other);
}

inline void Matrix2x2::operator=(const Matrix2x2& other) {
  m[0] = other.m[0];
  m[1] = other.m[1];
  m[2] = other.m[2];
  m[3] = other.m[3];
}

inline Matrix2x2 Matrix2x2::Identity() const {
  Matrix2x2 result_matrix;

  result_matrix.m[0] = 1.0f;
  result_matrix.m[1] = 0.0f;
  result_matrix.m[2] = 0.0f;
  result_matrix.m[3] = 1.0f;

  return Matrix2x2(result_matrix);
}

inline float Matrix2x2::Determinant() const {
	return ((m[0] * m[3]) - (m[1] * m[2]));
}

inline Matrix2x2 Matrix2x2::Inverse() const {

  float det = Determinant();
  if (det == 0.0f) {
    return Matrix2x2(0.0f);
  }

  Matrix2x2 inv = Adjoint();
  inv = inv.Transpose();
  inv /= det;
	return inv;
}

inline Matrix2x2 Matrix2x2::Multiply(const Matrix2x2& other) const {

  Matrix2x2 result_matrix;
  result_matrix.m[0] = m[0] * other.m[0] + m[2] * other.m[1];
  result_matrix.m[1] = m[1] * other.m[0] + m[3] * other.m[1];
  result_matrix.m[2] = m[0] * other.m[2] + m[2] * other.m[3];
  result_matrix.m[3] = m[1] * other.m[2] + m[3] * other.m[3];

  return result_matrix;
}

inline Matrix2x2 Matrix2x2::Adjoint() const {

  Matrix2x2 result_matrix;
  result_matrix.m[0] =  m[3];
  result_matrix.m[1] = -m[2];
  result_matrix.m[2] = -m[1];
  result_matrix.m[3] =  m[0];

  return result_matrix;
}

inline Matrix2x2 Matrix2x2::Transpose() const {
  Matrix2x2 result_matrix;
  result_matrix.m[0] = m[0];
  result_matrix.m[1] = m[2];
  result_matrix.m[2] = m[1];
  result_matrix.m[3] = m[3];

  return result_matrix;
}

inline Vector2 Matrix2x2::GetLine(int line) const {
  assert(line >= 0 && line < 2 && "line is out of the matrix");
	return Vector2(m[0+line],m[2+line]);
}

inline Vector2 Matrix2x2::GetColum(int colum) const {	
  assert(colum >= 0 && colum < 2 && "colum is out of the matrix");
	return Vector2(m[0 + colum * 2],m[1 + colum * 2 ]);
}


#endif
