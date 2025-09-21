#pragma once

#include <cmath>

class Vector2 {
public:
    float x, y;

    Vector2() : x(0), y(0) {}
    Vector2(float x, float y) : x(x), y(y) {}

    // Opérateurs
    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    Vector2 operator/(float scalar) const {
        return Vector2(x / scalar, y / scalar);
    }

    Vector2& operator+=(const Vector2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2& operator-=(const Vector2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    // Fonctions utilitaires
    float length() const {
        return std::sqrt(x * x + y * y);
    }

    float lengthSquared() const {
        return x * x + y * y;
    }

    Vector2 normalized() const {
        float len = length();
        if (len > 0) {
            return Vector2(x / len, y / len);
        }
        return Vector2(0, 0);
    }

    void normalize() {
        float len = length();
        if (len > 0) {
            x /= len;
            y /= len;
        }
    }

    float distance(const Vector2& other) const {
        return (*this - other).length();
    }

    float dot(const Vector2& other) const {
        return x * other.x + y * other.y;
    }

    // Interpolation linéaire
    static Vector2 lerp(const Vector2& a, const Vector2& b, float t) {
        return a + (b - a) * t;
    }
};
