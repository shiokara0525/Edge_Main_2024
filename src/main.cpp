#include <Arduino.h>

#include<motor_a\motor_a.h>
#include<AC\ac.h>
#include<Ball_senser\ball.h>
#include<line_include\line.h>
#include<move_ave\MA.h>
#include<timer.h>
#include<Cam\Cam.h>
#include<Kicker\kicker.h>

#include<Attack.h>
#include<defence.h>

AC ac;
Cam cam_front(3);
Cam cam_back(4);
BALL ball;
LINE line;

Attack attack;
Diffence defence;

Kicker kicker;

timer Ball_period;
timer Line_period;

motor_attack MOTOR;

int Mode = 0;
int Mode_old = 999;
float go_ang = 0;
int go_flag = 0;

int Kick = 0;

int color;
int ac_tirget;
int val_max = 200;

int PS4_r,PS4_theta;
int PS4R_r,PS4R_theta;
int Rotate_flag = 0;

int CHECK[2];

int Values[6];

void sendtoESP(const char*);

void setup(){
  Serial.begin(9600);
  Serial7.begin(115200);

  ball.begin();
  line.begin();
  kicker.setup();
  cam_front.begin();
  cam_back.begin();
  ac.setup();
  Mode = 0;

  sendtoESP("START");
}

void loop(){
  if(2500 < Line_period.read_us()){
    line.getLINE_Vec();
    Line_period.reset();
  }
  if(2500 < Ball_period.read_us()){
    ball.getBallposition();
    Ball_period.reset();
  }
  cam_front.getCamdata();
  cam_back.getCamdata();


  if(Mode == 0){
    if(Mode != Mode_old){
      Mode_old = Mode;
      MOTOR.motor_0();
    }

    kicker.run(Kick);
    Kick = 0;
  }
  else if(Mode == 1){
    if(Mode != Mode_old){
      Mode_old = Mode;
      attack.available_set(Values);
    }

    attack.attack();
    sendtoESP("CHECK");
  }
  else if(Mode == 2){
    if(Mode != Mode_old){
      Mode_old = Mode;
      ac.setup();
      kicker.stop();
    }

    angle goang(PS4_theta,true);
    float AC_val = ac.getAC_val() / 2;

    if(100 < PS4_r){
      MOTOR.moveMotor_0(goang,PS4_r,AC_val,0);
    }
    else{
      MOTOR.motor_ac(AC_val);
    }

    if(Rotate_flag){
      // Serial.print(" !!!!! ");
      ac.dir_target = ac.dir_n + PS4R_theta;
    }

    kicker.run(Kick);
    Kick = 0; 

    ac.print();
    Serial.println();
  }
  else if(Mode == 3){
    if(Mode != Mode_old){
      Mode_old = Mode;
      kicker.stop();
    }
  }
  else if(Mode == 99){
    if(Mode != Mode_old){
      Mode_old = Mode;
      kicker.stop();
    }
  }
  // ball.print();
  // Serial.print(" | ");
  // line.print();
  cam_front.print();
  Serial.println();
}


void sendtoESP(const char* message){
  int flag = 0;
  int send[2] = {0,0};
  if(strcmp(message,"START") == 0){
    flag = 1;
  }
  else if(strcmp(message,"BALL") == 0){
    flag = 2;
    send[0] = ball.vec.getX();
    send[1] = ball.vec.getY();
  }
  else if(strcmp(message,"LINE") == 0){
    flag = 3;
    if(line.LINE_on){
      send[0] = line.vec.getX() * 100;
      send[1] = line.vec.getY() * 100;
    }
    else{
      send[0] = 0;
      send[1] = 0;
    }
    Serial.print(" LINE send ");
    // line.print();
    line.vec.print();
  }
  else if(strcmp(message,"CAM") == 0){
    flag = 4;
    send[0] = cam_front.ang;
    send[1] = cam_front.Size;
  }
  else if(strcmp(message,"AC_DIR") == 0){
    flag = 5;
    send[0] = ac.getnowdir();
  }
  else if(strcmp(message,"CHECK") == 0){
    flag = 6;
    send[0] = attack.send_val[0];
    send[1] = attack.send_val[1];
  }

  uint8_t send_byte[7] = {38,0,0,0,0,0,37};
  send_byte[1] = flag;
  send_byte[2] = byte(send[0] >> 8);
  send_byte[3] = byte(send[0] & 0xFF);
  send_byte[4] = byte(send[1] >> 8);
  send_byte[5] = byte(send[1] & 0xFF);
  Serial7.write(send_byte,7);
  for(int i = 0; i < 7; i++){
    Serial.print(" ");
    Serial.print(send_byte[i]);
  }
  Serial.println();
}


