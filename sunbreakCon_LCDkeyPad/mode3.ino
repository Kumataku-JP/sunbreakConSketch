/*  モンハンライズサンブレイク  自動放置
 *  
 *  概要／
 *  自動で各種コマンドを自動で進めるプログラム
 *  
 *  種類／
 *  1.  闘技場を狩猟笛で放置しクリアするマクロ 体力回復(大)=初期値x旋律
 *  2.  極泉郷を狩猟笛で放置しクリアするマクロ 体力回復(大)=初期値x旋律
 *  3.  塔の秘境を狩猟笛で放置しクリアするマクロ 体力回復(大)=初期値x旋律
 *    ※ 実行中にボタン  上=X旋律、右=A旋律、左=XA旋律、下=多頭クエスト用（ターゲット変更）
 *  4.  Aボタンのリピート(2秒間隔)単純なマクロ
 *    ※ 実行中に下を押すと採集に移動を含めたマクロ
 *  
*/

void mode3() {

  if (value == 0) {
    lcd.setCursor(0, 0);
    displayString(value, 3);  //SelectAutoTyp>LR
  }

  keys = read_LCD_buttons(analogRead(0));

  /*  オートプレイ  */
  //オート選択
  if (!dateSet && !steyMode3 && keys == btnRIGHT) {
    value++;
    if (value > 4) value = 1;  // 1から4へトグル
    lcd.clear();
    lcdAuto();  //LCD表示
    lcd.setCursor(11, 1);
    lcd.print("SRT>S");
    delay(250);
  }
  if (!dateSet && !steyMode3 && keys == btnLEFT) {
    value--;
    if (value < 1) value = 4;  // 4から1へトグル
    lcd.clear();
    lcdAuto();  //LCD表示
    lcd.setCursor(11, 1);
    lcd.print("SRT>S");
    delay(250);
  }

  if (keys == btnSELECT && keysOld == btnNONE && value != 0) {
    steyMode3 = !steyMode3;
    if (steyMode3) {
      dateSet = true;
      lcd.setCursor(11, 1);
      lcd.print("STP>S");
    } else {
      dateSet = false;
      lcd.setCursor(11, 1);
      lcd.print("SRT>S");
    }
    delay(250);
  }
  //オート実行
  // steyModeがtrueの時に上下ボタンでtargetOnを切り替え
  if (steyMode3 && keys == btnDOWN && keysOld == btnNONE && value >= 1 && value <= 4) targetOn = !targetOn;
  // steyModeがtrueの時に上左右ボタンでmelodyTypeを切り替え
  if (steyMode3 && keys == btnUP && value >= 1 && value <= 3) melody = 0;     // X旋律
  if (steyMode3 && keys == btnRIGHT && value >= 1 && value <= 3) melody = 1;  // A旋律
  if (steyMode3 && keys == btnLEFT && value >= 1 && value <= 3) melody = 2;   // XA旋律
  keysOld = keys;

  if (value != 0 && steyMode3) {
    if (!isFirstRun) {
      if (!reStart) {
        startTimeS = millis();  // stopTimeが60秒経過しているかどうかをチェックしてから初期化
        startTime = millis();   // startTime の初期化
        elapsed = 0;
        referenceTime = millis();  // referenceTime を現在の millis() に設定
      }
      if (startTimeS == 0) {
        startTimeS = millis();  // 接続時の最初は初期化処理
        startTime = millis();   // startTime の初期化
        elapsed = 0;
        referenceTime = millis();  // referenceTime を現在の millis() に設定
      }
      startTimeA = millis();  // startTimeA の初期化
      startTimeB = millis();  // startTimeB の初期化
      runStop = false;
    }
    if (value == 1) {
      lcdMelody();
      elapsedTime();  //:LCD表示
      autoArenaX();
      if (targetOn) target();
    }
    if (value == 2) {
      lcdMelody();
      elapsedTime();  //:LCD表示
      autoInfernal();
      if (targetOn) target();
    }
    if (value == 3) {
      lcdMelody();
      elapsedTime();  //:LCD表示
      autoForlorn();
      if (targetOn) target();
    }
    if (value == 4) {
      lcdItem();      // 特産品LCD
      elapsedTime();  //:LCD表示
      repeatA();
      if (targetOn) repeatMove();  // 2分置きに移動
    }
  }

  //ストップ
  if (value != 0 && !steyMode3) {
    if (!runStop) {
      leftStickNeutral();
      if (value >= 1 && value <= 3) {
        lcd.setCursor(10, 0);
      } else {
        lcd.setCursor(9, 0);
      }
      lcd.print("STOP");
      lcd.setCursor(11, 1);
      lcd.print("SRT>S");
      dateSet = false;
      isFirstRun = false;
      skipExec = false;
      runStop = true;
      times = 0;
      stopTime = millis();  // ストップ時にstopTimeを計測開始
      delay(400);
    }
    if (millis() - stopTime > 60000) {
      reStart = false;  // ストップから60秒未満中
    } else {
      reStart = true;  // ストップから60秒経過中
      startTime = 0;   // ストップして60秒経過でstartTimeをリセット
    }
  }
}

/* LCD表示 */
void lcdAuto() {
  lcd.setCursor(0, 1);
  lcd.print("M");
  lcd.print(mode);
  lcd.print(".");
  displayString(mode, 0);  // Auto
  lcd.setCursor(0, 0);
  if (value >= 1 && value <= 3) {
    lcd.print(value);
    lcd.print(".");
  }
  displayString(value, 3);
  lcd.setCursor(11, 1);
  lcd.print("SRT>S");
}
/* 旋律LCD */
void lcdMelody() {
  lcd.setCursor(3, 1);
  displayString(melody, 6);  //旋律タイプ
}
/* 特産品LCD */
void lcdItem() {
  lcd.setCursor(3, 1);
  if (!targetOn) {
    if (languageFlag == 0) lcd.print("RepeatA");
    else if (languageFlag == 1) lcd.print(jp("ﾘﾋﾟｰﾄA  "));
  }
  if (targetOn) {
    if (languageFlag == 0) lcd.print("OnTheGo");
    else if (languageFlag == 1) lcd.print(jp("ｲﾄﾞｳｻｲｼｭ"));
  }
}
/* 経過時間表示 */
void elapsedTime() {
  unsigned long currentMillis = millis();  // 現在の時間を取得
  // 1秒ごとに点滅を切り替える
  if (currentMillis - previousMillis >= interval) {
    colonVisible = !colonVisible;    // 表示状態を反転させる
    previousMillis = currentMillis;  // 前回の時間を更新
  }
  // 経過時間を計算
  unsigned long elapsed = currentMillis - referenceTime;
  unsigned long minutes = elapsed / 60000;  // 経過時間を分に変換
  // 経過時間表示位置
  if (value >= 1 && value <= 3) {
    char text[3];
    lcd.setCursor(10, 0);
    if (targetOn) lcd.print("TGT");
    else lcd.print("RUN");
    // コロンの表示/非表示を切り替える
    if (colonVisible) lcd.print(":");
    else lcd.print(" ");
    sprintf(text, "%2lu", minutes);  // 経過時間を表示
    lcd.print(text);
  } else {
    char text[4];
    lcd.setCursor(9, 0);
    lcd.print("RUN");
    // コロンの表示/非表示を切り替える
    if (colonVisible) lcd.print(":");
    else lcd.print(" ");
    sprintf(text, "%3lu", minutes);  // 経過時間を表示
    lcd.print(text);
  }
}
