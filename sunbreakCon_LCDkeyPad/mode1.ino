/*  
 *  概要／
 *  傀異錬成操作のマクロ、キャプチャー記録型の連続錬成または連続錬成
 *  
 *  1.  錬成回数、使用琥珀(兼、上から何番目の琥珀)を指定
 *  2.  1で指定した琥珀を使用して単発の錬成
 *      ※錬成強化型を選び、手動で傀異錬成し傀異強化結果の画面にする
 *  3.  1で指定した錬成回数を繰返しキャプチャーする
 *  4.  1で指定した錬成回数を繰返す
 *      ※連続錬成する場合は錬成する傀異強化の種類にカーソルを合わせる 
*/

void mode1() {
  /*  傀異錬成  */
  if (!setupMode) {
    if (keys == btnRIGHT || keys == btnLEFT) {
      int direction = (keys == btnRIGHT) ? 1 : -1;
      value += direction;
      if (value < 1) value = 4;
      else if (value > 4) value = 1;
      delay(300);
      lcd.clear();   // LCD初期化
      lcdQurious();  // LCD初期表示
    }

    if (keys == btnSELECT && keysOld == btnNONE) {
      lcd.setCursor(10, 1);
      switch (value) {
        /* 錬成錬成設定 */
        case 1:
          setupMode = !setupMode;
          lcd.clear();   // LCD初期化
          lcdQurious();  // LCD初期表示
          break;
        case 2:
          commonQuriousMacro();
          choiceEssence();  // 琥珀を選択
          runQurious();     // 傀異錬成
          countR++;
          lcdQurious();  // LCD初期表示
          break;
        /* 連続傀異錬成 */
        case 3:
        case 4:
          if (numValue > 0) runMode = !runMode;
          break;
      }
    }
    keysOld = keys;
  }  // !setupModeここまで

  /* カウンター設定 */
  if (setupMode) {
    /* 錬成回数のセット */
    if ((keys == btnRIGHT || keys == btnLEFT) && keysOld == btnNONE) {
      int direction = (keys == btnRIGHT) ? 1 : -1;
      curPos += direction;
      if (curPos < 1) curPos = 5;
      else if (curPos > 5) curPos = 1;
    }
    // カーソル位置が12-15のときに上下ボタンを押すと数字を増加
    if (keys == btnUP || keys == btnDOWN) {
      if (curPos >= 1 && curPos <= 4) {
        int index = curPos - 1;
        int adjustment = (keys == btnUP) ? 1 : -1;
        digitsQ[index] = (digitsQ[index] + adjustment + 10) % 10;
      }
      // 使用琥珀を指定
      if (curPos == 5) {
        int adjustment = (keys == btnUP) ? 1 : -1;
        ess += adjustment;
        if (ess < 0) ess = 4;
        else if (ess > 4) ess = 0;
      }
      delay(100);
      lcd.clear();  // LCD初期化
      lcdSetQurious();
    }
    lcdQurious();  // カーソル表示位置の制御
    delay(100);

    /* 設定終了 */
    if (keys == btnSELECT && keysOld == btnNONE) {
      setupMode = !setupMode;
      lcd.noCursor();  // カーソル消灯
      lcdQurious();    // LCD初期表示
    }
    keysOld = keys;  // 前回のキー状態を記録
  }                  // setupModeここまで

  if (runMode) {
    if (firstRun) repeatCount = numValue;
    if (repeatCount > 0) {
      if (firstRun) {
        choiceQurious();  // 傀異強化の種類を選択
        firstRun = false;
      } else if (!firstRun) {
        choiceCurrent();  // 強化前のステータスを選択
      }
      choiceEssence();            // 琥珀を選択
      lcdQuriousRun();            // 錬成実行中のLCD表示
      runQurious();               // 傀異錬成
      repeatCount--;              // カウントをデクリメント
      if (value == 3) capture();  // value 3のみキャプチャー
      delay(600);
      lcd.setCursor(11, 0);
      lcd.print("c");
      char text[4];
      sprintf(text, "%4d", repeatCount);
      lcd.print(text);
    } else if (repeatCount == 0) {
      lcdQuriousEnd();  // 錬成終了時のLCD表示
      firstRun = true;
      runMode = false;
    }
  } else if (!runMode) {
    if (repeatCount > 0) {
      lcd.setCursor(12, 0);  // カーソル初期位置
      for (int i = 0; i < 4; i++) lcd.print(digitsQ[(int)i]);
      lcd.setCursor(12, 1);
      lcd.print("STOP");
      firstRun = true;
      repeatCount = false;
    }
  }
}  // mode1ここまで


