/*  マクロコントローラーシステム設定
 *   
 *  セレクトボタンで切り替え、設定後、上または下ボタンで本機に設定記録（EEPROM）
 *  ※ Arduinoは、EEPROMに記録できる上限があるので不要な操作はしないように。
*/

void mode0() {

  if (initialLcd) {
    if (value == 0) {
      lcd.setCursor(0, 0);
      displayString(5, 5);  //MENU
    }
  }

  keys = read_LCD_buttons(analogRead(0));

  /*  初期値設定  */
  //設定する項目の表示
  if (!dateSet) {
    if (keys == btnRIGHT) {
      value++;
      if (value > 4) value = 1;  // 1から4へトグル
      initialLcd = true;         // 実行後にフラグをセット
      lcd.clear();
      lcdSetting();  // LCD表示
      delay(250);
    }
    if (keys == btnLEFT) {
      value--;
      if (value < 1) value = 4;  // 4から1へトグル
      initialLcd = true;         // 実行後にフラグをセット
      lcd.clear();
      lcdSetting();  // LCD表示
      delay(250);
    }

    if (keys == btnSELECT) {
      delay(250);
      lcd.setCursor(0, 1);
      // ゲーム機タイプ（決定ボタン）
      if (value == 1) {
        bool configValue = consoleType;
        if (configValue == 0) configValue = 1;
        else if (configValue == 1) configValue = 0;
        switch (configValue) {
          case 0:
            lcd.print("Nintendo Switch ");
            consoleType = 0;
            break;
          case 1:
            lcd.print("PlayStation 5   ");
            consoleType = 1;
            break;
        }
      }
      // Rボタンマッピング
      if (value == 2) {
        bool configValue = mappingR;
        if (configValue == 0) configValue = 1;
        else if (configValue == 1) configValue = 0;
        switch (configValue) {
          case 0:
            if (consoleType == 0) lcd.print("R Button Default");       // R Button Default
            else if (consoleType == 1) lcd.print("R1Button Default");  // R1 Button Default
            mappingR = 0;
            break;
          case 1:
            if (consoleType == 0) lcd.print("R Button to RZ  ");       // R Button to RZ
            else if (consoleType == 1) lcd.print("R1Button to R2  ");  // R1 Button to R2
            mappingR = 1;
            break;
        }
      }
      // 日付登録
      if (value == 3) {
        bool configValue = dateSet;
        if (configValue) {
          dateSet = false;
          lcd.setCursor(0, 1);
          lcd.print("COM");
        } else {
          dateSet = true;
        }
      }
      // 言語選択
      if (value == 4) {
        bool configValue = languageFlag;
        if (configValue == 0) configValue = 1;
        else if (configValue == 1) configValue = 0;
        switch (configValue) {
          case 0:
            lcd.print("Language English");
            languageFlag = 0;
            break;
          case 1:
            lcd.print(jp("ｹﾞﾝｺﾞ ﾆﾎﾝｺﾞ     "));
            languageFlag = 1;
            break;
        }
      }
      memory = true;
    }
  }

  /* 日付設定 */
  if (dateSet) {
    lcd.setCursor(0, 1);
    lcd.print("SET ");
    // 1つ目のポジションでセレクトボタンが押された場合
    if (keys == btnSELECT && keysOld == btnNONE && value != 0) {
      dateSet = false;
      lcd.noCursor();
      lcd.setCursor(0, 1);
      lcd.print("DATE");
      delay(200);
    }
    // 左右ボタンで年月日カーソル送りトグル
    if (keys == btnLEFT && keysOld == btnNONE) {
      cursorPosition--;
      if (cursorPosition < 1) cursorPosition = 3;
    }
    if (keys == btnRIGHT && keysOld == btnNONE) {
      cursorPosition++;
      if (cursorPosition > 3) cursorPosition = 1;
    }
    // 上下ボタンが押された場合 選択されたカーソルの位置に応じて日付を増加させる
    if (keys == btnUP && keysOld == btnNONE) {
      // 年の設定
      if (cursorPosition == 1) {
        yearDate++;
        if (yearDate > 60) yearDate = 0;
        // 閏年
        if (monthDate == 2 && dayDate > daysInMonth(monthDate, yearDate)) {
          dayDate = daysInMonth(monthDate, yearDate);
        }
      }
      // 月の設定
      else if (cursorPosition == 2) {
        monthDate++;
        if (monthDate > 12) monthDate = 1;
        if (dayDate > daysInMonth(monthDate, yearDate)) {
          dayDate = daysInMonth(monthDate, yearDate);
        }
      }
      // 日の設定
      else if (cursorPosition == 3) {
        dayDate++;
        // 月の日数を超えた場合は1日に戻す
        if (dayDate > daysInMonth(monthDate, yearDate)) {
          dayDate = 1;
        }
      }
    }
    if (keys == btnDOWN && keysOld == btnNONE) {
      if (cursorPosition == 1) {
        yearDate--;
        if (yearDate < 0) yearDate = 60;
        if (monthDate == 2 && dayDate > daysInMonth(monthDate, yearDate)) {
          dayDate = daysInMonth(monthDate, yearDate);
        }
      } else if (cursorPosition == 2) {
        monthDate--;
        if (monthDate < 1) monthDate = 12;
        if (dayDate > daysInMonth(monthDate, yearDate)) {
          dayDate = daysInMonth(monthDate, yearDate);
        }
      } else if (cursorPosition == 3) {
        dayDate--;
        if (dayDate < 1) {
          dayDate = daysInMonth(monthDate, yearDate);
        }
      }
    }
    keysOld = keys;  // 前回のキー状態を記録
    lcdSetDate();    // 日付をLCDに表示する
    delay(100);
  }
}



