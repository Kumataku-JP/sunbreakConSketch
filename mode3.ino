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
 *  ※ 実行中にボタン  上=X旋律、右=A旋律、左=XA旋律、下=多頭クエスト用（ターゲット変更）
 *  5.  Aボタンのリピート(2秒間隔)単純なマクロ
 *    ※ 実行中に下を押すと採集に移動を含めたマクロ
 *  
*/

void mode3() {

  keys = read_LCD_buttons(analogRead(0));

  /*  オートプレイ  */
  //オート選択
  if (!runMode) {
    if ((keys == btnRIGHT || keys == btnLEFT) && !setupMode) {
      value += (keys == btnRIGHT) ? 1 : -1;
      value = toggleValue(value, 1, 5);  // 1-5トグル
      lcd.clear();
      lcdAuto();                // LCD表示
      if (value != 4) prg = 0;  // 4を途中でストップした時にprgを初期値に戻す
      delay(300);
    }
  }
  if (keys == btnSELECT && keysOld == btnNONE && value != 0) {
    runMode = !runMode;
    setupMode = runMode;
    lcd.clear();
    lcdAuto();  // LCD表示
    lcd.setCursor(11, 1);
    lcd.print(runMode ? "STP>S" : "SRT>S");
    delay(250);
  }
  /* オート実行中のモーション切り替え */
  if (runMode) {
    // runModeがtrueの時に上下ボタンでtargetOnを切り替え
    if (keys == btnDOWN && keysOld == btnNONE && value >= 1 && value <= 5) targetOn = !targetOn;
    // runModeがtrueの時に上左右ボタンでmelodyTypeを切り替え
    if (keys == btnUP && value >= 1 && value <= 4) melody = 0;     // X旋律
    if (keys == btnRIGHT && value >= 1 && value <= 4) melody = 1;  // A旋律
    if (keys == btnLEFT && value >= 1 && value <= 4) melody = 2;   // XA旋律
  }
  keysOld = keys;

  if (value != 0 && runMode) {
    if (isFirstRun) {
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
      case 1:  // 闘技場オート
        if (prg == 0) prg = 2;
        lcdCruise();          // 実行中項目LCD表示
        autoQuestCruising();  // 団子からオート
        if (prg == 6) {
          commonLcdMacro();  // LCD共通マクロ
          autoArena();        // 闘技場
        }
        break;
      case 2:  // 極泉郷オート
        if (prg == 0) prg = 2;
        lcdCruise();          // 実行中項目LCD表示
        autoQuestCruising();  // 団子からオート
        if (prg == 6) {
          commonLcdMacro();  // LCD共通マクロ
          autoInfernal();     // 極泉郷
        }
        break;
      case 3:  // 塔の秘境オート
        if (prg == 0) prg = 2;
        lcdCruise();          // 実行中項目LCD表示
        autoQuestCruising();  // 団子からオート
        if (prg == 6) {
          commonLcdMacro();  // LCD共通マクロ
          autoForlorn();      // 塔の秘境
        }
        break;
      case 4:                 // 闘技場クエスト受注から
        lcdCruise();          // 実行中項目LCD表示
        autoQuestCruising();  // クエスト受注からオート
        if (prg == 6) {
          commonLcdMacro();  // LCD共通マクロ
          autoArena();        // 闘技場
        }
        break;
      case 5:
        lcdItem();                   // 特産品LCD
        elapsedTime();               // LCD表示
        if (targetOn) repeatMove();  // 2分置きに移動
        repeatA();
        break;
    }
  }

  //ストップ
  if (value != 0 && !runMode) {
    if (!reStart) {
      prg = 0;
      if (value >= 1 && value <= 4) {
        lcd.setCursor(0, 0);
        lcd.print(value);
        lcd.print(".");
        displayString(value, 3);
        lcd.print("          ");
        lcd.setCursor(11, 1);
        lcd.print("SRT>S");
      }
    }
    if (!runStop) {
      leftStickNeutral();
      lcd.setCursor(value >= 1 && value <= 4 ? 10 : 9, 0);  // 決戦場と特産品の場合とのLCD位置の調整
      lcd.print("STOP");
      lcd.setCursor(10, 1);
      lcd.print("RSRT>S");
      setupMode = false;
      skipExec = false;
      isFirstRun = true;
      runStop = true;
      times = 0;
      stopTime = millis();  // ストップ時にstopTimeを計測開始
      delay(400);
    }
    reStart = (millis() - stopTime <= 60000);  // ストップから60秒経過中
    if (!reStart) startTime = 0;               // ストップして60秒経過でstartTimeをリセット
  }
}

/* 共通マクロ */
void commonLcdMacro() {
  lcdMelody();    // LCD表示旋律タイプ
  elapsedTime();  // LCD表示経過時間
  if (targetOn) target();
}

/* LCD制御=========================================================== */
/* 準備LCD */
void lcdCruise() {
  lcd.setCursor(2, 0);
  displayString(value, 3);  // ArenaAuto
  lcd.setCursor(3, 1);
  if (prg == 0) lcd.print("Plaza...");
  else if (prg == 1) lcd.print("Accept.");
  else if (prg == 2) lcd.print("TeaShop");
  else if (prg == 3) lcd.print("Dango..");
  else if (prg == 4) lcd.print("Loading");
  else if (prg == 5) lcd.print("Hunting");
}
/* 旋律タイプLCD */
void lcdMelody() {
  const char* melodyStrings[][2] = {
    { "MelodyX ", "ｾﾝﾘﾂ X  " },  // melody == 0
    { "MelodyA ", "ｾﾝﾘﾂ A  " },  // melody == 1
    { "MelodyXA", "ｾﾝﾘﾂ XA " },  // melody == 2
  };
  lcd.setCursor(3, 1);
  lcd.print((languageFlag == 0) ? melodyStrings[melody][0] : jp(melodyStrings[melody][1]));
}
/* 特産品LCD */
void lcdItem() {
  lcd.setCursor(3, 1);
  const char* text = (languageFlag == 0)
                       ? (targetOn ? "OnTheGo" : "RepeatA")
                       : (targetOn ? jp("ｲﾄﾞｳｻｲｼｭ") : jp("ﾘﾋﾟｰﾄA  "));
  lcd.print(text);
}
/* 経過時間LCD表示 */
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
  // 1列目LCD
  lcd.setCursor(0, 0);
  if (value >= 1 && value <= 4) {
    lcd.print(value);
    lcd.print(".");
  }
  displayString(value, mode);
  // 2列目LCD
  commonLcdRow2();
  lcd.setCursor(11, 1);
  lcd.print("SRT>S");
}