/* 共通マクロ */
void commonQuriousMacro() {
  lcdQuriousRun();  // 実行中のLCD表示
  choiceCurrent();  // 傀異強化の種類を選択
}
/* 各桁の数字を更新 */
void updateCountQurious() {
  updateCountGeneric(numValue, digitsQ, 4);  // digitsQを使って4桁の計算と表示
}

/* LCD制御=========================================================== */
/* 使用琥珀LCD */
void lcdEssence() {
  const char* essStrings[][2] = {
    { "Amber", "ｾｲｷ" },  // 0
    { "Plus", "ｼﾞｮｳ" },  // 1
    { "Prime", "ｾﾝ" },   // 2
    { "Royal", "ｵｳ" },   // 3
    { "Pure", "ｼﾝ" },    // 4
  };
  lcd.print((languageFlag == 0) ? essStrings[(int)ess][0] : jp(essStrings[(int)ess][1]));
}
/* 錬成設定中のLCD表示 */
void lcdSetQurious() {
  lcd.setCursor(12, 0);
  updateCountQurious();  // 各桁の数字を更新
  lcd.setCursor(3, 1);
  lcdEssence();      // 使用琥珀の表示
  cursorPosition();  // カーソル表示位置の制御
}
/* 錬成実行中のLCD表示 */
void lcdQuriousRun() {
  lcd.setCursor(11, 1);
  lcd.print("  Run");
}
/* 錬成終了時のLCD表示 */
void lcdQuriousEnd() {
  lcd.setCursor(13, 1);
  lcd.print("End");
}
/* LCD初期表示 */
void lcdQurious() {
  char text[4];
  commonLcdRow1();             // 1列目LCD0-1
  displayString(value, mode);  // 1列目LCD2-
  lcd.setCursor(11, 0);        // 1列目LCD共通カーソル位置
  if (value == 1) {
    if (!setupMode) {
      lcd.print("c");
      lcd.setCursor(12, 0);  // カーソル初期位置
      for (int i = 0; i < 4; i++) lcd.print(digitsQ[(int)i]);
      commonLcdRow2();         // 2列目LCD0-2
      displayString(0, mode);  // 2列目LCD3-
      lcd.setCursor(11, 1);
      lcd.print("SET>S");
    } else if (setupMode) {
      lcd.print("s");
      lcd.setCursor(12, 0);  // カーソル初期位置
      for (int i = 0; i < 4; i++) lcd.print(digitsQ[(int)i]);
      commonLcdRow2();  // 2列目LCD0-2
      lcdEssence();     // 琥珀表示
      lcd.setCursor(11, 1);
      lcd.print((languageFlag == 0) ? "Ess:" : jp("ｺﾊｸ:"));
      lcd.print(ess + 1);  // 使用琥珀の表示
      cursorPosition();    // カーソル表示位置の制御
    }
  } else if (value >= 2 && value <= 4) {
    if (value == 2) {
      lcd.print("t");
      sprintf(text, "%4d", countR);
      lcd.print(text);  // 傀異錬成回数カウンター
    } else if (value >= 3 && value <= 4) {
      lcd.print("c");
      sprintf(text, "%4d", numValue);
      lcd.print(text);  // 連続傀異錬成回数の表示
    }
    commonLcdRow2();  // 2列目LCD0-2
    lcdEssence();     // 使用琥珀LCD
    lcd.setCursor(11, 1);
    lcd.print("RUN>S");
  }
}
