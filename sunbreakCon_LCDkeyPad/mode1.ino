/*  モンハンライズサンブレイク  防具傀異錬成
 *  
 *  概要／
 *  傀異錬成操作のマクロ  使用琥珀は通常  または1番上の琥珀
 *  
 *  準備／
 *  ・手動で強化型を選び傀異錬成し傀異強化結果の画面にする
 *  ・連続錬成する場合は錬成する傀異強化の種類にカーソルを合わせる  
*/

void mode1() {

  char text[5];

  if (value == 0) {
    lcd.setCursor(0, 0);
    displayString(value, 1);  //SelectEssence>LR
  }

  keys = read_LCD_buttons(analogRead(0));

  /*  傀異錬成  */
  //使用琥珀選択
  if (!dateSet) {
    if (keys == btnRIGHT) {
      value++;
      if (value > 9) value = 1;  // 1から9へトグル
      lcd.clear();
      lcdQurious();  //LCD表示
      delay(250);
    }
    if (keys == btnLEFT) {
      value--;
      if (value < 1) value = 9;  // 9から1へトグル
      lcd.clear();
      lcdQurious();  //LCD表示
      delay(250);
    }

    if (keys == btnSELECT && keysOld == btnNONE && value != 0) {

      lcd.setCursor(10, 1);
      if (value == 1) {
        lcdQuriousRun();
        choiceCurrent();
        choiceEssence();
        countR++;
        lcdQuriousGoOn();
      } else if (value == 2) {
        lcdQuriousRun();
        choiceCurrent();
        amberEssencePlus();  // 精気琥珀・上
        choiceEssence();
        countR++;
        lcdQuriousGoOn();
      } else if (value == 3) {
        lcdQuriousRun();
        choiceCurrent();
        primeAmberEssence();  // 精気琥珀・尖
        choiceEssence();
        countR++;
        lcdQuriousGoOn();
      } else if (value == 4) {
        lcdQuriousRun();
        choiceCurrent();
        royalAmberEssence();  // 精気琥珀・王
        choiceEssence();
        countR++;
        lcdQuriousGoOn();
      } else if (value == 5) {
        QuriousRun = 1;
        lcdQuriousRun();
        choiceCurrent();
        pureAmberEssence();  // 精気琥珀・真
        choiceEssence();
        countR++;
        lcdQuriousGoOn();
      } else if (value == 6) {
        countR = 1;  //傀異錬成セットカウント
        lcd.setCursor(11, 1);
        lcd.print("     ");
        delay(500);
        lcd.setCursor(11, 1);
        lcd.print("RESET");
        delay(500);
      }
      //錬成数カウント
      if (value >= 1 && value <= 6) {
        lcd.setCursor(11, 0);
        lcd.print("t");
        sprintf(text, "%4d", countR);
        lcd.print(text);
      }
      /* 連続錬成記録*/
      if (value == 7) {
        int repeatExecution = numValue;
        lcdQuriousRun();
        firstRun = true;
        // 残りカウンター
        for (int i = repeatExecution - 1; i >= 0; i--) {
          // 傀異強化の種類を選択
          if (firstRun) {
            choiceQurious();  // 傀異強化の種類を選択
            firstRun = false;
          } else {
            choiceCurrent();
          }
          pureAmberEssence();  // 精気琥珀・真
          choiceEssence();
          capture();  // キャプチャー
          delay(600);
          lcd.setCursor(11, 0);
          lcd.print("c");
          sprintf(text, "%4d", i);
          lcd.print(text);
        }
        lcdQuriousEnd();
      }
      /* 連続錬成*/
      else if (value == 8) {
        int repeatExecution = numValue;
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
          delay(600);
          lcd.setCursor(11, 0);
          lcd.print("c");
          sprintf(text, "%4d", i);
          lcd.print(text);
        }
        lcdQuriousEnd();
      }
      /* 錬成回数のセット */
      if (value == 9) {
        dateSet = true;
        lcdSetQurious();
        delay(250);
      }
    }
    keysOld = keys;
  }  // !dateSetここまで


  if (dateSet) {
    if (keys == btnSELECT && keysOld == btnNONE && value != 0) {
      dateSet = !dateSet;
      lcd.noCursor();
      lcd.setCursor(11, 0);
      lcd.print("SET>S");
      lcd.setCursor(11, 1);
      lcd.print("c");
    }
    // 錬成回数のセット  左右ボタンでカーソルを左に移動
    if (keys == btnRIGHT && keysOld == btnNONE) {
      cursorPosition++;
      if (cursorPosition > 4) cursorPosition = 1;
    }
    if (keys == btnLEFT && keysOld == btnNONE) {
      cursorPosition--;
      if (cursorPosition < 1) cursorPosition = 4;
    }
    // カーソル位置が12-15のときに上下ボタンを押すと数字を増加
    if (keys == btnUP && keysOld == btnNONE) {
      if (cursorPosition >= 1 && cursorPosition <= 4) {
        int digitValueQurious = digitsQ[cursorPosition - 1];
        digitValueQurious = (digitValueQurious + 1) % 10;
        digitsQ[cursorPosition - 1] = digitValueQurious;
        updateValue();
      }
    }
    if (keys == btnDOWN && keysOld == btnNONE) {
      if (cursorPosition >= 1 && cursorPosition <= 4) {
        int digitValueQurious = digitsQ[cursorPosition - 1];
        digitValueQurious = (digitValueQurious - 1 + 10) % 10;
        digitsQ[cursorPosition - 1] = digitValueQurious;
        updateValue();
      }
    }
    keysOld = keys;  // 前回のキー状態を記録
    updateValue();   // LCDに表示する
    delay(100);
  }  // dateSetここまで
}  // mode1ここまで


