#include<Ball_senser\ball.h>

BALL::BALL(){
    ball_x.setLenth(10);
    ball_y.setLenth(10);
    ball_get_val_1.setLenth(50);
    ball_get_val_2.setLenth(50);
    B_dif_.setLenth(40);
    Bang_dif_.setLenth(40);
}


void BALL::begin(){
    Serial8.begin(115200);
}


int BALL::getBallposition(){
    float x = ball_x.returnAve();
    float y = ball_y.returnAve();
    get_val = get_1 + get_2;

    if(x == 0 && y == 0){
        flag = 0;
        return flag;
    }
    else{
        flag = 1;
    }

    if(45 < get_1 && 45 < get_2 && (51 < get_1 || 51 < get_2) && get_th < get_val){
        ball_get = 1;
    }
    else{
        ball_get = 0;
    }
    far_old = far;
    vec.set(x,y);
    ang = degrees(vec.getAngle());
    far = vec.getMagnitude();
    Bang_dif = Bang_dif_.sum(abs(ang) - abs(ang_old));
    ang_old = ang;

    world_far = 220 - far;
    world_vec.setX(world_far * cos(radians(ang)));
    world_vec.setY(world_far * sin(radians(ang)));
    vec_dif = B_dif_.sum(world_vec - world_vec_old);
    world_vec_old = world_vec;
    return flag;
}


void BALL::get_resister_1(int n){
    get_1 = ball_get_val_1.demandAve(n);
}


void BALL::get_resister_2(int n){
    get_2 = ball_get_val_2.demandAve(n);
}


void BALL::print(){
    Serial.print(" ang : ");
    Serial.print(ang);
    Serial.print(" far : ");
    Serial.print(far_);
    // Serial.print(" x : ");
    // Serial.print(x_pos);
    // Serial.print(" y : ");
    // Serial.print(y_pos);
    // Serial.print(" dx : ");
    // Serial.print(dx);
    Serial.print(" get_val : ");
    Serial.print(get_val);
    Serial.print(" get : ");
    Serial.print(ball_get);
    Serial.print(" get_1 : ");
    Serial.print(get_1);
    Serial.print(" get_2 : ");
    Serial.print(get_2);
    Serial.print(" flag : ");
    Serial.print(flag);
}