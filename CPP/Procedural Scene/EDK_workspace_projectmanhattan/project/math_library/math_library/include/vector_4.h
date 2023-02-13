#ifndef __Vector4_H__
#define __Vector4_H__ 1

#include "vector_3.h"
#include "matrix_3.h"
#include "assert.h"
#include "math_utils.h"

class Vector4 {
public:

	Vector4();
	Vector4(float x, float y, float z, float w);
	Vector4(Vector3 a, float w);
	Vector4(float a);
	Vector4(float* values_array);
	Vector4(const Vector4& other);
	~Vector4();
	
	Vector4 operator+(const Vector4& other) const;
	Vector4 operator+(float value) const;
	void operator+=(const Vector4& other);
	void operator+=(float value);
	Vector4 operator-(const Vector4& other) const;
	Vector4 operator-(float value) const;
	void operator -=(const Vector4& other);
	void operator -=(float value);

	Vector4 operator*(float value) const;
	void operator*=(float value);
	Vector4 operator/(float value) const;
	void operator/=(float value);
	bool operator==(const Vector4& other);
	bool operator!=(const Vector4& other);
	void operator=(const Vector4& other);

	float Magnitude() const;
	void Normalize();
	Vector4 Normalized() const;
	void Scale(Vector4 scale);
	float SqrMagnitude() const;

	static float Distance(const Vector4& a, const Vector4& b);
	static float DotProduct(Vector4 a, Vector4 b);
	static Vector4 Lerp(const Vector4& a, const Vector4& b, float index);	

	static const Vector4 one;
	static const Vector4 zero;

	float x;
	float y;
	float z;
	float w;

};

inline Vector4::Vector4() { 
}

inline Vector4::Vector4(float x, float y, float z, float w) {
  this->x = x;
  this->y = y;
  this->z = z;
  this->w = w;
}

inline Vector4::Vector4(Vector3 a, float w) {
  x = a.x;
  y = a.y;
  z = a.z;
  this->w = w;
}

inline Vector4::Vector4(float a) {
  x = a;
  y = a;
  z = a;
  w = a;
}

inline Vector4::Vector4(float* values_array) {
  x = values_array[0];
  y = values_array[1];
  z = values_array[2];
  w = values_array[3];
}

inline Vector4::Vector4(const Vector4& other) {
  x = other.x;
  y = other.y;
  z = other.z;
  w = other.w;
}

inline Vector4::~Vector4() { }

inline float Vector4::Magnitude() const{
	return float(sqrtf(SqrMagnitude()));
}

inline void Vector4::Normalize() {	
  *this = Normalized();
}

inline Vector4 Vector4::Normalized() const {
  float magnitude = Magnitude();
  assert(magnitude != 0.0f && " You can't divide by 0");
  float inverse_magnitude = 1.0f / magnitude;
  Vector4 normlaized_vector = *this * inverse_magnitude;

  return normlaized_vector;
}

inline void Vector4::Scale(Vector4 scale) {	
  x *= scale.x;
  y *= scale.y;
  z *= scale.z;
  w *= scale.w;
}

inline float Vector4::SqrMagnitude() const {
  return (DotProduct(*this, *this));
}

inline float Vector4::Distance(const Vector4& a, const Vector4& b) {
  return (b-a).Magnitude();
}

inline float Vector4::DotProduct(Vector4 a, Vector4 b) {
	return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w));
}

inline Vector4 Vector4::Lerp(const Vector4& a, const Vector4& b, float t) {	
  t = MathUtils::Clamp(t, 0.0f, 1.0f);
  Vector4 v_director = b - a;
  v_director = (v_director * t) + a;

  return v_director;
}

inline Vector4 Vector4::operator+(const Vector4& other) const{
  Vector4 result_vector = *this;
  result_vector += other;
	return result_vector;
}

inline Vector4 Vector4::operator+(float value) const{
  Vector4 result_vector = *this;
  result_vector += value;
  return result_vector;
}

inline void Vector4::operator+=(const Vector4& other){
  x += other.x;
  y += other.y;
  z += other.z;
  w += other.w;
}

inline void Vector4::operator+=(float value) {
  x += value;
  y += value;
  z += value;
  w += value;
}

inline Vector4 Vector4::operator-(const Vector4& other) const{
  Vector4 result_vector = *this;
  result_vector -= other;
  return result_vector;
}

inline Vector4 Vector4::operator-(float value) const{
  Vector4 result_vector = *this;
  result_vector -= value;
  return result_vector;
}

inline void Vector4::operator -=(const Vector4& other) {	
  x -= other.x;
  y -= other.y;
  z -= other.z;
  w -= other.w;
}

inline void Vector4::operator -=(float value) {
  x -= value;
  y -= value;
  z -= value;
  w -= value;
}

inline Vector4 Vector4::operator*(float value) const{
  Vector4 result_vector = *this;
  result_vector *= value;
  return result_vector;
}

inline void Vector4::operator*=(float value) {
  x *= value;
  y *= value;
  z *= value;
  w *= value;
}

inline Vector4 Vector4::operator/(float value) const{
  assert(value != 0.0f && "You can't divide by 0");
  
  Vector4 result_vector = *this;
  result_vector /= value;
  return result_vector;
}

inline void Vector4::operator/=(float value) {
  assert(value != 0.0f && "You can't divide by 0");
  x /= value;
  y /= value;
  z /= value;
  w /= value;
}

inline bool Vector4::operator==(const Vector4& other) {
  return (x == other.x && y == other.y && z == other.z && w == other.w);
}
inline bool Vector4::operator!=(const Vector4& other) {
  return !(*this == other);
}
inline void Vector4::operator=(const Vector4& other) {
  x = other.x;
  y = other.y;
  z = other.z;
  w = other.w;
}

#endif 
