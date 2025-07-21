#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>

/**
 * @brief Simple struct that represents a 2D vector.
 * 
 * Has two float values x and y.
 */
struct Vector2 {
    float x = 0, y = 0;

    Vector2 operator*(float scalar) const {
        return {x * scalar, y * scalar};
    }

    Vector2 operator/(float scalar) const {
        return {x / scalar, y / scalar};
    }

    float length() const {
        return std::sqrt(x * x + y * y);
    }

    Vector2 normalized() const {
        float len = length();
        return len > 0 ? *this / len : Vector2{0.0f, 0.0f};
    }

};

inline Vector2 rotate(const Vector2& vec, float radians) {
    float cosA = std::cos(radians);
    float sinA = std::sin(radians);
    return {
        vec.x * cosA - vec.y * sinA,
        vec.x * sinA + vec.y * cosA
    };
}

#endif //VECTOR2_H