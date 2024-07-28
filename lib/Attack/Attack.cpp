#include<Attack.h>


void Attack::available_set(int *check_val){
  ang_10 = Values[0];
  ang_30 = Values[1];
  ang_45 = Values[2];
  ang_90 = Values[3];
  AC_D = Values[4] / 100.0;
  A = 0;
  c = 0;
  float m1 = ang_10 / ((10 - 30) * (10 - 45) * (10 - 90));
  float m2 = ang_30 / ((30 - 10) * (30 - 45) * (30 - 90));
  float m3 = ang_45 / ((45 - 10) * (45 - 30) * (45 - 90));
  float m4 = ang_90 / ((90 - 10) * (90 - 30) * (90 - 45));
  RA_a = m1 + m2 + m3 + m4;
  RA_b = -(m1 * (30 + 45 + 90) + m2 * (10 + 45 + 90) + m3 * (10 + 30 + 90) + m4 * (10 + 30 + 45));
  RA_c = m1 * (30 * 45 + 30 * 90 + 45 * 90) + m2 * (10 * 45 + 10 * 90 + 45 * 90) + m3 * (10 * 30 + 10 * 90 + 30 * 90) + m4 * (10 * 30 + 10 * 45 + 30 * 45);
  RA_d = -(m1 * 30 * 45 * 90 + m2 * 10 * 45 * 90 + m3 * 10 * 30 * 90 + m4 * 10 * 30 * 45);
  Serial.print(" RA_a : ");
  Serial.print(RA_a);
  Serial.print(" RA_b : ");
  Serial.print(RA_b);
  Serial.print(" RA_c : ");
  Serial.print(RA_c);
  Serial.print(" RA_d : ");
  Serial.print(RA_d);
  Serial.println();
  go_val = val_max;
  play_time.reset();
  first_ang = ac.dir_n;
}