// 各桁の数字をディスプレイに表示
void updateValue() {
  lcd.setCursor(12, 1);
  numValue = digitsQ[0] * 1000 + digitsQ[1] * 100 + digitsQ[2] * 10 + digitsQ[3];
  for (int i = 0; i < 4; i++) {
    lcd.print(digitsQ[(int)i]);
  }
  if (dateSet) {
    lcd.cursor();
    lcd.setCursor(11 + cursorPosition, 1);  // カーソル位置を更新
  }
}

void lcdSetQurious() {
  lcd.setCursor(11, 0);
  lcd.print("COM>S");
  lcd.setCursor(11, 1);
  lcd.print("s");
  lcd.setCursor(12, 1);  // カーソルの初期位置
  numValue = digitsQ[0] * 1000 + digitsQ[1] * 100 + digitsQ[2] * 10 + digitsQ[3];
  for (int i = 0; i < 4; i++) {
    lcd.print(digitsQ[(int)i]);
  }
}

/* LCD表示 */
void lcdQurious() {
  char text[5];
  lcd.setCursor(0, 1);
  lcd.print("M");
  lcd.print(mode);
  lcd.print(".");
  displayString(mode, 0);  // Qurious
  lcd.setCursor(0, 0);
  lcd.print(value);
  lcd.print(".");
  lcd.setCursor(2, 0);
  displayString(value, 1);
  lcd.setCursor(11, 0);
  if (value >= 1 && value <= 5) {
    lcd.print("t");
    sprintf(text, "%4d", countR);
    lcd.print(text);
    lcd.setCursor(11, 1);
    lcd.print("RUN>S");
  } else if (value == 6) {
    lcd.print("t");
    sprintf(text, "%4d", countR);
    lcd.print(text);
    lcd.setCursor(11, 1);
    lcd.print("RST>S");
  } else if (value >= 7 && value <= 8) {
    lcd.print("c");
    sprintf(text, "%4d", numValue);
    lcd.print(text);
    lcd.setCursor(11, 1);
    lcd.print("RUN>S");
  } else if (value == 9) {
    lcd.print("SET>S");
    lcd.setCursor(11, 1);
    lcd.print("c");
    lcd.setCursor(12, 1);  // カーソルの初期位置
    for (int i = 0; i < 4; i++) {
      lcd.print(digitsQ[(int)i]);
    }
  }
}

void lcdQuriousRun() {
  lcd.setCursor(10, 1);
  lcd.print(" Run  ");
}
void lcdQuriousGoOn() {
  lcd.setCursor(10, 1);
  lcd.print("GoOn>S");
}
void lcdQuriousEnd() {
  lcd.setCursor(10, 1);
  lcd.print(" End  ");
}