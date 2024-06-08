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

class Attack{
    private:
        int A = 0;
        int B = 999;
        int c = 0;
        int ang_180 = 160;
        int ang_90 = 80;
        int ang_45 = 60;
        int ang_10 = 20;
        int cam_front_on = 0;
        int CFO_B = 999;
        int back_count = 0;
        int go_flag = 0;
        int back_flag = 0;

        int go_conf;

        int go_val;
        int go_val_back = 255;
        int goal_color;


        timer A_24_t;
        timer Timer;
        timer CFO_t;
    public:
        void attack();
        void available_set(int*);
        int* getCheckval();
        int check_val[6];
        int send_val[2];
};