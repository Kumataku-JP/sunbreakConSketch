/*  モンハンライズサンブレイク  マカ錬金
 *  
 *  概要／
 *  マカ錬金を10連するマクロ
 *  10回抽選して受取り画面で停止
 *  途中でストップ可
 *  
 *  準備／
 *  「迅錬丹」を使用するを選択しておく
 *  カーソル初期位置共通／「護石を受け取る」に置き実行
 *
 *  1.覇気     使用素材を6で指定セットしカーソル初期位置を「護石を受け取る」に置き実行
 *  
 *  2.円環    「傀異マカ錬金・円環」を選択し、錬金したいスキル選択し、
 *            素材にしたいページにカーソルを置き実行
 *            ※ただし、最下段と装備している護石がある場合には最上段列にカーソルを置かない
 *  
 *  3.覇気で使用する素材の位置を指定
 *    ab/cd  1つ目（a=ページ、b=上から何個目）  ／  2つ目（c=ページ、d=上から何個目）
 *    初期値  11/00  （1ページ目の1個目を指定）2つ目の素材指定なしで1つ目のみ選択
 *
 *  4.装備売却  1ページ(50装備)分を売却
 *            画面が高速にチラつくので癲癇等に注意
 *
 *
*/

void mode2() {

  keys = read_LCD_buttons(analogRead(0));

  /*  マカ錬金  */
  //マカ錬金選択
  if (!dateSet) {
    if (keys == btnRIGHT) {
      value++;
      if (value > 4) value = 1;  // 1から4へトグル
      lcd.clear();
      lcdMelding();  //LCD表示
      delay(250);
    }
    if (keys == btnLEFT) {
      value--;
      if (value < 1) value = 4;  // 4から1へトグル
      lcd.clear();
      lcdMelding();  //LCD表示
      delay(250);
    }

    if (keys == btnSELECT && keysOld == btnNONE && value != 0) {
      steyMode2 = !steyMode2;
    }
    keysOld = keys;

    if (steyMode2) {

      /* 素材複数選択設定 */
      if (value == 1) {
        dateSet = true;
        meldingStop = 4;
      }
      if (value == 2) {
        if (repeatCountMelding == 0) {
          meldingStop = 1;
          lcd.setCursor(10, 0);
          lcd.print("Run");
          lcd.setCursor(10, 1);
          lcd.print(" STP>S");
          meldingVigor();
        }
        if (repeatCountMelding > 0) {
          meldingStop = 1;
          lcd.noCursor();
          lcd.setCursor(7, 0);
          lcd.print("  ");
          lcdCountMelding(repeatCountMelding);  // カウントLCD表示
          lcd.setCursor(12, 0);
          lcd.print("R");
          lcd.setCursor(13, 0);
          lcdCount(repeatCountMelding - 1);  // カウントLCD表示
          lcd.setCursor(10, 1);
          lcd.print(" STP>S");
          meldingVigor();
          if (lap % 10 == 0) {
            repeatCountMelding--;  // カウントをデクリメント
            lap = 0;
          }
          if (repeatCountMelding == 0) {
            meldingStop = 5;
            repeatCountMelding = numDate;
            steyMode2 = false;
          }
        }
      }
      if (value == 3) {
        meldingStop = 1;
        lcd.setCursor(10, 0);
        lcd.print("Run");
        lcd.setCursor(10, 1);
        lcd.print(" STP>S");
        meldingCyclus();
      }
      // 装備売却
      else if (value == 4) {
        meldingStop = 2;
        lcd.setCursor(10, 0);
        lcd.print("RUN");
        lcd.setCursor(10, 1);
        lcd.print(" STP>S");
        sellEquipment();
      }
    }
    /* ストップ後のLCD表示 */
    if (!steyMode2) {
      switch (meldingStop) {
        case 0:
          break;
        case 1:
          lcd.setCursor(10, 0);
          lcd.print("STP");
          lcd.setCursor(10, 1);
          lcd.print("ReST>S");
          lap = 0;
          delay(500);
          meldingStop = 0;
          break;
        case 2:
          lcdMelding();  //LCD表示
          lcd.setCursor(10, 0);
          lcd.print("End");
          lcd.setCursor(10, 1);
          lcd.print(" SRT>S");
          lap = 0;
          delay(50);
          meldingStop = 0;
          break;
        case 3:
          lcdSetMaterial();
          lcd.setCursor(10, 0);
          lcd.print("End");
          lap = 0;
          delay(50);
          meldingStop = 0;
          break;
        case 4:
          lcd.setCursor(10, 1);
          lcd.print("C");
          delay(500);
          meldingStop = 0;
          break;
        case 5:
          lcdMelding();  //LCD表示
          lap = 0;
          delay(50);
          meldingStop = 0;
          break;
      }
    }
  }  //  !dateSetここまで

  /* 素材複数選択設定*/
  if (dateSet) {
    // 1つ目のポジションでセレクトボタンが押された場合
    if (keys == btnSELECT && keysOld == btnNONE && value != 0) {
      if (cursorPosition) {
        dateSet = false;
        steyMode2 = false;
        repeatCountMelding = numDate;
        lcd.noCursor();
      }
    }
    // 左ボタンが押された場合
    if (keys == btnLEFT && keysOld == btnNONE) {
      cursorPosition--;
      if (cursorPosition < 1) cursorPosition = 6;
    }
    // 右ボタンが押された場合
    else if (keys == btnRIGHT && keysOld == btnNONE) {
      cursorPosition++;
      if (cursorPosition > 6) cursorPosition = 1;
    }

    // 上下ボタンが押された場合 選択されたカーソルの位置に応じて増減させる
    if (keys == btnUP && keysOld == btnNONE) {
      if (cursorPosition == 1) {
        page_1++;  // 1つめページ設定
        if (page_1 > 9) page_1 = 1;
      } else if (cursorPosition == 2) {
        line_1++;  // 1つめ素材設定
        if (line_1 > 7) line_1 = 1;
      } else if (cursorPosition == 3) {
        page_2++;  // 2つめページ設定
        if (page_2 > 9) page_2 = 0;
      } else if (cursorPosition == 4) {
        line_2++;  // 2つめ素材設定
        if (line_2 > 7) line_2 = 0;
      }
      if (cursorPosition >= 5 && cursorPosition <= 6) {
        int digitValueMelding = digitsM[cursorPosition - 5];
        digitValueMelding = (digitValueMelding + 1) % 10;
        digitsM[cursorPosition - 5] = digitValueMelding;
      }
    }
    if (keys == btnDOWN && keysOld == btnNONE) {
      if (cursorPosition == 1) {
        page_1--;  // 1つめページ設定
        if (page_1 < 1) page_1 = 9;
      } else if (cursorPosition == 2) {
        line_1--;  // 1つめ素材設定
        if (line_1 < 1) line_1 = 7;
      } else if (cursorPosition == 3) {
        page_2--;  // 2つめページ設定
        if (page_2 < 0) page_2 = 9;
      } else if (cursorPosition == 4) {
        line_2--;  // 2つめ素材設定
        if (line_2 < 0) line_2 = 7;
      }
      if (cursorPosition >= 5 && cursorPosition <= 6) {
        int digitValueMelding = digitsM[cursorPosition - 5];
        digitValueMelding = (digitValueMelding - 1 + 10) % 10;
        digitsM[cursorPosition - 5] = digitValueMelding;
      }
    }
    keysOld = keys;  // 前回のキー状態を記録
    lcd.setCursor(14, 0);
    updateCountMelding();
    lcd.setCursor(3, 1);
    if (languageFlag == 0) lcd.print("MATL > ");
    else if (languageFlag == 1) lcd.print(jp("ｿｻﾞｲ > "));
    lcd.setCursor(10, 1);
    lcd.print("S");
    lcdSetMaterial();  // LCDに表示する
    delay(100);
  }  // dateSetここまで
}  // mode2ここまで


