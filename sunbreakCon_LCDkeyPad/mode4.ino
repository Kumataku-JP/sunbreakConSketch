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
    if ((keys == btnRIGHT || keys == btnLEFT) && keysOld == btnNONE && !dateSet) {
      if (!closeLottery) {
        value += (keys == btnRIGHT) ? 1 : -1;  // 右を押したか正否判定
        if (value > 2) value = 1;              // 1から2へトグル
        else if (value < 1) value = 2;         // 2から1へトグル
      } else if (closeLottery && keys == btnLEFT) {
        value = -1;  // closeLotteryがtrueの時に左ボタンを押した場合
      }
      lcd.clear();  // LCD初期化
      lcdAmiibo();  // LCD表示
      delay(250);
    }

    if (keys == btnSELECT && keysOld == btnNONE && value != 0) {
      steyMode4 = !steyMode4;
    }
    keysOld = keys;
    /* amiibo福引 */
    if (steyMode4) {
      if (value == 1) {
        /* LCD表示 */
        LotteryStop = 1;
        lcd.setCursor(0, 0);
        displayString(value, mode);
        lcd.setCursor(8, 0);
        showLcdDate();  // 日付をLCDに表示する
        lcd.setCursor(3, 1);
        lcdProgress();    // 処理進行状況
        showLcdAmiibo();  // 開始時にLCDに表示するための処理

        lottery();      // amiibo福引 prg = 5,6で実行なので前置き
        dateSetting();  // 日付変更
        lcd.setCursor(8, 0);
        showLcdDate();  // 日付をLCDに表示する

        /* 福引回数制限なしの場合 */
        if (!timesLeft) {
          lcd.setCursor(10, 1);
          lcd.print(" STP>S");
          if (prg == 7) prg = 0;  // ループ初期化
        }
        /* 福引回数指定した場合 */
        if (timesLeft) {
          if (repeatCount > 0) {
            lcd.setCursor(11, 1);
            lcd.print("t");
            showLcdCountLottery(repeatCount);  // カウントをLCDに表示する
            if (prg == 7) {
              repeatCount--;  // カウントをデクリメント
              prg = 0;        // ループ初期化
            }
          }
          if (repeatCount == 0) {
            LotteryStop = 2;
            repeatCount = numDate;
            steyMode4 = false;
          }
        }
        lcd.noCursor();
      }  // value == 1 ここまで
      if (value == 2) {
        dateSet = true;
      }

      if (value == -1) {
        lcd.setCursor(3, 1);
        lcd.print("Closing...");
        lcd.print("     ");
        settingScreenClose();  // クローズ動作マクロ
        // 初期化
        dayDate = savedDayDate;
        monthDate = savedMonthDate;
        yearDate = savedYearDate;
        steyMode4 = !steyMode4;
        closeLottery = false;
        isFirstRun = true;
        firstRun = true;  // 日付自動オンオフ
        value = 1;
        prg = 0;
        LotteryStop = 0;
        lcd.clear();
        lcdAmiibo();
        lcd.setCursor(0, 0);
        displayString(value, mode);  // Start Fukubiki>D
        lcd.setCursor(10, 1);
        lcd.print(" SRT>S");
        delay(250);
      }
    }  // steyMode4 ここまで

    if (!steyMode4) {
      switch (LotteryStop) {
        case 0:
          break;
        case 1:  // 強制停止
          lcd.setCursor(0, 0);
          lcd.print("CLOSE>L");
          lcd.setCursor(10, 1);
          lcd.print("RSRT>S");
          closeLottery = true;
          prg = 0;
          LotteryStop = 0;
          break;
        case 2:  // 指定数終了
          lcd.setCursor(0, 0);
          lcd.print("CLOSE>L");
          lcd.setCursor(10, 1);
          lcd.print("   END");
          closeLottery = true;
          prg = 0;
          LotteryStop = 0;
          break;
          delay(250);
      }
    }
  }

  /* dateSet カウンター  日付  設定モード */
  if (dateSet) {
    lcd.setCursor(7, 0);
    lcd.print("S");
    lcd.setCursor(11, 1);
    lcd.print("C");
    /* カウンター設定 */
    if (keys == btnSELECT && keysOld == btnNONE && value != 0) {
      // 設定された日付を保存する
      savedDayDate = dayDate;
      savedMonthDate = monthDate;
      savedYearDate = yearDate;

      if (numDate > 0) timesLeft = true;
      else if (numDate == 0) timesLeft = false;  // 福引回数を指定しているか否か
      repeatCount = numDate;

      dateSet = false;
      steyMode4 = false;
      lcd.noCursor();
      lcd.setCursor(7, 0);
      lcd.print("D");
    }

    /* データ設定 */
    // 日付登録
    if ((keys == btnLEFT || keys == btnRIGHT) && keysOld == btnNONE) {
      curPos += (keys == btnLEFT) ? -1 : 1;
      if (curPos < 1) curPos = 7;
      else if (curPos > 7) curPos = 1;
    }
    keysOld = keys;  // 前回のキー状態を記録

    if (keys == btnUP || keys == btnDOWN) {
      setDate();  // 日付を増加させる
      // amiibo福引回数指定
      if (curPos >= 4 && curPos <= 7) {
        int digitValueLottery = digitsL[curPos - 4];
        digitValueLottery = (keys == btnUP) ? (digitValueLottery + 1) % 10 : (digitValueLottery - 1 + 10) % 10;
        digitsL[curPos - 4] = digitValueLottery;
        lcdSetDateAmiibo();
      }
      delay(100);
    }
    lcdSetDateAmiibo();  // LCDに表示する
    delay(100);
  }
}

