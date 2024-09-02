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
 *  6. カレンダーの変更失敗により福引済みの状態になっていたら、セレクトボタン（タイミングで止まらないので長めに）で途中停止し
 *     手動でLCDの表示日付と同じになるように本体の日付を合わせる
 *     福引画面に戻りカーソルを指定位置にしセレクトボタンで再開
 *  7. 終了または途中停止後、左ボタンで本体日付を自動に戻すマクロモードにしセレクトボタンで実行をする
 *     ※締めの作業をしないと他のモードに変更不可。
*/

void mode4() {

  keys = read_LCD_buttons(analogRead(0));

  /* !setupMode 選択モード*/
  if (!setupMode) {
    if (!closeLottery && (keys == btnRIGHT || keys == btnLEFT)) {
      value += (keys == btnRIGHT) ? 1 : -1;
      value = toggleValue(value, 1, 2);  // 1-2トグル
      lcd.clear();                       // LCD初期化
      lcdAmiibo();                       // LCD表示
      delay(100);
    } else if (closeLottery && keys == btnLEFT) {
      value = -1;   // closeLotteryがtrueの時に左ボタンを押した場合
      lcdAmiibo();  // LCD表示
    }
    /* amiibo福引 */
    if (runMode) {
      if (value == 1) {
        setupMode = !setupMode;
      }
      if (value == 2) {
        /* LCD表示 */
        LotteryStop = 1;
        lcd.setCursor(0, 0);
        displayString(value, mode);
        lcd.setCursor(7, 0);
        showLcdDate();  // 日付をLCDに表示する
        lcd.setCursor(3, 1);
        lcdProgress();    // 処理進行状況
        showLcdAmiibo();  // 開始時にLCDに表示するための処理

        lottery();        // amiibo福引 prg = 5,6で実行なので前置き
        setupModeting();  // 日付変更
        lcd.setCursor(7, 0);
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
            runMode = !runMode;
          }
        }
        lcd.noCursor();
      }  // value == 2 ここまで
      if (value == -1) {
        lcd.setCursor(3, 1);
        lcd.print("Closing...");
        lcd.print("     ");
        settingScreenClose();  // クローズ動作マクロ
        // 初期化
        dayDate = savedDayDate;
        monthDate = savedMonthDate;
        yearDate = savedYearDate;
        runMode = !runMode;
        closeLottery = false;
        isFirstRun = true;
        firstRun = true;  // 日付自動オンオフ
        value = 2;
        prg = 0;
        LotteryStop = 0;
        lcd.clear();
        lcdAmiibo();
        lcd.setCursor(0, 0);
        displayString(value, mode);  // Start Fukubiki>D
        lcd.setCursor(10, 1);
        lcd.print(" SRT>S");
        delay(250);
      }  // value == -1 ここまで
    }    // runMode ここまで

    if (!runMode) {
      switch (LotteryStop) {
        case 1:  // 指定数終了
          lcd.setCursor(0, 0);
          lcd.print("CLOSE>L");
          lcd.setCursor(10, 1);
          lcd.print("RSRT>S");
          closeLottery = true;
          prg = 0;
          LotteryStop = 0;
          break;
        case 2:  // 強制停止
          lcd.setCursor(0, 0);
          lcd.print("CLOSE>L");
          lcd.setCursor(10, 1);
          lcd.print("   END");
          closeLottery = true;
          prg = 0;
          LotteryStop = 0;
          break;
      }
      delay(250);
    }
    if (keys == btnSELECT && keysOld == btnNONE && value != 0) {
      runMode = !runMode;
    }
    keysOld = keys;
  }
  /* setupMode カウンター  日付  設定モード */
  if (setupMode) {
    lcd.setCursor(3, 1);
    lcd.print((languageFlag == 0) ? "Setup  " : jp("ﾍﾝｺｳﾁｭｳ"));
    lcd.setCursor(11, 1);
    lcd.print("C");

    // 日付登録
    if ((keys == btnRIGHT || keys == btnLEFT) && keysOld == btnNONE) {
      int direction = (keys == btnRIGHT) ? 1 : -1;
      curPos += direction;
      if (curPos < 1) curPos = 7;
      else if (curPos > 7) curPos = 1;
    }

    /* データ設定 */
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

    /* 設定終了 */
    if (keys == btnSELECT && keysOld == btnNONE && value != 0) {
      // 設定された日付を保存する
      savedDayDate = dayDate;
      savedMonthDate = monthDate;
      savedYearDate = yearDate;
      if (numDate > 0) timesLeft = true;
      else if (numDate == 0) timesLeft = false;  // 福引回数を指定しているか否か
      repeatCount = numDate;
      setupMode = !setupMode;
      runMode = !runMode;
      lcd.noCursor();
      lcdAmiibo();
      delay(200);
    }
    keysOld = keys;  // 前回のキー状態を記録
  }
}

