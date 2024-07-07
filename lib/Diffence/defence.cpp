#include<defence.h>

void Diffence::available_set(){
  go_val = val_max;
}

int Diffence::get_A(){
  return A;
}

byte* Diffence::get_flag(){
  return_num[0] = ball.vec_velocity.getMagnitude();
  return_num[1] = Stop_flag;
  return_num[2] = A;
  return_num[3] = 99; 
  return return_num;
}

void Diffence::defence(){
  angle go_ang(ball.ang,true);         //進む角度のオブジェクト

  float AC_val = 100;                  //姿勢制御の出力
  int max_val = go_val;                //進む出力
  float target = ac_tirget;           //目標角度

  int AC_flag = 0;                     //0だったら絶対的な角度とる 1だったらゴール向く
  int kick_ = 0;                       //0だったらキックしない 1だったらキック
  int M_flag = 1;                      //1だったら動き続ける 0だったら止まる
  Stop_flag = 0;
  Lside_A = 0;

  /*---------------------------------------------------------状況判断ステート--------------------------------------------------------*/


  if(back_Flag == 1 && line.LINE_on == 0){  //角度がある程度あるかつラインの外だからゴールのほうに戻るよ
    A = 15;
    c = 0;
    line_F = 1;
  }

  Camback_on.enterState(cam_back.on);

  if(1000 < Camback_on.readStateTimer(0)){
    if(line.LINE_on == 0 || (abs(line.ang) < 15 || 165 < abs(line.ang))){
      A = 16;  //なんだこれ　自分にも分らん
    }
  }

  if(c == 0){  //平常時どうするか判定
    if(line.LINE_on == 1){
      if(ball.flag == 0 || (120 < abs(ball.ang) && abs(ball.ang) < 172 && cam_back.senter && cam_back.on)){  //ボールがないまたはゴールの端にいるときとまる
        A = 5;
      }
      else{
        A = 10;
      }
    }
    else{
      A = 20;
    }
  }


  /*---------------------------------------------------------動作ステート--------------------------------------------------------*/
  Center_A = 0;


  if(A == 5){  //ボールがない時止まる
    if(A != B){
      B = A;
    }
    M_flag = 3;
  }



  if(A == 10){  //ライントレース(アルゴリズムブログで書きたいな)
    if(A != B){
      B = A;
    }
    int go_flag = 0;
    double go_border[2];  //ボールの角度によって進む方向を変えるためのボーダーの変数(ラインに対して垂直な直線で進む角度の区分を分けるイメージ)
    angle balldir(ball.ang,true);  //ボールの角度を入れるオブジェクト
    if(2 < line.num){
      line.ang = 90;
    }

    if(line.ang < 0){
      go_border[0] = line.ang;
      go_border[1] = line.ang + 180;
    }
    else{
      go_border[0] = line.ang - 180;
      go_border[1] = line.ang;
    }

    balldir.to_range(go_border[0],false);  //ボールの角度をボーダーの範囲に収める(go_border[0] ~ go_border[1]+180)

    if(go_border[0] < balldir.degree && balldir.degree < go_border[1]){  //ボールの角度を区分分けする
      go_flag = 0;
    }
    else{
      go_flag = 1;
    }

    go_ang = go_border[go_flag] + 90;  //進む角度決定
    go_ang.to_range(180,true);  //進む角度を-180 ~ 180の範囲に収める

    int back_F = 0;

    if(165 < abs(go_ang.degree)){       //進む角度が真後ろにあるとき
      // go_ang += 180;
      back_F = 1;
    }
    else if(150 < abs(go_ang.degree)){
      M_flag = 0;
    }
    else if(115 < abs(go_ang.degree)){
      MOTOR.line_val = 1.5;
    }
    else if(abs(go_ang.degree) < 60){  //前めに進むとき
      MOTOR.line_val = 2;
    }
    else{                              //横に進むとき
      MOTOR.line_val = 1.0;
      if(cam_back.on == 0){
        if(cam_back.ang < 0){
          go_ang = -90;
        }
        else{
          go_ang = 90;
        }
      }
    }

    Center_A = 0;
    for(int i = 0; i < 2; i++){
      int dif_val = abs(ball.ang - go_border[i]);
      if(dif_val < stop_range && back_F == 0){  //正面方向にボールがあったら停止するよ
        if(20 < ball.vec_velocity.getMagnitude()){
          Stop_flag = 2;  //ボールの速度を原因にストップしてないフラグ
        }
        else{
          M_flag = 0;
          max_val = 0;
          Stop_flag = 1;  //普通に止まるフラグ
        }
      }
    }


    if(BALL_MAX_NUM * 1.5 < abs(ball.far) && abs(ball.ang) < 60){  //ぼーるが近くにあったら小突くやつ
      Center_A = 3;
    }

    if(push_flag){  //ロボットが押し込まれてたら
      if(ac.dir < 0){
        if(go_ang.degree < 0){
        }
        else{
          go_ang += 180;  //ロボットの動きを受け流す(意図はわからん)
        }
      }
      else{
        if(0 < go_ang.degree){
        }
        else{
          go_ang += 180;
        }
      }
      M_flag = 1;
    }

    Center.enterState(Center_A);

    if(300 < Center.readStateTimer(3) && 2000 < A_12_t.read_ms()){
      A = 12;
      c = 1;
      Center.enterState(0);
    }

    go_ang.to_range(180,true);  //進む角度を-180 ~ 180の範囲に収める
  }




  if(A == 12){  //ボールを小突くやつ
    if(A != B){
      B = A;
      Timer.reset();
      A_12_t.reset();
    }
    go_ang = ball.ang * 2;
    M_flag = 2;

    if(ball.ball_get){
      A = 13;
    }
    if(500 < Timer.read_ms()){
      A = 15;
    }
  }


  if(A == 13){
    if(A != B){
      B = A;
      Timer.reset();
    }
    go_ang = 0;
    M_flag = 2;
    AC_flag = 1;

    if(200 < Timer.read_ms()){
      kick_ = 1;
    }
    if(450 < Timer.read_ms() || line.LINE_on){
      A = 15;
      Lside_A = 1;
      c = 1;
    }
  }



  if(A == 15){  //ゴール際に戻る
    if(A != B){
      B = A;
      Timer.reset();
      line_F = 2;
      if(Lside_A == 1){
        line_F = 1;
      }
    }
    go_ang = -cam_back.ang + 180;
    M_flag = 2;
    max_val -= 30;
    if(line_F == 1){
      if(line.LINE_on == 0){
        line_F++;
      }
      // digitalWrite(LED,LOW);
    }
    if(line_F == 2){
      if(line.LINE_on == 1 && 200 < Timer.read_ms()){
        line_F++;
      }
      // digitalWrite(LED,HIGH);
    }
    if(line_F == 3){
      if(300 < Timer.read_ms()){
        line_F++;
      }
      go_ang = 0;
      // digitalWrite(LED,LOW);
    }

    if(line_F == 4){
      c = 0;
    }
  }


  if(A == 16){  //ラインがないときにゴールのほうに向く
    if(A != B){
      B = A;
    }
    go_ang = 0;
    M_flag = 2;
  }



  if(A == 20){  //ロボットがちょっと押し出されちゃったから戻る
    if(A != B){
      B = A;
      Timer.reset();
    }
    go_ang = line.ang_old;
    M_flag = 2;

    if(500 < Timer.read_ms()){
      if(90 < abs(line.ang_old) || back_Flag){
        A = 15;
        c = 1;
      }
    }
  }



  /*---------------------------------------------------------出力ステート--------------------------------------------------------*/


  back_Flag = 0;
  ac.dir_target = target;
  push_flag = 0;

  if(30 < abs(ac.dir)){  //ロボットが傾いてたら
    AC_val = ac.getAC_val() * 1.5;
    if(line.LINE_on == 0){
      if(abs(line.ang_old) < 90){
        if(90 < abs(ac.dir)){
          back_Flag = 1;  //ラインの外で傾いてたらゴールのほうに戻る
        }
        else{
          back_Flag = 0;  //ラインの外で傾いてなかったらラインのほうに戻る
        }
      }
      else{
        back_Flag = 1;  //ラインの上で傾いてたらゴールのほうに戻る
        M_flag = 0;
      }
    }
    else{
      if(ball.ball_get){  //ボールを持ってたら
        if(abs(line.ang_old) < 90){
          back_Flag = 0;  //ラインの外で傾いてたらラインのほうに戻る
        }
        push_flag = 1;  //ボールを持ってたら押し込まれたときの処理
        M_flag = 1;
      }
    }
  }
  else if(AC_flag == 0){
    AC_val = ac.getAC_val();
  }
  else if(AC_flag == 1){
    AC_val = ac.getCam_val(-cam_front.ang) * 1.2;
  }


  kicker.run(kick_);
  // Serial.print(" A : ");
  // Serial.print(A);
  // Serial.println();
  // M_flag = 3;


  if(M_flag == 1){
    MOTOR.moveMotor_L(go_ang,max_val,AC_val,line);
  }
  else if(M_flag == 0){
    MOTOR.motor_ac(AC_val);
  }
  else if(M_flag == 2){
    if(AC_flag){
      MOTOR.moveMotor_0(go_ang,max_val,AC_val,1);
    }
    else{
      MOTOR.moveMotor_0(go_ang,max_val,AC_val,0);
    }
  }
  else if(M_flag == 3){
    MOTOR.motor_0();
  }
}