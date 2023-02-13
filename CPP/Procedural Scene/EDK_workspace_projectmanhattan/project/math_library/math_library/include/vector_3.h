#ifndef __VECTOR3_H__
#define __VECTOR3_H__ 1

#include "math_utils.h"
#include "assert.h"
#include <stdio.h>
#include <math.h>

class Vector3 {

public:
	Vector3();
	Vector3(float value);
	Vector3(float x, float y, float z);
	Vector3(float* values_array);
	Vector3(const float* values_array);
	Vector3(const Vector3& other);
	~Vector3();

	Vector3 operator+(const Vector3& other) const;
	Vector3 operator+(float value) const;
	Vector3& operator+=(const Vector3& other);
	Vector3& operator+=(float value);
	Vector3 operator-(const Vector3& other) const;
	Vector3 operator-(float value) const;
	Vector3& operator-=(const Vector3& other);
	Vector3& operator-=(float value);
	bool operator==(const Vector3& other) const;
	bool operator!=(const Vector3& other) const;
	void operator=(const Vector3& other);
	void operator=(float value);
	Vector3 operator*(float value) const;
	Vector3& operator*=(float value);
	Vector3 operator/(float value) const;
	Vector3& operator/=(float value);

	float Magnitude() const;
	Vector3 Normalized() const;
	void Normalize();
	float SqrMagnitude() const;
	void Scale(const Vector3& other);

	static Vector3 Lerp(const Vector3& a, const Vector3& b, float t);
	static Vector3 LerpUnclamped(const Vector3& a, const Vector3& b, float t);
	static float DotProduct(const Vector3& a, const Vector3& b);
	static float Angle(const Vector3& a, const Vector3& b);
	static Vector3 CrossProduct(const Vector3& a,const Vector3& b);	
	static float Distance(const Vector3& a, const Vector3& b);
	static Vector3 Reflect(const Vector3& direction, const Vector3& normal);

	static const Vector3 up;
	static const Vector3 down;
	static const Vector3 right;
	static const Vector3 left;
	static const Vector3 forward;
	static const Vector3 back;
	static const Vector3 zero;
	static const Vector3 unit;

	float x;
	float y;
	float z;
};
inline Vector3::Vector3() {

}

inline Vector3::Vector3(float x, float y, float z) {
  this->x = x;
  this->y = y;
  this->z = z;
}

inline Vector3::Vector3(float * values_array) {
  x = *values_array;
  y = *(values_array+1);
  z = *(values_array+2);
}

inline Vector3::Vector3(const float * values_array) {
  x = *values_array;
  y = *(values_array + 1);
  z = *(values_array + 2);
}

inline Vector3::Vector3(float value) {
  x = value;
  y = value;
  z = value;
}

inline Vector3::Vector3(const Vector3& other) {
  x = other.x;
  y = other.y;
  z = other.z;
}

inline Vector3::~Vector3() {}

inline float Vector3::Magnitude() const {
  return float(sqrtf(SqrMagnitude()));
}

inline void Vector3::Normalize() {	
	*this = Normalized();
}

inline Vector3 Vector3::Normalized() const {
  float magnitude = Magnitude();
  //assert(magnitude != 0.0f && " You can't divide by 0");
  if (magnitude == 0.0) {
    return Vector3(.0f);
  }
  float inverse_magnitude = 1.0f / magnitude;
  Vector3 normlaized_vector = *this * inverse_magnitude;

  return normlaized_vector;
}

inline float Vector3::DotProduct(const Vector3& a, const Vector3& other)  {
	return ((a.x * other.x) + (a.y * other.y) + (a.z * other.z));
}

inline float Vector3::Angle(const Vector3& a, const Vector3& other)  {
  float ang = (a.DotProduct(a, other))/(a.Magnitude() * other.Magnitude());
  
  return float(acosf(ang));
}

inline Vector3 Vector3::CrossProduct(const Vector3& a, const Vector3& other)  {
	Vector3 result;
  result.x =  ((a.y * other.z) - (a.z * other.y));
  result.y =  (-(a.x * other.z) + (a.z * other.x));
  result.z =  ((a.x * other.y) - (a.y * other.x));

  return result;
}

inline float Vector3::SqrMagnitude() const {
  return DotProduct(*this, *this);
}

inline void Vector3::Scale(const Vector3& other) {	
  x *= other.x;
  y *= other.y;
  z *= other.z;
}

inline Vector3 Vector3::Lerp(const Vector3& a, const Vector3& b, float t) {
  t = MathUtils::Clamp(t, 0.0f, 1.0f); 
  return  LerpUnclamped(a, b, t);
}

inline Vector3 Vector3::LerpUnclamped(const Vector3& a, const Vector3& b, float t) {
  Vector3 v_director = b - a;
  v_director = (v_director * t) + a;
  return v_director;
}

inline float Vector3::Distance(const Vector3& a, const Vector3& b) {
  return (b-a).Magnitude();
}

inline Vector3 Vector3::Reflect(const Vector3& direction, const Vector3& normal) {
  float normal_magnitude = normal.Magnitude();
  assert(normal_magnitude != 0.0f && " You can't divide by 0");
  float x_projection = DotProduct(direction, normal) / normal_magnitude;

  Vector3 v_p_parallel = normal * x_projection;
  Vector3 v_p_orto = direction - v_p_parallel;

  Vector3 reflected =  v_p_orto - v_p_parallel;

	return reflected;
}

inline Vector3 Vector3::operator+(const Vector3& other) const {
  Vector3 result_vector = *this;
  result_vector += other;
  return result_vector;
}

inline Vector3 Vector3::operator+(float value) const {
  Vector3 result_vector = *this;
  result_vector += value;
  return result_vector;
}

inline Vector3& Vector3::operator+=(const Vector3& other) {
  x += other.x;
  y += other.y;
  z += other.z;
	return *this;
}

inline Vector3& Vector3::operator+=(float value) {
  x += value;
  y += value;
  z += value;
  return *this;
}

inline Vector3 Vector3::operator-(const Vector3& other) const {
  Vector3 result_vector = *this;
  result_vector -= other;
  return result_vector;
}

inline Vector3 Vector3::operator-(float value) const {
  Vector3 result_vector = *this;
  result_vector -= value;
  return result_vector;
}

inline Vector3& Vector3::operator-=(const Vector3& other) {
  x -= other.x;
  y -= other.y;
  z -= other.z;
	return *this;
}

inline Vector3& Vector3::operator-=(float value) {
  x -= value;
  y -= value;
  z -= value;
  return *this;
}

inline bool Vector3::operator==(const Vector3& other) const {
	return (x == other.x && y == other.y && z ==  other.z);
}

inline bool Vector3::operator!=(const Vector3& other) const {
	return !(*this == other);
}

inline void Vector3::operator=(const Vector3& other) {
  x = other.x;
  y = other.y;
  z = other.z;
}

inline void Vector3::operator=(float value) {	
  x = value;
  y = value;
  z = value;
}

inline Vector3 Vector3::operator*(float value) const {
  Vector3 result_vector = *this;
  result_vector *= value;
  return result_vector;
}

inline Vector3& Vector3::operator*=(float value) {	
  x *= value;
  y *= value;
  z *= value;

	return *this;
}

inline Vector3 Vector3::operator/(float value) const {
  assert(value != 0.0f && " You can't divide by 0");
  Vector3 result_vector = *this;
  result_vector /= value;
  return result_vector;
}

inline Vector3& Vector3::operator/=(float value) {	
  assert(value != 0.0f && " You can't divide by 0");
  x /= value;
  y /= value;
  z /= value;
  return *this;
}

#endif 