/* 日付制御=========================================================== */
/* 日付の値の制御 */
void setDate() {
  int direction = (keys == btnUP) ? 1 : -1;
  if (languageFlag == 0) {
    if (curPos == 1) {  // 月
      monthDate = (monthDate + direction + 12) % 12;
      if (monthDate == 0) monthDate = 12;                         // 月が0にならないように修正
      int daysInCurrentMonth = daysInMonth(monthDate, yearDate);  // 月を変更した後、日付がその月の日数を超えている場合に日付を修正
      if (dayDate > daysInCurrentMonth) dayDate = daysInCurrentMonth;
    } else if (curPos == 2) {  // 日
      dayDate += direction;
      int daysInCurrentMonth = daysInMonth(monthDate, yearDate);
      if (dayDate < 1) dayDate = daysInCurrentMonth;
      if (dayDate > daysInCurrentMonth) dayDate = 1;
    } else if (curPos == 3) {  // 年
      yearDate = (yearDate + direction + 61) % 61;
    }
  } else {
    if (curPos == 1) {  // 年
      yearDate = (yearDate + direction + 61) % 61;
    } else if (curPos == 2) {  // 月
      monthDate = (monthDate + direction + 12) % 12;
      if (monthDate == 0) monthDate = 12;                         // 月が0にならないように修正
      int daysInCurrentMonth = daysInMonth(monthDate, yearDate);  // 月を変更した後、日付がその月の日数を超えている場合に日付を修正
      if (dayDate > daysInCurrentMonth) dayDate = daysInCurrentMonth;
    } else if (curPos == 3) {  // 日
      dayDate += direction;
      int daysInCurrentMonth = daysInMonth(monthDate, yearDate);
      if (dayDate < 1) dayDate = daysInCurrentMonth;
      if (dayDate > daysInCurrentMonth) dayDate = 1;
    }
  }
  cursorPosition();
}
/* 月ごとの日数を返す関数 */
int daysInMonth(byte month, byte year) {
  switch (month) {
    case 2:  // 2月の場合
      return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) ? 29 : 28;
    case 4:
    case 6:
    case 9:
    case 11:  // 30日の月
      return 30;
    default:  // 31日の月
      return 31;
  }
}

/* LCD制御=========================================================== */
/* 進行中LCD */
void lcdProgress() {
  lcd.setCursor(3, 1);
  if (prg == 0) lcd.print("HOME...");
  else if (prg == 1) lcd.print("System.");
  else if (prg == 2) lcd.print("Moving.");
  else if (prg == 3) lcd.print("DATE...");
  else if (prg == 4) lcd.print("Change.");
  else if (prg == 5) lcd.print("Amiibo.");
  else if (prg == 6) lcd.print("Lottery");
}
/* 日付LCD表示制御 */
void lcdSetDateAmiibo() {
  lcd.setCursor(7, 0);
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
/* LCD制御=========================================================== */
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
    lcd.print("  ");
    lcd.setCursor(7, 0);
    showLcdDate();  // 日付をLCDに表示する
    lcd.setCursor(7, 0);
    lcd.print((languageFlag == 0) ? "M" : "Y");
    lcd.setCursor(11, 1);
    lcd.print("T");
    updateCountLottery();  // カウンター表示更新
  }
  if (value == 2) {
    if (timesLeft) {
      lcd.setCursor(11, 0);
      lcd.print("T");
      updateCountLottery();  // カウンター表示更新
    } else {
      lcd.setCursor(7, 0);
      showLcdDate();  // 日付をLCDに表示する
    }
    lcd.setCursor(10, 1);
    lcd.print(" SRT>S");
  }
}