/* 進行中LCD */
void lcdProgress() {
  lcd.setCursor(3, 1);
  if (prg == 0) lcd.print("HOME....");
  else if (prg == 1) lcd.print("System..");
  else if (prg == 2) lcd.print("Moving..");
  else if (prg == 3) lcd.print("DATE....");
  else if (prg == 4) lcd.print("Change..");
  else if (prg == 5) lcd.print("Amiibo..");
  else if (prg == 6) lcd.print("Lottery.");
}

/* 日付LCD表示制御 */
void lcdSetDateAmiibo() {
  lcd.setCursor(8, 0);
  showLcdDate();  // 日付をLCDに表示する
  lcd.setCursor(12, 1);
  updateCountLottery();
  cursorPosition();
}

// のこりの周回数をディスプレイに表示
void updateCountLottery() {
  numDate = digitsL[0] * 1000 + digitsL[1] * 100 + digitsL[2] * 10 + digitsL[3];
  for (int i = 0; i < 4; i++) {
    lcd.print(digitsL[i]);
  }
}

/* のこり福引回数LCD表示 */
void showLcdCountLottery(int count) {
  char text[5];
  sprintf(text, "%04d", count);
  lcd.print(text);
}
void showLcdAmiibo() {
  if (!timesLeft) {
    lcd.setCursor(10, 1);
    lcd.print(" STP>S");
  }
  /* 福引回数指定した場合 */
  if (timesLeft) {
    lcd.setCursor(11, 1);
    lcd.print("t");
    showLcdCountLottery(repeatCount);  // カウントをLCDに表示する
  }
}
/* LCD表示 */
void lcdAmiibo() {
  // 1列目LCD
  lcd.setCursor(0, 0);
  displayString(value, mode);
  // 2列目LCD
  commonLcdRow2();
  if (value == -1) {
    lcd.setCursor(0, 0);
    lcd.print("CLOSE>SELECT    ");
    lcd.setCursor(3, 1);
    lcd.print((languageFlag == 0) ? "SysDateRest  " : jp("ｼｽﾃﾑｶﾚﾝﾀﾞｼｮｷｶ"));
  }
  if (value == 1) {
    if (timesLeft) {
      lcd.setCursor(11, 0);
      lcd.print("T");
      updateCountLottery();  // カウンター表示更新
    } else {
      lcd.setCursor(8, 0);
      showLcdDate();  // 日付をLCDに表示する
    }
    lcd.setCursor(10, 1);
    lcd.print(" SRT>S");
  } else if (value == 2) {
    lcd.setCursor(7, 0);
    lcd.print("D");
    showLcdDate();  // 日付をLCDに表示する
    lcd.setCursor(11, 1);
    lcd.print("T");
    updateCountLottery();  // カウンター表示更新
  }
}
