/*  マクロコントローラーシステム設定
 *   
 *  セレクトボタンで切り替え、設定後、上または下ボタンで本機に設定記録（EEPROM）
 *  ※ Arduinoは、EEPROMに記録できる上限があるので不要な操作はしないように。
*/

void mode0() {

  keys = read_LCD_buttons(analogRead(0));
  /*  初期値設定  */
  //設定する項目の表示
  if (!dateSet) {
    if ((keys == btnRIGHT || keys == btnLEFT) && keysOld == btnNONE) {
      value += (keys == btnRIGHT) ? 1 : -1;  // 右を押したか正否判定
      if (value > 4) value = 1;              // 1から4へトグル
      else if (value < 1) value = 4;         // 4から1へトグル
      initialLcd = true;                     // 実行後にフラグをセット
      lcd.clear();                           // LCD初期化
      lcdSetting();                          // LCD表示
      delay(250);
    }
    keysOld = keys;  // 前回のキー状態を記録

    if (keys == btnSELECT) {
      lcd.setCursor(0, 1);
      bool configValue;

      switch (value) {
        case 1:  // ゲーム機タイプ（決定ボタン）
          configValue = !consoleType;
          consoleType = configValue;
          lcd.print(configValue ? "PlayStation 5  " : "Nintendo Switch");
          break;

        case 2:  // Rボタンマッピング
          configValue = !mappingR;
          mappingR = configValue;
          if (consoleType == 0) lcd.print(configValue ? "Button R to ZR  " : "Button R Default");
          else lcd.print(configValue ? "Button R1 to R2 " : "ButtonR1 Default");
          break;

        case 3:  // 日付登録
          dateSet = !dateSet;
          break;

        case 4:  // 言語選択
          configValue = !languageFlag;
          languageFlag = configValue;
          if (configValue) lcd.print(jp("ﾆﾎﾝｺﾞ  "));
          else lcd.print("English");
          break;
      }
      memory = true;
      delay(250);
    }
  }

  /* 日付設定 */
  if (dateSet) {
    lcd.setCursor(0, 1);
    lcd.print("SET ");

    if (keys == btnSELECT && keysOld == btnNONE && value != 0) {
      dateSet = !dateSet;
      lcd.noCursor();
      lcd.setCursor(0, 1);
      lcd.print("DATE");
    }

    // 左右ボタンで年月日カーソル送りトグル
    if (keysOld == btnNONE) {
      cursorPosition += (keys == btnLEFT) ? -1 : (keys == btnRIGHT) ? 1
                                                                    : 0;
      if (cursorPosition < 1) cursorPosition = 3;
      else if (cursorPosition > 3) cursorPosition = 1;
    }
    keysOld = keys;  // 前回のキー状態を記録

    // 上下ボタンが押された場合 選択されたカーソルの位置に応じて日付を増加させる
    if (keys == btnUP || keys == btnDOWN) {
      setDate();  // 日付を増加させる
    }
    lcdSetDate();  // 日付をLCDに表示する
    delay(200);
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
    confirmButton = (consoleType == 0) ? Button::A : Button::B;
    cancelButton = (consoleType == 0) ? Button::B : Button::A;
    mappingR1 = (mappingR == 0) ? Button::R : Button::ZR;
    mappingR2 = (mappingR == 0) ? Button::ZR : Button::R;
  }
  memory = false;
}


/* 日付LCD表示制御 */
void lcdSetDate() {
  lcdDate();
  lcd.print("     ");
  updateCursorPosition();
}

void updateCursorPosition() {
  if (cursorPosition == 1) lcd.setCursor(5, 1);        // 年の位置
  else if (cursorPosition == 2) lcd.setCursor(8, 1);   // 月の位置
  else if (cursorPosition == 3) lcd.setCursor(11, 1);  // 日の位置
  if (dateSet) {
    lcd.cursor();  // カーソル常時表示
  }
}
/* 日付を設定する */
void setDate() {
  int direction = (keys == btnUP) ? 1 : -1;  // btnUPなら+1、btnDOWNなら-1
  if (cursorPosition == 1) {                 // 年
    yearDate = (yearDate + direction + 61) % 61;
  } else if (cursorPosition == 2) {  // 月
    monthDate = (monthDate + direction + 12) % 12;
    if (monthDate == 0) monthDate = 12;  // 月が0にならないように修正
    // 月を変更した後、日付がその月の日数を超えている場合に日付を修正
    int daysInCurrentMonth = daysInMonth(monthDate, yearDate);
    if (dayDate > daysInCurrentMonth) {
      dayDate = daysInCurrentMonth;
    }
  } else if (cursorPosition == 3) {  // 日
    dayDate += direction;
    int daysInCurrentMonth = daysInMonth(monthDate, yearDate);
    if (dayDate < 1) dayDate = daysInCurrentMonth;
    if (dayDate > daysInCurrentMonth) dayDate = 1;
  }
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

/* LCD表示 */
void lcdSetting() {
  lcd.setCursor(0, 0);
  lcd.print(value);
  lcd.print(".");
  lcd.setCursor(2, 0);
  displayString(value, 5);
  // 2列目LCD
  lcd.setCursor(0, 1);
  if (value == 1) {
    if (consoleType == 0) lcd.print("Nintendo Switch");
    else if (consoleType == 1) lcd.print("PlayStation 5");
  }
  if (value == 2) {
    if (mappingR == 0 && consoleType == 0) lcd.print("Button R Default");       // R Button Default
    else if (mappingR == 0 && consoleType == 1) lcd.print("ButtonR1 Default");  // R1 Button Default
    else if (mappingR == 1 && consoleType == 0) lcd.print("Button R to ZR");    // R Button to RZ
    else if (mappingR == 1 && consoleType == 1) lcd.print("ButtonR1 to R2");    // R1 Button to R2
  }
  if (value == 3) {
    lcd.print("DATE");
    lcdSetDate();
  }
  if (value == 4) {
    if (languageFlag == 0) lcd.print("English");
    else if (languageFlag == 1) lcd.print(jp("ﾆﾎﾝｺﾞ"));
  }
}