#pragma once

#define DELTA_T 0.025
#include <Arduino.h>
#include<move_ave\MA.h>
#include<Vector\myVector.h>

class BALL{
    public:
        BALL();
        int getBallposition();
        void print();
        void begin();
        void get_data(byte *data){
            for(int i = 0; i < 10; i++){
                data_byte[i] = data[i + 1];
            }
        }
        MA Bang_dif_;
        MA_vec B_dif_;
        MA ball_get_val_1;
        MA ball_get_val_2;
        MA ball_down[4];
        MA Far;
        float ang;
        float ang_old;
        float far;
        float Bang_dif;

        float world_far;
        Vector2D world_vec;
        Vector2D world_vec_old;

        Vector2D vec;
        Vector2D vec_down;
        Vector2D vec_old;
        Vector2D vec_dif;
        Vector2D vec_velocity;

        int flag = 1;
        int get_val;
        int ball_get;
        int get_th = 80;


        int get_1;
        int get_2;
        int down_val[4];
    private:
        byte data_byte[10];
};