byte* Attack::getCheckval(){
  return_byte[0] = A;
  return_byte[1] = cam_front.ang + 60;
  if(!cam_front.on){
    return_byte[1] = 240;
  }
  return_byte[2] = cam_back.ang + 60;
  if(!cam_back.on){
    return_byte[2] = 240;
  }
  return_byte[3] = 99;
  return return_byte;
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
  rake_flag = 0;


  if(ball.flag == 0){
    c = 0;
  }


  if(c == 0){
    if(line.LINE_on == 1 || line.LINE_change == -1){
      A = 20;
    }
    else{
      if(line.side_flag != 0){
        A = 21;
      }
      else{
        if(ball.flag == 1){
          if(1 <= ball.ball_get && abs(ball.ang) < 45){
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
  }


  //----------------------------------------------------------動きの決定----------------------------------------------------------//


  if(A == 5){  //ボールがない時止まる
    if(A != B){
      B = A;
      Timer.reset();
    }

    if(Timer.read_ms() < 500){
      go_ang = 180;
      // max_val = 200;
    }
    else{
      M_flag = 0;
    }
  }



  if(A == 10){  //回り込むやつ
    if(A != B){
      go_flag = 0;
      // if(B == 21){
      //   if(ball.ang * line.ang_side > 0){
      //     if(abs(ball.ang) < 90){
      //       go_flag = 1;
      //       Serial.print(" !!! ");
      //     }
      //   }
      //   else{
      //     if(abs(ball.ang) < 45 && cam_front.on){
      //       go_flag = 2;
      //     }
      //     else if(90 < abs(ball.ang)){
      //       go_flag = 3;
      //       ballang_first = ball.ang;
      //     }
      //   }
      // }
      // Serial.print(" flag : ");
      // Serial.print(go_flag);
      B = A;
      Timer.reset();
    }

    float ball_ang = ball.ang;

    if(go_flag == 3){
      if(ballang_first * ball_ang < 0){
        go_flag = 0;
      }
      else{
        if(ball.ang < 0){
          ball_ang += 360;
        }
        else{
          ball_ang -= 360;
        }
      }
    }

    float confidencial_num = (ball.vec.getMagnitude() - 80) * 0.015;
    if(confidencial_num < 0){
      confidencial_num = 0;
    }
    int front_flag = 0;

    if(abs(ball_ang) < 15){
      // Serial.print(" SEC : 1 ");
      if(23 < cam_front.Size){
        go_ang = abs(ball_ang);
        if(ball_front.readStateTimer(1) < 400){
        }
        AC_flag = 1;
        // Serial.print(" NO ");
      }
      else{
        go_ang = abs(ball_ang);
        // Serial.print(" YES ");
      }
      // cam_front.print();
      front_flag = 1;
    }
    else if(abs(ball_ang) < 90){
      // go_ang = 0.000122 * pow(abs(ball_ang),3) - 0.0128 * pow(abs(ball_ang),2) + 2.10 * abs(ball_ang) - 9.87;
      go_ang = RA_a * pow(abs(ball_ang),3) + RA_b * pow(abs(ball_ang),2) + RA_c * abs(ball_ang) + RA_d;
    }
    else{
      go_ang = abs(ball_ang) + 50;
    }


    ball_front.enterState(front_flag);

    go_ang = go_ang.degree * (ball_ang < 0 ? -1 : 1);  //角度の正負を元に戻す

    // Serial.println();
    if(go_flag == 1){
      go_ang = ball_ang;
      if(abs(ball_ang) < 60){
        go_ang = 0;
      }
      else{
        go_flag = 0;
      }
    }
    else if(go_flag == 2){
      AC_flag = 1;
    }

    // Serial.print(" go_flag : ");
    // Serial.print(go_flag);
    // Serial.print(" ball_ang : ");
    // Serial.print(ball_ang);
    // Serial.print(" ang : ");
    // Serial.print(go_ang.degree);
    // Serial.print(" conf : ");
    // Serial.print(confidencial_num);
  }



  if(A == 11){  //ボール持ってるとき前進するやつ
    if(A != B){
      B = A;
      Timer.reset();
      CFO.reset();
    }
    cam_front_on = 0;

    if(cam_front.on == 1){  //カメラ見てるとき
      if(cam_front.on == 1 && (abs(cam_front.ang) < 20 || cam_front.senter)){  //正面にゴールあってゴールもある程度近くにある時
        cam_front_on = 1;  //打っていいよ
        go_ang = 0;
        AC_flag = 1;
        // dribbler_flag = 0;
      }
      else{
        go_ang = 0;
        AC_flag = 1;
      }
    }
    else{
      go_ang = 0;
      // kick_ = 1;
      if(400 < Timer.read_ms()){
        kick_ = 1;
      }
    }

    CFO.enterState(cam_front_on);
    if(cam_front_on == 1){  //打っていいよフラグ

      if(200 < CFO.readStateTimer()){
        kick_ = 1;  //打っていいよフラグが0.2秒立ってたら打つ
        Serial.print(" kick_1 ");
      }
      if(40 < cam_front.Size){
        kick_ = 1;  //ゴールが近い時は問答無用で打つ
        Serial.print(" kick_2 ");
      }
    }


    if(1500 < Timer.read_ms()){
      rake_flag = 1;
      target += 90;
      AC_flag = 0;
    }
    else if(2000 < Timer.read_ms()){
      rake_flag = 0;
    }

    // if(setplay_flag && 100 < Timer.read_ms()){
    //   kick_ = 1;
    // }
    // cam_front.print();
    // Serial.print(" kick_ : ");
    // Serial.print(kick_);
    // Serial.println();
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
    go_ang = degrees(line.vec_go.getAngle());


    if(line.LINE_change == -1){  //踏んでない状態から踏んでる状態になった時
      if(150 < abs(degrees(line.vec_first.getAngle()))){  //後ろにラインがあったら
        if(30 < abs(ball.ang) && abs(ball.ang) <= 85){
          c = 1;
          A = 25;  //前に行く
        }
        else if(85 < abs(ball.ang) && abs(ball.ang) < 120){
          c = 1;
          A = 26;  //横に行く
        }
      }
      else if(45 < (degrees(line.vec_first.getAngle())) && (degrees(line.vec_first.getAngle())) < 135){
        if(0 <= cam_back.ang){
          A = 25;
          c = 1;
        }
      }
      else if(-135 < (degrees(line.vec_first.getAngle())) && (degrees(line.vec_first.getAngle())) < -45){
        if(cam_back.ang <= 0){
          A = 25;
          c = 1;
        }
      }
      else if(abs(degrees(line.vec_first.getAngle())) < 45){  //前にラインがあったら
        if(cam_front.on && cam_front.senter){  //ゴール前だったら
          back_count++;
          if(back_count % 4 == 0 && abs(ball.ang) < 45 && !ball.ball_get){
            A = 22;  //ボールを押し込むやつ
            Serial.println(" line_front "); 
            c = 1;
          }
        }
        else if(cam_front.on == 0){  //notゴール前だったら
          back_count++;
          if(back_count % 4 == 0){
            A = 24;  //後ろに下がるやつ
            c = 1;
          }
        }
      }
    }
  }



  if(A == 21){  //サイド読んでるとき逃げるやつ
    if(A != B){
      B = A;
      Timer.reset();
    }
    back_flag = 1;
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



  if(A == 22){  //ボール押し込む
    if(A != B){
      B = A;
      Timer.reset();
    }
    // max_val = 150;
    go_ang = 0;
  
    if((line.LINE_on == 1 && (line.dis_X < -0.75 || (30 <= abs(line.ang) && abs(line.ang) <= 150))) || 5000 < Timer.read_ms() || 60 < abs(ball.ang)){
      A = 23;
    }
    if(400 < Timer.read_ms()){
      A = 23;
    }
  }



  if(A == 23){  //ボール押し込んだ後下がる
    if(A != B){
      B = A;
    }
    go_ang = 180;

    if(!line.LINE_on){
      c = 0;  //戻ってるけどラインを踏んでる限りこのステートから出ない
    }
  }



  if(A == 24){  //後ろに下がるやつ(LOP誘発)
    if(A != B){
      B = A;
      Timer.reset();
    }
    A_24_t.reset();
    if(Timer.read_ms() < 500){
      go_ang = 180;
      // max_val = 180;
    }
    else{
      M_flag = 0;
      if(abs(ball.ang) < 10 || ball.ball_get){
        c = 0;
      }
    }


    if(30 < abs(ball.ang) || 7000 < Timer.read_ms() || line.LINE_on){
      c = 0;
    }
  }



  if(A == 25){  //後ろのライン読んだとき前に進むやつ
    if(A != B){
      B = A;
      Timer.reset();
    }
    go_ang = 0;

    if(line.LINE_on == 1){
      c = 0;
    }
    else if(abs(ball.ang) <= 30 || 100 <= abs(ball.ang)){
      A = 26;
    }
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

    if(line.LINE_on || 5000 < Timer.read_ms() || abs(ball.ang) < 30 || 100 < abs(ball.ang)){
      c = 0;
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


  if(setplay_flag == 1){
    target += 45;
    if(1000 < play_time.read_ms()){
      setplay_flag = 0;
    }
  }
  else if(setplay_flag == 2){
    target -= 45;
    if(1000 < play_time.read_ms()){
      setplay_flag = 0;
    }
  }
  else if(setplay_flag == 3){
    target = first_ang;
    Serial.print(" !!!! ");
    if(1000 < play_time.read_ms()){
      setplay_flag = 0;
    }
  }

  int go_front_flag = 0;
  if(A == 10 || A == 11){
    if(abs(go_ang.degree) < 30){
      go_front_flag = 1;
    }
    else if(40 < abs(go_ang.degree) && abs(go_ang.degree) < 120){
      go_front_flag = 2;
    }
  }
  go_front.enterState(go_front_flag);
  if(200 < go_front.readStateTimer(1)){
    if(500 < go_front.readStateTimer(1)){
      max_val -= 30;
    }
    if(A == 10 && 600 < Timer.read_ms()){
      max_val -= 10;
    }
  }
  else if(400 < go_front.readStateTimer(2)){
    // max_val = 150;
  }

  // rake.enterState(rake_flag);
  // if(rake.readStateTimer(0) < 500 && 1000 < play_time.read_ms() && A == 11){
  //   target += 60;
  // }
  ac.dir_target = target;
  if(AC_flag == 0 || rake_flag){
    AC_val = ac.getAC_val();
  }
  else if(AC_flag == 1){
    AC_val = ac.getCam_val(-cam_front.ang) * AC_D;
    // Serial.print(" AC_val : ");
    // Serial.print(AC_val);
    // Serial.println();
  }

  kicker.run(kick_);
  // Serial.print(" A : ");
  // Serial.print(A);
  // ac.print();
  // Serial.print(" maxval : ");
  // Serial.print(max_val);
  // Serial.print(" AC_flag : ");
  // Serial.print(AC_flag);
  // Serial.print(" AC_val : ");
  // Serial.print(AC_val);
  // cam_front.print();
  // Serial.print(" | ");
  // Serial.print(" rake : ");
  // Serial.print(rake_flag);
  // Serial.print(" max_val : ");
  // Serial.print(max_val);
  // Serial.print(" setplay : ");
  // Serial.print(setplay_flag);
  // Serial.print(" first_dir : ");
  // Serial.print(first_ang);
  // Serial.println();

  if(back_flag == 1){
    max_val = go_val_back;
  }

  if(M_flag == 1){
    if(AC_flag == 1){
      MOTOR.moveMotor_0(go_ang,max_val,AC_val,1);
    }
    else{
      MOTOR.moveMotor_0(go_ang,max_val,AC_val,0);
    }
  }
  else if(M_flag == 0){
    MOTOR.motor_0();
  }

  if(MOTOR.NoneM_flag){
    // OLED_moving();
  }
}