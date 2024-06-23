#include<Attack.h>


void Attack::available_set(int *check_val){
  ang_10 = check_val[0];
  ang_45 = check_val[1];
  ang_90 = check_val[2];
  ang_180 = check_val[3];
  go_conf = check_val[4] / 100.0;

  goal_color = color;
  if(goal_color == 0){
    cam_front.color = 0;  //青が0 黄色が1
    cam_back.color = 1;  //青が0 黄色が1
  }
  else if(goal_color == 1){
    cam_front.color = 1;  //青が0 黄色が1
    cam_back.color = 0;  //青が0 黄色が1
  }
  go_val = val_max;
}


int* Attack::getCheckval(){
  return check_val;
}



void Attack::attack(){
  angle go_ang(ball.ang,true);         //進む角度のオブジェクト


  float AC_val = 100;                  //姿勢制御の出力
  int max_val = go_val;                //進む出力
  float target = ac_tirget;           //目標角度


  int AC_flag = 0;                     //0だったら絶対的な角度とる 1だったらゴール向く
  int kick_ = 0;                       //0だったらキックしない 1だったらキック
  int M_flag = 1;                      //1だったら動き続ける 0だったら止まる
  back_flag = 0;
  
  c = 0;

  if(line.LINE_change == -1){  //踏んでない状態から踏んでる状態になった時
    if(5 <= line.firstDir_flag && line.firstDir_flag <= 7){  //後ろにラインがあったら
      if(30 < abs(ball.ang) && abs(ball.ang) <= 85){
        c = 1;
        A = 25;  //前に行く
      }
      else if(85 < abs(ball.ang) && abs(ball.ang) < 120){
        c = 1;
        A = 26;  //横に行く
      }
    }
    else if(line.firstDir_flag == 11 || line.firstDir_flag <= 1){  //前にラインがあったら
      if(cam_front.on){  //ゴール前だったら
        back_count++;
        if(back_count % 4 == 0){
          Timer.reset();
          A = 22;  //ボールを押し込むやつ 
          c = 1;
        }
      }
      else{  //notゴール前だったら
        back_count++;
        if(back_count % 4 == 0){
          Timer.reset();
          A = 24;  //後ろに下がるやつ
          c = 1;
        }
      }
    }
  }



  if(A == 22){
    if((line.LINE_on == 0 || (-0.5 < line.dis_X && (abs(line.ang) < 30 || 150 < abs(line.ang)))) && Timer.read_ms() < 5000 && abs(ball.ang) < 30){
      c = 1;  //ボールを押し込み続けるやつ
    }
    else{
      A = 23; //押し込む状態から戻るやつ
    }
  }


  if(A == 23){
    if(line.LINE_on){
      c = 1;  //戻ってるけどラインを踏んでる限りこのステートから出ない
    }
  }
  


  if(A == 24){
    if(abs(ball.ang) < 45 && Timer.read_ms() < 7000 && line.LINE_on == 0){
      c = 1;  //中立点付近で止まってLOP誘発
    }
  }


  if(A == 25){
    if(30 < abs(ball.ang) && abs(ball.ang) < 100){
      c = 1;  //横に行くやつ
    }
    else{
      c = 1;
      A = 26;
    }
    if(line.LINE_on == 1){
      c = 0;
    }
  }
  if(A == 26){
    if(60 < abs(ball.ang) && abs(ball.ang) < 120){
      c = 1;
    }
    if(line.LINE_on == 1){
      c = 0;
    }
  }

  if(ball.flag == 0){
    c = 0;
  }


  if(c == 0){
    if(line.LINE_on == 1){
      A = 20;
    }
    else if(line.side_flag){
      A = 21;
    }
    else{
      if(ball.flag == 1){
        if(0){
          A = 11;
        }
        else{
          A = 10;
        }
      }
      else{
        A = 5;
      }
    }
  }


  //----------------------------------------------------------動きの決定----------------------------------------------------------//


  if(A == 5){  //ボールがない時止まる
    if(A != B){
      B = A;
      Timer.reset();
    }

    if(Timer.read_ms() < 500){
      go_ang = 180;
      max_val = 200;
    }
    else{
      M_flag = 0;
    }
  }



  if(A == 10){  //回り込むやつ
    if(A != B){
      go_flag = 0;
      if(B == 20 || B == 21){
        if(line.ang_old < 0){
          if(-90 < ball.ang && ball.ang < 30){
            go_flag = 1;
          }
        }
        else{
          if(-30 < ball.ang && ball.ang < 90){
            go_flag = 1;
          }
        }
      }
      B = A;
      Timer.reset();
    }

    if(90 < abs(ball.ang)){
      go_flag = 0;
    }

    float confidencial_num = (ball.vec_down.getMagnitude() - 50) * 0.066;

    if(abs(ball.ang) < 10){
      go_ang = abs(ball.ang);
      kick_ = 1;
    }
    else if(abs(ball.ang) < 45){
      go_ang = abs(ball.ang) * 3.0;
    }
    else if(abs(ball.ang) < 90){
      if(50 <= ball.vec_down.getMagnitude() && ball.vec_down.getMagnitude() < 65){
        go_ang = (confidencial_num + 1) * abs(ball.ang) + (1 - confidencial_num) * 45;
      }
      else if(ball.vec_down.getMagnitude() < 50){
        go_ang = abs(ball.ang) + 45;
      }
      else{
        go_ang = abs(ball.ang) * 2;
      }
    }
    else{
      if(50 <= ball.vec_down.getMagnitude() && ball.vec_down.getMagnitude() < 65){
        go_ang = abs(ball.ang) + (confidencial_num + 1) * 45.0;
      }
      else if(ball.vec_down.getMagnitude() < 50){
        go_ang = abs(ball.ang) + 45;
      }
      else{
        go_ang = abs(ball.ang) * 2;
      }
    }

    go_ang = go_ang.degree * (ball.ang < 0 ? -1 : 1);  //角度の正負を元に戻す

    if(go_flag == 1){
      go_ang = ball.ang;
    }
  }



  if(A == 11){  //ボール持ってるとき前進するやつ
    if(A != B){
      B = A;
      Timer.reset();
      CFO_t.reset();
    }
    cam_front_on = 0;

    if(cam_front.on == 1){
      if(abs(cam_front.ang) < 20 && 15 < cam_front.Size){
        cam_front_on = 1;
        go_ang = 0;
        AC_flag = 1;
        // dribbler_flag = 0;
      }
      else if(abs(cam_front.ang) < 60){
        go_ang = 0;
        AC_flag = 1;
      }
      else{
        go_ang = cam_front.ang * 1.0;
      }
    }
    else{
      go_ang = 0;
    }

    if(cam_front_on == 1){
      max_val -= 15;
      if(cam_front_on != CFO_B){
        CFO_B = cam_front_on;
        CFO_t.reset();
      }

      if(200 < CFO_t.read_ms()){
        kick_ = 1;
      }
      if(40 < cam_front.Size){
        kick_ = 1;
      }
    }
    else if(cam_front_on == 0){
      CFO_B = 0;
    }
  }



  if(A == 12){
    if(A != B){
      B = A;
      Timer.reset();
    }
    M_flag = 0;
  }



  if(A == 20){  //ラインから逃げるやつ
    angle line_ang(line.ang,true);
    if(A != B){
      B = A;
      Timer.reset();
    }
    back_flag = 1;
    // target = Line_target_dir;
    go_ang = line.decideGoang(line_ang,line.firstDir_flag);
  }



  if(A == 21){  //サイド読んでるとき逃げるやつ
    if(A != B){
      B = A;
      Timer.reset();
    }
    if(line.side_flag == 1){
      go_ang = -90;
    }
    else if(line.side_flag == 2){
      go_ang = 90;
    }
    else if(line.side_flag == 3){
      go_ang = 180;
    }
    else if(line.side_flag == 4){
      go_ang = 0;
    }
  }



  if(A == 22){
    Serial.print("!!!!!!!!!!!!!!!");
    if(A != B){
      B = A;
      Timer.reset();
    }
    max_val = 180;
    go_ang = 0;
  }



  if(A == 23){
    if(A != B){
      B = A;
    }
    go_ang = 180;
  }



  if(A == 24){
    if(A != B){
      B = A;
      Timer.reset();
    }
    A_24_t.reset();
    if(Timer.read_ms() < 500){
      go_ang = 180;
      max_val = 180;
    }
    else{
      M_flag = 0;
    }
  }



  if(A == 25){  //後ろのライン読んだとき前に進むやつ
    if(A != B){
      B = A;
      Timer.reset();
    }
    go_ang = 0;
  }



  if(A == 26){  //後ろのライン読んだとき横に進むやつ
    if(A != B){
      B = A;
    }
    if(ball.ang < 0){
      go_ang = -90;
    }
    else{
      go_ang = 90;
    }
  }



  if(A == 27){
    if(A != B){
      B = A;
      Timer.reset();
    }
    if(Timer.read_ms() < 100){
      go_ang = 180;
    }
    else{
      M_flag = 0;
    }
  }


  //----------------------------------------------------------出力(ここで行ってるのはフラグの回収のみ)----------------------------------------------------------//

  ac.dir_target = target;
  if(AC_flag == 0){
    AC_val = ac.getAC_val();
  }
  else if(AC_flag == 1){
    if(40 < cam_front.Size && 500 < A_24_t.read_ms()){
      AC_val = ac.getCam_val(-cam_front.ang) * 1.5;
    }
    else{
      AC_val = ac.getCam_val(-cam_front.ang);
    }
  }

  kicker.run(kick_);

  if(back_flag == 1){
    max_val = go_val_back;
  }

  if(M_flag == 1){
    MOTOR.moveMotor_0(go_ang,max_val,AC_val,0);
  }
  else if(M_flag == 0){
    MOTOR.motor_0();
  }

  Serial.print(" goang : ");
  Serial.print(go_ang.degree);
  Serial.println();
  send_val[0] = go_ang.degree;
  send_val[1] = cam_front.ang;
  // ball.print();
  // Serial.print(" A : ");
  // Serial.print(A);

  if(MOTOR.NoneM_flag){
    // OLED_moving();
  }
}