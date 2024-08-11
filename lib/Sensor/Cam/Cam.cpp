#include<Cam\Cam.h>


Cam::Cam(int flag){
    ang_.setLenth(20);
    ang_.reset();
    if(flag == 3){
        F = 3;
    }
    else if(flag == 4){
        F = 4;
    }
}


void Cam::begin(){
    if(F == 3){
        Serial3.begin(115200);
    }
    else if(F == 4){
        Serial4.begin(115200);
    }
}



int Cam::getCamdata(){
    if(color == BLUE){
        if(data_byte[1] != 0){
            ang = ang_.demandAve((data_byte[0] - 67)*3/4);
            Size = data_byte[1];
            senter = data_byte[2];
            on = 1;
            return 1;
        }
    }
    else if(color == YELLOW){
        if(data_byte[4] != 0){
            on = 1;
            ang = ang_.demandAve((data_byte[3] - 80)*3/4);
            Size = data_byte[4];
            senter = data_byte[5];
            return 1;
        }
    }
    on = 0;
    return 0;
}


void Cam::print(){
    Serial.print(" color : ");
    Serial.print(color);
    if(on == 0){
        Serial.print(" No block detected");
    }
    else{
        Serial.print("  ang : ");
        Serial.print(ang);
        Serial.print("  size : ");
        Serial.print(Size);
        Serial.print(" senter : ");
        Serial.print(senter);
    }
}