// EEPROMに書き込む(モード0のvalueが1から4で設定変更をした場合)
void settingMemory() {
  if (value >= 1 && value <= 4 && mode == 0 && memory) {
    byte data[] = { consoleType, mappingR, languageFlag, dayDate, monthDate, yearDate };
    size_t dataSize = sizeof(data);  // data のサイズを格納
    for (size_t i = 0; i < dataSize; i++) {
      EEPROM.write(i, data[i]);
    }
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Memory");
    delay(750);
    lcd.clear();
    switch (consoleType) {
      case 0:
        confirmButton = Button::A;
        cancelButton = Button::B;
        break;
      case 1:
        confirmButton = Button::B;
        cancelButton = Button::A;
        break;
    }
    switch (mappingR) {
      case 0:
        mappingR1 = Button::R;
        mappingR2 = Button::ZR;
        break;
      case 1:
        mappingR1 = Button::ZR;
        mappingR2 = Button::R;
        break;
    }
  }
  memory = false;
}


/* LCD表示 */
void lcdSetting() {
  lcd.setCursor(0, 0);
  lcd.print(value);
  lcd.print(".");
  lcd.setCursor(2, 0);
  if (value == 1) {
    displayString(value, 5);  //consoleType
    lcd.setCursor(0, 1);
    if (consoleType == 0) lcd.print("Nintendo Switch ");
    else if (consoleType == 1) lcd.print("PlayStation 5   ");
  }
  if (value == 2) {
    displayString(value, 5);  //R KEY Mapping
    lcd.setCursor(0, 1);
    if (mappingR == 0 && consoleType == 0) lcd.print("R Button Default");       // R Button Default
    else if (mappingR == 0 && consoleType == 1) lcd.print("R1Button Default");  // R1 Button Default
    else if (mappingR == 1 && consoleType == 0) lcd.print("R Button to RZ  ");  // R Button to RZ
    else if (mappingR == 1 && consoleType == 1) lcd.print("R1Button to R2  ");  // R1 Button to R2
  }
  if (value == 3) {
    displayString(value, 5);  //DATE Setting
    lcd.setCursor(0, 1);
    lcd.print("DATE");
    lcdSetDate();
  }
  if (value == 4) {
    displayString(value, 5);  //Language
    lcd.setCursor(0, 1);
    if (languageFlag == 0) lcd.print("Language English");
    else if (languageFlag == 1) lcd.print(jp("ｹﾞﾝｺﾞ ﾆﾎﾝｺﾞ     "));
  }
}

/* 日付LCD表示制御 */
void lcdSetDate() {
  char text[3];
  sprintf(text, "%02d", yearDate);
  lcd.print(text);
  lcd.print("/");
  sprintf(text, "%02d", monthDate);
  lcd.print(text);
  lcd.print("/");
  sprintf(text, "%02d", dayDate);
  lcd.print(text);
  lcd.print("     ");
  updateCursorPosition();
}

void updateCursorPosition() {
  if (cursorPosition == 1) lcd.setCursor(5, 1);        // 年の位置
  else if (cursorPosition == 2) lcd.setCursor(8, 1);   // 月の位置
  else if (cursorPosition == 3) lcd.setCursor(11, 1);  // 日の位置
  if (dateSet) {
    lcd.cursor();
  }
}

// 月ごとの日数を返す関数
int daysInMonth(byte month, byte year) {
  if (month == 2) {
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
      return 29;  // 閏年の2月
    } else {
      return 28;  // 閏年でない2月
    }
  } else if (month == 4 || month == 6 || month == 9 || month == 11) {
    return 30;  // 4, 6, 9, 11月は30日
  } else {
    return 31;  // その他の月は31日
  }
}
