/**
 * @brief 2次元ベクトルを表すクラスです。
 * 
 * このクラスは、2次元ベクトルの操作を行うための機能を提供します。
 * ベクトルの加算、減算、内積の計算、ベクトルの大きさの計算、ベクトルの正規化、
 * および疑似微分の計算が可能です。
 */

#pragma once

#include<Arduino.h>

// Getter for angle in radians

class Vector2D {
public:
    // Constructors
    Vector2D();
    Vector2D(double x, double y);

    // Destructor
    ~Vector2D();

    // Getters and setters
    double getX() const;
    double getY() const;
    void set(double x,double y);
    void setX(double x);
    void setY(double y);
    void print();

    // Vector operations
    Vector2D add(const Vector2D& other) const;
    Vector2D subtract(const Vector2D& other) const;
    double dotProduct(const Vector2D& other) const;
    double magnitude() const;
    Vector2D normalize() const;

    // Pseudo differentiation
    Vector2D differentiate(const Vector2D& previous) const;

    // Polar coordinates
    double getAngle() const;
    double getMagnitude() const;
    void setPolarCoordinates(double angle, double magnitude);

private:
    double m_x;
    double m_y;
    double m_angle;
    double m_magnitude;
};

double operator*(const Vector2D& v1, const Vector2D& v2);
Vector2D operator*(const Vector2D& v1, double scalar);
Vector2D operator*(double scalar, const Vector2D& v1);
Vector2D operator+(const Vector2D& v1, const Vector2D& v2);
Vector2D operator-(const Vector2D& v1, const Vector2D& v2);