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
Cam cam_front(3);  //前ゴール
Cam cam_back(4);   //後ろゴール
BALL ball;
LINE line;

Attack attack;     //アタッカー動作時の状態判断のクラス
Diffence defence;  //ディフェンス動作時の状態判断のクラス

Kicker kicker;     //キッカー動作のクラス

timer Ball_period;
timer Line_period;

timer Main_timer;
timer Mode_timer;
timer ball_Get;
timer line_Get;
timer ESP_send;

motor_attack MOTOR; //モーター動作のクラス

int Mode = 0;
int Mode_old = 999;
float go_ang = 0;
int go_flag = 0;

int goal_send_count = 0;
int line_send_count = 0;

int Kick = 0;

int testMode = 0;

int color;
int ac_tirget;
int val_max = 200;

int PS4_r,PS4_theta;
int PS4R_r,PS4R_theta;
int Rotate_flag = 0;

int line_on = 0;

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
  Main_timer.reset();
  if(2500 < Line_period.read_us()){
    line.getLINE_Vec();
    Line_period.reset();
  }
  if(2500 < Ball_period.read_us()){
    ball.getBallposition();
    // Serial.print(" Ball ");
    // Serial.print(Ball_period.read_ms());
    // Serial.println();
    Ball_period.reset();
  }
  cam_front.getCamdata();
  cam_back.getCamdata();


  if(Mode == 0){
    if(Mode != Mode_old){
      Mode_old = Mode;
      MOTOR.motor_0();
      Mode_timer.reset();
      line_on = 0;
      kicker.stop();
      Serial8.write(38);
      Serial8.write(10);
      Serial8.write(0);
      Serial8.write(37);
    }

    kicker.run(Kick);
    Kick = 0;
  }

  else if(Mode == 1){
    if(Mode != Mode_old){
      Mode_old = Mode;
      attack.available_set(Values);
      Mode_timer.reset();
      kicker.stop();
    }

    attack.attack();
  }

  else if(Mode == 2){
    if(Mode != Mode_old){
      Mode_old = Mode;
      defence.available_set();
      Mode_timer.reset();
      kicker.stop();
    }

    defence.defence();
  }

  else if(Mode == 3){
    if(Mode != Mode_old){
      Mode_old = Mode;
      kicker.stop();
    }
    if(testMode == 0){
      angle ang(0,true);
      float AC_val = ac.getAC_val();
      MOTOR.moveMotor_0(ang,val_max,AC_val,0);
    }
    else if(testMode == 1){
      for(int i = 0; i < 4; i++){
        MOTOR.Moutput(i,255);
        delay(500);
        MOTOR.Moutput(i,0);
        delay(100);
      }
    }
    else if(testMode == 3){
      float AC_val = ac.getAC_val();
      MOTOR.motor_ac(AC_val);
    }
    else if(testMode == 4){
      MOTOR.motor_0();
      kicker.TEST_();
    }
  }

  else if(Mode == 10){
    if(Mode != Mode_old){
      Mode_old = Mode;
      kicker.stop();
      MOTOR.motor_0();
    }
  }

  else if(Mode == 99){
    if(Mode != Mode_old){
      Mode_old = Mode;
      kicker.stop();

    }
    MOTOR.motor_0();
  }
  // ball.print();
  // Serial.print(" V : ");
  // ball.vec_velocity.print();
  // Serial.print(" | A : ");
  // ball.vec_acc.print();
  // Serial.println();
  // Serial.print(">ball_V:");
  // Serial.println(ball.vec_velocity.getMagnitude());
  // Serial.print(">ball_A:");
  // Serial.println(ball.vec_acc.getMagnitude());
  // Serial.print(" | ");
  // line.print();
  // cam_front.print();
  // cam_back.print();
  // Serial.print(" Timer : ");
  // Serial.print(Main_timer.read_us());
  // line.print_2();
  // Serial.println();
  // ball.vec_down.print();
  // Serial.print(" setup : ");
  // Serial.print(attack.setplay_flag);
  // ac.print();
  Serial.print(" Mode : ");
  Serial.print(Mode);
  Serial.println();
  // Main_timer.reset();
}


