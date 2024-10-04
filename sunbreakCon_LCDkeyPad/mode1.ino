/*  
 *  概要／
 *  傀異錬成操作のマクロ、キャプチャー記録型の連続錬成または連続錬成
 *  
 *  準備／
 *  ・手動で強化型を選び傀異錬成し傀異強化結果の画面にする
 *  ・連続錬成する場合は錬成する傀異強化の種類にカーソルを合わせる  
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
      lcdQurious();  //LCD表示
    }

    if (keys == btnSELECT && keysOld == btnNONE) {
      char text[5];
      lcd.setCursor(10, 1);
      int repeatExecution = numValue;
      switch (value) {
        /* 錬成錬成設定 */
        case 1:
          setupMode = !setupMode;
          lcd.clear();  // LCD初期化
          lcdQurious();
          break;
        case 2:
          commonQuriousMacro();
          choiceEssence();  // 琥珀を選択
          runQurious();     // 傀異錬成
          countR++;
          lcdQurious();
          break;
        /* 連続傀異錬成 */
        case 3:
        case 4:
          lcdQuriousRun();
          firstRun = true;
          for (int i = repeatExecution - 1; i >= 0; i--) {
            if (firstRun) {
              choiceQurious();  // 傀異強化の種類を選択
              firstRun = false;
            } else if (!firstRun) choiceCurrent();
            choiceEssence();            // 琥珀を選択
            runQurious();               // 傀異錬成
            if (value == 3) capture();  // case 6のみキャプチャー
            delay(600);
            lcd.setCursor(11, 0);
            lcd.print("c");
            sprintf(text, "%4d", i);
            lcd.print(text);
          }
          lcdQuriousEnd();
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
      lcd.clear();                 // LCD初期化
      commonLcdRow1();             // 1列目LCD0-1
      displayString(value, mode);  // 1列目LCD2-
      lcdSetQurious();
    }
    lcd.setCursor(11, 0);
    lcd.print("s");
    commonLcdRow2();  // 2列目LCD0-2
    lcdEssence();     // 琥珀表示
    lcd.setCursor(11, 1);
    lcd.print((languageFlag == 0) ? "Ess:" : jp("ｺﾊｸ:"));
    lcd.print(ess + 1);
    cursorPosition();
    delay(100);

    /* 設定終了 */
    if (keys == btnSELECT && keysOld == btnNONE) {
      setupMode = !setupMode;
      lcd.noCursor();
      lcd.setCursor(11, 1);
      lcd.print("SET>S");
      lcd.setCursor(11, 0);
      lcd.print("c");
    }
    keysOld = keys;  // 前回のキー状態を記録
  }                  // setupModeここまで
}  // mode1ここまで


/* 共通マクロ */
void commonQuriousMacro() {
  lcdQuriousRun();
  choiceCurrent();
}
// 各桁の数字を更新
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
void lcdSetQurious() {
  lcd.setCursor(3, 1);
  lcdEssence();
  lcd.setCursor(12, 0);
  updateCountQurious();
  cursorPosition();
}
void lcdQuriousRun() {
  lcd.setCursor(11, 1);
  lcd.print("  Run");
}
void lcdQuriousEnd() {
  lcd.setCursor(13, 1);
  lcd.print("End");
}

/* LCD表示 */
void lcdQurious() {
  char text[4];
  commonLcdRow1();             // 1列目LCD0-1
  displayString(value, mode);  // 1列目LCD2-
  lcd.setCursor(11, 0);
  if (value == 1) {
    lcd.print("c");
    lcd.setCursor(12, 0);  // カーソルの初期位置
    for (int i = 0; i < 4; i++) lcd.print(digitsQ[(int)i]);
    commonLcdRow2();  // 2列目LCD0-2
    if (!setupMode) {
      displayString(0, mode);  // 2列目LCD3-
      lcd.setCursor(11, 1);
      lcd.print("SET>S");
    }
  } else if (value == 2) {
    lcd.print("t");
    sprintf(text, "%4d", countR);
    lcd.print(text);
    commonLcdRow2();  // 2列目LCD0-2
    lcdEssence();
    lcd.setCursor(11, 1);
    lcd.print("RUN>S");
  } else if (value >= 3 && value <= 4) {
    lcd.print("c");
    sprintf(text, "%4d", numValue);
    lcd.print(text);
    commonLcdRow2();  // 2列目LCD0-2
    lcdEssence();
    lcd.setCursor(11, 1);
    lcd.print("RUN>S");
  }
}
