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
      lcdConfig();                           // LCD表示
      delay(250);
    }

    if (keys == btnSELECT && keysOld == btnNONE) {
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
    keysOld = keys;  // 前回のキー状態を記録
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
      delay(200);
    }

    // 左右ボタンで年月日カーソル送りトグル
    if (keysOld == btnNONE) {
      curPos += (keys == btnLEFT) ? -1 : (keys == btnRIGHT) ? 1
                                                            : 0;
      if (curPos < 1) curPos = 3;
      else if (curPos > 3) curPos = 1;
    }
    keysOld = keys;  // 前回のキー状態を記録

    // 上下ボタンが押された場合 選択されたカーソルの位置に応じて日付を増加させる
    if (keys == btnUP || keys == btnDOWN) {
      lcd.setCursor(5, 1);
      setDate();  // 日付を増加させる
    }
    lcd.setCursor(5, 1);
    showLcdDate();  // 日付をLCDに表示する
    lcd.print("     ");
    cursorPosition();
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


/* 日付を設定する */
void setDate() {
  int direction = (keys == btnUP) ? 1 : -1;  // btnUPなら+1、btnDOWNなら-1
  if (curPos == 1) {                         // 年
    yearDate = (yearDate + direction + 61) % 61;
  } else if (curPos == 2) {  // 月
    monthDate = (monthDate + direction + 12) % 12;
    if (monthDate == 0) monthDate = 12;  // 月が0にならないように修正
    // 月を変更した後、日付がその月の日数を超えている場合に日付を修正
    int daysInCurrentMonth = daysInMonth(monthDate, yearDate);
    if (dayDate > daysInCurrentMonth) {
      dayDate = daysInCurrentMonth;
    }
  } else if (curPos == 3) {  // 日
    dayDate += direction;
    int daysInCurrentMonth = daysInMonth(monthDate, yearDate);
    if (dayDate < 1) dayDate = daysInCurrentMonth;
    if (dayDate > daysInCurrentMonth) dayDate = 1;
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

/* LCD表示 */
void lcdConfig() {

  commonLcdRow1();  // 1列目LCD

  lcd.setCursor(0, 1);  // 2列目LCD
  switch (value) {
    case 1:
      lcd.print((consoleType == 0) ? "Nintendo Switch" : "PlayStation 5");
      break;
    case 2:
      if (consoleType == 0) lcd.print((mappingR == 0) ? "Button R Default" : "Button R to ZR");
      else lcd.print((mappingR == 0) ? "ButtonR1 Default" : "ButtonR1 to R2");
      break;
    case 3:
      lcd.print("DATE");
      lcd.setCursor(5, 1);
      showLcdDate();  // 日付をLCDに表示する
      lcd.print("     ");
      cursorPosition();
      break;
    case 4:
      lcd.print((languageFlag == 0) ? "English" : jp("ﾆﾎﾝｺﾞ"));
      break;
    default:
      lcd.print("Unknown");  // 予期しない値に対するデフォルトの表示
      break;
  }
}