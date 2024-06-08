#pragma once

#include<timer.h>
#include<move_ave\MA.h>


#define BLUE 0
#define YELLOW 1

class Cam{
    private:
        int B = 999;
        int A = 0;
        int F = 0;
    public:
        Cam(int);
        int data_byte[6];
        int getCamdata();
        float ang;
        float Size;
        int on = 0;
        int color = 0;
        int senter;
        MA ang_;
        void print();
        void begin();
};