#include<Vector\myVector.h>

// Default constructor

// Parameterized constructor

// Destructor
Vector2D::~Vector2D() {}

void Vector2D::setPosition(Pos pos){
    x = pos.return_x();
    y = pos.return_y();
    arg = pos.return_arg();
    azimuth = pos.return_azimuth();
    magnitude = pos.return_magnitude();
}

// Addition of two vectors
Vector2D Vector2D::add(const Vector2D& other) const {
    return Vector2D(x + other.x, y + other.y);
}

// Subtraction of two vectors
Vector2D Vector2D::subtract(const Vector2D& other) const {
    return Vector2D(x - other.x, y - other.y);
}

// Dot product of two vectors
double Vector2D::dotProduct(const Vector2D& other) const {
    return x * other.x + y * other.y;
}

// Normalization of the vector
Vector2D Vector2D::normalize() const{
    double mag = magnitude;
    return Vector2D(x / mag, y / mag);
}

// Pseudo differentiation
Vector2D Vector2D::differentiate(const Vector2D& previous) const {
    double dx = x - previous.x;
    double dy = y - previous.y;
    return Vector2D(dx, dy);
}



void Vector2D::print() {
    Serial.printf(" (θ,r) = (%.2f,%.2f)", degrees(return_arg()),return_magnitude());
}



double operator*(const Vector2D& v1, const Vector2D& v2) {  // Dot product
    return v1.dotProduct(v2);
}

Vector2D operator*(Vector2D& v1, double scalar) {  // Scalar multiplication
    return Vector2D(v1.return_x() * scalar, v1.return_y() * scalar);
}

Vector2D operator*(double scalar, Vector2D& v1) {  // Scalar multiplication
    return Vector2D(v1.return_x() * scalar, v1.return_y() * scalar);
}

Vector2D operator+(const Vector2D& v1, const Vector2D& v2) {  // Vector addition
    return v1.add(v2);
}

Vector2D operator-(const Vector2D& v1, const Vector2D& v2) {  // Vector subtraction
    return v1.subtract(v2);
}

Vector2D operator/(const Vector2D& v1, double scalar){
    return v1 * (1 / scalar);
}