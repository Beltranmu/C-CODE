#ifndef __MATHUTILS_H__
#define __MATHUTILS_H__ 1

class MathUtils {
	public:

		static float MathUtils::Clamp(float value, float minVal, float maxVal);
    static float MathUtils::DegreesToRadian(float angle);
	private:
		MathUtils();
		MathUtils(const MathUtils& copy);
		~MathUtils();
};
inline MathUtils::MathUtils() {}
inline MathUtils::MathUtils(const MathUtils& copy) {}
inline MathUtils::~MathUtils(){}

inline float MathUtils::DegreesToRadian(float angle) {
  return (3.1415f / 180.0f) * angle;
}

inline float MathUtils::Clamp(float value, float minVal, float maxVal) {
  if (value >= maxVal) { value = maxVal; }
  else if (value <= minVal) { value = minVal; }
	return value;
}
#endif