/* LCD表示 */
void lcdMelding() {
  lcd.setCursor(0, 0);
  lcd.print(value);
  lcd.print(".");
  displayString(value, 2);
  lcd.setCursor(0, 1);
  lcd.print("M");
  lcd.print(mode);
  lcd.print(".");
  displayString(mode, 0);  // Melding
  lcd.setCursor(10, 1);
  if (value >= 1 && value <= 2) {
    lcd.print(" SRT>S");
  }
  if (value == 1) {
    lcd.print("C");
    lcdSetMaterial();
    lcd.setCursor(11, 0);
    lcd.print("> T");
    for (int i = 0; i < 2; i++) {
      lcd.print(digitsM[(int)i]);
    }
  }
  if (value == 2) {
    lcd.setCursor(11, 1);
    lcdSetMaterial();
    lcd.setCursor(11, 0);
    lcd.print("SRT>S");
    if (repeatCountMelding > 0) {
      lcd.setCursor(7, 0);
      lcd.print("S");
      updateCountMelding();
      lcd.print(" SRT>S");
    }
  }
  if (value == 3) lcd.print(" SRT>S");
  if (value == 4) lcd.print("SELL>S");
}
/* 素材設定LCD表示 */
void lcdSetMaterial() {
  char text[2];
  lcd.setCursor(11, 1);
  sprintf(text, "%01d", page_1);
  lcd.print(text);
  sprintf(text, "%01d", line_1);
  lcd.print(text);
  lcd.print("/");
  sprintf(text, "%01d", page_2);
  lcd.print(text);
  sprintf(text, "%01d", line_2);
  lcd.print(text);
  meldingPosition();
}
/* カーソルの位置制御 */
void meldingPosition() {
  if (cursorPosition == 1) lcd.setCursor(11, 1);       //
  else if (cursorPosition == 2) lcd.setCursor(12, 1);  //
  else if (cursorPosition == 3) lcd.setCursor(14, 1);  //
  else if (cursorPosition == 4) lcd.setCursor(15, 1);  //
  else if (cursorPosition == 5) lcd.setCursor(14, 0);  //
  else if (cursorPosition == 6) lcd.setCursor(15, 0);  //
  if (dateSet) {
    lcd.cursor();
  }
}
// 各桁の数字をディスプレイに表示
void updateCountMelding() {
  numDate = digitsM[0] * 10 + digitsM[1];
  for (int i = 0; i < 2; i++) {
    lcd.print(digitsM[(int)i]);
  }
}
/* のこりマカ回数LCD表示 */
void lcdCountMelding(int count) {
  char text[3];
  lcd.setCursor(9, 0);
  lcd.print("S");
  sprintf(text, "%02d", count);
  lcd.print(text);
}