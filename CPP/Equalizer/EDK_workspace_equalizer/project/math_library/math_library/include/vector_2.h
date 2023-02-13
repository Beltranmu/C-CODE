#ifndef __VECTOR2_H__
#define __VECTOR2_H__ 1

#include <math.h>
#include <assert.h>
#include "math_utils.h"

class Vector2 {
 public:

  Vector2();
  Vector2(float x, float y);
  Vector2(const Vector2& copy);
  ~Vector2();

  Vector2 operator+(const Vector2& other) const;
  Vector2 operator+(float value);
  Vector2& operator+=(const Vector2& other);
  Vector2& operator+=(float value);
  Vector2 operator-(const Vector2& other) const;
  Vector2 operator-(float value) const;
  Vector2& operator-();
  Vector2& operator-=(const Vector2& other);
  Vector2& operator-=(float value);
  bool operator==(const Vector2& other) const;
  bool operator!=(const Vector2& other) const;
  void operator=(const Vector2& other);
  void operator=(float value);
  Vector2 operator*(float value) const;
  Vector2& operator*=(float value);
  Vector2 operator/(float value) const;
  Vector2& operator/=(float value);

  float Magnitude() const;
  void Normalize();
  Vector2 Normalized() const;

  void Scale(const Vector2 scale);

  float SqrMagnitude() const;
  static float Distance(const Vector2 a, const Vector2 b);

  static float DotProduct(Vector2 a, Vector2 b);

  static Vector2 Lerp(const Vector2 a, const Vector2 b, float t);
  static Vector2 LerpUnclamped(const Vector2 a, const Vector2 b, float t);

  static const Vector2 up;
  static const Vector2 down;
  static const Vector2 right;
  static const Vector2 left;
  static const Vector2 zero;
  static const Vector2 one;

  float x;
  float y;
};

inline Vector2::Vector2() {

}

inline Vector2::Vector2(float x, float y) {
	this->x = x;
	this->y = y;
}

inline Vector2::Vector2(const Vector2& other) {
  x = other.x;
  y = other.y;
}

inline Vector2::~Vector2() {}

inline Vector2 Vector2::operator+(const Vector2& other) const {
  Vector2 result = *this;
  result += other;
  return result;
}

inline Vector2 Vector2::operator+(float value) {
  Vector2 result = *this;
  result += value;
  return result;
}

inline Vector2& Vector2::operator+=(const Vector2& other){
  x += other.x;
  y += other.y;
  return *this;
}

inline Vector2& Vector2::operator+=(float value){
  x += value;
  y += value;
  return *this;
}

inline Vector2 Vector2::operator-(const Vector2& other) const {
  Vector2 result = *this;
  result -= other;
  return result;
}

inline Vector2 Vector2::operator-(float value) const {
  Vector2 result = *this;
  result -= value;
  return result;
}

inline Vector2& Vector2::operator-() {
  return *this * (-1.0f);
}

inline Vector2& Vector2::operator-=(const Vector2& other) {
  x -= other.x;
  y -= other.y;
  return *this;
}

inline Vector2& Vector2::operator-=(float value){
  x -= value;
  y -= value;
  return *this;
}

inline bool Vector2::operator==(const Vector2& value) const {
  return (x == value.x && y == value.y);
}

inline bool Vector2::operator!=(const Vector2& value) const {
  return !(*this == value);
}

inline void Vector2::operator=(const Vector2& other) {
  x = other.x;
  y = other.y;
}

inline void Vector2::operator=(float value) {
  x = value;
  y = value;
}

inline Vector2 Vector2::operator*(float value) const {
  Vector2 result = *this;
  result *= value;
  return result;
}

inline Vector2& Vector2::operator*=(float value) {  
  x *= value;
  y *= value;
  return *this;
}

inline Vector2 Vector2::operator/(float value) const {
  assert(value != 0.0f && " You can't divide by 0");
  Vector2 result = *this;
  result /= value;
  return result;
}

inline Vector2& Vector2::operator/=(float value) {
  assert(value != 0.0f && " You can't divide by 0");
  x /= value;
  y /= value;

  return *this;
}

inline float Vector2::Magnitude() const {
  return float(sqrtf(SqrMagnitude()));
}

inline void Vector2::Normalize() {
  *this = Normalized();
}

inline Vector2 Vector2::Normalized() const {
  float magnitude = Magnitude();
  assert(magnitude != 0.0f && " You can't divide by 0");
  float inverse_magnitude = 1.0f / magnitude;
  Vector2 normalized_vector = *this * inverse_magnitude;

  return normalized_vector;
}

inline void Vector2::Scale(const Vector2 scale){
  x *= scale.x;
  y *= scale.y;
}

inline float Vector2::SqrMagnitude() const {
  return (DotProduct(*this, *this));
}

inline float Vector2::Distance(const Vector2 a, const Vector2 b) {
  return (b-a).Magnitude();
}

inline Vector2 Vector2::Lerp(const Vector2 a, const Vector2 b, float t) {
  t = MathUtils::Clamp(t, 0.0f, 1.0f);
  return LerpUnclamped(a, b, t);
}

inline Vector2 Vector2::LerpUnclamped(const Vector2 a, const Vector2 b, float t) {
  Vector2 v_director = b - a;
  v_director = v_director * t  + a;
  return v_director;
}

inline float Vector2::DotProduct(Vector2 a, Vector2 b) {
  return ((a.x * b.x) + (a.y * b.y));
}

#endif 
