/*  モンハンライズサンブレイク  防具傀異錬成
 *  
 *  概要／
 *  傀異錬成操作のマクロ、キャプチャー記録型の連続錬成または連続錬成
 *  
 *  準備／
 *  ・手動で強化型を選び傀異錬成し傀異強化結果の画面にする
 *  ・連続錬成する場合は錬成する傀異強化の種類にカーソルを合わせる  
*/

void mode1() {

  char text[5];

  keys = read_LCD_buttons(analogRead(0));

  /*  傀異錬成  */
  //使用琥珀選択
  if (!setupMode) {
    if (keys == btnRIGHT || keys == btnLEFT) {
      value += (keys == btnRIGHT) ? 1 : -1;
      value = toggleValue(value, 1, 8);  // 1-8トグル
      lcd.clear();
      lcdQurious();  //LCD表示
      delay(300);
    }

    if (keys == btnSELECT && keysOld == btnNONE && value != 0) {
      lcd.setCursor(10, 1);
      int repeatExecution = numValue;
      switch (value) {
        case 1:
          commonQuriousMacro();
          choiceEssence();  // 精気琥珀
          countR++;
          lcdQuriousUpdate();
          break;
        case 2:
          commonQuriousMacro();
          amberEssencePlus();  // 精気琥珀・上
          choiceEssence();
          countR++;
          lcdQuriousUpdate();
          break;
        case 3:
          commonQuriousMacro();
          primeAmberEssence();  // 精気琥珀・尖
          choiceEssence();
          countR++;
          lcdQuriousUpdate();
          break;
        case 4:
          commonQuriousMacro();
          royalAmberEssence();  // 精気琥珀・王
          choiceEssence();
          countR++;
          lcdQuriousUpdate();
          break;
        case 5:
          QuriousRun = 1;
          commonQuriousMacro();
          pureAmberEssence();  // 精気琥珀・真
          choiceEssence();
          countR++;
          lcdQuriousUpdate();
          break;
          /* 連続錬成記録*/
        case 6:
        case 7:
          lcdQuriousRun();
          firstRun = true;
          for (int i = repeatExecution - 1; i >= 0; i--) {
            if (firstRun) {
              choiceQurious();  // 傀異強化の種類を選択
              firstRun = false;
            } else {
              choiceCurrent();
            }
            pureAmberEssence();  // 精気琥珀・真
            choiceEssence();
            if (value == 6) {
              capture();  // case 6のみキャプチャー
            }
            delay(600);
            lcd.setCursor(11, 0);
            lcd.print("c");
            sprintf(text, "%4d", i);
            lcd.print(text);
          }
          lcdQuriousEnd();
          break;
          /* 錬成回数のセット */
        case 8:
          setupMode = !setupMode;
          lcdQurious();
          break;
      }
    }
    keysOld = keys;
  }  // !setupModeここまで

  /* カウンター設定 */
  if (setupMode) {
    lcd.setCursor(11, 0);
    lcd.print("COM>S");
    lcd.setCursor(11, 1);
    lcd.print("s");
    /* 錬成回数のセット */
    if ((keys == btnRIGHT || keys == btnLEFT) && keysOld == btnNONE) {
      curPos += (keys == btnRIGHT) ? 1 : -1;
      curPos = toggleCurPos(curPos, 1, 4);  // 1-4トグル
    }
    // カーソル位置が12-15のときに上下ボタンを押すと数字を増加
    if (keys == btnUP || keys == btnDOWN) {
      if (curPos >= 1 && curPos <= 4) {
        int index = curPos - 1;
        int adjustment = (keys == btnUP) ? 1 : -1;
        digitsQ[index] = (digitsQ[index] + adjustment + 10) % 10;
        lcdSetQurious();
      }
      delay(100);
    }
    lcdSetQurious();  // 錬成回数設定をLCDに表示する
    delay(100);

    /* 設定終了 */
    if (keys == btnSELECT && keysOld == btnNONE && value != 0) {
      setupMode = !setupMode;
      lcd.noCursor();
      lcd.setCursor(11, 0);
      lcd.print("SET>S");
      lcd.setCursor(11, 1);
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
  numValue = 0;
  for (int i = 0; i < 4; i++) {
    numValue = numValue * 10 + digitsQ[i];
    lcd.print(digitsQ[i]);
  }
}

/* LCD制御=========================================================== */
void lcdSetQurious() {
  lcd.setCursor(12, 1);
  updateCountQurious();
  cursorPosition();
}

void lcdQuriousRun() {
  lcd.setCursor(10, 1);
  lcd.print(" Run  ");
}
void lcdQuriousUpdate() {
  char text[5];
  lcd.setCursor(11, 0);
  lcd.print("t");
  sprintf(text, "%4d", countR);
  lcd.print(text);
  lcd.setCursor(10, 1);
  lcd.print(" Run>S");
}
void lcdQuriousEnd() {
  lcd.setCursor(10, 1);
  lcd.print(" End  ");
}

/* LCD表示 */
void lcdQurious() {
  char text[5];
  // 1列目LCD
  commonLcdRow1();
  // 2列目LCD
  commonLcdRow2();

  lcd.setCursor(11, 0);
  if (value >= 1 && value <= 5) {
    lcd.print("t");
    sprintf(text, "%4d", countR);
    lcd.print(text);
    lcd.setCursor(11, 1);
    lcd.print("RUN>S");
  } else if (value >= 6 && value <= 7) {
    lcd.print("c");
    sprintf(text, "%4d", numValue);
    lcd.print(text);
    lcd.setCursor(11, 1);
    lcd.print("RUN>S");
  } else if (value == 8) {
    lcd.print("SET>S");
    lcd.setCursor(11, 1);
    lcd.print("c");
    lcd.setCursor(12, 1);  // カーソルの初期位置
    for (int i = 0; i < 4; i++) {
      lcd.print(digitsQ[(int)i]);
    }
  }
}
