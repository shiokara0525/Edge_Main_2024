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
extern int Values[];

class Attack{
    private:
        int A = 0;
        int B = 999;
        int c = 0;

        float RA_a = 1.8;  //45~90°の時の回り込みの係数
        float RA_b = 2.5;
        float RA_c = 1.8;
        float RA_d;
        float AC_D = 1.0;

        float ang_10 = 10;
        float ang_30 = 45;
        float ang_45 = 72;
        float ang_90 = 135;

        int cam_front_on = 0;
        int back_count = 0;
        int go_flag = 0;
        int back_flag = 0;

        int ballang_first;

        int rake_flag = 0;
        State rake;

        int first_ang = 0;

        timer play_time;

        int go_val;
        int go_val_back = 200;


        timer A_24_t;
        timer Timer;
        State CFO;

        byte return_byte[4];

        State ball_front;
    public:
        void attack();
        void available_set(int*);
        byte* getCheckval();
        int check_val[6];
        int send_val[2];
        int setplay_flag = 0;
};