#include<defence.h>

void Diffence::available_set(){
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

void Diffence::defence(){
  angle go_ang(ball.ang,true);         //進む角度のオブジェクト
  GOD = 0;

  float AC_val = 100;                  //姿勢制御の出力
  int max_val = go_val;                //進む出力
  float target = ac_tirget;           //目標角度

  int AC_flag = 0;                     //0だったら絶対的な角度とる 1だったらゴール向く
  int kick_ = 0;                       //0だったらキックしない 1だったらキック
  int M_flag = 1;                      //1だったら動き続ける 0だったら止まる

  /*---------------------------------------------------------状況判断ステート--------------------------------------------------------*/

  c = 0;

  if((45 < abs(line.ang) && abs(line.ang) < 135) && cam_back.on == 1 && cam_back.Size < 60){  //横向きにラインを踏んでるフラグ
    Lside_A = 1;
  }
  else{
    Lside_A = 0;
  }

  if(Lside_A == 1){          //(横向きにラインを踏み続けているか<=>コートの横向きのライン上にいるか)判定
    if(Lside_A != Lside_B){
      Lside_B = Lside_A;
      L_.reset();
    }
    if(1500 < L_.read_ms()){
      A = 15;
      c = 1;
      line_F = 1;
    }
  }
  else{
    if(Lside_A != Lside_B){
      Lside_B = Lside_A;
    }
  }

  if(back_Flag == 1 && line.LINE_on == 0){
    A = 15;
    Serial.print(" 15_1 !!!!!!!!!!!!!!!!!!! ");
    c = 0;
    line_F = 1;
  }

  if(cam_back.on == 1){
    CB_old = 1;
  }
  else{
    if(cam_back.on != CB_old){
      CB_old = cam_back.on;
      CB_t.reset();
    }
    if(1000 < CB_t.read_ms()){
      if(line.LINE_on == 0 || (abs(line.ang) < 15 || 165 < abs(line.ang))){
        A = 16;
      }
    }
  }

  if(sentor_A == 1){          //前にボールがあり続けるか判定
    if(sentor_A != sentor_B){
      sentor_B = sentor_A;
      sentor_t.reset();
    }
    if(3000 < sentor_t.read_ms()){
      A = 11;
      sentor_t.reset();
      sentor_A = 0;
    }
  }
  else if(sentor_A == 0){
    if(sentor_A != sentor_B){
      sentor_B = sentor_A;
    }
  }
  else if(sentor_A == 3){
    if(sentor_A != sentor_B){
      sentor_B = sentor_A;
      sentor_t.reset();
    }
    if(300 < sentor_t.read_ms() && 2000 < A_12_t.read_ms()){
      A = 12;
      Timer.reset();
      sentor_t.reset();
      sentor_A = 0;
    }
  }

  if(A == 11){     //前進し続けるか判定
    if(200 < sentor_t.read_ms() && kicker.kick_flag == 0){
      A = 15;
      c = 1;
      sentor_t.reset();
      if(line.LINE_on == 1){
        line_F = 1;
      }
      else{
        line_F = 2;
      }
    }
    else{
      c = 1;
    }

    if((300 < sentor_t.read_ms() && line.LINE_on == 1) || 75 < abs(ball.ang)){
      A = 15;
      c = 1;
      line_F = 1;
    }
  }

  if(A == 12){
    if(200 < Timer.read_ms() || 90 < abs(ball.ang)){
      if(line.LINE_on == 0){
        A = 15;
        c = 1;
      }
    }
    else{
      c = 1;
    }
  }

  if(A == 15){      //戻るとき後ろに下がり続けるか判定
    if(line_F != 4){
      c = 1;
    }
    else{
      line_F = 0;
    }
  }

  if(A == 20){
    if(500 < Timer.read_ms()){
      if(90 < abs(line.ang)){
        A = 15;
        c = 1;
      }
    }
  }

  if(c == 0){  //平常時どうするか判定
    if(line.LINE_on == 1){
      if(140 < abs(ball.ang) && cam_back.on == 1){
        A = 5;
      }
      else{
        A = 10;
      }
    }
    else{
      A = 20;
    }

    // if(cam_back.on == 0){
    //   if(!(15 < abs(line.ang) && abs(line.ang) < 165)){
    //     A = 16;
    //   }
    // }
  }


  /*---------------------------------------------------------動作ステート--------------------------------------------------------*/
  sentor_A = 0;


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

    if(140 < abs(go_ang.degree)){       //進む角度が真後ろにあるとき
      go_ang += 180;
      back_F = 1;
    }
    else if(130 < abs(go_ang.degree)){
      M_flag = 0;
    }
    else if(115 < abs(go_ang.degree)){
      max_val -= 60;
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

    sentor_A = 0;
    for(int i = 0; i < 2; i++){
      int dif_val = abs(ball.ang - go_border[i]);
      if(dif_val < stop_range && back_F == 0){  //正面方向にボールがあったら停止するよ
        if(ball.vec_velocity.getMagnitude() < 20 && ball.vec_acc.getMagnitude() < 20){
          M_flag = 0;
          max_val = 0;
          Serial.print(" STOP ");
          Serial.print(" V : ");
          Serial.print(ball.vec_velocity.getMagnitude());
          Serial.print(" A : ");
          Serial.print(ball.vec_acc.getMagnitude());
        }
        else{
          Serial.print(" STOP_NO ");
          Serial.print(" V : ");
          Serial.print(ball.vec_velocity.getMagnitude());
          Serial.print(" A : ");
          Serial.print(ball.vec_acc.getMagnitude());
        }
      }
    }

    if(160 < abs(ball.far) && abs(ball.ang) < 45 && ball.vec_velocity.getMagnitude() < 20){
      sentor_A = 3;
    }

    if(push_flag){
      if(ac.dir < 0){
        if(go_ang.degree < 0){
        }
        else{
          go_ang += 180;
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
    go_ang.to_range(180,true);  //進む角度を-180 ~ 180の範囲に収める
  }



  if(A == 11){  //ボールが前にあるから前進
    if(A != B){
      B = A;
      goang_old = ball.ang;
    }
    max_val += 30; 
    if(abs(ball.ang) < 10){
      go_ang = 0;
    }
    else{
      go_ang = ball.ang * 3;
    }
    if(ball.ball_get){
      if(ball.ball_get != Bget_B){
        Bget_B = ball.ball_get;
        Timer.reset();
      }
      if(300 < Timer.read_ms()){
        kick_ = 1;
        AC_flag = 1;
      }
    }
    else{
      Bget_B = 0;
    }

    if(30 < abs(ball.ang - goang_old)){
      A = 15;
    }
    M_flag = 2;
  }


   if(A == 12){
    if(A != B){
      B = A;
      Timer.reset();
      A_12_t.reset();
    }
    go_ang = ball.ang;
    M_flag = 2;
  }



  if(A == 15){  //ゴール際に戻る
    if(A != B){
      B = A;
      Timer.reset();
      line_F = 1;
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
  }


  if(A == 16){
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
  }



  /*---------------------------------------------------------出力ステート--------------------------------------------------------*/


  back_Flag = 0;
  ac.dir_target = target;
  push_flag = 0;

  if(30 < abs(ac.dir)){
    AC_val = ac.getAC_val() * 1.5;
    if(line.LINE_on == 0){
      if(abs(line.ang_old) < 90){
        back_Flag = 0;
      }
      else{
        back_Flag = 1;
        M_flag = 0;
      }
    }
    else{
      if(ball.ball_get){
        if(abs(line.ang_old) < 90){
          back_Flag = 0;
        }
        push_flag = 1;
        M_flag = 1;
      }
    }
  }
  else if(AC_flag == 0){
    AC_val = ac.getAC_val();
  }
  else if(AC_flag == 1){
    AC_val = ac.getCam_val(cam_front.ang);
  }


  kicker.run(kick_);
  // M_flag = 3;


  if(M_flag == 1){
    MOTOR.moveMotor_L(go_ang,max_val,AC_val,line);
  }
  else if(M_flag == 0){
    MOTOR.motor_ac(AC_val);
  }
  else if(M_flag == 2){
    MOTOR.moveMotor_0(go_ang,max_val,AC_val,0);
  }
  else if(M_flag == 3){
    MOTOR.motor_0();
  }
}