void sendtoESP(const char* message){
  byte flag = 0;
  int send[2] = {0,0};
  byte *send_num;
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
  else if(strcmp(message,"CAM_FRONT") == 0){
    flag = 4;
    if(cam_front.on){
      send[0] = cam_front.ang;
      send[1] = cam_front.Size;
    }
    else{
      send[0] = 0;
      send[1] = 0;
    }
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
  else if(strcmp(message,"NEOPIXEL_D") == 0){
    flag = 7;
    send_num = defence.get_flag();
  }
  else if(strcmp(message,"CAM_BACK") == 0){
    flag = 8;
    if(cam_back.on){
      send[0] = cam_back.ang;
      send[1] = cam_back.Size;
    }
    else{
      send[0] = 0;
      send[1] = 0;
    }
  }
  else if(strcmp(message,"LINE_ALL") == 0){
    flag = 9;
    send_num = line.data_byte;
    // for(int i = 0; i < 4; i++){
    //   Serial.print(" ");
    //   Serial.print(send_num[i]);
    // }
    // Serial.println();
  }
  else if(strcmp(message,"BALL_CATCH") == 0){
    flag = 10;
    send[0] = ball.catch_val;
  }
  else if(strcmp(message,"NEOPIXEL_A") == 0){
    flag = 11;
    send_num = attack.getCheckval();
  }

  uint8_t send_byte[7] = {38,flag,0,0,0,0,37};
  if(flag != 9 && flag != 7 && flag != 11){
    send_byte[2] = byte(send[0] >> 8);
    send_byte[3] = byte(send[0] & 0xFF);
    send_byte[4] = byte(send[1] >> 8);
    send_byte[5] = byte(send[1] & 0xFF);
  }
  else{
    for(int i = 0; i < 4; i++){
      send_byte[i + 2] = send_num[i];
    }
  }

  Serial7.write(send_byte,7);
  // for(int i = 0; i < 7; i++){
  //   Serial.print(" ");
  //   Serial.print(send_byte[i]);
  // }
  // Serial.println();
}


void serialEvent7(){  
  uint8_t data[5];
  unsigned int contain[2];
  int data_int;

  if(Serial7.available() < 5){
    return;
  }

  while(5 <= Serial7.available()){
    data[0] = Serial7.read();
    if(data[0] == 38){
      break;
    }
  }
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
    // Serial.print(" Mode : ");
    // Serial.print(Mode);
  }
  else if(data[1] == 2){
    if(data_int == 15){
      sendtoESP("AC_DIR");
      line_on = 1;
      Serial8.write(38);
      Serial8.write(10);
      Serial8.write(1);
      Serial8.write(37);
    }
    else if(data_int == 30){
      line_on = 1;
      Serial8.write(38);
      Serial8.write(10);
      Serial8.write(1);
      Serial8.write(37);
      if(line_send_count == 0){
        sendtoESP("LINE");
        line_send_count = 1;
      }
      else{
        sendtoESP("LINE_ALL");
        line_send_count = 0;
      }
    }
    else if(data_int == 50){
      sendtoESP("BALL");
    }
    else if(data_int == 80){
      if(goal_send_count == 0){
        sendtoESP("CAM_FRONT");
        goal_send_count = 1;
      }
      else{
        sendtoESP("CAM_BACK");
        goal_send_count = 0;
      }
      // sendtoESP("CAM_BACK");
    }
    else if(data_int == 90){
      sendtoESP("BALL_CATCH");
    }
  }
  else if(data[1] == 3){
    val_max = data_int;
  }
  else if(data[1] == 4){
    color = data_int;
    if(color == YELLOW){
      cam_front.color = YELLOW;
      cam_back.color = BLUE;
    }
    else{
      cam_front.color = BLUE;
      cam_back.color = YELLOW;
    }
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
  else if(data[1] == 13){
    sendtoESP("NEOPIXEL_D");
  }
  else if(data[1] == 14){
    testMode = data_int;
  }
  else if(data[1] == 16){
    sendtoESP("NEOPIXEL_A");
  }
  else if(data[1] == 17){
    attack.setplay_flag = data_int;
  }
  else{
    for(int i = 0; i < 6; i++){
      if(100 + i == data[1]){
        Values[i] = data_int;
        // Serial.print(" num : ");
        // Serial.print(i);
        // Serial.print(" recieve : ");
        // Serial.print(data_int);
      }
    }
  }
  // Serial.print(" sort : ");
  // Serial.print(data[1]);
  // Serial.print(" content : ");
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
  uint8_t read[12];
  int flag_break = 0;
  while(12 <= Serial6.available()){
    read[0] = Serial6.read();
    if(read[0] == 0xFF){
      flag_break = 1;
      break;
    }
  }

  if(flag_break == 0){
    return;
  }


  for(int i = 1; i < 12; i++){
    read[i] = Serial6.read();
  }

  if(read[0] == 0xFF && read[11] == 0xAA){
    ball.get_data(read);
    // Serial.println(ball_Get.read_us());
  }
  // ball_Get.reset();

  // for(int i = 0; i < 12; i++){
  //   Serial.print(" ");
  //   Serial.print(read[i]);
  // }
  // Serial.println();
}