#pragma once

#include<Arduino.h>
#include<Kicker\kicker.h>
#include<motor_a\motor_a.h>
#include<line_include\line.h>
#include<Ball_senser\ball.h>
#include<AC\ac.h>
#include<Cam\Cam.h>
#include<state\state.h>

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

        int Lside_A = 0;
        State Lside;
        
        int Center_A = 0;
        State Center;
        State Camback_on;

        timer A_12_t;
        int go_flag = 0;


        int A = 0;
        int B = 999;
        int c = 0;
        int stop_range = 20;
        const int far_th = 130;
        int go_val = 220;
        int print_flag = 1; // 1だったらシリアルプリントする
        int line_F = 0;     //A=15のとき、ステート抜けるか判定するフラグ
        int back_Flag = 0;  //A=15に行くフラグ
        int push_flag = 0;  //押し込まれてるフラグ

        int line_none_flag = 0;

        int A_15_flag;

        int Stop_flag;

        byte return_num[4];
    public:
        void defence();
        void available_set();
        int get_A();
        byte* get_flag();
};