#pragma once

#define DELTA_T 0.025
#include <Arduino.h>
#include<move_ave\MA.h>
#include<Vector\myVector.h>

class BALL{
    public:
        MA ball_x;
        MA ball_y;
        MA Bang_dif_;
        MA_vec B_dif_;
        MA ball_get_val_1;
        MA ball_get_val_2;
        float ang;
        float ang_old;
        float far;
        float far_old = 0;
        float x_pos;
        float y_pos;
        float Bang_dif;

        float world_far;
        Vector2D world_vec;
        Vector2D world_vec_old;

        Vector2D vec;
        Vector2D vec_old;
        Vector2D vec_dif;
        Vector2D vec_velocity;

        int flag = 1;
        float dx;
        float far_;
        int get_val;
        int ball_get;
        int get_th = 80;
        BALL();
        int getBallposition();
        void print();
        void begin();
        void get_resister_1(int);
        void get_resister_2(int);
        int get_1;
        int get_2;
};