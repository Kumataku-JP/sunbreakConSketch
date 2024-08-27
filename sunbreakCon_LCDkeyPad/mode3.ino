/*  モンハンライズサンブレイク  自動放置
 *  
 *  概要／
 *  自動で各種コマンドを自動で進めるプログラム
 *  
 *  種類／
 *  1.  闘技場を狩猟笛で放置しクリアするマクロ 体力回復(大)=初期値X旋律
 *  2.  極泉郷を狩猟笛で放置しクリアするマクロ 体力回復(大)=初期値X旋律
 *  3.  塔の秘境を狩猟笛で放置しクリアするマクロ 体力回復(大)=初期値X旋律
 *  4.  クエスト選択から始める場合のマクロ 闘技場のみ対応 討伐後selectボタンで停止し1分後に再開可能
 *    ※ 実行中にボタン  上=X旋律、右=A旋律、左=XA旋律、下=多頭クエスト用（ターゲット変更）
 *  5.  Aボタンのリピート(2秒間隔)単純なマクロ
 *    ※ 実行中に下を押すと採集に移動を含めたマクロ
 *  
*/

void mode3() {

  keys = read_LCD_buttons(analogRead(0));

  /*  オートプレイ  */
  //オート選択
  if (!steyMode3) {
    if (!dateSet && (keys == btnRIGHT || keys == btnLEFT)) {
      value += (keys == btnRIGHT) ? 1 : -1;
      if (value > 5) value = 1;  // 1から5へトグル
      if (value < 1) value = 5;  // 5から1へトグル
      lcd.clear();
      lcdAuto();                // LCD表示
      if (value != 4) prg = 0;  // 4を途中でストップした時にprgを初期値に戻す
      delay(250);
    }
  }
  if (keys == btnSELECT && keysOld == btnNONE && value != 0) {
    steyMode3 = !steyMode3;
    dateSet = steyMode3;
    lcd.setCursor(11, 1);
    lcd.print(steyMode3 ? "STP>S" : "SRT>S");
    delay(250);
  }
  /* オート実行中のモーション切り替え */
  if (steyMode3) {
    // steyModeがtrueの時に上下ボタンでtargetOnを切り替え
    if (keys == btnDOWN && keysOld == btnNONE && value >= 1 && value <= 5) targetOn = !targetOn;
    // steyModeがtrueの時に上左右ボタンでmelodyTypeを切り替え
    if (keys == btnUP && value >= 1 && value <= 4) melody = 0;     // X旋律
    if (keys == btnRIGHT && value >= 1 && value <= 4) melody = 1;  // A旋律
    if (keys == btnLEFT && value >= 1 && value <= 4) melody = 2;   // XA旋律
  }
  keysOld = keys;

  if (value != 0 && steyMode3) {
    if (!isFirstRun) {
      // stopTimeが60秒経過しているかどうかをチェックしてから初期化
      if (!reStart || startTimeS == 0) {
        startTimeS = startTime = referenceTime = millis();
        elapsed = 0;
      }
      //　startTimeA startTimeB の初期化
      startTimeA = startTimeB = millis();
      runStop = false;
    }
    switch (value) {
      case 1:
        lcdMelody();
        elapsedTime();  // LCD表示
        autoArena();
        if (targetOn) target();
        break;
      case 2:
        lcdMelody();
        elapsedTime();  // LCD表示
        autoInfernal();
        if (targetOn) target();
        break;
      case 3:
        lcdMelody();
        elapsedTime();  // LCD表示
        autoForlorn();
        if (targetOn) target();
        break;
      case 4:
        lcdCruise();
        autoArenaCruising();
        if (prg == 6) {
          lcdMelody();
          elapsedTime();  // LCD表示
          autoArena();
          if (targetOn) target();
        }
        break;
      case 5:
        lcdItem();      // 特産品LCD
        elapsedTime();  // LCD表示
        repeatA();
        if (targetOn) repeatMove();  // 2分置きに移動
        break;
    }
  }

  //ストップ
  if (value != 0 && !steyMode3) {
    if (!reStart) {
      prg = 0;
      if (value >= 1 && value <= 4) {
        lcd.setCursor(0, 0);
        lcd.print(value);
        lcd.print(".");
        displayString(value, 3);
        lcd.print("          ");
        lcd.setCursor(10, 1);
        lcd.print(" SRT>S");
      }
    }
    if (!runStop) {
      leftStickNeutral();
      lcd.setCursor(value >= 1 && value <= 4 ? 10 : 9, 0);  // 決戦場と特産品の場合とのLCD位置の調整
      lcd.print("STOP");
      lcd.setCursor(10, 1);
      lcd.print("ReST>S");
      dateSet = false;
      isFirstRun = false;
      skipExec = false;
      runStop = true;
      times = 0;
      stopTime = millis();  // ストップ時にstopTimeを計測開始
      delay(400);
    }
    reStart = (millis() - stopTime <= 60000);  // ストップから60秒経過中
    if (!reStart) startTime = 0;               // ストップして60秒経過でstartTimeをリセット
  }
}

/* 準備LCD */
void lcdCruise() {
  lcd.setCursor(2, 0);
  displayString(value, 3);  // ArenaAuto
  lcd.setCursor(3, 1);
  if (prg == 0) lcd.print("Plaza.  ");
  else if (prg == 1) lcd.print("Quest.. ");
  else if (prg == 2) lcd.print("TeaShop.");
  else if (prg == 3) lcd.print("Dango.. ");
  else if (prg == 4) lcd.print("Loading.");
  else if (prg == 5) lcd.print("Arena.. ");
}
/* 旋律LCD */
void lcdMelody() {
  lcd.setCursor(3, 1);
  displayString(melody, 6);  //旋律タイプ
}
/* 特産品LCD */
void lcdItem() {
  lcd.setCursor(3, 1);
  const char* text;
  if (languageFlag == 0) text = targetOn ? "OnTheGo" : "RepeatA";
  else if (languageFlag == 1) text = targetOn ? jp("ｲﾄﾞｳｻｲｼｭ") : jp("ﾘﾋﾟｰﾄA  ");
  lcd.print(text);
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
  lcd.setCursor((value >= 1 && value <= 4) ? 10 : 9, 0);
  lcd.print(targetOn && (value >= 1 && value <= 4) ? "TGT" : "RUN");
  lcd.print(colonVisible ? ":" : " ");
  char text[4];
  sprintf(text, (value >= 1 && value <= 4) ? "%2lu" : "%3lu", minutes);  // 経過時間を表示
  lcd.print(text);
}

/* LCD表示 */
void lcdAuto() {
  lcd.setCursor(0, 1);
  lcd.print("M");
  lcd.print(mode);
  lcd.print(".");
  displayString(mode, 0);  // Auto
  lcd.setCursor(0, 0);
  if (value >= 1 && value <= 4) {
    lcd.print(value);
    lcd.print(".");
  }
  displayString(value, 3);
  lcd.setCursor(11, 1);
  lcd.print("SRT>S");
}