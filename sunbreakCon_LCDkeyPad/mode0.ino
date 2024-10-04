/*  マクロコントローラーシステム設定
 *   
 *  セレクトボタンで切り替え、設定後4で本機に設定記録（EEPROM）
 *  ※ Arduinoは、EEPROMに記録できる上限があるので不要な操作はしないように。
*/

void mode0() {

  /*  初期値設定  */
  //設定する項目の表示
  if (keys == btnRIGHT || keys == btnLEFT) {
    int direction = (keys == btnRIGHT) ? 1 : -1;
    value += direction;
    if (value < 1) value = 4;
    else if (value > 4) value = 1;
    delay(300);
    initialLcd = true;  // 実行後にフラグをセット
    lcd.clear();        // LCD初期化
    lcdConfig();        // LCD表示
  }
  if (keys == btnSELECT && keysOld == btnNONE && value != 0) {
    lcd.clear();  // LCD初期化
    lcd.setCursor(0, 1);
    switch (value) {
      case 1:  // 言語選択
        languageFlag = !languageFlag;
        break;
      case 2:  // ゲーム機タイプ（決定ボタン）
        consoleType = !consoleType;
        //
        break;
      case 3:  // Rボタンマッピング
        mappingR = !mappingR;
        break;
      case 4:
        eepromUpdate();
        break;
    }
    lcdConfig();
    delay(250);
  }
  keysOld = keys;  // 前回のキー状態を記録
}

/* ============================================================== */
// EEPROMに書き込む(モード0のvalueが1から4で設定変更をした場合)
void eepromUpdate() {
  byte data[] = { languageFlag, consoleType, mappingR };
  size_t dataSize = sizeof(data);  // data のサイズを格納
  for (size_t i = 0; i < dataSize; i++) {
    EEPROM.update(i, data[i]);
  }
  lcd.clear();  // LCD初期化
  lcd.setCursor(0, 1);
  lcd.print("Update");
  delay(1000);
  value = 0;
  lcd.setCursor(0, 0);
  lcdSelect();                                                           // value = 0 1列目LCD0-
  commonLcdRow2();                                                       // 2列目LCD0-2
  displayString(0, mode);                                                // 2列目LCD3-
  confirmButton = (consoleType == 0) ? Button::A : Button::B;            // Switch
  cancelButton = (consoleType == 0) ? Button::B : Button::A;             // PS5
  minusButton = (consoleType == 0) ? Button::MINUS : Button::CAPTURE;    // Switch
  captureButton = (consoleType == 0) ? Button::CAPTURE : Button::MINUS;  // PS5
  mappingR1 = (mappingR == 0) ? Button::R : Button::ZR;
  mappingR2 = (mappingR == 0) ? Button::ZR : Button::R;
}

/* LCD制御=========================================================== */
/* LCD表示 */
void lcdConfig() {
  commonLcdRow1();             // 1列目LCD0-1
  displayString(value, mode);  // 1列目LCD2-
  lcd.setCursor(0, 1);         // 2列目LCD
  switch (value) {
    case 1:
      lcd.print(languageFlag ? jp("ﾆﾎﾝｺﾞ") : "English");
      break;
    case 2:
      lcd.print(consoleType ? "PS5" : "Switch");
      break;
    case 3:
      if (consoleType == 0) lcd.print(mappingR ? "R to ZR" : "R Default");
      else lcd.print(mappingR ? "R1 to R2" : "R1 Default");
      break;
    case 4:
      lcd.print("UPDATE>SELECT");
      break;
  }
}