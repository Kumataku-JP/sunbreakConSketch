/*  モンハンライズサンブレイクamiibo福引  Ver.11.0.
 *  
 *  概要／
 *  amiibo福引を連続して行うマクロ
 *  
 *  準備／
 *  1. Switchホーム画面  設定  本体時間を合わせる  オンのままでOK
 *  2. マクロコントローラーのシステム設定または同モード設定で日付をSwitch本体と合わせる
 *  3. 2段目の4桁で福引回数を指定する
 *  4. 福引回数を指定ならカウンター表示、ないのなら日付表示（ストップするまで続ける）
 *  5. コントローラーにamiiboを読み込めるようにセットする（初日の福引は手動でしてもしなくてもOK）
 *  6. 途中停止はできるが、景品受取りとホーム画面に移行する合間のみ可能。
 *  ※タイミングはカーソルが点灯した時（成功はシビア）福引回数指定を推奨。
 *  7. 終了または停止後、左ボタンで本体日付を自動に戻すマクロ実行
 *
*/

void mode4() {

  keys = read_LCD_buttons(analogRead(0));

  /* !dateSet 選択モード*/
  if (!dateSet) {
    if ((keys == btnRIGHT || keys == btnLEFT) && !closeLottery) {
      value += (keys == btnRIGHT) ? 1 : -1;  // 右を押したか正否判定
      if (value > 2) value = 1;              // 1から2へトグル
      else if (value < 1) value = 2;         // 2から1へトグル
      lcd.clear();                           // LCD初期化
      lcdAmiibo();                           // LCD表示
      delay(250);
    }

    if (keys == btnSELECT && keysOld == btnNONE && value != 0) {
      steyMode4 = !steyMode4;
    }
    keysOld = keys;
    /* amiibo福引 */
    if (value == 1) {
      if (steyMode4) {
        if (!isFirstRun) {
          repeatCount = numDate;
          isFirstRun = true;
          runStop = false;
          lcd.setCursor(0, 0);
          displayString(value, 4);  // LCD Fukubiki
          lcd.setCursor(9, 1);
          lcd.print(" STOP>S");
        }
        lcd.setCursor(0, 0);
        displayString(value, 4);
        //福引回数指定
        if (timesLeft) {
          if (repeatCount > 0) {
            lcd.noCursor();
            dateSetting();  //日付変更
            lcd.setCursor(9, 1);
            lcd.print("  ");
            lcdCount(repeatCount);  // カウントLCD表示
            lcd.setCursor(7, 0);
            lcd.print(" ");
            lcdDate();  // 日付LCD表示
            lottery();  // amiibo福引
            lcd.setCursor(9, 1);
            lcd.print("  ");
            if (prg == 5) {
              lcdCount(repeatCount - 1);  // カウントLCD表示
              closeLottery = true;        // 終了後日付自動オンに戻すフラグ
              repeatCount--;              // カウントをデクリメント
              prg = 0;
            }
          }
          if (repeatCount == 0) steyMode4 = false;
        } else if (!timesLeft) {
          lcd.setCursor(9, 1);
          lcd.print(" STOP>S");
          dateSetting();  //日付変更
          lcd.setCursor(8, 0);
          lcdDate();  //日付表示
          lottery();  //amiibo福引
          if (prg == 5) {
            closeLottery = true;
            prg = 0;
          }
        }
        // delay(200);
      }
      if (!steyMode4) {
        if (!runStop) {
          lcd.setCursor(0, 0);
          lcd.print("STOP   ");
          lcd.setCursor(9, 1);
          lcd.print("Close>L");
          delay(250);
        }
      }
      /* カレンダーリセット Switch本体の日付設定を自動に戻す*/
      if (keys == btnLEFT && closeLottery) {
        runStop = true;
        if (runStop) {
          lcd.setCursor(0, 0);
          lcd.print("CloseRun");
          settingScreenClose();  // クローズ動作マクロ
          dayDate = savedDayDate;
          monthDate = savedMonthDate;
          yearDate = savedYearDate;
          isFirstRun = false;
          closeLottery = false;
          firstRun = true;
          value = 1;
          prg = 0;
          delay(250);
          lcd.setCursor(0, 0);
          displayString(value, mode);  // Start Fukubiki>D
          lcd.print(" ");
          lcdAmiibo();
          lcd.setCursor(9, 1);
          lcd.print("  STR>S");
        }
      }
      lcd.noCursor();
    }  // value == 1ここまで
    if (value == 2) {
      if (steyMode4) {
        dateSet = true;
        delay(250);
      }
    }
  }

  /* dateSet カウンター  日付  設定モード */
  if (dateSet) {
    lcd.setCursor(7, 0);
    lcd.print("S");
    /* カウンター設定 */
    if (keys == btnSELECT && keysOld == btnNONE && value != 0) {
      // 設定された日付を保存する
      savedDayDate = dayDate;
      savedMonthDate = monthDate;
      savedYearDate = yearDate;

      if (numDate > 0) {
        timesLeft = true;
      } else if (numDate == 0) {
        timesLeft = false;
      }

      dateSet = false;
      steyMode4 = false;
      lcd.noCursor();
      lcd.setCursor(7, 0);
      lcd.print("D");
    }

    /* データ設定 */
    // 日付登録
    // 左右ボタンでカーソル送りトグル
    if (keysOld == btnNONE) {
      cursorPosition += (keys == btnLEFT) ? -1 : (keys == btnRIGHT) ? 1
                                                                    : 0;
      if (cursorPosition < 1) cursorPosition = 7;
      else if (cursorPosition > 7) cursorPosition = 1;
    }
    if (keys == btnUP || keys == btnDOWN) {
      setDate();  // 日付を増加させる
      // amiibo福引回数指定
      if (cursorPosition >= 4 && cursorPosition <= 7) {
        int digitValueLottery = digitsL[cursorPosition - 4];
        digitValueLottery = (keys == btnUP) ? (digitValueLottery + 1) % 10 : (digitValueLottery - 1 + 10) % 10;
        digitsL[cursorPosition - 4] = digitValueLottery;
        lcdSetDateAmiibo();
      }
    }
    keysOld = keys;      // 前回のキー状態を記録
    lcdSetDateAmiibo();  // LCDに表示する
    delay(200);
  }
}


