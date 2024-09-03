#pragma once

#include<Arduino.h>

class Pos{
    private:
        float x;
        float y;
        float azimuth;
        float arg;
        float magnitude;
    public:
        Pos(float X,float Y);
        Pos(float Azimuth,float Magnitude);
        void set_coodinate(float x,float y);
        void set_polar(float azimuth,float magnitude);
        float return_x();
        float return_y();
        float return_azimuth();
        float return_magnitude();
        float return_arg();
        void print();
};