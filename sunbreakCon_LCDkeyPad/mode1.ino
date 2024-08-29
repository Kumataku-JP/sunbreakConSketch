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

  keys = read_LCD_buttons(analogRead(0));

  /*  傀異錬成  */
  //使用琥珀選択
  if (!dateSet) {
    if ((keys == btnRIGHT || keys == btnLEFT) && keysOld == btnNONE) {
      value += (keys == btnRIGHT) ? 1 : -1;
      if (value > 8) value = 1;       // 1から8へトグル
      else if (value < 1) value = 8;  // 8から1へトグル
      lcd.clear();
      lcdQurious();  //LCD表示
      delay(250);
    }

    if (keys == btnSELECT && keysOld == btnNONE && value != 0) {
      lcd.setCursor(10, 1);
      int repeatExecution = numValue;

      switch (value) {
        case 1:
          lcdQuriousRun();
          choiceCurrent();
          choiceEssence();  // 精気琥珀
          countR++;
          lcdQuriousStart();
          break;
        case 2:
          lcdQuriousRun();
          choiceCurrent();
          amberEssencePlus();  // 精気琥珀・上
          choiceEssence();
          countR++;
          lcdQuriousStart();
          break;
        case 3:
          lcdQuriousRun();
          choiceCurrent();
          primeAmberEssence();  // 精気琥珀・尖
          choiceEssence();
          countR++;
          lcdQuriousStart();
          break;
        case 4:
          lcdQuriousRun();
          choiceCurrent();
          royalAmberEssence();  // 精気琥珀・王
          choiceEssence();
          countR++;
          lcdQuriousStart();
          break;
        case 5:
          QuriousRun = 1;
          lcdQuriousRun();
          choiceCurrent();
          pureAmberEssence();  // 精気琥珀・真
          choiceEssence();
          countR++;
          lcdQuriousStart();
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
          dateSet = !dateSet;
          lcdQurious();
          break;
      }
    }
    keysOld = keys;
  }  // !dateSetここまで

  /* カウンター設定 */
  if (dateSet) {
    lcd.setCursor(11, 0);
    lcd.print("COM>S");
    lcd.setCursor(11, 1);
    lcd.print("s");
    if (keys == btnSELECT && keysOld == btnNONE && value != 0) {
      dateSet = !dateSet;
      lcd.noCursor();
      lcd.setCursor(11, 0);
      lcd.print("SET>S");
      lcd.setCursor(11, 1);
      lcd.print("c");
    }
    // 錬成回数のセット  左右ボタンでカーソルを左に移動
    if (keysOld == btnNONE) {
      curPos += (keys == btnLEFT) ? -1 : (keys == btnRIGHT) ? 1
                                                            : 0;
      if (curPos < 1) curPos = 4;
      if (curPos > 4) curPos = 1;
    }
    keysOld = keys;  // 前回のキー状態を記録

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
  }  // dateSetここまで
}  // mode1ここまで

void lcdSetQurious() {
  lcd.setCursor(12, 1);
  updateCountQurious();
  cursorPosition();
}

// 各桁の数字をディスプレイに表示
void updateCountQurious() {
  numValue = 0;
  for (int i = 0; i < 4; i++) {
    numValue = numValue * 10 + digitsQ[i];
    lcd.print(digitsQ[i]);
  }
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

void lcdQuriousRun() {
  lcd.setCursor(10, 1);
  lcd.print(" Run  ");
}
void lcdQuriousStart() {
  lcd.setCursor(10, 1);
  lcd.print(" SRT>S");
}
void lcdQuriousEnd() {
  lcd.setCursor(10, 1);
  lcd.print(" End  ");
}