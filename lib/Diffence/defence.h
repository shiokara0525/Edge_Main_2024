#pragma once

#include<Arduino.h>
#include<Kicker\kicker.h>
#include<motor_a\motor_a.h>
#include<line_include\line.h>
#include<Ball_senser\ball.h>
#include<AC\ac.h>
#include<Cam\Cam.h>

extern AC ac;
extern Cam cam_front;
extern Cam cam_back;
extern BALL ball;
extern LINE line;
extern motor_attack MOTOR;
extern Kicker kicker;

extern int ac_tirget;
extern int color;
extern int val_max;

class Diffence{
    private:
        timer Timer;
        int M_time;
        int GOD = 0;

        int Lside_A = 0;
        int Lside_B = 999;
        timer L_;
        timer sentor_t;
        timer A_12_t;
        int sentor_A = 0;
        int sentor_B = 999;
        int go_flag = 0;

        int Bget_B = 999;

        int A = 0;
        int B = 999;
        int c = 0;
        int stop_range = 10;
        int P_range = 30;
        const int far_th = 130;
        int go_val = 220;
        int goal_color;
        int print_flag = 1;// 1だったらシリアルプリントする
        int line_F = 0;
        int back_Flag = 0;
        int goang_old = 0;
        int push_flag = 0;
        int M_F = 0;

        int CB_old = 999;
        timer CB_t;
    public:
        void defence();
        void available_set();
};