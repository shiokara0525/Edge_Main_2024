#include<Attack.h>


void Attack::available_set(int *check_val){
  RA_a = Values[0] / 100.0;
  RA_b = Values[1] / 100.0;
  Serial.printf(" RA_a : %f RA_b : %f\n",RA_a,RA_b);
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
          if(1 <= ball.ball_get){
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
            go_flag = 1;  //ライン際でボールにあてに行く
          }
        }
        else{
          if(-30 < ball.ang && ball.ang < 90){
            go_flag = 1;  //ライン際でボールにあてに行く
          }
        }
      }
      B = A;
      Timer.reset();
    }

    if(90 < abs(ball.ang)){
      go_flag = 0;
    }

    float confidencial_num = (ball.vec.getMagnitude() - 130) * 0.02;

    if(abs(ball.ang) < 15){
      go_ang = abs(ball.ang);
    }
    else if(abs(ball.ang) < 45){
      go_ang = abs(ball.ang) * RA_b;
    }
    else if(abs(ball.ang) < 90){
      if(130 <= ball.vec.getMagnitude() && ball.vec.getMagnitude() < 180){
        go_ang = (confidencial_num * (RA_a - 1) + 1) * abs(ball.ang) + (1 - confidencial_num) * 45;
      }
      else if(ball.vec.getMagnitude() < 130){
        go_ang = abs(ball.ang) + 45;
      }
      else{
        go_ang = abs(ball.ang) * RA_a;
      }
    }
    else{
      if(130 <= ball.vec.getMagnitude() && ball.vec.getMagnitude() < 180){
        go_ang = abs(ball.ang) + (confidencial_num + 1) * 45.0;
      }
      else if(ball.vec.getMagnitude() < 130){
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

    if(cam_front.on == 1){  //カメラ見てるとき
      if(abs(cam_front.ang) < 20 && 15 < cam_front.Size){  //正面にゴールあってゴールもある程度近くにある時
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
      kick_ = 1;
    }

    if(cam_front_on == 1){  //打っていいよフラグ
      max_val -= 15;
      if(cam_front_on != CFO_B){
        CFO_B = cam_front_on;
        CFO_t.reset();  //RIZINGでタイマーリセット
      }

      if(200 < CFO_t.read_ms()){
        kick_ = 1;  //打っていいよフラグが0.2秒立ってたら打つ
      }
      if(40 < cam_front.Size){
        kick_ = 1;  //ゴールが近い時は問答無用で打つ
      }
    }
    else if(cam_front_on == 0){
      CFO_B = 0;
    }

    if(1750 < Timer.read_ms()){
      rake_flag = 0;
      Timer.reset();
    }
    else if(1500 < Timer.read_ms()){
      rake_flag = 1;
      target += 90;
      AC_flag = 0;
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
            A = 22;  //ボールを押し込むやつ 
            c = 1;
          }
        }
        else{  //notゴール前だったら
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
    Serial.print("!!!!!!!!!!!!!!!");
    if(A != B){
      B = A;
      Timer.reset();
    }
    max_val = 150;
    go_ang = 0;
  
    if((line.LINE_on == 1 && (line.dis_X < -1.65 || (30 <= abs(line.ang) && abs(line.ang) <= 150))) || 5000 < Timer.read_ms() || 60 < abs(ball.ang)){
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
      max_val = 180;
    }
    else{
      M_flag = 0;
    }


    if(45 < abs(ball.ang) || 7000 < Timer.read_ms() || line.LINE_on){
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
  if(AC_flag == 0 || rake_flag){
    AC_val = ac.getAC_val();
  }
  else if(AC_flag == 1){
    AC_val = ac.getCam_val(-cam_front.ang) * 1.5;
  }

  kicker.run(kick_);
  // Serial.print(" A : ");
  // Serial.print(A);
  // Serial.print(" AC_flag : ");
  // Serial.print(AC_flag);
  // Serial.print(" AC_val : ");
  // Serial.print(AC_val);
  // Serial.print(" | ");
  // Serial.print(" rake : ");
  // Serial.print(rake_flag);
  // Serial.print(" max_val : ");
  // Serial.print(max_val);

  if(back_flag == 1){
    max_val = go_val_back;
  }

  if(M_flag == 1){
    if(AC_flag){
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