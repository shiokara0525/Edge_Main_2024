#include<Vector\myVector.h>

// Default constructor
Vector2D::Vector2D() : m_x(0.0), m_y(0.0) {}

// Parameterized constructor
Vector2D::Vector2D(double x, double y) : m_x(x), m_y(y) {
    m_angle = atan2(y, x);
    m_magnitude = sqrt(x * x + y * y);
}

// Destructor
Vector2D::~Vector2D() {}

// Getter for x coordinate
double Vector2D::getX() const {
    return m_x;
}

// Getter for y coordinate
double Vector2D::getY() const {
    return m_y;
}

// Setter for x coordinate
void Vector2D::setX(double x) {
    m_x = x;
    m_angle = atan2(m_y, x);
    m_magnitude = sqrt(x * x + m_y * m_y);
}

// Setter for y coordinate
void Vector2D::setY(double y) {
    m_y = y;
    m_angle = atan2(y, m_x);
    m_magnitude = sqrt(m_x * m_x + y * y);
}


void Vector2D::set(double x,double y){
    m_x = x;
    m_y = y;
    m_angle = atan2(y,x);
    m_magnitude = sqrt(x * x + y * y);
}


// Addition of two vectors
Vector2D Vector2D::add(const Vector2D& other) const {
    return Vector2D(m_x + other.m_x, m_y + other.m_y);
}

// Subtraction of two vectors
Vector2D Vector2D::subtract(const Vector2D& other) const {
    return Vector2D(m_x - other.m_x, m_y - other.m_y);
}

// Dot product of two vectors
double Vector2D::dotProduct(const Vector2D& other) const {
    return m_x * other.m_x + m_y * other.m_y;
}

// Magnitude of the vector
double Vector2D::magnitude() const {
    return sqrt(m_x * m_x + m_y * m_y);
}

// Normalization of the vector
Vector2D Vector2D::normalize() const {
    double mag = magnitude();
    return Vector2D(m_x / mag, m_y / mag);
}

// Pseudo differentiation
Vector2D Vector2D::differentiate(const Vector2D& previous) const {
    double dx = m_x - previous.m_x;
    double dy = m_y - previous.m_y;
    return Vector2D(dx, dy);
}


double Vector2D::getAngle() const {
    return atan2(m_y, m_x);
}

// Getter for magnitude
double Vector2D::getMagnitude() const {
    return magnitude();
}

// Setter for polar coordinates
void Vector2D::setPolarCoordinates(double angle, double magnitude) {
    m_x = magnitude * cos(angle);
    m_y = magnitude * sin(angle);
}



void Vector2D::print() {
    Serial.printf(" (θ,r) = (%.2f,%.2f)", degrees(getAngle()),getMagnitude());
}



double operator*(const Vector2D& v1, const Vector2D& v2) {  // Dot product
    return v1.dotProduct(v2);
}

Vector2D operator*(const Vector2D& v1, double scalar) {  // Scalar multiplication
    return Vector2D(v1.getX() * scalar, v1.getY() * scalar);
}

Vector2D operator*(double scalar, const Vector2D& v1) {  // Scalar multiplication
    return Vector2D(v1.getX() * scalar, v1.getY() * scalar);
}

Vector2D operator+(const Vector2D& v1, const Vector2D& v2) {  // Vector addition
    return v1.add(v2);
}

Vector2D operator-(const Vector2D& v1, const Vector2D& v2) {  // Vector subtraction
    return v1.subtract(v2);
}