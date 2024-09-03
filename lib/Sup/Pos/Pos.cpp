#include<Pos\Pos.h>

Pos::Pos(float X,float Y){
    this->x = X;
    this->y = Y;
    arg = degrees(atan2(Y,X));
    azimuth = 90 - arg;
}

Pos::Pos(float Azimuth,float Magnitude){
    this->azimuth = Azimuth;
    this->magnitude = Magnitude;
    arg = 90 - azimuth;
    x = magnitude * cos(radians(arg));
    y = magnitude * sin(radians(arg));
}

void Pos::set_coodinate(float X,float Y){
    this->x = X;
    this->y = Y;
    arg = degrees(atan2(Y,X));
    azimuth = 90 - arg;
}

void Pos::set_polar(float Azimuth,float Magnitude){
    this->azimuth = Azimuth;
    this->magnitude = Magnitude;
    arg = 90 - azimuth;
    x = magnitude * cos(radians(arg));
    y = magnitude * sin(radians(arg));
}

float Pos::return_x(){
    return x;
}

float Pos::return_y(){
    return y;
}

float Pos::return_azimuth(){
    return azimuth;
}

float Pos::return_magnitude(){
    return magnitude;
}

float Pos::return_arg(){
    return arg;
}

void Pos::print(){
    Serial.print(" x : ");
    Serial.print(x);
    Serial.print(" y : ");
    Serial.print(y);
    Serial.print(" azimuth : ");
    Serial.print(azimuth);
    Serial.print(" magnitude : ");
    Serial.print(magnitude);
    Serial.print(" arg : ");
    Serial.print(arg);
    Serial.println();
}