void serialEvent7(){  
  uint8_t data[5];
  unsigned int contain[2];
  int data_int;

  if(Serial7.available() < 5){
    return;
  }

  data[0] = Serial7.read();
  if(data[0] != 38){
    return;
  }
  for(int i = 1; i < 5; i++){
    data[i] = Serial7.read();
    // Serial.print(data[i]);
    // Serial.print(" ");
  }

  contain[0] = uint16_t(data[2]) << 8;
  contain[1] = uint16_t(data[3]);
  data_int = contain[0] | contain[1];


  if(data[1] == 1){
    Mode = data_int;
    Serial.print(" Mode : ");
    Serial.print(Mode);
  }
  else if(data[1] == 2){
    if(data_int == 15){
      sendtoESP("AC_DIR");
    }
    else if(data_int == 30){
      sendtoESP("LINE");
    }
    else if(data_int == 50){
      sendtoESP("BALL");
    }
    else if(data_int == 80){
      sendtoESP("CAM");
    }
  }
  else if(data[1] == 3){
    val_max = data_int;
  }
  else if(data[1] == 4){
    color = data_int;
  }
  else if(data[1] == 5){
    ball.get_th = data_int;
  }
  else if(data[1] == 6){
    MOTOR.NoneM_flag = data_int;
  }
  else if(data[1] == 7){
    ac.getnowdir();
    ac_tirget = ac.dir_n;
    ac.setup_2();
    // Serial.print(" !!!!!!!!!!! ");
    // ac.print();
  }
  else if(data[1] == 8){
    Kick = 1;
  }
  else if(data[1] == 9){
    int y = data[2] - 128;
    int x = data[3] - 128;
    PS4_theta = degrees(atan2(y,x));
    PS4_r = sqrt(x * x + y * y);
    
    if(100 < sqrt(x * x + y * y)){
      go_flag = 1;
    }
    else{
      go_flag = 0;
    }
    // Serial.println();
  }
  else if(data[1] == 10){
    Serial8.write(38);
    Serial8.write(0);
    Serial8.write(data_int);
    Serial8.write(37);
  }
  else if(data[1] == 11){
    int y = data[2] - 128;
    int x = data[3] - 128;
    PS4R_theta = degrees(atan2(y,x));
    PS4R_r = sqrt(x * x + y * y);
    
    if(100 < sqrt(x * x + y * y)){
      Rotate_flag = 1;
    }
    else{
      Rotate_flag = 0;
    }
    // Serial.print("111111");
  }
  else{
    for(int i = 0; i < 6; i++){
      if(12 + i == data[1]){
        Values[i] = data_int;
        Serial.print(" num : ");
        Serial.print(11 + i);
        Serial.print(" send : ");
        Serial.print(data_int);
      }
    }
  }
  // Serial.print(" ");
  // Serial.print(data[1]);
  // Serial.print(" ");
  // Serial.print(data_int);
  
  // Serial.println();
}

void serialEvent3(){
  uint8_t reBuf[8];
  if(Serial3.available() < 8){
    return;
  }
  reBuf[0] = Serial3.read();

  if(reBuf[0] != 38){
    return;
  }

  for(int i = 1; i < 8; i++){
    reBuf[i] = Serial3.read();
  }

  if(reBuf[0] == 38 && reBuf[7] == 37){
    for(int i = 0; i < 6; i++){
      cam_back.data_byte[i] = reBuf[i+1];
    }
    // Serial.print("sawa1");
  }

  // for(int i = 0; i < 8; i++){
  //   Serial.print(" ");
  //   Serial.print(reBuf[i]);
  // }
  // Serial.println();
}



void serialEvent4(){
  uint8_t reBuf[8];
  if(Serial4.available() < 8){
    return;
  }
  reBuf[0] = Serial4.read();

  if(reBuf[0] != 38){
    return;
  }

  for(int i = 1; i < 8; i++){
    reBuf[i] = Serial4.read();
  }

  if(reBuf[0] == 38 && reBuf[7] == 37){
    for(int i = 0; i < 6; i++){
      cam_front.data_byte[i] = reBuf[i+1];
    }
    // Serial.print("sawa1");
  }

  // for(int i = 0; i < 8; i++){
  //   Serial.print(" ");
  //   Serial.print(reBuf[i]);
  // }
  // Serial.println();
}



void serialEvent8(){
  // Serial.println(" sawa3 ");
  uint8_t read[6];
  if(Serial8.available() < 6){
    return;
  }
  read[0] = Serial8.read();
  if(read[0] != 38){
    return;
  } 

  for(int i = 1; i < 6; i++){
    read[i] = Serial8.read();
  }

  if(read[0] == 38 && read[5] == 37){
    for(int i = 0; i < 4; i++){
      line.data_byte[i] = read[i+1];
    }
  }

  // for(int i = 0; i < 6; i++){
  //   Serial.print(read[i]);
  //   Serial.print(" ");
  // }
  // Serial.println();
}



void serialEvent6(){
  uint8_t read[8];
  unsigned int contain[4];
  int x,y;
  if(Serial6.available() < 12){
    return;
  }
  read[0] = Serial6.read();
  if(read[0] != 0xFF){
    return;
  }
  for(int i = 1; i < 12; i++){
    read[i] = Serial6.read();
  }

  if(read[0] == 0xFF && read[11] == 0xAA){
    for(int i = 0; i < 10; i++){
      ball.data_byte[i] = read[i+1];
    }
  }

  // for(int i = 0; i < 8; i++){
  //   Serial.print(" ");
  //   Serial.print(read[i]);
  // }
  // Serial.println();
}