/* 日付LCD表示制御 */
void lcdSetDateAmiibo() {
  lcdDate();
  lcd.setCursor(12, 1);
  updateCountLottery();
  updateLotteryPosition();
}

// 各桁の数字をディスプレイに表示
void updateCountLottery() {
  numDate = digitsL[0] * 1000 + digitsL[1] * 100 + digitsL[2] * 10 + digitsL[3];
  for (int i = 0; i < 4; i++) {
    lcd.print(digitsL[(int)i]);
  }
}
/* カーソルポジション */
void updateLotteryPosition() {
  if (cursorPosition == 1) lcd.setCursor(9, 0);        // 年の位置
  else if (cursorPosition == 2) lcd.setCursor(12, 0);  // 月の位置
  else if (cursorPosition == 3) lcd.setCursor(15, 0);  // 日の位置
  else if (cursorPosition == 4) lcd.setCursor(12, 1);  // 1000の位
  else if (cursorPosition == 5) lcd.setCursor(13, 1);  // 100の位
  else if (cursorPosition == 6) lcd.setCursor(14, 1);  // 10の位
  else if (cursorPosition == 7) lcd.setCursor(15, 1);  // 1の位
  if (dateSet) {
    lcd.cursor();
  }
}

/* LCD日付データ表示 */
void lcdDate() {
  char text[3];
  sprintf(text, "%02d", yearDate);
  lcd.print(text);
  lcd.print("/");
  sprintf(text, "%02d", monthDate);
  lcd.print(text);
  lcd.print("/");
  sprintf(text, "%02d", dayDate);
  lcd.print(text);
}
/* のこり福引回数LCD表示 */
void lcdCount(int count) {
  char text[5];
  lcd.print("t");
  sprintf(text, "%4d", count);
  lcd.print(text);
}

/* LCD表示 */
void lcdAmiibo() {
  lcd.setCursor(0, 0);
  displayString(value, mode);  // Start Fukubiki>D
  lcd.setCursor(0, 1);
  lcd.print("M");
  lcd.print(mode);
  lcd.print(".");
  displayString(mode, 0);  // Auto
  if (value == 1) {
    lcd.print(" ");
    if (timesLeft) {
      lcd.setCursor(7, 0);
      lcd.print("Times");
      updateCountLottery();
    } else {
      lcd.setCursor(8, 0);
      lcdDate();
    }
    lcd.setCursor(9, 1);
    lcd.print("  STR>S");
  } else if (value == 2) {
    lcd.setCursor(6, 0);
    lcd.print(" ");
    lcd.print("D");
    lcdDate();
    lcd.setCursor(11, 1);
    lcd.print("C");
    updateCountLottery();
  }
}
